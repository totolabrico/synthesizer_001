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

typedef struct s_list
{
	void	    		*content;
	struct s_list	*next;
}	t_list;

typedef struct s_osc
{
  float *freq;
  float phase_error;
  float phase;
} t_osc;


snd_pcm_t *alsa_setup(snd_pcm_t *handle);
snd_pcm_sframes_t alsa_write(snd_pcm_t *handle, snd_pcm_sframes_t frames, int buffer[], long len);
snd_pcm_t *alsa_close(snd_pcm_t *handle, snd_pcm_sframes_t frames, long len);

void	lstadd_back(t_list **lst, t_list *new);
void	lstadd_front(t_list **lst, t_list *new);
void	lstclear(t_list **lst, void (*del)(void*));
void	lstdelone(t_list *lst, void (*del)(void*));
void	lstiter(t_list *lst, void (*f)(void *));
t_list	*lstlast(t_list *lst);
t_list	*lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*lstnew(void *content);
int	lstsize(t_list *lst);
t_list	*lstget(t_list *list, int id);

void run(float freq, float lfofreq);
int *master_write(int *buffer, t_list *list);
void inc_id(int *id, int len, int gen, int genmax);
int oscillo(int i, float freq);

void *osc_new(float *freq);
float osc_setphase_error(t_osc *osc);
float osc_getvalue(t_osc *osc, int i);
void osc_setphase(t_osc *osc);
float *osc_setfreq(t_osc *osc, float *freq);
float osc_getfreq(t_osc *osc);

#endif
