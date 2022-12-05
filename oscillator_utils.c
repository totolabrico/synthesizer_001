#include "main.h"

void oscillator_setfreq(t_oscillator *oscillator, float freq)
{
  oscillator->freq = freq;
  oscillator->phase_error = oscillator_setphase_error(oscillator);
}

float oscillator_setphase_error(t_oscillator *oscillator)
{
  float error;

  error = (1 / (float)(SAMPLERATE / SAMPLELEN)) * oscillator->freq;
  error -= roundf(error);
  return error;
}

float oscillator_getvalue(t_oscillator *oscillator, int i)
{
  float n;
  float f;
  float p;
  float value;

  f = oscillator->freq;
  p = oscillator->phase;
  n = (float)i;
  value = sin((n / (SAMPLERATE / f) + p) * 2 * M_PI );// * INT_MAX  / 2;
  return value;
}

void oscillator_setphase(t_oscillator *oscillator)
{
  oscillator->phase += oscillator->phase_error;
  oscillator->phase -= (int)(oscillator->phase);
}
