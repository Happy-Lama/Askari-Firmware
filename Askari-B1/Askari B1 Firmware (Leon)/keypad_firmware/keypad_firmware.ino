#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns

const byte led_indicator_pins[] = {13, 12, 11, 10, 9};

const byte state_indicator_led = led_indicator_pins[0];  //led is on for inputing new password off otherwise

String data_from_serial = ""; //stores data received from serial

String data_from_keypad = ""; //stores key presses from keypad

byte inputing_new_passcode = 0;
 
char Keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte row_pins[ROWS] = {5,4,3,2}; //connect to the row pinouts of the keypad
byte col_pins[COLS] = {8,7,6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(Keys), row_pins, col_pins, ROWS, COLS); 

byte indicator_led = 1;

void setup(){
  Serial.begin(9600);

  //set all the led_indicator_pins to OUTPUT
  for(byte i = 0; i < 5; i++){
    pinMode(led_indicator_pins[i], OUTPUT);
  }
  
}
  
void loop(){

  if(check_serial()){
    resolve_serial_msg();
    data_from_serial = "";  
  }
  
  char key_pressed = customKeypad.getKey();
  
  if (key_pressed){
    Serial.println(key_pressed);
    if(key_pressed == '#'){
      byte valid_input = check_data(data_from_keypad);
      if(valid_input){
        Serial.print(data_from_keypad);
      }
      keypad_input_cleanup();
    } else if(key_pressed == '*'){
      data_from_keypad += key_pressed;
      inputing_new_passcode = 1;
      digitalWrite(state_indicator_led, 1);
    } else if(inputing_new_passcode && (data_from_keypad.lastIndexOf("*") == 5)){
      data_from_keypad += key_pressed;
      update_led_indicators();
      indicator_led++;  
    } else {
      data_from_keypad += key_pressed;
      update_led_indicators();
      indicator_led++;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int check_serial(){

  //check serial for incoming data and save the data to 
  //data_from_serial variable
  
  if(Serial.available()){
    while(Serial.available()){
      char data = Serial.read();
      data_from_serial += data;
    }
    return 1;
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void keypad_input_cleanup(){

  //resets keypad input variables to default states
  
  data_from_keypad = "";
  inputing_new_passcode = 0;
  indicator_led = 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

byte check_data(String data){

  //function to check if data string meets transmission requirements
  //i.e is the right length for the required function
  
  if(inputing_new_passcode){
    if(data_from_keypad.length() != 10){
      flash_leds(2, 200);
      return 0;
    }
  } else {
    if(data_from_keypad.length() != 4){
      flash_leds(2, 200);
      return 0;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void flash_leds(byte number_of_flashes, long int delay_duration){

  //utility function to flash leds
  
  for(uint8_t i = 0; i < number_of_flashes; i++){
    for(uint8_t j = 0; j < 4; j++){
      digitalWrite(led_indicator_pins[j + 1], HIGH);
    }
    
    delay(delay_duration);
    
    for(uint8_t j = 0; j < 4; j++){
      digitalWrite(led_indicator_pins[j + 1], LOW);
    }
    
    delay(delay_duration);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void resolve_serial_msg(){

  //outputs different flashing signals showing alarm system response
  
  if(data_from_serial.indexOf("INVALID PASSCODE") >= 0){
    flash_leds(3, 200);
  }
  if(data_from_serial.indexOf("INVALID. NO PASSCODE SET") >= 0){
    flash_leds(4, 200);
  }
  if(data_from_serial.indexOf("INVALID PASSCODE LENGTH") >= 0){
    flash_leds(2, 200);
  }
  if(data_from_serial.indexOf("SUCCESS") >= 0){
    flash_leds(1, 700);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_led_indicators(){

  //update the led indicators
  
  digitalWrite(led_indicator_pins[indicator_led], HIGH);
  if(indicator_led > 4){
    indicator_led = 1;
  }
}
