#include "TMFutabaRS.h"
#include <M5Stack.h>

// Serial2(M5StackはSerial1が使えない)
const int PinRX = 16; // From BTE094 TXD (Blue)
const int PinTX = 17; // From BTE094 RXD (GREEN)

// サーボの定義
int SrvID = 1;
TMFutabaRS servo(SrvID);

void setup() {
  M5.begin();
  M5.Power.begin();

  // サーボ制御用
  Serial2.begin(115200, SERIAL_8N1, PinRX, PinTX);

  // サーボにサーボ制御用シリアルをセット
  servo.begin(&Serial2);

  // トップメッセージ
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(BLUE);

    // 下部文字の表示
  M5.Lcd.fillRect(0, 224, 320, 16, WHITE);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.setCursor(28 + 1, 224 + 1);
  M5.Lcd.print("-- Press Any Buttom --");

    // トルクオン
  servo.TorqueOn();
  // 0度へ移動
  servo.Move(0, 10);
}

void loop() {
  // put your main code here, to run repeatedly:

}
