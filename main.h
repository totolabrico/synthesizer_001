#ifndef SYNTH_001_H
#define SYNTH__001_H
#include "/usr/include/alsa/asoundlib.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
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
  float freq;
	float amp;
  float phase_error;
  float phase;
} t_osc;

typedef struct s_note
{
	t_list	*osc;
}	t_note;

snd_pcm_t *pcm_setup(snd_pcm_t *handle);
snd_pcm_sframes_t pcm_write(snd_pcm_t *handle, snd_pcm_sframes_t frames, int buffer[], long len);
snd_pcm_t *pcm_close(snd_pcm_t *handle, snd_pcm_sframes_t frames, long len);
void pcm_loop(snd_pcm_t *handle, t_list *notes);
int *master_write(int *buffer, t_list *list);
snd_seq_t *midi_setup(snd_seq_t *seq_handle);
void midi_loop(snd_seq_t *seq_handle);

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

void *osc_new(float freq, float amp);
void osc_del(void *p);
float osc_setphase_error(t_osc *osc);
float osc_getvalue(t_osc *osc, int i);
void osc_setphase(t_osc *osc);
void osc_setfreq(t_osc *osc, float freq);
float osc_getfreq(t_osc *osc);
void osc_setamp(t_osc *osc, float amp);
float osc_getamp(t_osc *osc);

float additive_value(t_list *list, int i);
t_list *osclistnew(float freq, float amp, int size);
void *note_new(float freq, float amp);
void note_clear(t_note *note);
void note_setfreq(t_list *list, float freq);
void print_osclist(t_list *list);
void print_note(t_note *note);
void print_notes(t_list *list);

#endif
