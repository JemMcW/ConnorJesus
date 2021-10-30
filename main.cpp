// Simple WAV file player example
//

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// Which pin will trigger the sound
#define TRIGGER_PIN 8

// Heaven Output Pins
#define HEAVEN_PIN1 3
#define HEAVEN_PIN2 4

// Hell Ouput Pins
#define HELL_PIN1   5
#define HELL_PIN2   6
#define HELL_PIN3   9

#define NUM_HEAVEN_FILES 1
#define NUM_HELL_FILES 1

// Higher Number results in more heaven sounds
#define RATIO 4

// Maximum number of each in a row
#define MAX_HEAVENS 5
#define MIN_HEAVENS 2
#define MAX_HELLS 1

// DONT CHANGE BELOW HERE
// ******************************************

#define HEAVEN_FILE_NAME "HEAVEN%d.wav"
#define HELL_FILE_NAME "HELL%d.wav"

AudioPlaySdWav      playWav1;
AudioOutputAnalog   audioOutput;

AudioConnection     patchCord1(playWav1, 0, audioOutput, 0);
AudioConnection     patchCord2(playWav1, 1, audioOutput, 1);

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

void setup() {

  Serial.begin(9600);
  delay(3000);

  srand(micros());
  delay(rand()%10);
  srand(micros());

  pinMode(TRIGGER_PIN, INPUT_PULLDOWN);
  pinMode(HEAVEN_PIN1, OUTPUT);
  pinMode(HEAVEN_PIN2, OUTPUT);
  pinMode(HELL_PIN1, OUTPUT);
  pinMode(HELL_PIN2, OUTPUT);

  AudioMemory(8);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  if (!(SD.begin(SDCARD_CS_PIN)))
  {
    while (1)
    {
      digitalWrite(HEAVEN_PIN1, LOW);
      digitalWrite(HEAVEN_PIN2, LOW);
      digitalWrite(HELL_PIN1, LOW);
      digitalWrite(HELL_PIN2, LOW);
      delay(500);
      digitalWrite(HEAVEN_PIN1, HIGH);
      digitalWrite(HEAVEN_PIN2, HIGH);
      digitalWrite(HELL_PIN1, HIGH);
      digitalWrite(HELL_PIN2, HIGH);
      delay(500);
    }
  }
}

void ErrorBlink()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(HEAVEN_PIN1, LOW);
    digitalWrite(HEAVEN_PIN2, LOW);
    digitalWrite(HELL_PIN1, LOW);
    digitalWrite(HELL_PIN2, LOW);
    delay(250);
    digitalWrite(HEAVEN_PIN1, HIGH);
    digitalWrite(HEAVEN_PIN2, HIGH);
    digitalWrite(HELL_PIN1, HIGH);
    digitalWrite(HELL_PIN2, HIGH);
    delay(250);
  }
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  if (!playWav1.play(filename))
    ErrorBlink();

}

void PlayHeaven()
{
  char FileName[13];

  int FileNum = rand() % NUM_HEAVEN_FILES + 1;
  snprintf(FileName, 13, HEAVEN_FILE_NAME, FileNum);

  playFile(FileName);

  digitalWrite(HEAVEN_PIN1, HIGH);
  digitalWrite(HEAVEN_PIN2, HIGH);
}

void PlayHell()
{
  char FileName[13];

  int FileNum = rand() % NUM_HELL_FILES + 1;
  snprintf(FileName, 13, HELL_FILE_NAME, FileNum);

  playFile(FileName);

  digitalWrite(HELL_PIN1, HIGH);
  digitalWrite(HELL_PIN2, HIGH);
}


void loop() {
  static int HeavenCount = 0;
  static int HellCount = 0;
  bool devil = false;

  if (digitalRead(TRIGGER_PIN) == HIGH)
  {
    if ((HellCount >= MAX_HELLS) or (HeavenCount < MIN_HEAVENS) or ((millis() % RATIO != 0) and (HeavenCount < MAX_HEAVENS)))
    // if (millis() % RATIO != 0)
    {
      PlayHeaven();
      HeavenCount++;
      HellCount = 0;
    }
    else
    {
      PlayHell();
      HeavenCount = 0;
      HellCount++;
      devil = true:
    }
delay(259);
  }

  

  while (playWav1.isPlaying())
  {
    if (digitalRead(TRIGGER_PIN) == HIGH)
      break;
    if (devil)
{
    digitalWrite(HELL_PIN3, HIGH);
    delay(100);
    digitalWrite(HELL_PIN3, LOW);
    
    delay(100);
}

  }

  digitalWrite(HEAVEN_PIN1, LOW);
  digitalWrite(HEAVEN_PIN2, LOW);
  digitalWrite(HELL_PIN1, LOW);
  digitalWrite(HELL_PIN2, LOW);
  digitalWrite(HELL_PIN3, LOW);
  playWav1.stop();
  delay(200);

}
