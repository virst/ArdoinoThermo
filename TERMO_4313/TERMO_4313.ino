
//задействованные контакты ардуино
int leds[8] = {9, 11, 13, 15, 16, 10, 12, 14};
int zone[4] = {4, 5, 6, 7};

#define MONITOR_INTERVAL 1000

byte digitBuffer[4];

void setup() {
  for (int i = 0; i < 8; i++) pinMode(leds[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(zone[i], OUTPUT);
}


float f = 1.1;
unsigned long time = 0;


void loop()
{
  ShowFloat(f);

  if ( time < millis() ) {
    ShowLine();
    f = GetTemp();
    //f=f+1;
    time = millis() + MONITOR_INTERVAL;
  }


}

void ShowFloat(float f)
{
  int f2 = ((int)(f * 10));
  digitBuffer[3] = f2 % 10; f2 = f2 / 10;
  digitBuffer[2] = f2 % 10; f2 = f2 / 10;
  digitBuffer[1] = f2 % 10; f2 = f2 / 10;
  if (f < 0) digitBuffer[0] = 11;
  else digitBuffer[0] = 10;
  if (digitBuffer[1] == 0)
  {
    digitBuffer[1] = digitBuffer[0];
    digitBuffer[0] = 10;
  }

  showDisplay();
}

void ShowLine()
{
  byte n = 0b10111111;
  for (int j = 0; j < 4; j++) digitalWrite(zone[j], HIGH );
  for (int j = 0; j < 8; j++)  digitalWrite(leds[j], ((n >> j) & 1u) ? HIGH : LOW );
  delay(1);
}

void showDisplay() {
  const byte digit[12] = { //маска для цифры
    0b11000000,//0
    0b11111001,//1
    0b10100100,//2
    0b10110000,//3
    0b10011001,//4
    0b10010010,//5
    0b10000010,//6
    0b11111000,//7
    0b10000000,//8
    0b10010000,//9
    0b11111111,//10(non)
    0b10111111,//11(-)
  };
  const byte chr[4] = { // маска для разряда
    0b00001000,//1
    0b00000100,//2
    0b00000010,//3
    0b00000001,//4

  };

  const byte dot = 0b01111111;


  //выполняем один проход по всем цифрам
  for (byte i = 0; i <= 3; i++)
  {

    byte n = digit[digitBuffer[i]];
    if (i == 2) n = n & dot;
    for (int j = 0; j < 4; j++) digitalWrite(zone[j], LOW );
    for (int j = 0; j < 8; j++)  digitalWrite(leds[j], ((n >> j) & 1u) ? HIGH : LOW );
    for (int j = 0; j < 4; j++) digitalWrite(zone[j], (j == i) ? HIGH : LOW );
    delay(1);
  }
}
