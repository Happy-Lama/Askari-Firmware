
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void resetWDT(){
//Function (ISR for timer 1) to reset the wdt
  wdt_reset();         //actual wdt reset function.
} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_to_serial(){
//function to print data to serial monitor
  while(gsm_serial.available()!=0){
    Serial.write(gsm_serial.read()); 
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int check_serial(){

//checks serial for any incoming data and stores data in a variable
//data_from_serial -> variable that stores data received from serial
  if(Serial.available() > 0){
    while(Serial.available() > 0){
      incoming_char = (char)Serial.read();
      data_from_serial += incoming_char;
      delay(10);
    }
    return 1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void intruder_alert(){

/*
--function called when the askari is in a triggered state
--Sounds the alarm and activates the GSM to call the registered phone numbers
*/

  Serial.println("SIREN ON");
  digitalWrite(actionRelay, 1);

  intruder_alert_call();
  
  digitalWrite(Buzzer, 1);
  digitalWrite(Buzzer, 0);
  
  digitalWrite(actionRelay, 0);
  Serial.println("SIREN OFF");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggle_buzzer(int on_time, int off_time){
//function to toggle buzzer for specified times given
  if(buzzer_state == 0){
    if(millis() - current_alarm >= off_time){
      digitalWrite(Buzzer, !buzzer_state);
      current_alarm = millis();
      buzzer_state = !buzzer_state;
      number_of_buzzes++;
    }
  }
  if(buzzer_state == 1){
    if(millis() - current_alarm >= on_time){
      digitalWrite(Buzzer, !buzzer_state);
      current_alarm = millis();
      buzzer_state = !buzzer_state;
      number_of_buzzes++;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arm_system(){
  //function to arm askari system
  toggle_buzzer(200, 800);
  
  if(number_of_buzzes == 20){
    number_of_buzzes = 0;
    toggle_buzzer(600, 0);
    buzzer_state = 0;
    arming = 0;
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void armed_state(){

/*
--check sensor states and update indicators
--change alarm state when any zone is triggered
*/

  for(uint8_t j = 0; j < 7; j++){
    sensor_state[j] = digitalRead(sensor[j]);
    digitalWrite(indicator[j], !sensor_state[j]);
    if(sensor_state[j] == 0)
      alarm_state = 1;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sound_alarm(){
  
/*
--function to sound buzzer and alarm when
--any sensor is triggered
*/
 
  toggle_buzzer(alarm_state_delay_on, alarm_state_delay_off = 850);
  
  if(number_of_buzzes == 22){
    number_of_buzzes = 0;
    alarm_state = 0;
    delay(1000);
    intruder_alert();
  }
}
