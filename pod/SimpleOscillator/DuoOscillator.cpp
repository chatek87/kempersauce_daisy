#include "daisy_pod.h"
#include "daisysp.h"

#define NUM_WAVEFORMS 4

using namespace daisy;
using namespace daisysp;

DaisyPod   hw;
Oscillator osc1, osc2;       // Define two oscillators
Parameter  p_freq1, p_freq2; // Define parameters for both oscillators

uint8_t waveforms[NUM_WAVEFORMS] = {
    Oscillator::WAVE_SIN,
    Oscillator::WAVE_TRI,
    Oscillator::WAVE_POLYBLEP_SAW,
    Oscillator::WAVE_POLYBLEP_SQUARE,
};

static float freq1, freq2;
float        sig1, sig2;
static int   waveform1, waveform2, octave;

static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    hw.ProcessDigitalControls();

    waveform1 += hw.encoder.Increment();
    waveform1 = DSY_CLAMP(waveform1, 0, NUM_WAVEFORMS);
    osc1.SetWaveform(waveforms[waveform1]);

    waveform2 += hw.encoder.Increment();
    waveform2 = DSY_CLAMP(waveform2, 0, NUM_WAVEFORMS);
    osc2.SetWaveform(waveforms[waveform2]);

    if(hw.button2.RisingEdge())
        octave++;
    if(hw.button1.RisingEdge())
        octave--;
    octave = DSY_CLAMP(octave, 0, 4);


    // Convert MIDI to frequency and multiply by octave size for both oscillators
    freq1 = mtof(p_freq1.Process() + (octave * 12));
    osc1.SetFreq(freq1);

    freq2 = mtof(p_freq2.Process() + (octave * 12));
    osc2.SetFreq(freq2);

    // Audio Loop
    for(size_t i = 0; i < size; i += 2)
    {
        // Process both oscillators and mix their output
        sig1 = osc1.Process();
        sig2 = osc2.Process();
        out[i]
            = sig1
              + sig2; // You can adjust how you want to mix the two oscillators
        out[i + 1]
            = sig1
              + sig2; // You can adjust how you want to mix the two oscillators
    }
}

void InitSynth(float samplerate)
{
    // Init freq Parameter to knob1 using MIDI note numbers for both oscillators
    // min 10, max 127, curve linear
    p_freq1.Init(hw.knob1, 0, 127, Parameter::LINEAR);
    p_freq2.Init(hw.knob2, 0, 127, Parameter::LINEAR);

    osc1.Init(samplerate);
    osc1.SetAmp(1.f);

    osc2.Init(samplerate);
    osc2.SetAmp(1.f);

    waveform1 = 0;
    waveform2 = 0;
    octave    = 0;
}

int main(void)
{
    float samplerate;

    // Init everything
    hw.Init();
    hw.SetAudioBlockSize(4);
    samplerate = hw.AudioSampleRate();
    InitSynth(samplerate);

    // start callbacks
    hw.StartAdc();
    hw.StartAudio(AudioCallback);

    while(1) {}
}
