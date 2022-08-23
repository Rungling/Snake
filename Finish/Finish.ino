#include <FastLED.h>
#define BUTTON1 16
#define BUTTON2 17
#define NUM_LEDS 98
#define DATA_PIN 13
#define BRIGHTNESS 40
CRGB leds[NUM_LEDS];
byte leds_ads[] = {3,
                   5,  7,  9, 11, 13,
                   15, 17, 19, 21, 23,
                   25, 27, 29, 31, 33,
                   35, 37, 39, 41, 43,
                   45, 47, 49, 51, 53,
                   55, 57, 59, 61, 63,
                   65, 67, 69, 71, 73,
                   75, 77, 79, 81, 83,
                   85, 87, 89, 91, 93,
                   97
                  };
byte BUTTON[][2] = {{BUTTON1, 1}, {BUTTON2, 1}};
byte Player_count = 2;
byte finish = 46;
int Event [][3] = {
  { 1,   0,   5},
  { 3,   0,  -2},
  { 7,   0,   0}, //Random
  {11,   0, -11},
  {15,   0,   3},
  {17,   1,   0},
  {19,   2,   0},
  {39,   2,   0},
  {22,   0,  -7},
  {28,   0,   4},
  {31,   0,   3},
  {35,   0,   3},
  {43,   0, -3}
};
String Event_txt[] {
  "NEPTUNE \n+ 5 point",
  "URANUS \nGoto NEPTUNE",
  "SATURN \nRoll !",
  "You meeting with Alien \nGo to Start point",
  "JUPITER \n+ 3 point",
  "SPACE STATION \nYou need some rest 1 turn",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  ""
};
uint32_t Player[][2] = {
  {0xFFFFFF, 0xFFFFFF},
  {0xFFFFFF, 0xFFFFFF},                   //สี,สิทธิ์การเดิน,ตำแหน่ง,สถานะ,เวลา
  {0xFFFFFF, 0xFFFFFF},
  {0xFFFFFF, 0xFFFFFF}
};
int  Player_walk[4];                    //สิทธิ์การเดิน
byte Player_address[4];                 //ตำแหน่งขอวเพลเยอร์
boolean Player_status[4];               //สถานะของผู้เล่น
String Player_name[4];                  //ชื่อของเพลเยอร์
uint32_t COLOR [] {
  0xFF0000,
  0xFF1493,
  0x0000FF,
  0x00FFFF,
  0x00FF00,
  0xFFFF00,
};
String Color_str[] {
  "red",
  "pink",
  "blue",
  "cyan",
  "lime",
  "Yellow",
};
String Name[] {
  "EL",
  "Kingster",
  "Spaceman",
  "Pearl",
  "Flogy",
  "Bundit",
};
void setup() {
  Serial.begin(9600);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(2, 0);
  FastLED.addLeds<WS2813, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  randomSeed(analogRead(0));
  for (byte i = 0; i < sizeof(BUTTON) / sizeof(BUTTON[0]); i++) {
    pinMode(BUTTON[i][0], INPUT_PULLUP);
  }
}

