#include "main.h"

t_list *osclistnew(int size)
{
  t_list *list;
  int i;
  //float a;

  list = NULL;
  if (size == 0)
    return list;
  i = 0;
  //amp = amp / 2;
  while (i < size)
  {
    //a = 50 / freq;
  //  printf("%f \n", a);
    lstadd_back(&list, lstnew(osc_new(0 ,0)));
    //freq = freq * 2;
    //amp = amp / 4;
    i++;
  }

  return list;
}

void osclst_set(t_list *list, char cmd, float val1, char op, float val2)
{
  t_osc *osc;

  while(list)
  {
    osc = (t_osc*)list->content;
    if (cmd == 'f')
      osc_setfreq(osc, val1);
    else if (cmd == 'a')
      osc_setamp(osc, val1);
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

