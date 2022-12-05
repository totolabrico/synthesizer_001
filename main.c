#include "main.h"

int main(int argc, char **argv)
{
  if (argc == 2)
    run(strtof(argv[1], NULL));
  else
    printf("please enter frequency\n");
  return 0;
}

void run(float freq)
{
  int buffer[SAMPLELEN];              /* some random data */
  snd_pcm_t *handle;
  snd_pcm_sframes_t frames;
  long len;
  t_oscillator oscillator;

  len = SAMPLELEN;
  handle = alsa_setup(handle);
  oscillator_setfreq(&oscillator, freq);
  while(1)
  {
    master_write(&buffer[0], &oscillator);
    frames = alsa_write(handle, frames, buffer, len);
  }
  handle = alsa_close(handle, frames, len);
}
