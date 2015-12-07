#include <SerialCommand.h>

int speakerPin = 9;

SerialCommand SCmd;

int length = 12; // the number of notes
//char notes[] = "ccggaagffeeddc "; // a space represents a rest
char notes[] = "ddcd a adgGd";
int beats[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int tempo = 150;

void toneDelay(int microseconds) {
  unsigned long startTime = micros();
  while (micros() - startTime < microseconds) {}
}

void toneDelayLong(int milliseconds) {
  unsigned long startTime = millis();
  while (millis() - startTime < milliseconds) {}
}

void playTone(int tone, int duration) {
  unsigned long startTime;
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    toneDelay(tone);
    digitalWrite(speakerPin, LOW);
    toneDelay(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'G', 'g', 'a', 'b', 'C' };
  //int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  int tones[] = { 1804, 1607, 1519, 1275, 1204, 2145, 1014, 956 }; // all of these are sharp except for G

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);

  SCmd.addCommand("Hello", SayHello);
  SCmd.addDefaultHandler(unrecognized);
}

int play = 0;
void loop() {
  if (play == 0) {
    /* TODO: Think of a way to functionize (maybe macro?) song playing.
     *       This method currently pauses execution of
     *       the loop so any other things wouldn't be happening  :(
     */
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        toneDelayLong(beats[i] * tempo); // rest
      } else {
        playNote(notes[i], beats[i] * tempo);
      }

      // pause between notes
      toneDelayLong(tempo / 2);
    }
    play = 1;
  }

  SCmd.readSerial();
}

void SayHello() {
  Serial.println("Hello yourself!");
}

void unrecognized() {
  Serial.println("Dont know that command");
}

//byte readFromSerial() {
//   while (Serial.available() > 0) {
//    // read the incoming byte:
//    c = Serial.read();
//    if (c == '\r') {
//      c = Serial.read();
//      Serial.println("");
//      incomingMsg[MSG_SIZE] = '\0';
//      return 1;
//    } else {
//      if (pos < MSG_SIZE) {
//        incomingMsg[pos++] = c;
//      } else {
//        /* shift all of the characters to the left */
//        for (i=0;i<(MSG_SIZE-1);i++) {
//          incomingMsg[i] = incomingMsg[i+1];
//        }
//        incomingMsg[MSG_SIZE-1] = c;
//      }
//
//      Serial.print(c);
//    }
//  }
//  return 0;
//}

//void execCmd(char *cmd) {
//  Serial.println("Evaluating:");
//  Serial.println(cmd);
//  if (strcmp(cmd, "Hello") == 0) {
//    // say what you got:
//    Serial.println("Hey sup.");
//    Serial.flush();
//  }
//  pos = 0;
//  memset(cmd, 0, sizeof(char) * MSG_SIZE);
//}
