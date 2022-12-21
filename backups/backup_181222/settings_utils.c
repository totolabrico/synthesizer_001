#include "main.h"

void pcm_initsettings(t_pcmsettings *settings, snd_pcm_t *handle, snd_pcm_channel_area_t *areas, short int *samples, t_list **notes, t_list **env_settings)
{
  settings->handle = handle;
  settings->areas = areas;
  settings->samples = samples;
  settings->notes = notes;
  settings->env = env_settings;
}

void midi_initsettings(t_midisettings *settings, snd_seq_t *handle, t_list **notes, t_list **env_settings)
{
    settings->handle = handle;
    settings->notes = notes;
    settings->env = env_settings;
}

t_oscsettings *oscsettings_new(float freq, float amp)
{
  t_oscsettings *s;

  s = malloc(sizeof(t_oscsettings));
  if (!s)
    return NULL;
  s->freq = freq;
  s->amp = amp;
  return s;
}

t_list *osclstsettings_new(int size)
{
  t_list *l;
  int i;

  l = NULL;
  i = 0;
  while (i < size)
  {
    lstadd_back(&l, lstnew(oscsettings_new(1,1)));
    i++;
  }
  return l;
}

void oscsettings_print(t_oscsettings *oscset)
{
  printf("osc settings -> freq:%f amp:%f\n",oscset->freq, oscset->amp);
}

void osclstsettings_print(t_list *list)
{
  t_oscsettings *oscset;

  while(list)
  {
    oscset = (t_oscsettings *)list->content;
    oscsettings_print(oscset);
    list = list->next;
  }
}

void oscsettings_set(t_oscsettings *oscset, char set, float value)
{
  if (set =='f')
    oscset->freq = value;
  else if (set == 'a')
    oscset->amp = value;
}

void osclstsettings_set(t_list *list, int id, char set, float value)
{
  t_oscsettings *oscset;
  int i;

  i = 0;
  while(list && i <= id)
  {
    if (i == id)
    {
      oscset = (t_oscsettings*)list->content;
      oscsettings_set(oscset, set, value);
    }
    list = list->next;
    i++;
  }
}

void oscsettings_clear(void *addr)
{
  t_oscsettings *oscset;

  oscset = (t_oscsettings *)addr;
  free(oscset);
}