void loop() {
  FastLED.clear();
  FastLED.show();
  for (byte i = 0; i < 4; i++) {
    Player[i][0] = 0xFFFFFF;
    Player_walk[i] = 0;
    Player_address[i] = 0;
    Player_status[i] = 0;
    Player[i][2] = 0xFFFFFF;
  }

  Player_count = PLAYER();
  Serial.println(String(Player_count) + " Player");
  delay(1000);
  name_select();
  byte Player_stay = Player_count;
  while (Player_stay > 1) {
    for (byte i = 0; i < Player_count; i++) {
      Player_stay = check_Player(Player_count);                                //ตรวจสอบจำนวนผู้เล่น
      if (Player_stay <= 1)break;
      if (Player_status[i] == 0) {                                                  //ตรวจสอบว่าเข้าเส้นชัยแล้วหรือยัง
        Player_walk[i]++;
        if (Player_walk[i] == 1) {                                                //ตรวจสอบสิทธิ์การเดิน
          for (byte k = 0; k < sizeof(leds_ads) / sizeof(leds_ads[0]); k++) {
            LED(k, 0x404040);
          }
          for (byte k = 0; k < Player_count; k++) {
            LED(Player_address[k], Player[k][0]);
          }
          Round(i, dice(Player_address[i], i));                                      //ทอยลูกเต๋า
          Player_walk[i]--;
          for (byte j = 0; j < sizeof(Event) / sizeof(Event[0]); j++) {         //ตรวจสอบว่ามีเหตุการเกิดขึ้นมั้ย
            if (Player_address[i] == Event[j][0]) {
              Serial.println("EVENT");
              Serial.println(Event_txt[j]);
              Serial.println("Press ENTER to continue");
              while (true) {
                byte state = state_BUTTON();
                if (state != 0) {
                  if (state == 2) {
                    break;
                  }
                }
              }
              if (j == 2) {
                RanD(i);
                break;
              }
              if (Event[j][1] != 0) {

                Player_walk[i] -=  Event[j][1];
              } else {
                Round(i, Event[j][2]);
              }

            }
          }
        }
      }

      if (Player_address[i] == finish) {
        Player_status[i] = 1;
        Player[i][2] = millis();
        Serial.println("GOAL !!!!");
      }
    }
  }

  rankF();
}
//------------------------------------------------------------จัดอันดับ------------------------------------------------------------//
void rankF() {
  String rank[4];
  for (byte j = 0; j < Player_count; j++) {
    byte count = 0;
    for (byte i = 0; i < Player_count; i++) {
      if (Player[j][2] > Player[i][2]) {
        count++;
      }
    }
    rank[count] = "Player " + String(j + 1);
  }
  for (byte i = 0; i < Player_count; i++) {
    Serial.print(" " + rank[i]);
  }
  Serial.println();
}
//------------------------------------------------เช็คคนที่ยังอยู่ในการเดินทาง-----------------------------------------------------------//
byte check_Player(byte Py) {
  byte count = 0;
  for (byte i = 0; i < Py; i++) {
    if (Player_status[i] == 0) {
      count++;
    }
  }
  return count;
}
//----------------------------------------------------------สุ่ม-------------------------------------------------------------------//
void RanD(byte i) {
  int delayTime = 100;
  byte check ;
  for (int k = 0; k < 10; k++) {
    check = random(3);
    Serial.println(check);
    delay(delayTime);
    delayTime += 100;
  }

  switch (check) {
    case 0:
      Serial.println("point -3");
      Round(i, -3);
      break;
    case 1:
      Serial.println("point 3");
      Round(i, 3);
      break;
    case 2:
      Serial.println("Turn -1");
      Player_walk[i] -=  1;
      break;
  }
}
//--------------------------------------------------------------เดิน--------------------------------------------------------------//
byte Round(byte i, int num_dice) {
  byte check = Player_address[i] + num_dice;
  if (num_dice < 0) {
    num_dice = abs(num_dice);
    for (byte j = 1 ; j <= num_dice; j++) {
      LED(Player_address[i], 0x404040);
      Player_address[i] -= 1;
      if (Player_address[i] > finish) {
        Player_address[i] = 0;
      }

      for (byte k = 0; k < Player_count; k++) {
        LED(Player_address[k], Player[k][0]);
        delay(5);
      }
      LED(Player_address[i], Player[i][0]);
      Serial.print("Player ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(Player_address[i]);
      delay(500);
    }
  } else {
    for (byte j = 1 ; j <= num_dice; j++) {
      if (Player_address[i] < finish) {
        LED(Player_address[i], 0x404040);
        Player_address[i] += 1;
        for (byte k = 0; k < Player_count; k++) {
          LED(Player_address[k], Player[k][0]);
          delay(5);
        }
        LED(Player_address[i], Player[i][0]);
        Serial.print("Player ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(Player_address[i]);

        delay(500);
      }
      else {
        for (byte k = 0; k < (sizeof(leds_ads) / sizeof(leds_ads[0])) - 1; k++) {
          LED(k, 0x404040);
        }
        for (byte j = 1; j <= (check - finish); j++) {
          LED(Player_address[i], 0x404040);
          Player_address[i] -= 1;
          for (byte k = 0; k < Player_count; k++) {
          LED(Player_address[k], Player[k][0]);
          delay(5);
        }
          LED(Player_address[i], Player[i][0]);
          Serial.print("Player ");
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.println(Player_address[i]);
          delay(500);
        }
        break;
      }
    }
  }
}
//--------------------------------------------------------------ทอยลูกเต๋า--------------------------------------------------------------//
byte dice(uint32_t Adress, int Py) {
  randomSeed(analogRead(0));
  byte count = 0;
  byte point;
  bool state = true;
  uint32_t delaytime = millis();
  Serial.println(Player_name[Py] + " Tern");
  Serial.println("Press Select/Random");
  while (true) {

    if (millis() - delaytime > 200  ) {
      delaytime = millis();
      count ++;
      if (state) {
        state = false;
        LED(Adress, 0x000000);
      } else {
        state = true;
        LED(Adress, Player[Py][0]);
      }
      if (count > 6) {
        count = 1;
      }
      //  Serial.println(count);
    }
    byte state = state_BUTTON();
    if (state != 0) {
      if (state == 1) {
        LED(Adress, Player[Py][0]);
        point = random(1, 7);
        delaytime = 200;
        for (byte i = 0; i < 6; i++) {
          //   Serial.println(random(1,6));
          delay(delaytime);
          delaytime += 200;
        }
        Serial.println("Dice :" + String(point));
        return point;
      }
    }
  }
}

//----------------------------------------------------------------เลือกชื่อ---------------------------------------------------------------//
void name_select() {
  for (byte i = 0; i < Player_count; i++) {
    byte count_2 = 0;
    Serial.print("PLAYER ");
    Serial.print(i);
    Serial.println(" NAME");
    while (true) {
      Serial.println(Name[count_2]);
      byte state = state_BUTTON();
      for (byte j = 0; j < Player_count; j++) {
        if (Name[count_2] == Player_name[j]) {
          count_2++;
        }
      }
      if (state != 0) {
        if (state == 1) {
          count_2++;
        } else {
          Player_name[i] = Name[count_2];
          color_select(i);
          break  ;
        }


        if (count_2 >= sizeof(Name) / sizeof(Name[0])) {
          count_2 = 0;
        }
      }
    }
  }
}
//----------------------------------------------------------------เลือกสี---------------------------------------------------------------//
void color_select(byte Py) {
  byte count = 0;

  Serial.println("SELECT NAME ");
  while (true) {
    for (byte j = 0; j < Player_count; j++) {
      if (COLOR[count] == Player[j][0]) {
        count++;
      }
    }
    LED(0, COLOR[count]);
    Serial.println(Color_str[count]);
    byte state = state_BUTTON();
    if (state != 0) {
      if (state == 1) {
        count++;
      } else {
        Player[Py][0] = COLOR[count];
        FastLED.clear();
        FastLED.show();
        break  ;
      }
      if (count >= sizeof(COLOR) / sizeof(COLOR[0])) {
        count = 0;
      }
    }
  }

}
//----------------------------------------------------------------กำหนดเพลเยอร์-------------------------------------------------------------//
byte PLAYER () {
  byte count = 2;
  Serial.println(String(count) + " Player");
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
      Serial.println(String(count) + " Player");
    }
  }
}
//-------------------------------------------------------------------ปุ่ม--------------------------------------------------------------------//
byte state_BUTTON () {
  for (byte i = 0; i < sizeof(BUTTON) / sizeof(BUTTON[0]); i++) {
    if (!digitalRead(BUTTON[i][0]) and BUTTON[i][1]) {
      BUTTON[i][1] = 0;
      if (i == 0) {
        ledcWriteTone(0, 4186);
        delay(50);
      } else {
        ledcWriteTone(0, 2093);
        delay(50);
      }
      ledcWriteTone(0, 0);
      return i + 1;
    } else if (digitalRead(BUTTON[i][0]) and !BUTTON[i][1]) {
      BUTTON[i][1] = 1;
    }
    delay(5);
  } return 0;
}
///------------------------------------------------------------------------ไฟ-------------------------------------------------------------------//
void LED(byte Adress, uint32_t color) {
  if (Adress == 0) {
    for (byte i = 0 ; i <= leds_ads[Adress]; i++) {
      leds[i] = color;
    }
  } else {

    for (byte ads = leds_ads[Adress - 1] + 1 ; ads <= leds_ads[Adress]; ads++) {
      leds[ads] = color;
    }
  }
  FastLED.show();
}
