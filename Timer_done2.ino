

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int calls = 1;   // Execution count, so this doesn't run forever
int maxCalls = 10;   // Maximum number of times the Choreo should be executed



#define ledPin  13                  // LED connected to digital pin 13

#define buttonPin 4                // button on pin 4



int value = LOW;                    // previous value of the LED

int buttonState;                    // variable to store button state

int lastButtonState;                // variable to store last button state

int blinking;                       // condition for blinking - timer is timing

long interval = 100;                // blink interval - change to suit

long previousMillis = 0;            // variable to store last time LED was updated

long startTime ;                    // start time for stop watch

long elapsedTime ;                  // elapsed time for stop watch

int fractional;                     // variable used to store fractional part of time







void setup()

{

   Serial.begin(9600);
 delay(4000);
  while(!Serial);
  Bridge.begin();


   pinMode(ledPin, OUTPUT);         // sets the digital pin as output



   pinMode(buttonPin, INPUT);       // not really necessary, pins default to INPUT anyway

   digitalWrite(buttonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.



}



void loop()

{

    // check for button press

   buttonState = digitalRead(buttonPin);                   // read the button state and store



   if (buttonState == LOW && lastButtonState == HIGH  &&  blinking == false){     // check for a high to low transition

      // if true then found a new button press while clock is not running - start the clock



      startTime = millis();                                   // store the start time

      blinking = true;                                     // turn on blinking while timing

      delay(5);                                               // short delay to debounce switch

      lastButtonState = buttonState;                          // store buttonState in lastButtonState, to compare next time

     

   }



   else if (buttonState == LOW && lastButtonState == HIGH && blinking == true){     // check for a high to low transition

      // if true then found a new button press while clock is running - stop the clock and report



        elapsedTime =   millis() - startTime;              // store elapsed time

        blinking = false;                                  // turn off blinking, all done timing

        lastButtonState = buttonState;                     // store buttonState in lastButtonState, to compare next time


        
       // routine to report elapsed time 

        Serial.print( (int)(elapsedTime / 1000L));         // divide by 1000 to convert to seconds - then cast to an int to print



        Serial.print(".");                             // print decimal point



        // use modulo operator to get fractional part of time 

       fractional = (int)(elapsedTime % 1000L);



       // pad in leading zeros - wouldn't it be nice if 

       // Arduino language had a flag for this? :)

       if (fractional == 0)

          Serial.print("000");      // add three zero's

       else if (fractional < 10)    // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros

          Serial.print("00");       // add two zeros

       else if (fractional < 100)

          Serial.print("0");        // add one zero



       Serial.println(fractional);  // print fractional part of time 



   }



   else{

      lastButtonState = buttonState;                         // store buttonState in lastButtonState, to compare next time

   }



   // blink routine - blink the LED while timing

   // check to see if it's time to blink the LED; that is, the difference

   // between the current time and last time we blinked the LED is larger than

   // the interval at which we want to blink the LED.



   if ( (millis() - previousMillis > interval) ) {



      if (blinking == true){

         previousMillis = millis();                         // remember the last time we blinked the LED



         // if the LED is off turn it on and vice-versa.

         if (value == LOW)

            value = HIGH;

         else

            value = LOW;

         digitalWrite(ledPin, value);

      }

      else{

         digitalWrite(ledPin, LOW);                         // turn off LED when not blinking

      }

   }
   
   if (calls <= maxCalls) {
    Serial.println("Running AppendValues - Run #" + String(calls++));
    
    TembooChoreo AppendValuesChoreo;

    // Invoke the Temboo client
    AppendValuesChoreo.begin();

    // Set Temboo account credentials
    AppendValuesChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendValuesChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendValuesChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    AppendValuesChoreo.addInput("RefreshToken", "1/kyt6mwj-zU65CHN8KslU42P0g0x1A2QGHUmp3WSubBM");
    AppendValuesChoreo.addInput("ClientSecret", "nAy77IF9TJRbeIM2I46W63Ht");
    AppendValuesChoreo.addInput("Values", "[\n  [\n    \fractional,\n    \"200\",\n    \"300\"\n  ]\n]");
    AppendValuesChoreo.addInput("ClientID", "990476920871-ndo8ou5p864b363cmmm3t3fuup9cti3v.apps.googleusercontent.com");
    AppendValuesChoreo.addInput("SpreadsheetID", "1Fog2Fpxu4ZjroY8XTyxt4bYcSSAdWm9DUe4gHt4h2tg");
    
    // Identify the Choreo to run
    AppendValuesChoreo.setChoreo("/Library/Google/Sheets/AppendValues");
    
    // Run the Choreo; when results are available, print them to serial
    AppendValuesChoreo.run();
    
    while(AppendValuesChoreo.available()) {
      char c = AppendValuesChoreo.read();
      Serial.print(c);
    }
    AppendValuesChoreo.close();
  }

  Serial.println("Waiting...");
  
}





