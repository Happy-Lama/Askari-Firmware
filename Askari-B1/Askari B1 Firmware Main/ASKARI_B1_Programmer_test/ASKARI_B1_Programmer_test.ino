#include <SoftwareSerial.h>
#include <EEPROM.h>      //Library for EEPROM;

SoftwareSerial GSM_SERIAL(8, 7);

int sensor[7] = {A0, A1, A2, A3, A4, A5, 2};
int indicator[7] = {13, 12, 11, 10, 9, 6, 5};
int actionRelay = 4;
//int GSM_switch = 13;
int Buzzer = 3;
int i, j, k, l, m;
bool sensor_state[7];

String phone_numbers[3] = {"", "", ""};
String phone_numbers_all = "";
char numbers[39];


String everything = "";
char incoming_char;


void setup() {
  
  Serial.begin(9600);
  GSM_SERIAL.begin(9600);
  
  pinMode(sensor[0], INPUT);
  pinMode(sensor[1], INPUT);
  pinMode(sensor[2], INPUT);
  pinMode(sensor[3], INPUT);
  pinMode(sensor[4], INPUT);
  pinMode(sensor[5], INPUT);
  pinMode(sensor[6], INPUT);
 
  
  //define led indicator pins(anodes)
  pinMode(indicator[0], OUTPUT);
  pinMode(indicator[1], OUTPUT);
  pinMode(indicator[2], OUTPUT);
  pinMode(indicator[3], OUTPUT);
  pinMode(indicator[4], OUTPUT);
  pinMode(indicator[5], OUTPUT);
  pinMode(indicator[6], OUTPUT);
  
  //pinMode(actionRelay, OUTPUT);

  pinMode(Buzzer, OUTPUT);

  /////LED_ON(1);

  //INITIAL BUZZING
  for(k = 0; k < 10; k++){
    Serial.println("INITIAL BUZZING");
    digitalWrite(Buzzer, 1);
    delay(120);
    digitalWrite(Buzzer, 0);
    delay(880);
    
    }

  digitalWrite(Buzzer, 1);
  delay(600);
  digitalWrite(Buzzer, 0);

}

void loop() {
  // put your main code here, to run repeatedly:

  //CHECK SENSOR STATES
  for(j = 0; j < 7; j++){

    sensor_state[j] = digitalRead(sensor[j]);
    
    }
  
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
  Serial.print("SENSOR 1: ");
  Serial.println(sensor_state[0]);
  Serial.print("SENSOR 2: ");
  Serial.println(sensor_state[1]);
  Serial.print("SENSOR 3: ");
  Serial.println(sensor_state[2]);
  Serial.print("SENSOR 4: ");
  Serial.println(sensor_state[3]);
  Serial.print("SENSOR 5: ");
  Serial.println(sensor_state[4]);
  Serial.print("SENSOR 6: ");
  Serial.println(sensor_state[5]);
  Serial.print("SENSOR 7: ");
  Serial.println(sensor_state[6]);
 

  //UPDATE INDICATORS
  for(i = 0; i < 7; i++){

    digitalWrite(indicator[i], !sensor_state[i]);
    
    }



  for(l = 0; l < 7; l++){

  if(sensor_state[l] == 0){

    for(m = 0; m < 11; m++){
      
      Serial.println("ALARM BUZZING");
      digitalWrite(Buzzer, 1);
      delay(150);
      digitalWrite(Buzzer, 0);
      delay(850);
    
    }

    //ACTION(l);

    break;

    }

  }
   
  delay(3000);
  

  //CHECK SERIAL
  everything = "";
  
  if(Serial.available() >0){
       
      while(Serial.available() >0){
        
        incoming_char = Serial.read(); //Get the character from the cellular serial port.
        Serial.print(incoming_char); //Print the incoming character to the terminal.
        everything = everything + incoming_char;
        
        }

        Serial.println("EVERYTHING");
        Serial.println(everything);
    
        if(everything != ""){
          
          programPhoneNumbers(everything);

          Serial.println("");
          Serial.println("PHONE NUMBERS PROGRAMMED");
          
        }
        
          

  }

}
