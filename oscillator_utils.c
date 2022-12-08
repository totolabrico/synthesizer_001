#include "main.h"

void *osc_new(float *freq)
{
  t_osc *osc;

  osc = malloc(sizeof(t_osc));
  osc->freq = osc_setfreq(osc, freq);
  return (void*)osc;
}

float osc_getfreq(t_osc *osc)
{
  return *osc->freq;
}

float *osc_setfreq(t_osc *osc, float *freq)
{
  osc->freq = freq;
  osc->phase_error = osc_setphase_error(osc);
  return freq;
}

float osc_setphase_error(t_osc *osc)
{
  float error;

  error = (1 / (float)(SAMPLERATE / SAMPLELEN)) * *osc->freq;
  error -= roundf(error);
  return error;
}

float osc_getvalue(t_osc *osc, int i)
{
  float n;
  float f;
  float p;
  float value;

  f = *osc->freq;
  p = osc->phase;
  n = (float)i;
  value = sin((n / (SAMPLERATE / f) + p) * 2 * M_PI );// * INT_MAX  / 2;
  if (i == SAMPLELEN - 1)
    osc_setphase(osc);
  return value;
}

void osc_setphase(t_osc *osc)
{
  osc->phase += osc->phase_error;
  osc->phase -= (int)(osc->phase);
}
