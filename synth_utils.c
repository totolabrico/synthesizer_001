#include "main.h"

t_list *osclistnew(float freq, float amp, int size)
{
  t_list *list;
  int i;

  list = NULL;
  if (size == 0)
    return list;
  i = 0;
  amp = amp / 2;
  while (i < size)
  {
    lstadd_back(&list, lstnew(osc_new(freq, amp)));
    freq = freq * 2;
    amp = amp / 4;
    i++;
  }
  return list;
}

void print_osclist(t_list *list)
{
  t_osc *osc;
  while(list)
  {
    osc = (t_osc *)list->content;
    printf("osc freq: %f, amp:%f\n", osc_getfreq(osc), osc_getamp(osc));
    list = list->next;
  }
}

void *note_new(float freq, float amp)
{
  t_note * note;
  t_list * osc;
  osc = osclistnew(freq, amp, 10);
  note = malloc(sizeof(note));
  note->osc = osc;
  return (void *)note;
}

void print_note(t_note *note)
{
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

void note_clear(t_note *note)
{
  t_list *osc;

  osc = (t_list *)note->osc;
  lstclear(&osc, osc_del);
  free(note);
}

void note_setfreq(t_list *list, float freq)
{
  t_osc *osc;

  while(list)
  {
    osc = (t_osc*)list->content;
    osc_setfreq(osc, freq);
    freq = freq * 2;
    list = list->next;
  }
}

float additive_value(t_list *list, int i)
{
  t_osc *osc;
  float res;
  int size;

  res = 0;
  size = lstsize(list);
  while(list)
  {
    osc = (t_osc *)list->content;
    res += osc_getvalue(osc, i) / (float)size;
    list = list->next;
  }
  return res;
}
