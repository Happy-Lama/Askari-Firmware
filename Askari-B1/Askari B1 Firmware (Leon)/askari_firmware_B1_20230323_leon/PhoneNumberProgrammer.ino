
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void program_phone_numbers(String msg){
  
/*
--function for programming phone numbers received from phone number programmer device
--check validity of input
--and if valid extract the phone numbers to numbers[] array and update the numbers stored in EEPROM
--if invalid reject input
*/

  Serial.println("MSG RECEIVED");
     
  int PNC_confirm = msg.indexOf("ASKARI*PNC");
  int phone_numbers_length;

  String numbers_str = "";

          if(PNC_confirm >= 0){
          
            numbers_str = msg.substring(PNC_confirm + 10, PNC_confirm + 49);
        
            Serial.println("PHONE NUMBERs: ");
            Serial.println(numbers_str);
            phone_numbers_all = numbers_str;
            phone_numbers_length = numbers_str.length();
            Serial.println("LENGTH: ");
            Serial.println(phone_numbers_length);

            numbers_str.toCharArray(numbers, 40); //40 instead of 39 because character arrays weirdly have an extra null character

            update_EEPROM_numbers();
            delay(1000);

            Serial.println("");
            Serial.println("PHONE NUMBERS PROGRAMMED INTO MEMORY");

            }else{
              Serial.println("UNAUTHORIZED NUMBER");
            }

      
          Serial.println("E-O-C");
      
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int validate_phone_number(String phone_number){
  
/*
--function to validate phone number
--to see if it is a registered phone number of one of the owners of the askari system
*/

  for(int i = 0; i < MAX_NUMBERS; i++){
    if(phone_number == "\"" + phone_numbers[i] + "\"")
      return 1;
  }
  return 0;
}
