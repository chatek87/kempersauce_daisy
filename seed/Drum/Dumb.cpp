#include "daisy_seed.h"
#include "daisysp.h"
#include "daisy_pod.h"  // Added for parameter control

using namespace daisy;
using namespace daisysp;

DaisyPod  hw;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

Oscillator osc;
WhiteNoise noise;

AdEnv kickVolEnv, kickPitchEnv, snareEnv;

Switch kick, snare;

Parameter p_knob1, p_knob2; // Declare p_knob1 and p_knob2

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float osc_out, noise_out, snr_env_out, kck_env_out, sig;
    // Get rid of any bouncing
    snare.Debounce();
    kick.Debounce();

    // If you press the kick button...
    if (kick.RisingEdge())
    {
        // Trigger both envelopes!
        kickVolEnv.Trigger();
        kickPitchEnv.Trigger();
    }

    // If you press the snare button trigger its envelope
    if (snare.RisingEdge())
    {
        snareEnv.Trigger();
    }

    // Use the values of p_knob1 and p_knob2 to set attack and decay times
    float attackTime = 0.01f + (0.99f * p_knob1.Process());
    float decayTime = 0.01f + (0.99f * p_knob2.Process());

    // Set the attack and decay times of kickVolEnv
    kickVolEnv.SetTime(ADENV_SEG_ATTACK, attackTime);
    kickVolEnv.SetTime(ADENV_SEG_DECAY, decayTime);

    // Prepare the audio block
    for (size_t i = 0; i < size; i += 2)
    {
        // Get the next volume samples
        snr_env_out = snareEnv.Process();
        kck_env_out = kickVolEnv.Process();

        // Apply the pitch envelope to the kick
        osc.SetFreq(kickPitchEnv.Process());
        // Set the kick volume to the envelope's output
        osc.SetAmp(kck_env_out);
        // Process the next oscillator sample
        osc_out = osc.Process();

        // Get the next snare sample
        noise_out = noise.Process();
        // Set the sample to the correct volume
        noise_out *= snr_env_out;

        // Mix the two signals at half volume
        sig = 0.5f * noise_out + 0.5f * osc_out;

        // Set the left and right outputs to the mixed signals
        out[i] = sig;
        out[i + 1] = sig;
    }
}

int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();
    hardware.SetAudioBlockSize(4);
    float samplerate = hardware.AudioSampleRate();

    // Initialize oscillator for kick drum
    osc.Init(samplerate);
    osc.SetWaveform(Oscillator::WAVE_TRI);
    osc.SetAmp(1);

    // Initialize noise
    noise.Init();

    // Initialize envelopes, this one's for the snare amplitude
    snareEnv.Init(samplerate);
    snareEnv.SetTime(ADENV_SEG_ATTACK, 0.01f);
    snareEnv.SetTime(ADENV_SEG_DECAY, 0.2f);
    snareEnv.SetMax(1);
    snareEnv.SetMin(0);

    // This envelope will control the kick oscillator's pitch
    // Note that this envelope is much faster than the volume
    kickPitchEnv.Init(samplerate);
    kickPitchEnv.SetTime(ADENV_SEG_ATTACK, 0.01f);
    kickPitchEnv.SetTime(ADENV_SEG_DECAY, 0.05f);
    kickPitchEnv.SetMax(400);
    kickPitchEnv.SetMin(50);

    // This one will control the kick's volume
    kickVolEnv.Init(samplerate);
    kickVolEnv.SetTime(ADENV_SEG_ATTACK, 0.01f);
    kickVolEnv.SetTime(ADENV_SEG_DECAY, 1.0f); // Initial value
    kickVolEnv.SetMax(1);
    kickVolEnv.SetMin(0);

    // Initialize the kick and snare buttons on pins 27 and 28
    // The callback rate is samplerate / blocksize (48)
    snare.Init(hardware.GetPin(27), samplerate / 48.f);
    kick.Init(hardware.GetPin(28), samplerate / 48.f);

    // Initialize parameter knobs
    hw.Init();
    p_knob1.Init(hw.knob1, 0.0f, 1.0f, Parameter::LINEAR); // Adjusted the range for knob1
    p_knob2.Init(hw.knob2, 0.0f, 1.0f, Parameter::LINEAR); // Adjusted the range for knob2
    hw.StartAdc();

    // Start calling the callback function
    hardware.StartAudio(AudioCallback);

    // Loop forever
    while (true);
}
