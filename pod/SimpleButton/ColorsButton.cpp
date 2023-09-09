#include "daisy_pod.h"

using namespace daisy;

DaisyPod hw;

enum LedColor {
    RED,
    GREEN,
    BLUE,
    YELLOW
};

LedColor currentColor = RED;
bool ledOn = true; // Variable to track LED state

void SetLedColor(LedColor color, bool on) {
    if (on) {
        switch (color) {
            case RED:
                hw.led1.Set(true, false, false); // Red
                hw.led2.Set(true, false, false); // Red
                break;
            case GREEN:
                hw.led1.Set(false, true, false); // Green
                hw.led2.Set(false, true, false); // Green
                break;
            case BLUE:
                hw.led1.Set(false, false, true); // Blue
                hw.led2.Set(false, false, true); // Blue
                break;
            case YELLOW:
                hw.led1.Set(true, true, false); // Yellow
                hw.led2.Set(true, true, false); // Yellow
                break;
        }
    } else {
        hw.led1.Set(false, false, false); // Turn off LED1
        hw.led2.Set(false, false, false); // Turn off LED2
    }
    hw.UpdateLeds();
}

int main(void)
{
    hw.Init();

    while(1)
    {
        hw.ProcessDigitalControls();

        if (ledOn && hw.button1.RisingEdge())
        {
            // Cycle to the next LED color
            currentColor = static_cast<LedColor>((static_cast<int>(currentColor) + 1) % 4);
            SetLedColor(currentColor, ledOn);
        }

        if(hw.button2.RisingEdge())
        {
            // Toggle LED state
            ledOn = !ledOn;
            SetLedColor(currentColor, ledOn);
        }

        hw.UpdateLeds();
    }
}
