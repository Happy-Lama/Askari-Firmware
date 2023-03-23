#include <SoftwareSerial.h>

#define ARM_SYSTEM "ARM"
#define DISARM_SYSTEM "DISARM" 
#define MAX_NUMBERS 3

SoftwareSerial gsm_serial(8, 7);

String sms_msg = "";

int armed = 0;

String phonenumbers[] = {"+256785343043", "+256785343043", "+256785343043"};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gsm_serial.begin(9600);
  delay(5000);

  gsm_serial.println("AT");
  delay(1000);

  gsm_serial.println("AT+CMGF=1");
  delay(100);

  gsm_serial.println("AT+CNMI=2,2,0,0,0");
  delay(100);
//  Serial.println(ARM_SYSTEM);
}

void loop() {
  
  if(gsm_serial.available() > 0){
    while(gsm_serial.available() > 0){
      char c = gsm_serial.read();
      sms_msg += c;
      delay(10);
    }
    Serial.println(sms_msg);
//    if(sms_msg.indexOf("ERROR") != -1){
//      Serial.println("GSM_ERROR");
//    }
    if(sms_msg != ""){
      
      int number_index = sms_msg.indexOf("\"");
      String phone_number = sms_msg.substring(number_index, number_index + 15);
//      Serial.println(phone_number);
      
      int msg_index = sms_msg.lastIndexOf("\"");
      String msg = sms_msg.substring(msg_index + 3, sms_msg.length() - 2); 
      
      if(validate_phone_number(phone_number) && msg == ARM_SYSTEM){
        armed = 1;
        sendMessage("System Armed", phone_number);
      }
      if(validate_phone_number(phone_number) && msg == DISARM_SYSTEM){
        armed = 0;
        sendMessage("System Disarmed", phone_number);
      }
    }
    sms_msg = "";
  }  
}

int validate_phone_number(String phone_number){
  for(int i = 0; i < MAX_NUMBERS; i++){
    if(phone_number == "\"" + phonenumbers[i] + "\"")
      return 1;
  }
  return 0;
}

void sendMessage(String msg, String phone_number){

  gsm_serial.println("AT+CMGS=" + phone_number + "\r");
  delay(1000);
  gsm_serial.println(msg);
  delay(1000);
  gsm_serial.println((char)26);
  delay(1000);

//  gsm_serial.println("AT+CNMI=2,2,0,0,0");
}
