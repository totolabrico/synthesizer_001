#ifndef SYNTH_001_H
#define SYNTH__001_H
#include "/usr/include/alsa/asoundlib.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pthread.h>
#define SAMPLERATE 44100
#define SAMPLELEN 441
#define LATENCY 100000 // woaw 50000 seems nice
#define NBCHANNELS 1
#define GAIN 2

///////////////////// il y a tres certainement des leaks !

typedef struct s_list
{
	void	    		*content;
	struct s_list	*next;
}	t_list;

typedef struct s_osc
{
	float value;
	float freq;
	float amp;
	float phase_error;
	float phase;
	float gain;
} t_osc;

typedef struct s_note
{
	int pitch;
	int velocity;
	float amp;
	float value;
	t_list	*osc;
	t_list	*env;
}	t_note;

typedef struct s_pcmsettings
{
	snd_pcm_t *handle;
	t_list **notes;
} t_pcmsettings;

typedef struct s_midisettings
{
	snd_seq_t *handle;
	t_list **notes;
} t_midisettings;

snd_pcm_t *pcm_setup(snd_pcm_t *handle);
snd_pcm_sframes_t pcm_write(snd_pcm_t *handle, snd_pcm_sframes_t frames, int buffer[], long len);
snd_pcm_t *pcm_close(snd_pcm_t *handle, snd_pcm_sframes_t frames, long len);
void *pcm_loop(void *addr);
int *master_write(int *buffer, t_list **list);
snd_seq_t *midi_setup(snd_seq_t *seq_handle);
void *midi_loop(void *addr);
void *midi_check(snd_seq_t *seq_handle);
void midiconnexion_setup();

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
t_list *lstpop(t_list *list, void (*del)(void*), int id);

void *osc_new(float freq, float amp);
void osc_del(void *p);
float osc_setphase_error(t_osc *osc);
float osc_setvalue(t_osc *osc, int i);
void osc_setphase(t_osc *osc);
void osc_setfreq(t_osc *osc, float freq);
float osc_getfreq(t_osc *osc);
void osc_setamp(t_osc *osc, float amp);
float osc_getamp(t_osc *osc);

t_list *osclistnew(int size);
void osclst_set(t_list *list, char cmd,float val1, char op, float val2);
float osclst_getaddvalue(t_list *list, int i);
void print_osclist(t_list *list);

void *note_new(int pitch, int velocity);
void note_clear(void *addr);
void print_note(t_note *note);
void print_notes(t_list *list);
t_note *note_get(t_list * list, int pitch);
int note_getid(t_list * list, int pitch);
void note_setvelocity(t_note *note, int velocity);
float note_getvalue(t_note *note);
float note_setvalue(t_note *note, int i);
t_list *notes_purge(t_list *list);

float ptof(int pitch);
float do_op(float val1, char c, float val2);

#endif
