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

void *note_new(int pitch, int velocity)
{
  t_note * note;
  t_list * osc;

  note = malloc(sizeof(t_note));
  note->pitch = pitch;
  note->velocity = velocity;
  osc = osclistnew((float)ptof(pitch), (float)velocity / 127, 10);
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

void note_clear(void *addr)
{
  t_note *note;
  t_list *osc;

  note = (t_note *)addr;
  osc = (t_list *)note->osc;
  lstclear(&osc, osc_del);
  free(note);
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
