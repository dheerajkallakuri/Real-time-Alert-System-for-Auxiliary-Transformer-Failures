/*
Unmount the GSM shield when connecting to the PC
Commands from software:
'start' or any string: Runs the code for fetching and Updating numbers
'fetch': Arduino sends the contacts stored on EEPROM
'update': Arduino waits for a string of updated contacts and replaces the old contacts stored on EEPROM with the updated ones.
'X': Arduino stops waiting for more digits to come for updating contacts and writes all the received digits to EEPROM
'close': Arduino runs the shield code
'END': Arduino executed the command given
*/

#include <EEPROM.h>
//initializing variables
String phoneNumbers; 
String response;

boolean connectedToSoftware = false;

//Maintains the buffer time in which arduino can connect to the software 
unsigned long bufferTime = 0; //300000; //buffer time of 5 minutes
unsigned long startTime = millis();
unsigned long currentTime;
unsigned long elapsedTime;
unsigned long counterIncrementTime = 3600000; //updates counter after 1 hour

//initializing variables for signal
const int signalUpPin = 13;
const int signalDownPin = 8;
const int redLedPin = 12;
const int greenLedPin = 11;

//UP signal
static boolean prevUpState = false;
static boolean currentUpState = false;
//Down signal
static boolean prevDownState = false;
static boolean currentDownState = false;

//messages to be sent
//Signal Up
String statusUpWorking = "UP AT OK!\n";
String statusUpNotWorking = "UP AT FAIL!\n";
//Signal Down
String statusDownWorking = "DN AT OK!\n";
String statusDownNotWorking = "DN AT FAIL!\n";

