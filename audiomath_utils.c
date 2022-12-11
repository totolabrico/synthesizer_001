#include "main.h"

float ptof(int pitch)
{
float semitone_ratio;
float c0; /* for frequency of MIDI Note 0 */
float c6; /* for frequency of Middle C */
float frequency; /* . . . which we want to find, */
/* calculate required numbers */
semitone_ratio = pow(2, 1/12.0); /* approx. 1.0594631 */
/* find Middle C, three semitones above low A = 220 */
c6 = 440.0 * pow(semitone_ratio, 3);
/* MIDI Note 0 is C, 5 octaves below Middle C */
c0 = c6 * pow(0.5, 6);
/* calculate a frequency for a given MIDI Note Number */
/* C# above A = 440 */
frequency = c0 * pow(semitone_ratio, pitch);
//printf("MIDI Note %d has frequency %f\n", midinote,frequency);
return frequency;
}
