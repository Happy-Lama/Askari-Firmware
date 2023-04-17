
#include <SoftwareSerial.h>
#include <avr/wdt.h>     //Library for the watch dog timer; For avoiding hanging by WDT auto reset
#include <TimerOne.h>  //Library for configuring timer 1 for Watch dog timer counting
#include <EEPROM.h>      //Library for EEPROM
 
#define ARM_SYSTEM "ARM"
#define DISARM_SYSTEM "DISARM"
#define MAX_NUMBERS 3
#define SECURITY_BREACH_SMS "SECURITY BREACH"
#define ARMED_STATE_IDX 39
#define PASSCODE_IDX 40

SoftwareSerial gsm_serial(8, 7);

uint8_t sensor_zone[7] = {A0, A1, A2, A3, A4, A5, 2};
uint8_t zone_state_indicator[7] = {13, 12, 11, 10, 9, 6, 5};
uint8_t actionRelay = 4;
uint8_t Buzzer = 3;
bool sensor_zone_state[7];

String phone_numbers[3] = {"", "", ""};
String phone_numbers_all = "";
char numbers[39];

//Variables for receiving data from serial
String data_from_serial = "";
char incoming_char;

String sms_msg = "";

//System State Variables
bool armed = 0;
bool arming = 0;
bool alarm_state = 0;

//buzzer timer variables
long int alarm_on_duration = 150;
long int alarm_off_duration = 850;
long int current_alarm = 0;
uint8_t buzzer_state = 0;
long int buzzer_start_time = 0;
const long int alarm_duration = 20000; //in milliseconds
const long int arming_duration = 10000; //in milliseconds
bool buzzer_timer_started = 0;

String current_passcode = "";

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  gsm_serial.begin(9600);
  
  pinMode(sensor_zone[0], INPUT);
  pinMode(sensor_zone[1], INPUT);
  pinMode(sensor_zone[2], INPUT);
  pinMode(sensor_zone[3], INPUT);
  pinMode(sensor_zone[4], INPUT);
  pinMode(sensor_zone[5], INPUT);
  pinMode(sensor_zone[6], INPUT);
 
  
  //define led indicator pins(anodes)
  pinMode(zone_state_indicator[0], OUTPUT);
  pinMode(zone_state_indicator[1], OUTPUT);
  pinMode(zone_state_indicator[2], OUTPUT);
  pinMode(zone_state_indicator[3], OUTPUT);
  pinMode(zone_state_indicator[4], OUTPUT);
  pinMode(zone_state_indicator[5], OUTPUT);
  pinMode(zone_state_indicator[6], OUTPUT);

  Timer1.initialize(1000000); //Initializing the timer so as to use it and also setting its period to 1,000,000 micro seconds = 1,000 milli seconds = 1 second                                 
  Timer1.attachInterrupt(resetWDT); //Interrupt for timer 1 to reset the wdt every after a period while the rest of the code operates normally
  wdt_enable(WDTO_8S); 
  
  pinMode(actionRelay, OUTPUT);

  pinMode(Buzzer, OUTPUT);
  delay(5000);

  gsm_serial.println("AT");
  delay(1000);

  gsm_serial.println("AT+CMGF=1");
  delay(100);

  gsm_serial.println("AT+CNMI=2,2,0,0,0");
  delay(100);

  //load saved phone numbers in eeprom to memory
  if(EEPROM.read(0) != 255){
    update_phone_numbers();
  }

//check for perrvious alarm state and load it in the armed variable
//for when there are unexpected power outages and the alarm was armed before
  if(EEPROM.read(ARMED_STATE_IDX) != 255){
    armed = EEPROM.read(ARMED_STATE_IDX);
    Serial.println("ALARM SYSTEM ARMED STATE RETRIEVED");
  }

//check if there is a passcode stored in the eeprom and load it in current_passcode variable
  if(EEPROM.read(PASSCODE_IDX) != 255){
    for(uint8_t i = 0; i < 4; i++){
      current_passcode += (char)EEPROM.read(PASSCODE_IDX + i);
    }
    Serial.println("PASSCODE RETRIEVED");
    Serial.println(current_passcode);
  }

}

void loop() {

   //system functions decision tree
   
  if(check_gsm() && sms_msg != ""){
    check_sms_command(sms_msg);
  }
    
  if(check_serial() && data_from_serial != ""){
    if(data_from_serial.indexOf("ASKARI*PNC") >= 0){
      program_phone_numbers(data_from_serial);
      update_phone_numbers();
    } else if(data_from_serial.length() <= 10){
      check_serial_command(data_from_serial);
    }
    data_from_serial = "";
  }
  
  if(armed){
    if(arming){
      arm_system();
    }
    armed_state();
  }

  if(alarm_state){
    sound_alarm();
  }

}
