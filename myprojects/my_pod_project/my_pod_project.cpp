#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;
Oscillator osc;
// Parameter knob1, knob2;
DaisyPod::Knob knob1;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    hw.ProcessAllControls();
    for (size_t i = 0; i < size; i++)
    {
        // Read the value of knob1 to control the pitch
        float pitch = hw.GetKnobValue(knob1);
        
        // Map the knob value to a suitable frequency range
        float frequency = 100.0f + pitch * 800.0f; // Adjust the range as needed

        // Set the frequency of the sine wave oscillator
        osc.SetFreq(frequency);

        // Generate a sine wave and send it to both audio output channels
        float sine_wave = osc.Process();  // Generate the next sample of the sine wave
        out[0][i] = sine_wave;           // Left channel
        out[1][i] = sine_wave;           // Right channel
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    // Initialize the oscillator with a sine wave
    osc.Init(hw.AudioSampleRate());
    osc.SetWaveform(Oscillator::WAVE_SIN);

    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    while (1) {}
}
