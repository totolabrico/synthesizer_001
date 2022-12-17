#include "main.h"

snd_pcm_t *pcm_setup(snd_pcm_t *handle)
{
  static char *device = "default";
  int err;

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
      printf("Playback open error: %s\n", snd_strerror(err));
      exit(EXIT_FAILURE);
  }
  if ((err = snd_pcm_set_params(handle,
                    SND_PCM_FORMAT_S32,
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

void *pcm_loop(void *addr)
{
  t_pcmsettings *pcm_settings;
  int buffer[SAMPLELEN];
  snd_pcm_sframes_t frames;

  pcm_settings = (t_pcmsettings *)addr;
  while(1)
  {
    master_write(&buffer[0], pcm_settings->notes);
    frames = pcm_write(pcm_settings->handle, frames, buffer, (long)SAMPLELEN);
  }
  pcm_settings->handle = pcm_close(pcm_settings->handle, frames, (long)SAMPLELEN);
  return addr;
}

snd_pcm_sframes_t pcm_write(snd_pcm_t *handle, snd_pcm_sframes_t frames, int buffer[], long len)
{
  frames = snd_pcm_writei(handle, buffer, len);
  if (frames < 0)
    frames = snd_pcm_recover(handle, frames, 0);
  if (frames < 0)
    printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
  return frames;
}

snd_pcm_t *pcm_close(snd_pcm_t *handle, snd_pcm_sframes_t frames, long len)
{
  int err;

  if (frames > 0 && frames < len)
    printf("Short write (expected %li, wrote %li)\n", len, frames);
  //  freq+=10;
  err = snd_pcm_drain(handle);
  if (err < 0)
    printf("snd_pcm_drain failed: %s\n", snd_strerror(err));
  snd_pcm_close(handle);
  return handle;
}
