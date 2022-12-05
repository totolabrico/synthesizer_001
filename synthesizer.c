#include "main.h"

int *master_write(int *buffer, t_oscillator *oscillators, int nbosc)
{
  float freq;
  float amp;
  for (int i = 0; i < SAMPLELEN; i++)
  {
    freq = oscillator_getvalue(&oscillators[0], i) * INT_MAX / 2;
    if(oscillators[1].freq == 0)
      amp = 1;
    else
      amp = oscillator_getvalue(&oscillators[1], i);
    buffer[i] = (int)(freq * amp);
  }
  for (int j = 0; j < nbosc; j++)
    oscillator_setphase(&oscillators[j]);
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
