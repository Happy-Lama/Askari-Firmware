#include <EEPROM.h>      //Library for EEPROM;

String phone_numbers[3] = {"", "", ""};
char number1[13]; //= "+256754319267";
char number2[13]; //= "+256702357235";
char number3[13]; //= "+256781150481";

String number1_str = "+256754319267";
String number2_str = "+256702357235";
String number3_str = "+256781150481";


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  number1_str.toCharArray(number1, 13);
  number2_str.toCharArray(number2, 13);
  number3_str.toCharArray(number3, 13);
  

  int ee_PUT_Address = 0;   //Location we want the data to be put.

  for(unsigned int i = 0; i < 13; i++){ 
    
    EEPROM.write(ee_PUT_Address, number1[i]);

    ee_PUT_Address++;
  }


  for(unsigned int i = 0; i < 13; i++){ 
    
    EEPROM.write(ee_PUT_Address, number2[i]);

    ee_PUT_Address++;
  }

  for(unsigned int i = 0; i < 13; i++){ 
    
    EEPROM.write(ee_PUT_Address, number3[i]);

    ee_PUT_Address++;
  }

  
  int ee_GET_Address = 0;                       //integer variable to act as an address pointer to the starting EEPROM memeory address being written to starting at 0
  //EEPROM.get(eeAddress, f);

  for(unsigned int i = 0; i < 13; i++){ 

    phone_numbers[0] += (char)EEPROM.read(ee_GET_Address);
    
    ee_GET_Address++;
  }

  for(unsigned int i = 0; i < 13; i++){ 

    phone_numbers[1] += (char)EEPROM.read(ee_GET_Address);
    
    ee_GET_Address++;
  }

  for(unsigned int i = 0; i < 13; i++){ 

    phone_numbers[2] += (char)EEPROM.read(ee_GET_Address);
    
    ee_GET_Address++;
  }

 
  
  Serial.println("NUMBER 1: ");
  Serial.println(phone_numbers[0]);

  Serial.println("NUMBER 2: ");
  Serial.println(phone_numbers[1]);

  Serial.println("NUMBER 3: ");
  Serial.println(phone_numbers[2]);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);

}
