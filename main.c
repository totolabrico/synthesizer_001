#include "main.h"

int main(void)
{
  snd_pcm_t *pcm_handle = NULL;
  snd_seq_t *seq_handle = NULL;
  t_list *notes = NULL;

  lstadd_back(&notes, lstnew(note_new(201,1)));
  lstadd_back(&notes, lstnew(note_new(100,1)));
  //note_clear();
  //print_notes(notes);
  pcm_handle = pcm_setup(pcm_handle);
  seq_handle = midi_setup(seq_handle);
  pcm_loop(pcm_handle, notes);
  //midi_loop(seq_handle);
  return 0;
}

void pcm_loop(snd_pcm_t *handle, t_list *notes)
{
  int buffer[SAMPLELEN];
  snd_pcm_sframes_t frames;

  while(1)
  {
    master_write(&buffer[0], notes);
    frames = pcm_write(handle, frames, buffer, (long)SAMPLELEN);
  }
  handle = pcm_close(handle, frames, (long)SAMPLELEN);
}

int *master_write(int *buffer, t_list *list)
{
  float amp = 1;
  float out;
  t_note *note;
  int i = 0;
  t_list *l;
  int nbnotes = lstsize(list);

  while(i < SAMPLELEN)
  {
    //fm = osc_getvalue(&oscs[1], i);
    //osc_setfreq(&oscs[0], (float)(freq + fm));
    //out = osc_getvalue(signal, i);
    out = 0;
    l = list;
    while (l)
    {
      note = (t_note *)l->content;
      out += additive_value(note->osc, i) / nbnotes;
      l =l->next;
    }
    buffer[i] = (int)(out * amp  * INT_MAX / 2);
    i++;
  }
  return buffer;
}
