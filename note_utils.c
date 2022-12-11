#include "main.h"

void *note_new(int pitch, int velocity)
{
  t_note * note;
  t_list * osc;

  note = malloc(sizeof(t_note));
  note->pitch = pitch;
  note->velocity = velocity;
  osc = osclistnew((float)ptof(pitch), (float)velocity / 127, 5);
  note->osc = osc;
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

float note_getvalue(t_note *note, int i)
{
  return osclst_getaddvalue(note, i);
}

t_list *notes_purge(t_list *list)
{
  t_list *l;
  t_note *note;
  int i;

  l = list;
  //printf("note purge : %d\n", lstsize(l));
  i = 0;
  while (l)
  {
    note = (t_note *)l->content;
    if(note->velocity == 0)
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
