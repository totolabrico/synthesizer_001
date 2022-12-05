#include "main.h"

int main(int argc, char **argv)
{
  float lfofreq;

  lfofreq = 0;
  if (argc >= 2 && argc <= 3)
  {
    if (argc == 3)
      lfofreq = strtof(argv[2], NULL);
    run(strtof(argv[1], NULL), lfofreq);
  }
  else
    printf("please enter frequency\n");
  return 0;
}

void run(float freq, float lfofreq)
{
  int buffer[SAMPLELEN];            /* some random data */
  snd_pcm_t *handle;
  snd_pcm_sframes_t frames;
  t_oscillator oscillator[2];

  handle = NULL;
  handle = alsa_setup(handle);
  oscillator_setfreq(&oscillator[0], freq);
  oscillator_setfreq(&oscillator[1], lfofreq);

  while(1)
  {
    master_write(&buffer[0], &oscillator[0], 2);
    frames = alsa_write(handle, frames, buffer, (long)SAMPLELEN);
  }
  handle = alsa_close(handle, frames, (long)SAMPLELEN);
}
