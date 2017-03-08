//for segway board:

void initAllPins()
{
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  
  pinMode(M_RX_SOFT, INPUT);
  pinMode(M_TX_SOFT, OUTPUT);

  pinMode(ADC, INPUT);

  pinMode(SPEAKER, OUTPUT);

  pinMode(SWITCH_TOP, INPUT);
  pinMode(SWITCH_BOT, INPUT);
}
