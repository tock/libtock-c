#include <buzzer.h>
#include <stdlib.h>
#include <timer.h>

// Notes in the form of (note_frequency, note_delay in musical terms)
int melody[] = {

NOTE_C5,1, // Hail
NOTE_GS4,2, NOTE_AS4,2, // to the
NOTE_C5,2, NOTE_GS4,2, // victors
NOTE_AS4,2, NOTE_C5,2, // valiant
NOTE_CS5,1, // Hail
NOTE_AS4,2, NOTE_C5,2, // to the
NOTE_CS5,2, NOTE_AS4,2, // conqring
NOTE_C5,2, NOTE_CS5,2, // heroes
NOTE_DS5,1, // Hail
NOTE_F5,-2, NOTE_C5,8, // hail to
NOTE_C5,2, NOTE_CS5,2, // michi
NOTE_GS4,2, NOTE_AS4,2, // gan, the
NOTE_C5,1, // leader
NOTE_AS4,2, NOTE_GS4,2, // ers and
NOTE_DS5,1, // best

REST,4, REST,2,

NOTE_C5,1, // Hail
NOTE_GS4,2, NOTE_AS4,2, // to the
NOTE_C5,2, NOTE_GS4,2, // victors
NOTE_AS4,2, NOTE_C5,2, // valiant
NOTE_CS5,1, // Hail
NOTE_AS4,2, NOTE_C5,2, // to the
NOTE_CS5,2, NOTE_AS4,2, // conqring
NOTE_C5,2, NOTE_CS5,2, // heroes
NOTE_DS5,1, // Hail
NOTE_F5,-2, NOTE_C5,8, // hail to
NOTE_C5,2, NOTE_CS5,2, // michi
NOTE_GS4,2, NOTE_AS4,2, // gan, the
NOTE_C5,2, NOTE_DS5,2, // champions
NOTE_C5,-2, NOTE_AS4,4, // of the
NOTE_GS4,1, // west

};

#define TEMPO 300

int main(void) {
  if (!buzzer_exists()) {
    printf ("There is no available buzzer\n");
    return -1;
  }

  printf ("Rousing chorus of, the victors!\n");
  int notes     = sizeof(melody) / sizeof(melody[0]) / 2;
  int wholenote = (60000 * 4) / TEMPO;
  for (int note = 0; note < notes * 2; note = note + 2) {

    // calculates the duration of each note
    int divider       = melody[note + 1];
    int note_duration = 0;
    if (divider > 0) {
      // regular note, just proceed
      note_duration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      note_duration  = (wholenote) / abs(divider);
      note_duration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone_sync(melody[note]*3, note_duration * 0.9);
  }
}
