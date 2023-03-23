void programPhoneNumbers(String msg){

  Serial.println("MSG RECEIVED");
     
  int PNC_confirm = msg.indexOf("ASKARI*PNC");
  int phone_numbers_length;

  String numbers_str = "";

          if(PNC_confirm >= 0){
          
            numbers_str = msg.substring(PNC_confirm + 10, PNC_confirm + 49);
        
            Serial.println("PHONE NUMBERs: ");
            Serial.println(numbers_str);
            //numbers_str = numbers_str.trim();
            phone_numbers_length = numbers_str.length();
            Serial.println("LENGTH: ");
            Serial.println(phone_numbers_length);

            numbers_str.toCharArray(numbers, 40); //40 instead of 39 because character arrays weridly have an extra null character

            //update_EEPROM_numbers();

            }else{
              Serial.println("UNAUTHORIZED NUMBER");
              }

      
          Serial.println("E-O-C");
      
          //GSM_SERIAL.flush();
      
      
}
