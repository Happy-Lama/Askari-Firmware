//Function (ISR for timer 1) to reset the wdt
void resetWDT(){
  wdt_reset();         //actual wdt reset function.
}
