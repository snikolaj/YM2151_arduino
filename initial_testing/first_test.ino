#include <SPI.h>

#define OE_595 2
#define OE_165 3
#define LAT_595 4

#define YM_CS 5
#define YM_WR 6
#define YM_RD 7
#define YM_A0 8

void setup() {
  Serial.begin(115200);

  pinMode(OE_595, OUTPUT);
  pinMode(OE_165, OUTPUT);
  pinMode(LAT_595, OUTPUT);

  pinMode(YM_CS, OUTPUT);
  pinMode(YM_WR, OUTPUT);
  pinMode(YM_RD, OUTPUT);
  pinMode(YM_A0, OUTPUT);


  digitalWrite(OE_595, HIGH);
  digitalWrite(OE_165, LOW);
  digitalWrite(LAT_595, LOW);


  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

}

void loop() {
  YMwrite(0xAA, 0x40);
  Serial.println(YMread(0x40));
  
  delay(1000);
}

void YMwrite(uint8_t data, uint8_t address){
  digitalWrite(YM_CS, LOW);
  YMaddr(address);
    
  digitalWrite(YM_WR, LOW); // write address to chip
  digitalWrite(YM_WR, HIGH);

  writeData(data);

  digitalWrite(YM_A0, HIGH);
  digitalWrite(YM_WR, LOW);
  digitalWrite(YM_WR, HIGH);
  digitalWrite(YM_CS, HIGH);
}

uint8_t YMread(uint8_t address){
  digitalWrite(YM_CS, LOW);
  YMaddr(address);
  
  digitalWrite(YM_RD, LOW); // write address to chip
  digitalWrite(YM_RD, HIGH);

  digitalWrite(OE_595, HIGH); // make sure no bus contention can happen with SN74HC595
  digitalWrite(YM_A0, HIGH);
  digitalWrite(YM_RD, LOW);

  uint8_t retval = readData(); // read with SN74HC165

  digitalWrite(YM_RD, HIGH);
  digitalWrite(YM_CS, HIGH); // return everything to normal state

  return retval;
}

void YMaddr(uint8_t address){
  digitalWrite(YM_A0, LOW); // low is address, high is data
  digitalWrite(YM_CS, LOW); // chip select
  
  writeData(address); // write address to bus

}

void writeData(uint8_t data){
  SPI.transfer(data);
  digitalWrite(LAT_595, HIGH);
  digitalWrite(LAT_595, LOW);
  digitalWrite(OE_595, LOW);
}

uint8_t readData(){
  digitalWrite(OE_165, HIGH); // latch in data on SN74HC165
  digitalWrite(OE_165, LOW);
  digitalWrite(OE_165, HIGH);

  return SPI.transfer(0);
}
