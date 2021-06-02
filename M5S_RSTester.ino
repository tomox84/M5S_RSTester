#include "TMFutabaRS.h"
#include <M5Stack.h>

// Serial2(M5StackはSerial1が使えない)
const int PinRX = 16; // From BTE094 TXD (Blue)
const int PinTX = 17; // From BTE094 RXD (GREEN)

// サーボの定義
TMFutabaRS srvOut(1);
TMFutabaRS srvIn(6);

// 動作モード
int OpeMode = 1;

//-- Setup --
void setup() {
  M5.begin();
  M5.Power.begin();

  // サーボ制御用
  Serial2.begin(115200, SERIAL_8N1, PinRX, PinTX);

  // サーボにサーボ制御用シリアルをセット
  srvOut.begin(&Serial2);
  srvIn.begin(&Serial2);

  // 画面設定
  M5.Lcd.setRotation(0);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);

  // 画面表示
  DispBack();

  // サーボPos(位置)のリセット
  ResetPos();
}

void loop() {
  // サーボ操作モード
  if (OpeMode == 1) {
    ModeServo();
  }
  else if (OpeMode == 2) {
    ModeIDSelect();
  }

  M5.update();
  delay(50);
}

// サーボPos(位置)のリセット
void ResetPos(void) {
  // トルクオン
  srvIn.TorqueOn();
  srvOut.TorqueOn();
  // 0度へ移動
  srvIn.Move(0, 0);
  srvOut.Move(0, 0);
  // Goalへの移動を待つ
  srvIn.WaitGoal(5);

  // トルクオフ
  srvIn.TorqueOff();
}

// サーボ操作モード
void ModeServo(void) {
  // 移動させるか
  boolean isMove = false;

  // --srvIn 情報表示
  M5.Lcd.setCursor(30, 130);
  if (srvIn.MemoryMap42to59()) {
    int pos = (int)(srvIn.CurrentPosition) / 10;
    if (pos > 150) M5.Lcd.print("Over range ");
    else if (pos < -150) M5.Lcd.print("Under range");
    else {
      M5.Lcd.printf("%4d deg   ", pos);
      isMove = true;
    }
  }
  else {
    M5.Lcd.print("N/A        ");
  }

  // 入力が範囲内ならサーボ移動
  if (isMove) srvOut.Move(srvIn.CurrentPosition, 0);

  // --srvOut 情報表示
  M5.Lcd.setCursor(30, 210);
  if (srvOut.MemoryMap42to59()) {
    int pos = (float)(srvOut.CurrentPosition) / 10;
    M5.Lcd.printf("%4d deg  %3d mA", pos, srvOut.CurrentCurrent);
  }
  else {
    M5.Lcd.print("N/A             ");
  }

  // ボタン
  if (M5.BtnC.wasPressed()) {
    // OpeModeを2に変更(ID選択モードへ)
    OpeMode = 2;
    DispBack();
  }
}

// ID選択モード
void ModeIDSelect(void) {
  // トルクオフ
  srvOut.TorqueOff();
  
  // ボタン入力
  if (M5.BtnA.wasPressed()) {
    // IDを増やす
    unsigned char newId = srvOut.Id() + 1;
    if (newId > 99) newId = 99;
    srvOut.ChangeId(newId);
    DispChangedID();
  }
  if (M5.BtnB.wasPressed()) {
    // IDを減らす
    unsigned char newId = srvOut.Id() - 1;
    if (newId < 1) newId = 1;
    srvOut.ChangeId(newId);
    DispChangedID();
  }
  if (M5.BtnC.wasPressed()) {
    // OpeModeを1に変更(サーボ操作モードへ)
    OpeMode = 1;
    DispBack();
    ResetPos();
  }
}

// 画面の表示
void DispBack(void) {
  if (OpeMode == 1) {
    // タイトル
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.fillRect(0, 0, 240, 30, WHITE);
    M5.Lcd.setCursor(30, 8);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.print("RS Servo Tester");

    // ボタンのメニュー表示
    M5.Lcd.setTextColor(BLUE, WHITE);
    M5.Lcd.fillRect(1, 226, 30, 60, WHITE);
    M5.Lcd.setCursor(5, 249);
    M5.Lcd.print("ID");

    // ID
    M5.Lcd.setTextColor(WHITE, BLUE);
    M5.Lcd.setCursor(30, 100);
    M5.Lcd.printf("in ID:%2d", srvIn.Id());
    M5.Lcd.setCursor(30, 180);
    M5.Lcd.printf("outID:%2d", srvOut.Id());
  }
  if (OpeMode == 2) {
    // タイトル
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.fillRect(0, 0, 240, 30, WHITE);
    M5.Lcd.setCursor(50, 8);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.print("ID Select");

    // ボタンのメニュー表示
    M5.Lcd.setTextColor(BLUE, WHITE);
    M5.Lcd.fillRect(1, 34, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 57);
    M5.Lcd.print(">>");

    M5.Lcd.fillRect(1, 130, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 153);
    M5.Lcd.print("<<");

    M5.Lcd.fillRect(1, 226, 30, 60, WHITE);
    M5.Lcd.setCursor(5, 249);
    M5.Lcd.print("Do");

    // ID表示
    M5.Lcd.fillRect(78, 132, 92, 54, WHITE);
    DispChangedID();
  }
}

// 変更したID表示
void DispChangedID(void) {
  M5.Lcd.setTextColor(BLUE, WHITE);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(90, 144);
  M5.Lcd.printf("%3d", srvOut.Id());
  M5.Lcd.setTextSize(2);
}
