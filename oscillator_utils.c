#include "main.h"

void *osc_new(float freq, float amp)
{
  t_osc *osc;
  osc = malloc(sizeof(t_osc));
  //osc->freq =
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
  osc->phase_error = osc_setphase_error(osc);
}

float osc_getamp(t_osc *osc)
{
  return osc->amp;
}

void osc_setamp(t_osc *osc, float amp)
{
    osc->amp = amp;
    //printf("set value : %f\n", amp);
}

float osc_setphase_error(t_osc *osc)
{
  float error;

  error = (1 / (float)(SAMPLERATE / SAMPLELEN)) * osc->freq;
  error -= roundf(error);
  return error;
}

float osc_setvalue(t_osc *osc, int velocity, int i)
{
  float f = osc->freq;
  float p = osc->phase;
  float a = osc->amp;
  float n = (float)i;
  /*
  if (velocity == 0)
  {
    a = a - (float)a * 0.05;
    osc_setamp(osc, a);
  }*/
  osc->value = sin((n / (SAMPLERATE / f) + p) * 2 * M_PI ) * a;
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
