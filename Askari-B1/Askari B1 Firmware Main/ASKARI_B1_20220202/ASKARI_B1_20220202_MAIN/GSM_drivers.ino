void GSM_ACTION(int l){

  /////////////////////////////////
  //update_phone_numbers();
  
  Serial.println("GSM_ACTION");
  
  for(uint8_t pp = 0; pp < 3; pp++){
    
    //CALL

    if(phone_numbers[pp] != "0000000000000"){
      
      Serial.println("Calling number " + String(pp) + " ..."); 
      //delay(1000);
  
      GSM_SERIAL.println("ATD" + phone_numbers[pp] + ";\r\n");
      delay(1000);
      toSerial();
      
      delay(20000); // wait for 20 seconds...
      GSM_SERIAL.println("ATH"); //hang up
      delay(1000);
      toSerial();
  
      delay(5000);

    }else{
      delay(5000);
    }

  }

  //delay(5000);
  
  for(uint8_t qq = 0; qq < 3; qq++){
    
    //TEXT

    if(phone_numbers[qq] != "0000000000000"){
      
      Serial.println ("Sending SMS to number " + String(qq) + " ...");
      GSM_SERIAL.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);
      toSerial();
      //Serial.println ("Set SMS Number");
      GSM_SERIAL.println("AT+CMGS=\"" + phone_numbers[qq] + "\"\r"); //Mobile phone number to send message
      delay(1000);
      toSerial();
      String SMS = "SECURITY BREACH DETECTED AT SENSOR " + String(l+1);
      GSM_SERIAL.println(SMS);
      delay(100);
      GSM_SERIAL.println((char)26);// ASCII code of CTRL+Z
      delay(5000);

    }else{
      delay(7000);
    }
  }

}
