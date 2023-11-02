#include <Servo.h>

#include <DFRobot_ID809.h>

DFRobot_ID809 fingerprint;
Servo doorServo;

// Button press durations
const int dblLongDur = 2000;
const int longDur = 1000;

bool unlocked = false; // If the door is unlocked (will accept any print)
bool dbPressed = false; // Debounce state
bool pressed = false; // True state
int menuID = 0; // ID the menu is on (for writing and deleting)
unsigned long menuBlinkTime = 0; // Time of last menu ID light blink
unsigned long pressTime = 0; // Time of last (true) button press down
unsigned long dbTime = 0; // Debounce time of last state change
unsigned long servoOn = 0; //time the servo was last enabled

void dblFlash(int duration) {
  digitalWrite(13,HIGH);
  delay(duration);
  digitalWrite(13,LOW);
  delay(duration);
  digitalWrite(13,HIGH);
  delay(duration);
  digitalWrite(13,LOW);
}

void capturePrint(int ID) {
  //Fingerprint Capture
  //Set ready status
  while(true){
    //Indicate ready
    fingerprint.ctrlLED(fingerprint.eKeepsOn,fingerprint.eLEDBlue, 0);
    //Collect Fingerprint
    if((fingerprint.collectionFingerprint(/*timeout = */0)) == ERR_ID809){
      fingerprint.ctrlLED(fingerprint.eBreathing,fingerprint.eLEDRed, 0); //indicate failure
      continue; //try again
    }
    //Store Fingerprint
    if(fingerprint.storeFingerprint(ID) != ERR_ID809){
      //Indicate success
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eKeepsOn, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */0);
      //Wait for finger release
      while(fingerprint.detectFinger());
      //Turn off LED and return
      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eNormalClose, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */0);
      return;
    }else{
      fingerprint.ctrlLED(fingerprint.eKeepsOn,fingerprint.eLEDRed, 0);
      delay(1000);
      //delay(10000);
      //Serial.println(fingerprint.getErrorDescription());
    }
    //Wait for finger release
    while(fingerprint.detectFinger());
  }
}

int verifyPrint(){
  //Collect Fingerprint
  while(!fingerprint.detectFinger()){
    //Show ready if waiting for finger.
    fingerprint.ctrlLED(fingerprint.eKeepsOn,fingerprint.eLEDCyan, 0);
    delay(200);
  }
  if(fingerprint.collectionFingerprint(/*timeout = */0) == ERR_ID809){
    fingerprint.ctrlLED(fingerprint.eBreathing,fingerprint.eLEDRed, 1); // show error
    delay(1000);
  }
  // Search fingerprint
  int readID = fingerprint.search();
  if(readID != 0) {
    //indicate success
    fingerprint.ctrlLED(fingerprint.eFastBlink,fingerprint.eLEDGreen,readID);
  } else {
    //Indicate rejection
    dblFlash(50);
    fingerprint.ctrlLED(fingerprint.eFastBlink,fingerprint.eLEDRed,2);
  }
  return readID;
}

void openDoor(){
  doorServo.attach(10);
  servoOn = millis();
  doorServo.write(130);
}

void setup() {
  //setup debug LED
  pinMode(13,OUTPUT); // debug LED
  doorServo.attach(10); // motor
  servoOn=millis();
  pinMode(5,INPUT_PULLUP); // button
  pinMode(11,OUTPUT); // door unlocked LED
  pinMode(4,INPUT); // finger sensor ring
  digitalWrite(13,LOW);
  //Setup sensor
  Serial.begin(115200);
  fingerprint.begin(Serial);
  delay(500); //wait for connection
  while(fingerprint.isConnected() == false){
    //connection failed - flash error
    dblFlash(500);
    delay(1000);
  }
  fingerprint.setSecurityLevel(1);
  doorServo.write(0);
}

void loop() {
  // Servo detachment
  if(millis()-servoOn > 2000 && doorServo.attached()){
    doorServo.write(0);
    delay(2000);
    doorServo.detach();
  }
  // Menu blinking and timeout
  if(menuID != 0){ // we're in the menu mode
    if((millis() - menuBlinkTime) > (500*menuID + 1000)){
      //It has been 1 second past the end of the last blink
      menuBlinkTime = millis();
      fingerprint.ctrlLED(fingerprint.eFastBlink,fingerprint.eLEDMagenta,menuID);
    }
    if((millis() - pressTime) > 15000){
      menuID = 0; // exit menu
    }
  }
  // Finger scanning
  if(digitalRead(4)){
    if(unlocked){
      // If unlocked, just open the door and flash green.
      openDoor();
      fingerprint.ctrlLED(fingerprint.eFastBlink,fingerprint.eLEDGreen,1);
    } else {
      // If locked, scan print and open door if accepted.
      //Prepare servo while fingerprint is scanning.
      doorServo.attach(10);
      doorServo.write(70);
      if(verifyPrint() != 0){
        openDoor();
        //Wait for finger release
        while(fingerprint.detectFinger());
      }
    }
  }
  // Debounce
  int button = !digitalRead(5);
  if(dbPressed != button){
    dbPressed = button;
    dbTime = millis();
  }
  // True button press handling
  if(millis() - dbTime > 40){ // Button has been pressed longer than 70ms
    if(dbPressed != pressed){ // New button state is different from previous true state
      pressed = dbPressed; // Update true state
      if(pressed){
        pressTime = millis(); // Update pressTime if the button is down
      } else {
        if((millis()-pressTime) > dblLongDur){ // if double long press
          if(menuID){ // If in the menu
            // one white breath to indicate delete
            fingerprint.ctrlLED(fingerprint.eBreathing,fingerprint.eLEDWhite, 1);
            fingerprint.delFingerprint(menuID); // delete print
            menuID = 0; //exit menu
          }
        } else if((millis()-pressTime) > longDur){ // if long press
          if(menuID){ //if in the menu
            // Flash ID in blue
            fingerprint.ctrlLED(fingerprint.eFastBlink,fingerprint.eLEDBlue,menuID);
            delay(500*menuID + 200); // wait for blinking to be done
            capturePrint(menuID);
            menuID = 0; 
          } else {
            unlocked = !unlocked; // long press outside menu locks/unlocks.
          }
        } else {
          dblFlash(100);
          menuID++; // Increment menu ID
          if(menuID > 10){
            menuID = 1;
          }
          menuBlinkTime = 0; //reset blink time to blink new ID
        }
      }
    } else { // New button state is the same as previous button state
      if(pressed){
        if((millis()-pressTime) > dblLongDur){ // if double long press
          if(menuID){ // if in the menu
            // Set LED constant white to indicate erase.
            fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDWhite, 0);
          }
        } else if((millis()-pressTime) > longDur){ // if long press
          if(menuID){ //if in the menu
            // Set LED constant blue to indicate write.
            fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDBlue, 0);
          } else { // not in the menu
            if(unlocked){
              analogWrite(11,0); // turn off unlocked warning
            } else {
              analogWrite(11,50); // light unlocked warning
            }
          }
        }
      }
    }
  }
}
























































