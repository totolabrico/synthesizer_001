#include "main.h"

snd_pcm_t *alsa_setup(snd_pcm_t *handle)
{
  static char *device = "default";
  int err;

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
      printf("Playback open error: %s\n", snd_strerror(err));
      exit(EXIT_FAILURE);
  }
  if ((err = snd_pcm_set_params(handle,
                    SND_PCM_FORMAT_S32,
                    //SND_PCM_FORMAT_U8,
                    SND_PCM_ACCESS_RW_INTERLEAVED,
                    1,
                    SAMPLERATE,
                    1,
                    LATENCY)) < 0) {   /* 0.5sec */
      printf("Playback open error: %s\n", snd_strerror(err));
      exit(EXIT_FAILURE);
  }
  return handle;
}

snd_pcm_sframes_t alsa_write(snd_pcm_t *handle, snd_pcm_sframes_t frames, int buffer[], long len)
{
  frames = snd_pcm_writei(handle, buffer, len);
  if (frames < 0)
    frames = snd_pcm_recover(handle, frames, 0);
  if (frames < 0)
    printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
  return frames;
}

snd_pcm_t *alsa_close(snd_pcm_t *handle, snd_pcm_sframes_t frames, long len)
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
