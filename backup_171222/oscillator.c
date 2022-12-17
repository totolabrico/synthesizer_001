#include "main.h"

void *osc_new(float freq, float amp)
{
  t_osc *osc;
  osc = malloc(sizeof(t_osc));
  osc_setfreq(osc, freq);
  osc_setamp(osc, amp);
  osc->value = 0.0;
  osc->phase = 0;
  return (void*)osc;
}

void osc_del(void *p)
{
    t_osc *osc;

    osc = (t_osc *)p;
    free(osc);
}

float osc_getfreq(t_osc *osc)
{
  return osc->freq;
}

void osc_setfreq(t_osc *osc, float freq)
{
  osc->freq = freq;
  if (freq > 30)
    osc->gain = 10 / freq;
  else
    osc->gain = 1;
  osc->phase_error = osc_setphase_error(osc);
}

float osc_getamp(t_osc *osc)
{
  return osc->amp;
}

void osc_setamp(t_osc *osc, float amp)
{
    osc->amp = amp;
}

float osc_setphase_error(t_osc *osc)
{
  float error;

  error = (1 / (float)(SAMPLERATE / SAMPLELEN)) * osc->freq;
  error -= roundf(error);
  return error;
}

float osc_setvalue(t_osc *osc, int i)
{
  float n = (float)i;

  osc->value = sin((n / (SAMPLERATE / osc->freq) + osc->phase) * 2 * M_PI) * osc->amp * osc->gain;
  //printf("i : %d, freq : %f, amp : %f, phase : %f, value: %f\n", i, f, a ,p, value);
  if (i == SAMPLELEN - 1)
    osc_setphase(osc);
  return osc->value;
}

void osc_setphase(t_osc *osc)
{
  osc->phase += osc->phase_error;
  osc->phase -= (int)(osc->phase);
}