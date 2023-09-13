// Include necessary headers
#include "daisy_pod.h"
#include "daisysp.h"
#include <math.h>

using namespace daisy;
using namespace daisysp;

DaisyPod   hw;
Oscillator osc1, osc2;
Svf        filt;
ReverbSc   verb;
AdEnv      env;
Parameter  p_xf, p_vamt, p_dec, p_vtime;

// Define the scale and the current note index
const static float scale1[5] = {12, 9, 9, 9, 7}; // Change the scale as needed
const static float scale2[9]
    = {4, 4, 7, 4, 2, 0, 0, 4, 0}; // Change the scale as needed
static int current_note_index1 = 0;
static int current_note_index2 = 0;

// Function to get the next note in the cycle
static float get_new_note1()
{
    float note = scale1[current_note_index1];
    current_note_index1
        = (current_note_index1 + 1) % 5; // Cycling through 5 notes
    return note;
}
// Function to get the next note in the cycle
static float get_new_note2()
{
    float note = scale2[current_note_index2];
    current_note_index2
        = (current_note_index2 + 1) % 9; // Cycling through 9 notes
    return note;
}

static float freq1, freq2;
// float        sig, rawsig, filtsig, sendsig, wetvl, wetvr;
float sig1, sig2, rawsig1, rawsig2, filtsig, sendsig, wetvl, wetvr;
float xf, vamt, dec, time;

// Audio callback function
static void audio(AudioHandle::InterleavingInputBuffer  in,
                  AudioHandle::InterleavingOutputBuffer out,
                  size_t                                size)
{
    hw.ProcessDigitalControls();

    if(hw.button1.RisingEdge())
    {
        freq1 = mtof(48.0f + get_new_note1());
        osc1.SetFreq(freq1);
        env.SetTime(ADENV_SEG_DECAY, dec);
        env.Trigger();
        // Reset the other oscillator if it was active
        osc2.SetFreq(0.0f);
    }

    if(hw.button2.RisingEdge())
    {
        freq2 = mtof(48.0f + get_new_note2());
        osc2.SetFreq(freq2);
        env.SetTime(ADENV_SEG_DECAY, dec);
        env.Trigger();
        // Reset the other oscillator if it was active
        osc1.SetFreq(0.0f);
    }


    // Get Parameters
    xf   = p_xf.Process();
    vamt = p_vamt.Process();
    dec  = p_dec.Process();

    // Audio Loop
    for(size_t i = 0; i < size; i += 2)
    {
        // Process
        rawsig1 = osc1.Process();
        rawsig2 = osc2.Process();
        sig1    = rawsig1 * env.Process();
        sig2    = rawsig2 * env.Process();
        filt.Process(sig1);
        filt.Process(sig2);
        filtsig = filt.Low();
        sendsig = filtsig * vamt;
        verb.Process(sendsig, sendsig, &wetvl, &wetvr);
        out[i]     = (filtsig + (wetvl)) * 0.707f;
        out[i + 1] = (filtsig + (wetvr)) * 0.707f;
    }
}

// Initialization function
void InitSynth(float samplerate)
{
    // Synth Parameters.
    p_xf.Init(hw.knob1, 10.0f, 12000.0f, Parameter::LOGARITHMIC);
    p_dec.Init(hw.knob1, 0.2f, 5.0f, Parameter::EXPONENTIAL);
    p_vamt.Init(hw.knob2, 0.0f, 1.0f, Parameter::LINEAR);
    p_vtime.Init(hw.knob2, 0.4f, 0.95f, Parameter::LINEAR);
    dec = 0.62;
    // Init Osc and Nse
    osc1.Init(samplerate);
    osc1.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc1.SetFreq(100.0f);
    osc1.SetAmp(.5f);
    osc2.Init(samplerate);
    osc2.SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    osc2.SetFreq(100.0f);
    osc2.SetAmp(.5f);
    env.Init(samplerate);
    env.SetCurve(-15.0f);
    env.SetTime(ADENV_SEG_ATTACK, 0.002f);
    env.SetTime(ADENV_SEG_DECAY, 2.6f);
    env.SetMax(1.f);
    env.SetMin(0.f);
    filt.Init(samplerate);
    filt.SetRes(0.5f);
    filt.SetDrive(0.8f);
    filt.SetFreq(2400.0f);
    verb.Init(samplerate);
    verb.SetFeedback(0.87);
    verb.SetLpFreq(10000.0f);
}

int main(void)
{
    float samplerate;
    // Init
    hw.Init();
    hw.SetAudioBlockSize(4);
    samplerate = hw.AudioSampleRate();
    InitSynth(samplerate);
    // Start Callbacks
    hw.StartAdc();
    hw.StartAudio(audio);
    while(1) {}
}
