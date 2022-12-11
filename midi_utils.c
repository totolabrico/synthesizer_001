#include "main.h"

snd_seq_t *midi_setup(snd_seq_t *seq_handle)
{
	snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0); // Create a alsa client.
	snd_seq_set_client_name(seq_handle, "MIDI_SYNTH_001"); // Give you client a name (not expicity needed, will work without it).
	snd_seq_create_simple_port(seq_handle, "in", SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_APPLICATION);// Create a MIDI in port.
  return seq_handle;
}

void *midi_loop(void *addr)
{
	t_midisettings *midi_settings;
	snd_seq_event_t *ev;
	t_list **notes;
	t_note *note;

	ev = NULL;
	midi_settings = (t_midisettings *)addr;
	notes = midi_settings->notes;
	while(1)
	{
		snd_seq_event_input(midi_settings->handle, &ev);
		if (ev->type == SND_SEQ_EVENT_NOTEON) // 6
		{
			int pitch = ev->data.note.note;
			int velocity = ev->data.note.velocity;
			note = note_get(*notes, pitch);
			if (velocity != 0)
				lstadd_back(notes, lstnew(note_new(pitch,velocity)));
			else
			{
				note_setvelocity(note, 0);
			}
		}
	}
	return (addr);
}
