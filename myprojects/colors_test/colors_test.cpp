#include "daisy_pod.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisyPod hw;

float redValue1   = 0.0f;
float redValue2   = 0.0f;
float greenValue1 = 0.0f;
float greenValue2 = 0.0f;
float blueValue1  = 0.0f;
float blueValue2  = 0.0f;

const float tolerance = 0.05f;

bool editRed;
bool editGreen;

static Color color1;
static Color color2;
bool         ledSwitcher = false; // Flag to toggle between LED1 and LED2


void ToggleLEDSelection()
{
    if(hw.encoder.FallingEdge())
    {
        ledSwitcher
            = !ledSwitcher; // Toggle the LED selection when encoder button is pressed and released.
    }
}

void AdjustBlue(float *blueValue)
{
    if(hw.encoder.Increment() == 1)
    {
        *blueValue += 0.1f;
        if(*blueValue > 1.0f)
        {
            *blueValue = 1.0f;
        }
    }
    if(hw.encoder.Increment() == -1)
    {
        *blueValue -= 0.1f;
        if(*blueValue < 0.0f)
        {
            *blueValue = 0.0f;
        }
    }
}

void AudioCallback(AudioHandle::InputBuffer  in,
                   AudioHandle::OutputBuffer out,
                   size_t                    size)
{
    ToggleLEDSelection();

    float knob1Pos = hw.knob1.GetRawFloat();
    float knob2Pos = hw.knob2.GetRawFloat();

    if(!ledSwitcher)
    {
        // Adjusting LED1
		// AdjustRed(&redValue1);
        if(fabs(knob1Pos - redValue1) <= tolerance)
        {
            editRed = true;
        }
        if(editRed)
        {
            redValue1 = hw.knob1.GetRawFloat();
        }

        if(fabs(knob1Pos - greenValue1) <= tolerance)
        {
            editGreen = true;
        }
        if(editGreen)
        {
            greenValue1 = hw.knob2.GetRawFloat();
        }

        AdjustBlue(&blueValue1);

        color1.Init(redValue1, greenValue1, blueValue1);

        hw.led1.SetColor(color1);
    }
    else
    {
		// AdjustRed();
        if(fabs(knob2Pos - redValue2) <= tolerance)
        {
            editRed = true;
        }
        if(editRed)
        {
            redValue2 = hw.knob1.GetRawFloat();
        }

        if(fabs(knob1Pos - greenValue1) <= tolerance)
        {
            editGreen = true;
        }
        if(editGreen)
        {
            // here is the problem. GetRawFloat() only returns a value the instant it is called.
            // and in the instant it is called, knob2 is in the same position it was
            // when ledSwitcher was false (previous state, editing led1)
            greenValue2 = hw.knob2.GetRawFloat();
        }

        AdjustBlue(&blueValue2);


        color2.Init(redValue2, greenValue2, blueValue2);

        hw.led2.SetColor(color2);
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
