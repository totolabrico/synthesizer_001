#include "main.h"

int *master_write(int *buffer, t_oscillator *oscillator)
{
  for (int i = 0; i < SAMPLELEN; i++)
  {
    buffer[i] = oscillator_getvalue(oscillator, i);
  }
  oscillator_setphase(oscillator);
  return buffer;
}

void inc_id(int *id, int len, int gen, int genmax)
{
  id[gen] += 1;
  if (id[gen] > len)
  {
    id[gen] = 0;
    if(gen < genmax - 1)
      inc_id(id, len, gen + 1, genmax);
  }
}
