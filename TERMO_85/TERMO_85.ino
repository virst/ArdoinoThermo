
//задействованные контакты ардуино
#define SCLK 2
#define RCLK 1
#define DIO 0

#define MONITOR_INTERVAL 60000

byte digitBuffer[4];

void setup() {
  pinMode(RCLK, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(DIO, OUTPUT);
}


float f = 2.1;
unsigned long time = 0;


void loop()
{
  ShowFloat(f);

  if ( time < millis() ) {
    ShowLine();
    f = GetTemp();
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
  digitalWrite(RCLK, LOW); // Открываем защелку

  shiftOut(DIO, SCLK, MSBFIRST, 0b10111111); // Отправляем байт с "числом"
  shiftOut(DIO, SCLK, MSBFIRST, 0b00001111); // выбираем разряд
  digitalWrite(RCLK, HIGH); // Защелкиваем регистры (завтавляем защелку выставить состояние выводов соответственно отправленным байтам)
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
    digitalWrite(RCLK, LOW); // Открываем защелку
    byte n = digit[digitBuffer[i]];
    if (i == 2) n = n & dot;
    shiftOut(DIO, SCLK, MSBFIRST, n); // Отправляем байт с "числом"
    shiftOut(DIO, SCLK, MSBFIRST, chr[i]); // выбираем разряд
    digitalWrite(RCLK, HIGH); // Защелкиваем регистры (завтавляем защелку выставить состояние выводов соответственно отправленным байтам)
    delay(1);
  }
}