//Also configures in the buffer time provided whether Arduino is connected to Software
void setup() {
  //Signal Setup
  pinMode(signalUpPin, INPUT);
  pinMode(signalDownPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  //for communicating with PC
  Serial.begin(9600);
  while(!Serial.available()){
    delay(10000);
    currentTime = millis();
    elapsedTime = currentTime - startTime;
    if(elapsedTime > bufferTime) {
      break;
    }   
  }
  if(Serial.available()){
   connectedToSoftware = true;

   //clears input buffer on Arduino
   while (Serial.available() > 0){
    char ch = Serial.read();
   }
   Serial.println("Arduino Ready"); 
   delay(100);
   Serial.println("END");
  }
}

void loop() {
  if(connectedToSoftware){
    communicateWithSoftware();
  }
  else {
    delay(1000);
    currentUpState = digitalRead(signalUpPin);
    currentDownState = digitalRead(signalDownPin);     
    //Condition 
    if(currentUpState != prevUpState || currentDownState != prevDownState) {
      prevUpState = currentUpState;
      prevDownState = currentDownState;
      checkSignalStatusSendSMS();
      delay(1000);
    }
    //check for update time    
    currentTime = millis();
    elapsedTime = currentTime - startTime;
    if(elapsedTime >= counterIncrementTime){
      int n = EEPROM.read(1000) + 1;
      EEPROM.write(1000, n);
      startTime = millis();
      if(EEPROM.read(1000) >= 4){
        EEPROM.write(1000, 0);
        checkSignalStatusSendSMS();
        delay(1000);
      }
    }
  }  
}

//Connected to Software
void communicateWithSoftware(){
	while(Serial.available() < 5); //wait for characters to arrive at input buffer
    response = Serial.readString(); //read command sent from software
    if(response == "update"){
        Serial.println("Updating"); //send to software
        String receivingDigits = "";
        boolean contactsReceived = false;
        char digit;
        while(!( Serial.available() )); //wait for software to send digits
        while(!contactsReceived){
          if(Serial.available() > 0){
            digit = Serial.read();
            if(digit == 'X') contactsReceived = true; //X means all digits sent
            else receivingDigits += digit;
          }
        }
        phoneNumbers = receivingDigits;
        updatePhoneNumbers(phoneNumbers);
        response = "";
        Serial.println("END");
        delay(100);
    }
    if(response == "fetch"){
        phoneNumbers = fetchPhoneNumbers(); //return digits stored on EEPROM
        Serial.println(phoneNumbers);
        response = "";
        Serial.println("END");
        delay(100);
    }
    if(response == "close"){
        Serial.println("Closing");
        Serial.println("END");
        connectedToSoftware = false;
        delay(10000);
    }
}

//Checks signal status to send SMS
void checkSignalStatusSendSMS(){
  if(currentUpState == HIGH && currentDownState == HIGH){
    digitalWrite(redLedPin, HIGH);
    delay(200);
    digitalWrite(greenLedPin, HIGH);
    delay(200);
    contactList(statusUpWorking + statusDownWorking);
  }
  else if(currentUpState == HIGH && currentDownState == LOW) {
    digitalWrite(redLedPin, HIGH);
    delay(200);
    digitalWrite(greenLedPin, LOW);
    delay(200);
    contactList(statusUpWorking + statusDownNotWorking);
  }
  else if(currentUpState == LOW && currentDownState == HIGH) {
    digitalWrite(redLedPin, LOW);
    delay(200);
    digitalWrite(greenLedPin, HIGH);
    delay(200);
    contactList(statusUpNotWorking + statusDownWorking);
  }
  else {
    digitalWrite(redLedPin, LOW);
    delay(200);
    digitalWrite(greenLedPin, LOW);
    delay(200);
    contactList(statusUpNotWorking + statusDownNotWorking);
  }
}

//Fetches the numbers stored on the EEPROM
String fetchPhoneNumbers() {
  //will contain all stored numbers in format - 98765432109876543210...
  String phoneNumbers = "";
  int totalDigits = int(EEPROM.read(0)); //total digits of phone number stored
  /*
  EEPROM stores data in byte format 
  EEPROM.read() returns data in byte format
  */
  for(int i = 1; i < totalDigits+1; i++) {
    int digit = EEPROM.read(i);
    phoneNumbers.concat(char(digit));
  }  
  return phoneNumbers; 
}

//Clears the EEPROM completely and Writes the new phone numbers
void updatePhoneNumbers(String phoneNumbers) {
  //clear all stored data
  for(int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i,0);  
  }
  int totalDigits = phoneNumbers.length();
  EEPROM.write(0,totalDigits);
  char allDigits[totalDigits+1];
  phoneNumbers.toCharArray(allDigits, totalDigits+1);
  for(int i = 0; i < totalDigits; i++) {
    EEPROM.write(i+1, byte(allDigits[i]));  
  }
}

//sends SMS by retrieving individual phone numbers from EEPROM
void contactList(String msg){
  //Set timezone to IST
  Serial.println("AT+CLTS=1");
  delay(10000);
  //clear buffer
  //while(Serial.available() < 60);
  response = Serial.readString();
  response = "";
  //get localtime
  Serial.println("AT+CCLK?");
  //get the response
  while(Serial.available() > 0 || response.indexOf("OK") == -1){
    response += Serial.readString();
  }
  //string to add in message
  String timestamp = "TIME: " + response.substring(20,37);
  //reads digits from EEPROM
  int totalDigits = EEPROM.read(0); //total digits stored in EEPROM
  String contact = "";
  delay(5000);
  for(int i = 1; i <= totalDigits; i++){
    int digit = EEPROM.read(i);
    contact.concat(char(digit));
    if (i % 10 == 0) {
        sendSMS(contact, msg + timestamp);
        delay(5000);
        contact = "";        
    }
  }
}

//sending sms
void sendSMS(String contact, String msg) {
  Serial.print("AT\r");
  delay(1000);
  Serial.print("AT+CMGF=1\r"); // Set the shield to SMS mode
  delay(1000);
  Serial.print("AT+CMGS = \"+91"); //setting up contact with country code
  Serial.print(contact);
  Serial.println("\"");
  delay(1000);
  Serial.println(msg); //the content of the message
  delay(1000);
  Serial.print((char)26);//the ASCII code of the ctrl+z is 26 (required according to the datasheet)
  delay(100);
}
