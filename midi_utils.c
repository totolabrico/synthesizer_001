#include "main.h"

snd_seq_t *midi_setup(snd_seq_t *seq_handle)
{
	snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0); // Create a alsa client.
	snd_seq_set_client_name(seq_handle, "MIDI_SYNTH_001"); // Give you client a name (not expicity needed, will work without it).
	snd_seq_create_simple_port(seq_handle, "in", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);// Create a MIDI in port.
  return seq_handle;
}

void midi_loop(snd_seq_t *seq_handle)
{
		snd_seq_event_t *ev = NULL;
		snd_seq_event_input(seq_handle, &ev); // Read incomming MIDI event (blocking, the program will wait for an event)
    printf("midi_loop\n");
		switch(ev->type){	// switch on the type of the MIDI event
			case SND_SEQ_EVENT_NOTEON: // play a note
				if(ev->data.note.velocity != 0){
					printf("note on:  Channel[%d] Note[%3d] velocity(%3d)\n",
					ev->data.note.channel+1, ev->data.note.note, ev->data.note.velocity);
				}
      break;
			// NOTE: no break; makes a fallthrough to case below. Used because velocity 0 is same as a note off
			case SND_SEQ_EVENT_NOTEOFF: // turn off a note
				printf("note off: Channel[%d] Note[%3d] velocity(%3d)\n",
				ev->data.note.channel+1, ev->data.note.note, ev->data.note.velocity);
			break;

			case SND_SEQ_EVENT_PGMCHANGE: // change sound
				printf("program change: on Channel[%d] to no.[%2x] \n",
				ev->data.note.channel+1,ev->data.control.value+1);
			break;
			case SND_SEQ_EVENT_PITCHBEND: // pich bend
				printf("pitch bend: Channel[%d] value(%5d) \n",
				ev->data.note.channel+1, ev->data.control.value);
			break;

			// MIDI System Real-Time Messages
			case SND_SEQ_EVENT_START:	printf("START\n");	break;
			case SND_SEQ_EVENT_CONTINUE:	printf("CONTINUE\n");	break;
			case SND_SEQ_EVENT_STOP:	printf("STOP\n");	break;
			case SND_SEQ_EVENT_CLOCK:	/*printf("CLOCK\n");*/	break; // 24 clocks = 1 quarter note
			case SND_SEQ_EVENT_TICK:	printf("TICK\n");	break;

			// MIDI controll change messages
			// https://www.alsa-project.org/alsa-doc/alsa-lib/group___m_i_d_i___controllers.html
			case SND_SEQ_EVENT_CONTROLLER: // Control change MIDI message
				switch(ev->data.control.param){ // switch on cc number according to MIDI specification. See MIDI spec. to implement more cases
					case MIDI_CTL_MSB_BANK: // 0: // CC0: Bank select
						printf("Bank select: on Channel[%d] to no.[%d]\n",
						ev->data.note.channel+1,ev->data.control.value);
					break;
					case MIDI_CTL_MSB_MODWHEEL: // Modulation Wheel
						printf("modulation wheel: on Channel[%d] Value(%d)\n",
						ev->data.note.channel+1,ev->data.control.value);
					break;
					// Channel Mode Messages
					case 120: // CC120: Mute channel, turn off notes immediatly
					case 123: // CC123: All MIDI Notes OFF
						printf("Panic! All notes turned off, realeases hanged notes\n");
					break;
					default: // knob change or not recognized (there is more to decode here according to the MIDI spec.)
						printf("control change:  Channel[%d] CC[%d] Value(%3d)\n",
						ev->data.note.channel+1, ev->data.control.param, ev->data.control.value);
					break;
				}
			break;
			default:
        break;
				//printf("unrecognized message: event type no. %d\n",ev->type);
			break;
		}
}
