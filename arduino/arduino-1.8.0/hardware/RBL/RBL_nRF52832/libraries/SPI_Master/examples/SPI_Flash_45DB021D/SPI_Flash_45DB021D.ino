/*
 * Copyright (c) 2016 RedBear
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
/*
 * @brief Flash chip: 45DB021D
 */

#include <SPI_Master.h>

void Flash_Buff_WriteBytes(uint16_t addr, uint8_t *pbuf, uint16_t len) {
  uint16_t index;

  digitalWrite(10, LOW);
  delayMicroseconds(200);

  SPI_Master.transfer(0x84);
  SPI_Master.transfer(0x00);
  SPI_Master.transfer( (uint8_t)(addr>>8) );
  SPI_Master.transfer( (uint8_t)addr );
  for(index=0; index<len; index++) {
    SPI_Master.transfer(*pbuf);
    pbuf++;
  }

  delayMicroseconds(200);
  digitalWrite(10, HIGH);
}

void Flash_Buff_ReadBytes(uint16_t addr, uint8_t *pbuf, uint16_t len) {
  uint16_t index;

  digitalWrite(10, LOW);
  delayMicroseconds(200);

  SPI_Master.transfer(0xD1);
  SPI_Master.transfer(0x00);
  SPI_Master.transfer( (uint8_t)(addr>>8) );
  SPI_Master.transfer( (uint8_t)addr );
  for(index=0; index<len; index++) {
    *pbuf = SPI_Master.transfer(0x00);
    pbuf++;
  }

  delayMicroseconds(200);
  digitalWrite(10, HIGH);
}

uint8_t i;
uint8_t wt_buf[7] = {'H','e','l','l', 'o', '\r', '\n'};
uint8_t rd_buf[7];

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  Serial.println("SPI Start ");
  pinMode(D13, OUTPUT);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  // SPI_Master.begin();
  SPI_Master.begin(DEFAULT_SCK, DEFAULT_MOSI, DEFAULT_MISO);
  SPI_Master.setFrequency(SPI_4M);

  delay(1000);
  Flash_Buff_WriteBytes(0, wt_buf, 7);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);

  memset(rd_buf, 0x00, 7);
  Flash_Buff_ReadBytes(0, rd_buf, 7);
  for(i=0; i<7; i++)
    Serial.write(rd_buf[i]);
}

