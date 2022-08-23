#define BUTTON1 16
#define BUTTON2 14
byte BUTTON[][2] = {{BUTTON1, 1}, {BUTTON2, 1}};
byte Player_count = 1;
byte finish = 6;
byte Event [][3] = {{1, 0, 6}, {3, 0, 1}, {7, 0, 0}, {11, 0, 0}, {15, 14, 18}, {17, 1, 6}, {19, 2, 0}, {39, 2, 0}, {22, 0, 15}, {28, 0, 21}, {31, 0, 31}, {35, 0, 28}, {43, 0, 28}, {46, 0, 6}};
uint32_t Player[][4] = {
  {0xFFFFFF, 0, 0, 0},
  {0xFFFFFF, 0, 0, 0},
  {0xFFFFFF, 0, 0, 0},
  {0xFFFFFF, 0, 0, 0}
};

uint32_t COLOR [] {
  0xFF0000,
  0xFF1493,
  0x9400D3,
  0x0000FF,
  0x00FFFF,
  0x00FF00,
  0x006400,
  0xFFFF00,
  0xD2691E
};
String Color_str[] {
  "red",
  "pink",
  "violet",
  "blue",
  "cyan",
  "lime",
  "green",
  "Yellow",
  "chocolate"
};
void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < sizeof(BUTTON) / sizeof(BUTTON[0]); i++) {
    pinMode(BUTTON[i][0], INPUT_PULLUP);
  }
  Serial.println(String(Player_count) + " Player");
}


void loop() {
  for (byte i = 0; i < 4; i++) {
    Player[i][0] = 0xFFFFFF;
    Player[i][1] = 0;
    Player[i][2] = 0;
    Player[i][3] = 0;
  }

  //Player_count = PLAYER();
  //Serial.println(String(Player_count) + " Player");
  // color_select(Player_count);
  while (true) {
    for (byte i = 0; i < Player_count; i++) {
      Player[i][1]++;
      if (Player[i][1] > 0) {
        Round(i, dice());
        Player[i][1]--;
        for (byte j = 0; j < sizeof(Event) / sizeof(Event[0]); j++) {
          if (Player[i][2] == Event[j][0]) {
            Serial.println("Hello" + String(Event[j][1]));
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------FUNTION----------------------------------------------------------------//
byte Round(byte i, int num_dice) {
  int check = Player[i][2] + num_dice;
  if (num_dice < 0) {
    num_dice = abs(num_dice);
    for (int j = 1 ; j <= num_dice; j++) {
      if (Player[i][2] < finish) {
        Player[i][2] -= 1;
        Serial.println("Player " + String(i + 1) + ": " + (Player[i][2]));
        delay(500);
      }
    }
  } else {
    for (int j = 1 ; j <= num_dice; j++) {
      if (Player[i][2] < finish) {
        Player[i][2] += 1;
        Serial.println("Player " + String(i + 1) + ": " + (Player[i][2]));
        delay(500);
      }
      else {
        for (int j = 1; j <= (check - finish); j++) {
          Player[i][2] -= 1;
          Serial.println("Player " + String(i + 1) + ": " + (Player[i][2]));
          delay(500);
        }
        break;
      }
    }
  }
}
//----------------------------------------------------------------ทอยลูกเต๋า---------------------------------------------------------------//
byte dice() {
  byte count = 0;
  byte point;
  uint32_t delaytime = millis();
  while (true) {
    if (millis() - delaytime > 500  ) {
      delaytime = millis();
      count ++;
      if (count > 6) {
        count = 1;
      }
      //  Serial.println(count);
    }
    byte state = state_BUTTON();
    if (state != 0) {
      point = random(1, 6);
      delaytime = 200;
      for (int i = 0; i < 6; i++) {
        //   Serial.println(random(1,6));
        delay(delaytime);
        delaytime += 200;
      }
      Serial.println("Dice :" + String(point));
      return point;
    }
  }
}
//----------------------------------------------------------------เลือกสี---------------------------------------------------------------//
void color_select(byte count) {
  for (byte i = 0; i < count; i++) {
    byte count_2 = 0;
    Serial.println("PLAYER :" + String(i + 1));
    while (true) {
      Serial.println(Color_str[count_2]);
      byte state = state_BUTTON();
      for (int j = 0; j < count; j++) {
        if (COLOR[count_2] == Player[j][0]) {
          count_2++;
        }
      }
      if (state != 0) {
        if (state == 1) {
          count_2++;
        } else {
          Player[i][0] = COLOR[count_2];
          break  ;
        }
        if (count_2 >= sizeof(COLOR) / sizeof(COLOR[0])) {
          count_2 = 0;
        }
      }
    }
  }
}
//----------------------------------------------------------------กำหนดเพลเยอร์---------------------------------------------------------------//
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
//----------------------------------------------------------------ปุ่ม---------------------------------------------------------------//
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
