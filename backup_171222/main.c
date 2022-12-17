#include "main.h"

void *input_routine(void *addr)
{
  t_list *list;
  char input[1];

  list = (t_list *)addr;
  read(1, &input, 1);
  lstclear(&list, &oscsettings_clear);
  exit(0);
}

int main(int argc, char **argv)
{
    if (argc == 2)
      run(argv[1]);
    else
      run("default");
    return 0;
}

void run(char *machine_name)
{
  snd_pcm_t *pcm_handle = NULL;
  snd_seq_t *seq_handle = NULL;
  t_list *notes = NULL;
  pthread_t thread_pcm, thread_midi, thread_input;
  t_pcmsettings pcm_settings;
  t_midisettings midi_settings;
  t_list *env_settings;
  /*
  lstadd_back(&notes, lstnew(note_new(33,0)));
  notes = lstpop(notes, &note_clear, 0);
  notes = notes_purge(notes);
  print_notes(notes);
  */
  pcm_handle = pcm_setup(pcm_handle);
  seq_handle = midi_setup(seq_handle);
  if (strcmp(machine_name, "pi") == 0)
  {
    system("aconnect 20 128");
    system("aconnect 24 128");
  }
  else
  {
    system("aconnect 20 129");
    system("aconnect 24 129");
  }
  //midiconnexion_setup();
  env_settings = osclstsettings_new(3);
  osclstsettings_set(env_settings, 0, 'f', 1.2);
  osclstsettings_set(env_settings, 1, 'f', 0.3);
  //osclstsettings_print(env_settings);
  pcm_initsettings(&pcm_settings, pcm_handle, &notes, &env_settings);
  midi_initsettings(&midi_settings, seq_handle, &notes, &env_settings);
  pthread_create(&thread_pcm, NULL, &pcm_loop, (void *)&pcm_settings);
  pthread_create(&thread_midi, NULL, &midi_loop, (void *)&midi_settings);
  pthread_create(&thread_input, NULL, &input_routine, (void *)env_settings);
  pthread_join(thread_pcm, (void *)&pcm_settings);
  pthread_join(thread_midi, (void *)&midi_settings);
  pthread_join(thread_input, NULL);
}

int *master_write(int *buffer, t_list **list)
{
  t_list *l;
  t_note *note;
  float out;
  int i = 0;

  while(i < SAMPLELEN)
  {
    out = 0;
    l = *list;
    while (l)
    {
      note = (t_note *)l->content;
      out += note_setvalue(note, i);
      if (l)
        l =l->next;
    }
    buffer[i] = (int)(out * (float)GAIN * INT_MAX / 2);
    *list = notes_purge(*list);
    i++;
  }
  return buffer;
}
