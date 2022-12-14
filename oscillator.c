#include "main.h"

void *osc_new(float freq, float amp)
{
  t_osc *osc;
  osc = malloc(sizeof(t_osc));
  osc_setfreq(osc, freq);
  osc_setamp(osc, amp);
  osc->phase = 0;
  return (void*)osc;
}

void osc_del(void *p)
{
    t_osc *osc;

    osc = (t_osc *)p;
    free(osc);
}

void osc_setrate(t_osc *osc, float freq)
{
  osc->rate = 1 /((SAMPLERATE * NBCHANNELS) / freq);
}

float osc_getfreq(t_osc *osc)
{
  return osc->freq;
}

void osc_setfreq(t_osc *osc, float freq)
{
  osc->freq = freq;
  osc->phase_error = osc_setphase_error(osc);
  osc_setgain(osc, freq);
  osc_setrate(osc, freq);
}

void osc_setgain(t_osc *osc, float freq)
{
  if (freq > 30)
    osc->gain = 10 / freq;
  else
    osc->gain = 1;
  osc_setvol(osc);
}

float osc_getamp(t_osc *osc)
{
  return osc->amp;
}

void osc_setamp(t_osc *osc, float amp)
{
    osc->amp = amp;
    osc_setvol(osc);
}

void osc_setvol(t_osc *osc)
{
  osc->vol = osc->amp * osc->gain;
}

float osc_setphase_error(t_osc *osc)
{
  float error;

  error = (1 / (float)(SAMPLERATE / SAMPLELEN)) * osc->freq;
  error -= roundf(error);
  return error;
}

float osc_getvalue(t_osc *osc, int i)
{
  float n = (float)i;
  float value;

  value = sin((n * osc->rate + osc->phase) * TWOPI) * osc->vol;
  //value = sin(1);
  //printf("i : %d, freq : %f, amp : %f, phase : %f, value: %f\n", i, f, a ,p, value);
  
  if (i == SAMPLELEN * NBCHANNELS - 1)
    osc_setphase(osc);
    
  return value;
}

void osc_setphase(t_osc *osc)
{
  osc->phase += osc->phase_error;
  osc->phase -= (int)(osc->phase);
}