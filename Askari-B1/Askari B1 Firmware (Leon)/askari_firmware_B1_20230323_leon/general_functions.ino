
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void resetWDT(){
  
//Function (ISR for timer 1) to reset the wdt

  wdt_reset();         //actual wdt reset function.
} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void print_to_serial(){
  
//function to print data from GSM to serial monitor

  while(gsm_serial.available()!=0){
    Serial.write(gsm_serial.read()); 
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int check_serial(){
  
/*
--checks serial for any incoming data and store the data
--in data_from_serial
*/
  
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

void check_serial_command(String msg){

  //function resolves whether serial input command sent is to input a new passcode
  //or to arm system i.e the passcode itself
  
  if(msg.startsWith("*")){
    update_serial_passcode(msg);
  } else {
    check_serial_passcode(msg);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_serial_passcode(String msg){

/*
--function to update the current serial passcode
  checks if there is already a default passcode
  and updates accordingly.
--To make a new passcode, a msg starting with *0000* should be sent
  if no passcode already registered and a msg starting with *current_passcode*
  if there is a set passcode already.
*/

  if(current_passcode == ""){
    int new_password_code = msg.indexOf("*0000*");
    if(new_password_code >= 0){
      current_passcode = msg.substring(new_password_code + 6);
    } else {
      Serial.println("INVALID PASSCODE");
    }
  } else if(current_passcode != ""){
    int new_password_code = msg.indexOf("*" + current_passcode + "*");
    if(new_password_code >= 0){
      current_passcode = msg.substring(new_password_code + current_passcode.length() + 2);
    }else{
      Serial.println("INVALID PASSCODE");
    }
  }

  if(current_passcode.length() != 4){
    Serial.println("INVALID PASSCODE LENGTH");
  } else {
    Serial.println("SUCCESS");
    update_passcode_eeprom(current_passcode);
  }
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void check_serial_passcode(String msg){

  //responds to input passcode from keypad
  
  if(current_passcode == ""){
    Serial.println("INVALID. NO PASSCODE SET");
  } else {
    if(msg == current_passcode){
      armed = !armed;
      update_alarm_state();
      if(armed == 1){
        arming = 1;
      } else {
        alarm_state_cleanup();
      }
      Serial.println("SUCCESS");
    }
  }
}

void intruder_alert(){

/*
--function called when the askari is in a triggered state
--Sounds the alarm and activates the GSM to call the registered phone numbers
*/

  Serial.println("SIREN ON");
  digitalWrite(actionRelay, 1);

  intruder_alert_call();
  
//  digitalWrite(actionRelay, 0);
//  Serial.println("SIREN OFF");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggle_buzzer(int on_time, int off_time){
  
//function to toggle buzzer for specified times given

  if(buzzer_state == 0){
    if(millis() - current_alarm >= off_time){
      digitalWrite(Buzzer, !buzzer_state);
      current_alarm = millis();
      buzzer_state = !buzzer_state;
    }
  }
  if(buzzer_state == 1){
    if(millis() - current_alarm >= on_time){
      digitalWrite(Buzzer, !buzzer_state);
      current_alarm = millis();
      buzzer_state = !buzzer_state;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arm_system(){
  
  //function to start system arm sequence
  
  toggle_buzzer(200, 800);

  int buzzer_action_time_up = buzzer_timer(arming_duration, 1);
  
  if(buzzer_action_time_up){
    reset_buzzer_timer();
    buzzer_timer_started = 0; 
    toggle_buzzer(600, 0);
    arming = 0;
    buzzer_state = 0;
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void armed_state(){

/*
--check sensor states and update indicators
--change alarm state when any zone is triggered
*/

  for(uint8_t j = 0; j < 7; j++){
    sensor_zone_state[j] = digitalRead(sensor_zone[j]);
    digitalWrite(zone_state_indicator[j], !sensor_zone_state[j]);
    if(sensor_zone_state[j] == 0)
      alarm_state = 1;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sound_alarm(){
  
/*
--function to sound buzzer and alarm when
--any sensor is triggered
*/

//  buzzer_timer_start();
  
  toggle_buzzer(alarm_on_duration, alarm_off_duration);

  int buzzer_action_time_up = buzzer_timer(alarm_duration, 1);
  
  if(buzzer_action_time_up){
    reset_buzzer_timer();
    buzzer_timer_started = 0;
    alarm_state = 0;
    delay(1000);
    intruder_alert();
  }
  
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void reset_buzzer_timer(){
  
  //resets the buzzer timer
   
  buzzer_start_time = millis();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void alarm_state_cleanup(){
  
  // resets buzzer timer
  //sets buzzer state to off (0)
  //turns off siren in case it is on 
  
  buzzer_state = 0;
  reset_buzzer_timer();
  buzzer_timer_started = 0;
  alarm_state = 0;
  digitalWrite(Buzzer, 0);
  digitalWrite(actionRelay, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int buzzer_timer(long int duration, bool start_timer){
  
  //timer for buzzer action duration
  //returns 1 if duration is over
  //else returns 0
  
  if(start_timer){
    if(!buzzer_timer_started){
      buzzer_timer_started = 1;
      reset_buzzer_timer();
    }

    if(millis() - buzzer_start_time >= duration){
      return 1;
    } else {
      return 0;
    }
  }
  
}
