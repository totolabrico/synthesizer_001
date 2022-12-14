#include "main.h"

void *note_new(int pitch, int velocity)
{
  t_note *note;

  note = malloc(sizeof(t_note));
  note->pitch = pitch;
  note_setvelocity(note, velocity);
  //note->velocity = velocity;
  //note->amp = (float)(velocity) / 127;
  note->value = 0;
  note->osc = osclistnew(2);
  osclst_set(note->osc, 'f', (float)ptof(pitch), '*', 2);
  osclst_set(note->osc, 'a', (float)velocity / 127, '/', 2);
  note->env = osclistnew(2);
  osclst_set(note->env, 'f', 2, '+', 0.2);
  osclst_set(note->env, 'a', 1, '+', 0);
  return (void *)note;
}

void print_note(t_note *note)
{
  printf("note : pitch = %d, velocity = %d\n", note->pitch, note->velocity);
  print_osclist(note->osc);
}

void print_notes(t_list *list)
{
  t_note *note;
  while (list)
  {
    note = (t_note *)list->content;
    print_note(note);
    list = list->next;
  }
}

void note_setvelocity(t_note *note, int velocity)
{
  note->velocity = velocity;
  if (velocity != 0)
    note->amp = (float)(velocity) / 127;
}

void note_clear(void *addr)
{
  t_note *note;
  t_list *osc;

  note = (t_note *)addr;
  osc = (t_list *)note->osc;
  lstclear(&osc, osc_del);
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
  //  printf("pitch : %d\n", note->pitch);
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
  //  printf("pitch : %d\n", note->pitch);
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
 // printf("set value : %f\n", note->value);
  return note->value;
}

float note_getvalue(t_note *note)
{
  return note->value;
}

t_list *notes_purge(t_list *list)
{
  t_list *l;
  t_note *note;
  int i;
  float v;

  l = list;
  i = 0;  
  while (l)
  {
    note = (t_note *)l->content;
    v = note->value;
    if(note->velocity == 0 && v < 0.001 && v > -0.001)
    {
        list = lstpop(list, &note_clear, i);
        i = 0;
        l = list;
    }
    else
    {
      l = l->next;
      i++;
    }
  }
  return list;
}
