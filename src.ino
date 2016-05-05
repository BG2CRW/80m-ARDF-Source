#include <Wire.h>
byte address[] = {0, 1, 2, 3, 4, 5, 6, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
byte data[] = {0x01, 0x00, 0xb4, 0x41, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xff, 0xfc, 0xf2, 0x47, 0xff, 0xfc, 0xf2, //3.50  0
               0x01, 0x00, 0xb4, 0x40, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xf8, 0xa9, 0x92, 0x47, 0xf8, 0xa9, 0x92, //3.51  1
               0x01, 0x00, 0xb4, 0x40, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xdc, 0x09, 0x12, 0x47, 0xdc, 0x09, 0x12, //3.52  2
               0x01, 0x00, 0xb4, 0x40, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xdc, 0xa9, 0xb2, 0x47, 0xdc, 0xa9, 0xb2, //3.53  3
               0x01, 0x00, 0xb4, 0x40, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xfa, 0xcb, 0xb2, 0x47, 0xfa, 0xcb, 0xb2, //3.54  4
               0x01, 0x00, 0xb4, 0x3e, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0x89, 0x92, 0x92, 0x47, 0x89, 0x92, 0x92, //3.55  5
               0x01, 0x00, 0xb4, 0x3f, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xe9, 0xa8, 0xa2, 0x47, 0xe9, 0xa8, 0xa2, //3.56  6
               0x01, 0x00, 0xb4, 0x3e, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xe6, 0x95, 0x92, 0x47, 0xe6, 0x95, 0x92, //3.57  7
               0x01, 0x00, 0xb4, 0x3f, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xea, 0xf9, 0xf2, 0x47, 0xea, 0xf9, 0xf2, //3.58  8
               0x01, 0x00, 0xb4, 0x38, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0x9d, 0x17, 0x12, 0x07, 0x9d, 0x17, 0x12, //3.59  9
               0x01, 0x00, 0xb4, 0x3f, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xf8, 0x1b, 0xd2, 0x47, 0xf8, 0x1b, 0xd2, //3.60  MO
               0x01, 0x00, 0xb4, 0x01, 0x02, 0x50, 0x40, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x02, 0x00, 0x00, 0xbf, 0xa0, 0x03, 0x09, 0xbf, 0xa0, 0x03
              };//144
byte CDCE913_I2C_ADDRESS = 0x65;
int time = 100;
int choose;
int voltage;
int n = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  Wire.begin();
  Serial.begin(9600);

  choose = digitalRead(5) + digitalRead(6) * 2 + digitalRead(7) * 4 + digitalRead(8) * 8;
  if (choose > 10) choose = 10;

  for (int i = 3; i < 22; i++)
  {
    byte addr = address[i];
    byte dat = data[i + choose * 22];
    CDCE913_write(addr, dat);
  }

}

void loop() {

  Serial.println(choose);
  Serial.println(CDCE913_read(30), HEX);
  voltage = analogRead(A2); //637---4.07v,3.7v 582
  Serial.println(voltage);
  speak(choose);
  if (voltage < 582)
  {
    n++;
    if (n == 10)
    {
      digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
      digitalWrite(4, LOW);
      while (1);
    }
  }
}
byte CDCE913_read(byte addr)
{
  Wire.beginTransmission(CDCE913_I2C_ADDRESS);
  Wire.write(addr | 0x80);
  Wire.endTransmission();

  Wire.requestFrom(CDCE913_I2C_ADDRESS, 1);
  while (!Wire.available())    ;
  return Wire.read();
}

byte CDCE913_write(byte addr, byte data)
{
  Wire.beginTransmission(CDCE913_I2C_ADDRESS);
  Wire.write(addr | 0X80);
  Wire.write(data);
  Wire.endTransmission(true);

  return 1;
}
void DASH()
{
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(4, HIGH);
  delay(4 * time);            // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(4, LOW);
  delay(2 * time);
}
void DOT()
{
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(4, HIGH);
  delay(1.2*time);              // wait for a second
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(4, LOW);
  delay(1.5* time);
}
void Longrest()
{
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(4, LOW);
  delay(3 * time);
}
void speak(int way)
{
  if (way == 0)
  {
    DASH();
    DASH();
    DASH();
    DASH();
    DASH();
    Longrest();
    Longrest();
  }

  if (way == 1)
  {
    DASH();
    DASH();
    Longrest();
    DASH();
    DASH();
    DASH();
    Longrest();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 2)
  {
    DASH();
    DASH();
    Longrest();
    DASH();
    DASH();
    DASH();
    Longrest();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 3)
  {
    DASH();
    DASH();
    Longrest();
    DASH();
    DASH();
    DASH();
    Longrest();
    DOT();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 4)
  {
    DASH();
    DASH();
    Longrest();
    DASH();
    DASH();
    DASH();
    Longrest();
    DOT();
    DOT();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 5)
  {
    DASH();
    DASH();
    Longrest();
    DASH();
    DASH();
    DASH();
    Longrest();
    DOT();
    DOT();
    DOT();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 6)
  {
    DASH();
    DOT();
    DOT();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 7)
  {
    DASH();
    DASH();
    DOT();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 8)
  {
    DASH();
    DASH();
    DASH();
    DOT();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 9)
  {
    DASH();
    DASH();
    DASH();
    DASH();
    DOT();
    Longrest();
    Longrest();
  }
  if (way == 10)
  {
    DASH();
    DASH();
    Longrest();
    DASH();
    DASH();
    DASH();
    Longrest();
    Longrest();
  }
}
