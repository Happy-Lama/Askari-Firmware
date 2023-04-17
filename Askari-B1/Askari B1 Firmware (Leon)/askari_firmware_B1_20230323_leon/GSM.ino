
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void intruder_alert_call(){

/*
--function that initiates intruder alert protocol
--call the registered phone numbers to alert of a break-in or fire
--message the registered phone numbers to alert of a security breach
*/
  
  if(sim_card_error){
    Serial.println();
    Serial.println("ERROR: SIM CARD MISSING OR UNRESPONSIVE");
    Serial.println();

    gsm_serial.println("AT");
  delay(1000);

  gsm_serial.println("AT+CMGF=1");
  delay(100);
  
  gsm_serial.println("AT+CNMI=2,2,0,0,0");
  delay(100);

  //check for any sim card error
  check_gsm();
  if(check_error(sms_msg)){
    sim_card_error = 1;
  } else {
    sim_card_error = 0;
  }
  Serial.println(sms_msg);
  sms_cleanup();
    
  } else {
    Serial.println("Intruder Alert Call");
  
    for(uint8_t n = 0; n < 3; n++){
    
    //CALL

      if(phone_numbers[n] != ""){ 
        Serial.println("Calling number " + String(n) + " ..."); 
        call_number(phone_numbers[n]);
        delay(5000);
      }

    }

  //delay(5000);
  
    for(uint8_t m = 0; m < MAX_NUMBERS; m++){
    
    //TEXT

      if(phone_numbers[m] != ""){
      
        Serial.println ("Sending SMS to number " + String(m) + " ...");
        send_message(SECURITY_BREACH_SMS, phone_numbers[m]);
        delay(5000);

      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void check_sms_command(String sms_msg){
  
/*
--function that checks for sms commands in received sms message
  executes the command if the phone number is valid i.e registered
  in the system

--registered sms commands are "ARM" and "DISARM"
*/

  int number_index = sms_msg.indexOf("\"");
  String phone_number = sms_msg.substring(number_index, number_index + 15);
      
  int msg_index = sms_msg.lastIndexOf("\"");
  String msg = sms_msg.substring(msg_index + 3, sms_msg.length() - 2); 
      
  if(validate_phone_number(phone_number) && msg == ARM_SYSTEM){
    armed = 1;
    arming = 1;
    update_alarm_state();
    send_message("System Armed", phone_number);
    sms_msg = "";
  } else if(validate_phone_number(phone_number) && msg == DISARM_SYSTEM){
    armed = 0;
    update_alarm_state();
    alarm_state_cleanup();
    send_message("System Disarmed", phone_number);
    sms_msg = "";
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void send_message(String msg, String phone_number){
  
/*
--utility function to send messages to a given phone number
--uses gsm_serial
*/


  gsm_serial.println("AT+CMGS=" + phone_number + "\r");
  delay(1000);
  gsm_serial.println(msg);
  delay(1000);
  gsm_serial.println((char)26);
  delay(1000);

  check_gsm_error();
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void call_number(String number){

/*
--function used to call numbers
--takes in a number and dials that number for 20seconds then hangs up
*/


  gsm_serial.println("ATD" + number + ";\r\n");
  delay(1000);
  
  check_gsm_error();
  
  delay(20000); // wait for 20 seconds...
  gsm_serial.println("ATH");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int check_gsm(){

/*
--function used to check gsm_serial for any input from GSM
--appends the input from GSM to sms_msg variable 
--returns 1 when input is available and has been appended in sms_msg; 0 otherwise
*/

  if(gsm_serial.available() > 0){
    while(gsm_serial.available() > 0){
      char c = gsm_serial.read();
      sms_msg += c;
      delay(5);
    }
    return 1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int check_error(String msg){
  
  //checks for error messages from gsm_serial
  //if any returns 1
  //otherwise return 0

  if(msg.indexOf("ERROR") >= 0){
    return 1;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sms_cleanup(){
  sms_msg = "";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int check_gsm_error(){
  
  //checks for error messages from gsm_serial
  //if any returns 1
  //otherwise return 0

  check_gsm();
  if(check_error(sms_msg)){
    Serial.println();
    Serial.println("ERROR: NO CREDIT ON SIM CARD");
    Serial.println();
  }
  sms_cleanup();
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
