#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;

float redValue   = 0.5f;
float greenValue = 0.5f;
bool  editRed;
bool  editGreen;

const float tolerance = 0.05f;

static Color on;
static Color off;

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    on.Init(1.0f, 1.0f, 1.0f);
    off.Init(0.0f, 0.0f, 0.0f);

    float knob1Pos = hw.knob1.GetRawFloat();
    float knob2Pos = hw.knob2.GetRawFloat();

    if(fabs(knob1Pos - redValue) <= tolerance)
    {
        hw.led1.SetColor(on);
    }
    else
    {
        hw.led1.SetColor(off);
    }

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
