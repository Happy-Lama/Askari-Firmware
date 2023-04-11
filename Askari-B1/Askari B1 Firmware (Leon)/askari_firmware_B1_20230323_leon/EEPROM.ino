
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_phone_numbers(){

/*
--function used to update phone numbers in the phone number array
--reads phone numbers that are stored in the EEPROM and appends them to the phone numbers array
*/

  Serial.println("UPDATING PHONE NUMBERS...");

  phone_numbers[0] = "";
  phone_numbers[1] = "";
  phone_numbers[2] = "";

  int ee_GET_Address = 0;    //integer variable to act as an address pointer to the starting EEPROM memory address being written to starting at 0


  Serial.println("phone numbers all:");
  Serial.println(String(numbers));
  
  for(uint8_t i = 0; i < 13; i++){ 

    phone_numbers[0] += (char)EEPROM.read(ee_GET_Address);
    
    ee_GET_Address++;

  }

  Serial.println("phone number 1:");
  Serial.println(phone_numbers[0]);

  for(uint8_t i = 0; i < 13; i++){ 

    phone_numbers[1] += (char)EEPROM.read(ee_GET_Address);
    
    ee_GET_Address++;
  
  }

  Serial.println("phone number 2:");
  Serial.println(phone_numbers[1]);

  for(uint8_t i = 0; i < 13; i++){ 

    phone_numbers[2] += (char)EEPROM.read(ee_GET_Address);
    
    ee_GET_Address++;

  }

  Serial.println("phone number 3:");
  Serial.println(phone_numbers[2]);

  

  Serial.println("DONE UPDATING PHONE NUMBERS!");
  
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_EEPROM_numbers(){

/*
--function that updates stored phone numbers in EEPROM
--updates from the numbers[] array that is filled when new phone numbers are programmed into EEPROM
*/

  int starting_address = 0;

  for(uint8_t i = 0; i < 39; i++){ 
              
    EEPROM.update((starting_address + i), numbers[i]);
    delay(100);
          
   }
}

void update_alarm_state(){
  EEPROM.update(ARMED_STATE_IDX, armed);
  delay(100);
}
