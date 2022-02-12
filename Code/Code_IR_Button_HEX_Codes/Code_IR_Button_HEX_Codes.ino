 /**********************************************************************************
 *  TITLE: Get the Hex code from any IR remote.
 *  Click on the following links to learn more. 
 *  YouTube Video: https://youtu.be/dtHGJcNkaMo
 *  Related Blog : https://iotcircuithub.com/arduino-projects/
 *  by Tech StudyCell
 * 
 *  Download the Libraries:
 *  IRremote Library 3.3.0: https://github.com/Arduino-IRremote/Arduino-IRremote
 **********************************************************************************/

#include <IRremote.h>
 
int IR_RECV_PIN = A0;  //Update the pin as per circuit
 
IRrecv irrecv(IR_RECV_PIN);
decode_results results;
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
 
void loop()
{
  if (irrecv.decode(&results))
  {
  Serial.println(results.value, HEX); //print the HEX code
  irrecv.resume();
  }
}
