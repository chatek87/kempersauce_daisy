#include "daisy_pod.h"
#include "daisysp.h"
#include <stdio.h>
#include <string.h>

using namespace daisy;
using namespace daisysp;

DaisyPod   hw;
Oscillator osc[20];
Svf        filt;
bool       noteOn[20] = {false}; // Flag to track note state

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    float sig;
    for(size_t i = 0; i < size; i += 2)
    {
        for(int j = 0; j < 20; j++)
        {
            sig = osc[j].Process();
            filt.Process(sig);
            out[i] = out[i + 1] = filt.Low();
        }
    }
}

// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    switch(m.type)
    {
        case NoteOn:
        {
            NoteOnEvent p = m.AsNoteOn();
            char        buff[512];
            sprintf(buff,
                    "Note Received:\t%d\t%d\t%d\r\n",
                    m.channel,
                    m.data[0],
                    m.data[1]);
            hw.seed.usb_handle.TransmitInternal((uint8_t *)buff, strlen(buff));
            // Check if the incoming note is MIDI note 60 (middle C)
            if(p.note == 60)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[0].SetFreq(mtof(p.note));
                    osc[0].SetAmp((p.velocity / 127.0f));
                    noteOn[0] = true;
                }
                else // Note is turned off
                {
                    noteOn[0] = false;
                }
            }
            else if(p.note == 61)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[1].SetFreq(mtof(p.note));
                    osc[1].SetAmp((p.velocity / 127.0f));
                    noteOn[1] = true;
                }
                else // Note is turned off
                {
                    noteOn[1] = false;
                }
            }
            else if(p.note == 62)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[2].SetFreq(mtof(p.note));
                    osc[2].SetAmp((p.velocity / 127.0f));
                    noteOn[2] = true;
                }
                else // Note is turned off
                {
                    noteOn[2] = false;
                }
            }
            else if(p.note == 63)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[3].SetFreq(mtof(p.note));
                    osc[3].SetAmp((p.velocity / 127.0f));
                    noteOn[3] = true;
                }
                else // Note is turned off
                {
                    noteOn[3] = false;
                }
            }
            else if(p.note == 64)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[4].SetFreq(mtof(p.note));
                    osc[4].SetAmp((p.velocity / 127.0f));
                    noteOn[4] = true;
                }
                else // Note is turned off
                {
                    noteOn[4] = false;
                }
            }
            else if(p.note == 65)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[5].SetFreq(mtof(p.note));
                    osc[5].SetAmp((p.velocity / 127.0f));
                    noteOn[5] = true;
                }
                else // Note is turned off
                {
                    noteOn[5] = false;
                }
            }
            else if(p.note == 66)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[6].SetFreq(mtof(p.note));
                    osc[6].SetAmp((p.velocity / 127.0f));
                    noteOn[6] = true;
                }
                else // Note is turned off
                {
                    noteOn[6] = false;
                }
            }
            else if(p.note == 67)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[7].SetFreq(mtof(p.note));
                    osc[7].SetAmp((p.velocity / 127.0f));
                    noteOn[7] = true;
                }
                else // Note is turned off
                {
                    noteOn[7] = false;
                }
            }
            else if(p.note == 68)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[8].SetFreq(mtof(p.note));
                    osc[8].SetAmp((p.velocity / 127.0f));
                    noteOn[8] = true;
                }
                else // Note is turned off
                {
                    noteOn[8] = false;
                }
            }
            else if(p.note == 69)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[9].SetFreq(mtof(p.note));
                    osc[9].SetAmp((p.velocity / 127.0f));
                    noteOn[9] = true;
                }
                else // Note is turned off
                {
                    noteOn[9] = false;
                }
            }
            else if(p.note == 70)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[10].SetFreq(mtof(p.note));
                    osc[10].SetAmp((p.velocity / 127.0f));
                    noteOn[10] = true;
                }
                else // Note is turned off
                {
                    noteOn[10] = false;
                }
            }
            else if(p.note == 71)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[11].SetFreq(mtof(p.note));
                    osc[11].SetAmp((p.velocity / 127.0f));
                    noteOn[11] = true;
                }
                else // Note is turned off
                {
                    noteOn[11] = false;
                }
            }
            else if(p.note == 72)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[12].SetFreq(mtof(p.note));
                    osc[12].SetAmp((p.velocity / 127.0f));
                    noteOn[12] = true;
                }
                else // Note is turned off
                {
                    noteOn[12] = false;
                }
            }
            else if(p.note == 73)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[13].SetFreq(mtof(p.note));
                    osc[13].SetAmp((p.velocity / 127.0f));
                    noteOn[13] = true;
                }
                else // Note is turned off
                {
                    noteOn[13] = false;
                }
            }
            else if(p.note == 74)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[14].SetFreq(mtof(p.note));
                    osc[14].SetAmp((p.velocity / 127.0f));
                    noteOn[14] = true;
                }
                else // Note is turned off
                {
                    noteOn[14] = false;
                }
            }
            else if(p.note == 75)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[15].SetFreq(mtof(p.note));
                    osc[15].SetAmp((p.velocity / 127.0f));
                    noteOn[15] = true;
                }
                else // Note is turned off
                {
                    noteOn[15] = false;
                }
            }
            else if(p.note == 76)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[16].SetFreq(mtof(p.note));
                    osc[16].SetAmp((p.velocity / 127.0f));
                    noteOn[16] = true;
                }
                else // Note is turned off
                {
                    noteOn[16] = false;
                }
            }
            else if(p.note == 77)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[17].SetFreq(mtof(p.note));
                    osc[17].SetAmp((p.velocity / 127.0f));
                    noteOn[17] = true;
                }
                else // Note is turned off
                {
                    noteOn[17] = false;
                }
            }
            else if(p.note == 78)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[18].SetFreq(mtof(p.note));
                    osc[18].SetAmp((p.velocity / 127.0f));
                    noteOn[18] = true;
                }
                else // Note is turned off
                {
                    noteOn[18] = false;
                }
            }
            else if(p.note == 79)
            {
                if(p.velocity > 0) // Note is turned on
                {
                    // osc.SetFreq(440);
                    osc[19].SetFreq(mtof(p.note));
                    osc[19].SetAmp((p.velocity / 127.0f));
                    noteOn[19] = true;
                }
                else // Note is turned off
                {
                    noteOn[19] = false;
                }
            }
        }
        break;
        case NoteOff:
        {
            NoteOffEvent p = m.AsNoteOff();
            // Check if the incoming note is MIDI note 60 (middle C)
            if(p.note == 60 && noteOn[0])
            {
                osc[0].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[0] = false;
            }
            if(p.note == 61 && noteOn[0])
            {
                osc[1].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[1] = false;
            }
            if(p.note == 62 && noteOn[0])
            {
                osc[2].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[2] = false;
            }
            if(p.note == 63 && noteOn[0])
            {
                osc[3].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[3] = false;
            }
            if(p.note == 64 && noteOn[0])
            {
                osc[4].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[4] = false;
            }
            if(p.note == 65 && noteOn[0])
            {
                osc[5].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[5] = false;
            }
            if(p.note == 66 && noteOn[0])
            {
                osc[6].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[6] = false;
            }
            if(p.note == 67 && noteOn[0])
            {
                osc[7].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[7] = false;
            }
            if(p.note == 68 && noteOn[0])
            {
                osc[8].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[8] = false;
            }
            if(p.note == 69 && noteOn[0])
            {
                osc[9].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[9] = false;
            }
            if(p.note == 70 && noteOn[0])
            {
                osc[10].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[10] = false;
            }
            if(p.note == 71 && noteOn[0])
            {
                osc[11].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[11] = false;
            }
            if(p.note == 72 && noteOn[0])
            {
                osc[12].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[12] = false;
            }
            if(p.note == 73 && noteOn[0])
            {
                osc[13].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[13] = false;
            }
            if(p.note == 74 && noteOn[0])
            {
                osc[14].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[14] = false;
            }
            if(p.note == 75 && noteOn[0])
            {
                osc[15].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[15] = false;
            }
            if(p.note == 76 && noteOn[0])
            {
                osc[16].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[16] = false;
            }
            if(p.note == 77 && noteOn[0])
            {
                osc[17].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[17] = false;
            }
            if(p.note == 78 && noteOn[0])
            {
                osc[18].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[18] = false;
            }
            if(p.note == 79 && noteOn[0])
            {
                osc[19].SetAmp(
                    0.0f); // Turn off the sound when the middle C note is released
                noteOn[19] = false;
            }
        }
        break;
        case ControlChange:
        {
            ControlChangeEvent p = m.AsControlChange();
            switch(p.control_number)
            {
                case 1:
                    // CC 1 for cutoff.
                    filt.SetFreq(mtof((float)p.value));
                    break;
                case 2:
                    // CC 2 for res.
                    filt.SetRes(((float)p.value / 127.0f));
                    break;
                default: break;
            }
            break;
        }
        default: break;
    }
}

// Main -- Init, and Midi Handling
int main(void)
{
    // Init
    float samplerate;
    hw.Init();
    hw.SetAudioBlockSize(4);
    hw.seed.usb_handle.Init(UsbHandle::FS_INTERNAL);
    System::Delay(250);

    // Synthesis
    samplerate = hw.AudioSampleRate();
    for(int i = 0; i < 20; i++)
    {
        osc[i].Init(samplerate);
        osc[i].SetWaveform(Oscillator::WAVE_POLYBLEP_SAW);
    }

    filt.Init(samplerate);

    // Start stuff.
    hw.StartAdc();
    hw.StartAudio(AudioCallback);
    hw.midi.StartReceive();
    for(;;)
    {
        hw.midi.Listen();
        // Handle MIDI Events
        while(hw.midi.HasEvents())
        {
            HandleMidiMessage(hw.midi.PopEvent());
        }
    }
}
