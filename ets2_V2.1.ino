#include <SPI.h>
#include <MFRC522.h>
#include <LCD5110_Graph.h>
#include <Keyboard.h>


#define SS_PIN 7
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
LCD5110 myGLCD(8, 9, 10, 11, 12);

extern uint8_t SmallFont[];

String uidString;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);


  delay(3000);

  myGLCD.clrScr();
  myGLCD.print("Welcome", CENTER, 0);
  //myGLCD.print("Efekan", CENTER, 30);
  myGLCD.update();

  Serial.println("Okumaya hazir ...");
}

void loop() {
  myGLCD.print("Enter Car Key", CENTER, 15);
  myGLCD.print("earobotics", CENTER, 40);
  myGLCD.update();
  RfidScan();
  delay(100);
}

bool carLockState = true;

void dump_byte_array(byte *buffer, byte bufferSize) {
  char message[bufferSize];
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? ” 0″ : ” “);
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.print("   =   ");

  uidString = String(mfrc522.uid.uidByte[0]) + " " + String(mfrc522.uid.uidByte[1]) + " " + String(mfrc522.uid.uidByte[2]) + " " + String(mfrc522.uid.uidByte[3]);

  Serial.print(uidString);

  Serial.println();

  myGLCD.clrScr();
  myGLCD.print(uidString , CENTER, 0);
  myGLCD.update();

  if (uidString == "144 24 28 131") {

    if (carLockState) {
      printUnlockMessage();
      carLockState = false;
    }
    else {
      carLockState = true;
      motorstop();

    }
  }
  else {
    printLockMessage();
  }
}



void RfidScan()
{
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
}

void printUnlockMessage()
{
  Serial.println("Motor Starting");
  myGLCD.print("Motor Starting", CENTER, 10);
  myGLCD.update();
  delay(200);
  Keyboard.press('E');
  delay(100);
  Keyboard.releaseAll();

  delay(1000);

  Serial.println("Motor Started");
  myGLCD.clrScr();
  myGLCD.print("Motor Started", CENTER, 0);
  myGLCD.update();

}

void printLockMessage()
{
  Serial.println("Checking");
  myGLCD.print("Cheking", CENTER, 10);
  myGLCD.print("Key", CENTER, 20);
  myGLCD.update();

  delay(2000);

  Serial.println("Wrong Key");
  myGLCD.clrScr();
  myGLCD.print("Wrong Key", CENTER, 0);
  myGLCD.update();
}

void motorstop()
{
  Serial.println("Motor Stoping");
  myGLCD.print("Motor Stoping", CENTER, 10);
  myGLCD.update();
  delay(200);
  Keyboard.press('E');
  delay(100);
  Keyboard.releaseAll();
  delay(1000);

  Serial.println("Motor Stopped");
  myGLCD.clrScr();
  myGLCD.print("Motor Stopped", CENTER, 0);
  myGLCD.update();

}

void printTestMessage()
{
  Serial.println("Testing");
  myGLCD.print("Testing", CENTER, 10);
  myGLCD.print("Please Wait", CENTER, 20);
  myGLCD.update();

  delay(2000);

  Serial.println("Lock Car");
  myGLCD.clrScr();
  myGLCD.print("Test OK", CENTER, 0);
  myGLCD.update();
  delay(1000);
  myGLCD.clrScr();
}
