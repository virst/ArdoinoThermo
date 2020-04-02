#include <OneWire.h>

OneWire ds(3); // на пине 10 (нужен резистор 4.7 КОм)

float GetTemp()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  if ( !ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return -999;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    return -888;
  }

  // первый байт определяет чип
  switch (addr[0]) {
    case 0x10:
      // Serial.println(" Chip = DS18S20"); // или более старый DS1820
      type_s = 1;
      break;
    case 0x28:
      //  Serial.println(" Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
      //  Serial.println(" Chip = DS1822");
      type_s = 0;
      break;
    default:
      //   Serial.println("Device is not a DS18x20 family device.");
      return -777;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44); // начинаем преобразование, используя ds.write(0x44,1) с "паразитным" питанием
  delay(1000); // 750 может быть достаточно, а может быть и не хватит
  // мы могли бы использовать тут ds.depower(), но reset позаботится об этом
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  for ( i = 0; i < 9; i++) { // нам необходимо 9 байт
    data[i] = ds.read();
  }

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // разрешение 9 бит по умолчанию
    if (data[7] == 0x10) {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // при маленьких значениях, малые биты не определены, давайте их обнулим
    if (cfg == 0x00) raw = raw & ~7; // разрешение 9 бит, 93.75 мс
    else if (cfg == 0x20) raw = raw & ~3; // разрешение 10 бит, 187.5 мс
    else if (cfg == 0x40) raw = raw & ~1; // разрешение 11 бит, 375 мс
    //// разрешение по умолчанию равно 12 бит, время преобразования - 750 мс
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
 ds.reset_search();
  return celsius;
}
