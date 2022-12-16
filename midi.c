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
	t_list *sustained = NULL;
	int sustain = 0;
	t_note *note;

	ev = NULL;
	midi_settings = (t_midisettings *)addr;
	notes = midi_settings->notes;
	while(1)
	{
		snd_seq_event_input(midi_settings->handle, &ev);
		if (ev->type == SND_SEQ_EVENT_CONTROLLER) // 10
		{	
			if (ev->data.control.param == 64)
			{
				sustain = ev->data.control.value;
				if (sustain == 0)
					end_sustain(&sustained);
			}
			else
				controller_set(notes, midi_settings->env, ev->data.control.param, ev->data.control.value);
		}
		if (ev->type == SND_SEQ_EVENT_NOTEON) // 6
		{
			int pitch = ev->data.note.note;
			int velocity = ev->data.note.velocity;
			note = note_get(*notes, pitch);
			if(!note)
				lstadd_back(notes, lstnew(note_new(pitch,velocity, midi_settings->env)));
			else
			{
				if (velocity != 0)
					note_setvelocity(note, velocity);
				else if (sustain == 0)
					note_setvelocity(note, 0);
				else
					lstadd_back(&sustained, lstnew((void *)note));
			}
		}
	}
	return (addr);
}

void end_sustain(t_list **notes)
{
	t_list *l;
	t_note *note;
	l = *notes;

	while (l)
	{
		note = (t_note *)l->content;
		note_setvelocity(note, 0);
		l = l->next;
	}
	lstclear(notes, NULL);
}

void controller_set(t_list **notes, t_list **envset, int param, int value)
{
	float f;
	int x, y;

	printf("control change: CC[%d] Value(%3d)\n",param, value);
	f = (float)value / 127 * 4;
	param = param - 16;
	x = param /4;
	y = param % 4;
	if (x == 0 && y < 3)
	{
		osclstsettings_set(*envset, y, 'f', f);
		notes_setenv(notes, envset, y);
	}
	else if (x == 1 && y < 3)
	{
		osclstsettings_set(*envset, y, 'a', f);
		notes_setenv(notes, envset, y);
	}
}
