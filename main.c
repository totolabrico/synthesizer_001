#include "main.h"

void *input_routine(void *addr)
{
  t_list *list;
  char input[1];

  list = (t_list *)addr;
  lstclear(&list, &note_clear);
  read(1, &input, 1);
  exit(0);
}

int main(void)
{
  snd_pcm_t *pcm_handle = NULL;
  snd_seq_t *seq_handle = NULL;
  t_list *notes = NULL;
  pthread_t thread_pcm, thread_midi, thread_input;
  t_pcmsettings pcm_settings;
  t_midisettings midi_settings;

  //lstadd_back(&notes, lstnew(note_new(33,127)));
  //printf("%d\n",note_getid(notes, 33));
  //notes = lstpop(notes, &note_clear, 0);
  //print_notes(notes);

  pcm_handle = pcm_setup(pcm_handle);
  seq_handle = midi_setup(seq_handle);
  system("aconnect 20 129");
  //midiconnexion_setup();
  pcm_settings.handle = pcm_handle;
  pcm_settings.notes = &notes;
  midi_settings.handle = seq_handle;
  midi_settings.notes = &notes;
  pthread_create(&thread_pcm, NULL, &pcm_loop, (void *)&pcm_settings);
  pthread_create(&thread_midi, NULL, &midi_loop, (void *)&midi_settings);
  pthread_create(&thread_input, NULL, &input_routine, NULL);
  pthread_join(thread_pcm, (void *)&pcm_settings);
  pthread_join(thread_midi, (void *)&midi_settings);
  pthread_join(thread_input, NULL);
  return 0;
}

int *master_write(int *buffer, t_list **list)
{
  float amp = 1;
  float out;
  t_note *note;
  int i = 0;
  t_list *l;
  int nbnotes = lstsize(*list);

  while(i < SAMPLELEN)
  {
    //fm = osc_getvalue(&oscs[1], i);
    //osc_setfreq(&oscs[0], (float)(freq + fm));
    //out = osc_getvalue(signal, i);
    out = 0;
    l = list[0];
    while (l)
    {
      note = (t_note *)l->content;
      out += additive_value(note->osc, i) / nbnotes * GAIN;
      l =l->next;
    }
    buffer[i] = (int)(out * amp  * INT_MAX / 2);
    i++;
  }
  return buffer;
}
