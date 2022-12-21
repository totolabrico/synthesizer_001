#include "main.h"

void notes_setenv(t_list **notes, t_list **settings, int id)
{
  t_list *l;
  t_note *note;

  l = *notes;
  while (l)
  {
    note = (t_note*)l->content;
      note_setenv(note, settings, id);
    l = l->next; 
  }
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