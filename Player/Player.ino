#define BUTTON1 16
#define BUTTON2 14
byte BUTTON[][2] = {{BUTTON1, 1}, {BUTTON2, 1}};
byte Player_count = 2;
void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < sizeof(BUTTON) / sizeof(BUTTON[0]); i++) {
    pinMode(BUTTON[i][0], INPUT_PULLUP);
  }
  Serial.println(String(Player_count) + " Player");
}


void loop() {
  Player_count = PLAYER();
  Serial.println(String(Player_count) + " Player");
  

}
byte PLAYER () {
  byte count = 2;
  while (true) {
    byte state = state_BUTTON();
    if (state != 0) {
      if (state == 1) {
        count++;
      } else {
        return count ;
      }
      if (count > 4) {
        count = 2;
      }
       Serial.println(String(count));
    }
  }
}

int state_BUTTON () {
  for (byte i = 0; i < sizeof(BUTTON) / sizeof(BUTTON[0]); i++) {
    if (!digitalRead(BUTTON[i][0]) and BUTTON[i][1]) {
      //Serial.println(" i = "+String(i)+" BUTTON = "+String(digitalRead(BUTTON[i][0])));
      BUTTON[i][1] = 0;
      return i + 1;
    } else if (digitalRead(BUTTON[i][0]) and !BUTTON[i][1]) {
      BUTTON[i][1] = 1;
    }
    delay(5);
  } return 0;
}
