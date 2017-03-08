//functions for speaker actions:

void beginInitTone()
{
  #ifndef DISABLE_AUDIO
  tone(SPEAKER, 300, 300);
  delay(150);
  tone(SPEAKER, 750, 300);
  delay(150);
  tone(SPEAKER, 1500, 300);
  #endif
}

void initTone()
{
  #ifndef DISABLE_AUDIO
  tone(SPEAKER,2000,100);
  delay(200);
  tone(SPEAKER,2000,100);
  delay(200);
  tone(SPEAKER,2000,100);
  #endif
}

