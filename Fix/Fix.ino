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
