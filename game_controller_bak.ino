/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/


#include <XInput.h>
class Button
{

  private:
    unsigned long last_debounce_time = 0;
    unsigned long debounce_delay = 12;
    bool last_reading = false;

  public:
    String button_name;
    int button_xbox;
    int button_pin;
    bool button_state = false;


  Button(){};
  
  Button(String button_name, int button_xbox, int button_pin)
  {
    this->button_name        = button_name;
    this->button_xbox        = button_xbox;
    this->button_pin         = button_pin;
    this->button_state       = false;
    this->last_debounce_time = 0;
  };

  void set_state(bool reading){
    //if there's still noise here, reset delay
    if (this->last_reading != reading){
      this->last_debounce_time = millis();
    }

    //only run this if we need up update
    if(this->button_state != reading){      
      //this only fires if we have read the same state consistently for "debounce delay" time
      if (this->last_debounce_time + this->debounce_delay < millis()){
        //Serial.println(String(this->button_name)+" "+String(this->button_state));
        this->last_debounce_time = millis();
        this->button_state = reading;
      }
    }
   this->last_reading = reading;
  }
};



int button_len = 6;
Button buttons[] = {
  Button("top_1",      BUTTON_A,  6),
  Button("top_2",      BUTTON_B,  7),
  Button("top_3",      BUTTON_X,  8),
  Button("bottom_1",   BUTTON_Y,  2),
  Button("bottom_2",   BUTTON_LB, 3),
  Button("bottom_3",   BUTTON_RB, 4),
};

int dpad_len = 4;
Button dpad[] {
 Button("dpad_up",   NULL, 16),
 Button("dpad_down", NULL, 15),
 Button("dpad_left", NULL, 14),
 Button("dpad_right", NULL, 10),

};

// the setup routine runs once when you press reset:
void setup() {

  //Start XInput xbobx controller library
  XInput.begin();

  // initialize serial communication at 9600 bits per second:
  //Serial.begin(9600);

  //set pinmode of all input pins
  for (int i = 0; i < button_len; i++) {
    pinMode(buttons[i].button_pin, INPUT_PULLUP);
  }
  for (int i = 0; i < dpad_len; i++) {
    pinMode(dpad[i].button_pin, INPUT_PULLUP);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:

  //set buttons
  for (int i = 0; i < button_len; i++) {
    int reading = !digitalRead(buttons[i].button_pin);
    buttons[i].set_state(reading);
    XInput.setButton(buttons[i].button_xbox, buttons[i].button_state);
  }

  
  //set dpad
  for (int i = 0; i < dpad_len; i++) {
    int reading = !digitalRead(dpad[i].button_pin);
    dpad[i].set_state(reading);
  }
  XInput.setDpad(dpad[0].button_state, dpad[1].button_state, dpad[2].button_state, dpad[3].button_state);



  
  delay(1);        // delay in between reads for stability  
}














