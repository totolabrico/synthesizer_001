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

float osclst_getaddvalue(t_note *note, int i)
{
  t_list *list;
  t_osc *osc;
  float res;
  int size;

  list = note->osc;
  res = 0;
  size = lstsize(list);
  while(list)
  {
    osc = (t_osc *)list->content;
    if (osc)
    {
      osc_setvalue(osc, note->velocity, i);
      res += osc->value / (float)size;
    }
    list = list->next;
  }
  return res;
}

void osclst_setfreq(t_list *list, float freq)
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
