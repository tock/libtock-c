#include <buzzer.h>
#include <stdlib.h>
#include <timer.h>

// Adapted from https://github.com/robsoncouto/arduino-songs

// Notes in the form of (note_frequency, note_delay in musical terms)
int melody[] = {

  NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,
  NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
  NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
  NOTE_E4,-4, NOTE_D4,8,  NOTE_D4,2,

  NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,
  NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
  NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
  NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2,

  NOTE_D4,4,  NOTE_D4,4,  NOTE_E4,4,  NOTE_C4,4,
  NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_C4,4,
  NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_D4,4,
  NOTE_C4,4,  NOTE_D4,4,  NOTE_G3,2,

  NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,
  NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
  NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
  NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2

};

#define TEMPO 114

int main(void) {
  if (!buzzer_exists()) {
    printf ("There is no available buzzer\n");
    return -1;
  }

  printf ("Ode of Joy\n");
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
    tone_sync(melody[note] * 3, note_duration * 0.9);
  }
}
