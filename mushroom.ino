// main program for controllng LEDs of the big schroom
// -- Push button gives 1 when it's pushed, 0 otherwise,
//      - connect to pin 2

//include libraries
#include <Time.h>
#include <TimeLib.h>

// definitions
#define NON_INTERACTIVE 0    // shroom mode
#define INTERACTIVE 1        // shroom mode
#define LOW_GLOW 2           // shroom mode
#define MODE_TIME 1      // amount of time [min] in a mode



// constant variables
const int buttonPin = 2;     // the number of the pushbutton pin

// variables that will change:
int shroom_status = 0;
int endtime;

void setup() {
  pinMode(buttonPin, INPUT);    // for manualy switching off the shroom
  Serial.begin(9600);
  

}

void loop() {
  time_t t = now();     //for minute() function
  // The different modes for the shroom
  switch (shroom_status) {
    
    case NON_INTERACTIVE:
      Serial.println("In noninteractive mode");
      
      if (minute(t) < endtime) {
        // do programming of LEDs here
      }
      else  endtime =  SetShroomStatus( &shroom_status, INTERACTIVE, &t);    // switch to interactive mode
      
      if ( digitalRead(buttonPin) == HIGH ) { endtime =  SetShroomStatus( &shroom_status, LOW_GLOW, &t);}  // check if button is being pushed
      break;
    
    case INTERACTIVE:
      Serial.println("In interactive mode");
      
      if (minute(t) < endtime) {
        // do programming of LEDs and interaction here
      }
      else  endtime =  SetShroomStatus( &shroom_status, NON_INTERACTIVE, &t);    // switch to interactive mode
      
      
      if ( digitalRead(buttonPin) == HIGH ) { endtime =  SetShroomStatus( &shroom_status, LOW_GLOW, &t);}  // check if button is being pushed
      break;
    
    case LOW_GLOW:
      Serial.println("In low glow mode");
      if (minute(t) < endtime){
        // programming of low glow show here
      }
      else {
        // exit low glow mode
        endtime = SetShroomStatus(&shroom_status, INTERACTIVE, &t);
      }

      break;
    
    default:
      Serial.println("In default");
      endtime = SetShroomStatus(&shroom_status, NON_INTERACTIVE, &t); // reset
      
      break;
  } // end of switch statement
  
  
  delay(2000);
  
} // end of loop()

// Sets shroom_status and clocks starting minute
// inputs:
//  int* shroom_ptr: pointer to variable shroom_status
//  int SCHROOM_STATUS: chosen status
//  time_t* t_ptr: pointer to variable t
// returns:
//  int time_end: what minutes(t) needs to be in order to switch status
int SetShroomStatus(int* shroom_ptr, int SHROOM_STATUS, time_t* t_ptr){
  if (SHROOM_STATUS > LOW_GLOW) SHROOM_STATUS = NON_INTERACTIVE; // unvalid, RESET
  
  *shroom_ptr = SHROOM_STATUS;
  int t_start = minute(*t_ptr);
  if ( (t_start + MODE_TIME) > 59 )    return (t_start + MODE_TIME - 60);  // overflow
  else                                 return (t_start + MODE_TIME);       // no overflow
  
}
