#include "main.h"

void *note_new(int pitch, int velocity, t_list **env)
{
  t_note *note;

  note = malloc(sizeof(t_note));
  note->pitch = pitch;
  note_setvelocity(note, velocity);
  note->value = 0;
  note->osc = osclstnew(2);
  osclst_set(note->osc, &osc_setfreq, (float)ptof(pitch), '*', 2);
  osclst_set(note->osc, &osc_setamp, (float)velocity / 127, '/', 2);
  note_newenv(note, env);
  return (void *)note;
}

void print_note(t_note *note)
{
  printf("note : pitch = %d, velocity = %d\n", note->pitch, note->velocity);
  print_osclist(note->osc);
  print_osclist(note->env);
}

void note_setvelocity(t_note *note, int velocity)
{
  note->velocity = velocity;
  if (velocity != 0)
    note->amp = (float)(velocity) / 0.127;
}

void note_setenv(t_note *note, t_list **settings, int id)
{
  t_list *l;
  t_list *s;
  t_osc *osc;
  t_oscsettings *oscset;

  l = note->env;
  s = *settings;
  osc = (t_osc *)lstget(l, id)->content;
  oscset = (t_oscsettings *)lstget(s, id)->content;
  osc_setfreq(osc, oscset->freq);
  osc_setamp(osc, oscset->amp);
}

void note_newenv(t_note *note, t_list **settings)
{
  t_list *l;
  int i;

  l = *settings;
  note->env = osclstnew(lstsize(*settings));
  i = 0;
  while (l)
  {
    note_setenv(note, settings, i);
    l = l->next;
    i++;
  }
}

void note_clear(void *addr)
{
  t_note *note;
  t_list *lst;

  note = (t_note *)addr;
  lst = (t_list *)note->osc;
  lstclear(&lst, osc_del);
  lst = (t_list *)note->env;
  lstclear(&lst, osc_del);
  free(note);
}

t_note *note_get(t_list * list, int pitch)
{
  t_note *note;
  int i;

  i = 0;
  while(list)
  {
    note = (t_note *)list->content;
    if (note->pitch == pitch)
      return (note);
    list = list->next;
    i++;
  }
  return (NULL);
}

int note_getid(t_list * list, int pitch)
{
  t_note *note;
  int i;

  i = 0;
  while(list)
  {
    note = (t_note *)list->content;
    if (note->pitch == pitch)
      return (i);
    list = list->next;
    i++;
  }
  return (-1);
}

float note_setvalue(t_note *note, int i)
{
  float out;
  float env;

  out = osclst_getaddvalue(note->osc, i);
  env = osclst_getaddvalue(note->env, i);
  note->value = out * note->amp *env;
  return note->value;
}

float note_getvalue(t_note *note)
{
  return note->value;
}
