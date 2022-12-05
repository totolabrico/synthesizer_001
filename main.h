#ifndef SYNTH_001_H
#define SYNTH__001_H
#include "/usr/include/alsa/asoundlib.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#define SAMPLERATE 44100           /* stream rate */
#define SAMPLELEN 441       /* stream rate */
#define LATENCY 500000
#define NBCHANNELS 1

typedef struct s_oscillator
{
  float freq;
  float phase_error;
  float phase;
} t_oscillator;

snd_pcm_t *alsa_setup(snd_pcm_t *handle);
snd_pcm_sframes_t alsa_write(snd_pcm_t *handle, snd_pcm_sframes_t frames, int buffer[], long len);
snd_pcm_t *alsa_close(snd_pcm_t *handle, snd_pcm_sframes_t frames, long len);

void run(float freq, float lfofreq);
int *master_write(int *buffer, t_oscillator *oscillators, int nbosc);
void inc_id(int *id, int len, int gen, int genmax);
int oscillo(int i, float freq);

float oscillator_setphase_error(t_oscillator *oscillator);
float oscillator_getvalue(t_oscillator *oscillator, int i);
void oscillator_setphase(t_oscillator *oscillator);
void oscillator_setfreq(t_oscillator *oscillator, float freq);


#endif
