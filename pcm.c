#include "main.h"

snd_pcm_t *pcm_setup_handle()
{
  snd_pcm_t *handle;
  static char *device = "default";
  int err;
  //snd_pcm_format_t format = (snd_pcm_format_t)FORMAT;

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
      printf("Playback open error: %s\n", snd_strerror(err));
      exit(EXIT_FAILURE);
  }
  if ((err = snd_pcm_set_params(handle,
                    FORMAT,
                    SND_PCM_ACCESS_RW_INTERLEAVED,
                    NBCHANNELS,
                    SAMPLERATE,
                    1,
                    LATENCY)) < 0) {
      printf("Playback open error: %s\n", snd_strerror(err));
      exit(EXIT_FAILURE);
  }
  return handle;
}

short int *pcm_setup_samples()
{
  short int *res;
  res =  malloc((SAMPLELEN * NBCHANNELS * snd_pcm_format_physical_width(FORMAT)) / 8);
  return res;
}

snd_pcm_channel_area_t *pcm_setup_areas(short int *samples)
{
  snd_pcm_channel_area_t *areas;
  unsigned int chn;

  areas = calloc(NBCHANNELS, sizeof(snd_pcm_channel_area_t));
    if (areas == NULL) {
        printf("No enough memory\n");
        exit(EXIT_FAILURE);
    }
    for (chn = 0; chn < NBCHANNELS; chn++) {
        areas[chn].addr = samples;
        areas[chn].first = chn * snd_pcm_format_physical_width(FORMAT);
        areas[chn].step = NBCHANNELS * snd_pcm_format_physical_width(FORMAT);
    }
  return areas;
}

void master_write(snd_pcm_channel_area_t *areas, t_list **list)
{
  t_list *l;
  t_note *note;
  short int *samples[NBCHANNELS];
  unsigned int chn;
  float out;
  int i = 0;
  
  for (chn = 0; chn < NBCHANNELS; chn++) 
  {
    if ((areas[chn].first % 8) != 0) 
      {
        printf("areas[%u].first == %u, aborting...\n", chn, areas[chn].first);
        exit(EXIT_FAILURE);
      }
        samples[chn] = (((short int *)areas[chn].addr) + (areas[chn].first / 4));
        if ((areas[chn].step % 16) != 0) {
            printf("areas[%u].step == %u, aborting...\n", chn, areas[chn].step);
            exit(EXIT_FAILURE);
        }
  }

  while(i < SAMPLELEN * NBCHANNELS)
  {
    out = 0;
    l = *list;
    while (l)
    {
      note = (t_note *)l->content;
      out += note_setvalue(note, i);
      if (l)
        l =l->next;
    }
    for (int j = 0; j < NBCHANNELS; j++)
    {
    *(samples[j]) = (short int)(out * (float)GAIN * 32767);
    samples[j] ++;
    }
    *list = notes_purge(*list);
    i++;
  }
}

snd_pcm_sframes_t pcm_write(t_pcmsettings *settings, snd_pcm_sframes_t frames, long len)
{
  frames = snd_pcm_writei(settings->handle, settings->samples, len);
  if (frames < 0)
    frames = snd_pcm_recover(settings->handle, frames, 0);
  if (frames < 0)
    printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
  return frames;
}

void *pcm_loop(void *addr)
{
  t_pcmsettings *pcm_settings;
  snd_pcm_sframes_t frames;

  pcm_settings = (t_pcmsettings *)addr;
  while(1)
  {
    master_write(pcm_settings->areas, pcm_settings->notes);
    frames = pcm_write(pcm_settings, frames, (long)SAMPLELEN);
  }
  pcm_settings->handle = pcm_close(pcm_settings, frames, (long)SAMPLELEN);
  return addr;
}

snd_pcm_t *pcm_close(t_pcmsettings *settings, snd_pcm_sframes_t frames, long len)
{
  int err;

  if (frames > 0 && frames < len)
    printf("Short write (expected %li, wrote %li)\n", len, frames);
  //  freq+=10;
  err = snd_pcm_drain(settings->handle);
  if (err < 0)
    printf("snd_pcm_drain failed: %s\n", snd_strerror(err));

  free(settings->areas);
  free(settings->samples);
  snd_pcm_close(settings->handle);
  return settings->handle;
}
