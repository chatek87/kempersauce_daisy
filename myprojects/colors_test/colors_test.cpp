#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;

float        blueValue = 0.0f;
static Color color1;
static Color color2;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    float redValue   = hw.knob1.GetRawFloat();
    float greenValue = hw.knob2.GetRawFloat();
    if(hw.encoder.Increment() == 1)
    {
        blueValue += 0.1f;
        if(blueValue > 1.0f)
        {
            blueValue = 1.0f;
        }
    }
    if(hw.encoder.Increment() == -1)
    {
        blueValue -= 0.1f;
        if(blueValue < 0.0f)
        {
            blueValue = 0.0f;
        }
    }

    color1.Init(redValue, greenValue, blueValue);

    hw.led1.SetColor(color1);
    // hw.led1.Set(redValue, greenValue, blueValue);


    hw.UpdateLeds();

    hw.ProcessAllControls();
    for(size_t i = 0; i < size; i++)
    {
        out[0][i] = in[0][i];
        out[1][i] = in[1][i];
    }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4); // number of samples handled per callback
    hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    while(1) {}
}
