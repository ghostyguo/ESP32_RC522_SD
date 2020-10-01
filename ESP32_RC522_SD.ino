/*
  ESP32 SD card read/write with MFRC522 RFID reader

  created   2020/10/01
  by Ghosty Guo
  
  This example code is in the public domain.
*/

#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>

//forward declare
void saveNUID();
int readNUID();

constexpr uint8_t MFRC522_RST_PIN = 4; 
constexpr uint8_t MFRC522_SS_PIN =  5;
constexpr uint8_t SD_CS_PIN =  15;

MFRC522 rfid(MFRC522_SS_PIN, MFRC522_RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
char NUID[20]; 

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //
  
  // RC522 init
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  // SD card init
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  //saveNUID("testing 1, 2, 3.");
}

void loop() {
  if (readNUID()) {
    saveNUID();
  }
}

int readNUID()
{
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return 0;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return 0;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return 0;
  }

  for (byte i = 0; i < rfid.uid.size; i++) {
    uint8_t LB = (rfid.uid.uidByte [i] & 0x0F);
    uint8_t MB = (rfid.uid.uidByte [i] & 0xF0) >> 4;
    NUID[i*3] = ' ';
    NUID[i*3+1] = (MB>=10) ? (MB-10+'A') : MB+'0';
    NUID[i*3+2] = (LB>=10) ? (LB-10+'A') : LB+'0';
  }
  Serial.print(F("The NUID tag is:"));
  Serial.println(NUID);
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  return 1;
}

void saveNUID()
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(NUID);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}



/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
