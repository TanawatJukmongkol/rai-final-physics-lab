#include "./alarm.hpp"

void  alarm(bool cb(void))
{
  while (cb())
  {
    tone(BUZZER_PIN, NOTE_A3, 1000);
    delay(1000);
    tone(BUZZER_PIN, NOTE_D5, 150);
    delay(20);
    tone(BUZZER_PIN, NOTE_DS5, 150);
    delay(20);
    tone(BUZZER_PIN, NOTE_E5, 150);
    delay(20);
    tone(BUZZER_PIN, NOTE_F5, 500);
    delay(500);
  }
}
