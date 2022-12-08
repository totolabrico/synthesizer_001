#include "main.h"

int main(int argc, char **argv)
{
  float lfofreq;

  lfofreq = 0;
  if (argc >= 2 && argc <= 3)
  {
    if (argc == 3)
      lfofreq = strtof(argv[2], NULL);
    //printf("%f\n", lfofreq);
    run(strtof(argv[1], NULL), lfofreq);
  }
  else
    printf("please enter frequency\n");
  return 0;
}

void run(float Freq, float Lfofreq)
{
  int buffer[SAMPLELEN];            /* some random data */
  snd_pcm_t *handle;
  snd_pcm_sframes_t frames;
  t_list *list;
  float freq = Freq;
  float lfofreq = Lfofreq;
  list = NULL;
  list = lstnew(osc_new(&freq));
  lstadd_back(&list, lstnew(osc_new(&lfofreq)));
  //printf("%d\n", lstsize(list));
  handle = NULL;
  handle = alsa_setup(handle);
  while(1)
  {
    master_write(&buffer[0], list);
    frames = alsa_write(handle, frames, buffer, (long)SAMPLELEN);
  }
  handle = alsa_close(handle, frames, (long)SAMPLELEN);
}

int *master_write(int *buffer, t_list *list)
{
  t_list *el =lstget(list, 0);
  t_osc *signal = (t_osc *)el->content;
  t_list *el2 =lstget(list, 1);
  t_osc *env = (t_osc *)el2->content;
  //float *freq = env->freq;
  //printf("%f\n", *freq);
  float amp = 0.5;
  float out;
  for (int i = 0; i < SAMPLELEN; i++)
  {
    //fm = osc_getvalue(&oscs[1], i);
    //osc_setfreq(&oscs[0], (float)(freq + fm));
    out = osc_getvalue(signal, i);
    amp = osc_getvalue(env, i);
    buffer[i] = (int)(out * amp  * INT_MAX / 2);
  }
  return buffer;
}
