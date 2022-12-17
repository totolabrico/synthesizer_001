#include "main.h"

t_list *osclstnew(int size)
{
  t_list *list;
  int i;

  list = NULL;
  if (size == 0)
    return list;
  i = 0;
  while (i < size)
  {
    lstadd_back(&list, lstnew(osc_new(0 ,0)));
    i++;
  }
  return list;
}

void osclst_set(t_list *list, void (*f)(t_osc *, float), float val1, char op, float val2)
{
  t_osc *osc;

  while(list)
  {
    osc = (t_osc*)list->content;
    f(osc, val1);
    val1 = do_op(val1, op, val2);
    list = list->next;
  }
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

float osclst_getaddvalue(t_list *list, int i)
{
  t_osc *osc;
  float res;
  int size;

  res = 0;
  size = lstsize(list);
  while(list)
  {
    osc = (t_osc *)list->content;
    if (osc)
    {
      osc_setvalue(osc, i);
      res += osc->value / (float)size;
    }
    list = list->next;
  }
  return res;
}
