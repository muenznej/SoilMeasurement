#include <RH_ASK.h>
//#include <SPI.h>

#define LED_TIME 50
#define TRANS_TIME 200 // blink time for the LED
#define LED_RECIEVING 4

#define RH_BUF_LEN 4
#define RH_ASK_MAX_MESSAGE_LEN RH_BUF_LEN
uint8_t soil_data[RH_BUF_LEN] = { 0x00, 0x00, 0x00, 0x00};
uint8_t rh_id = 0;

RH_ASK driver(2000, 11, 12);


void setup() {
  pinMode(LED_RECIEVING, OUTPUT);
  Serial.begin(9600);
  if (!driver.init()) {
    Serial.println("init failed");
  }
  else {
    Serial.println("init successful");
  }
}

void loop() {
  digitalWrite(LED_RECIEVING, LOW);
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver.recv(buf, &buflen)) {
    driver.printBuffer("Received:", buf, buflen);
    memcpy(&soil_data, buf, buflen );
    digitalWrite(LED_RECIEVING, HIGH);
    
//    _delay_ms(1000);
    Serial.println(soil_data[0], DEC);
    Serial.println(soil_data[1], DEC);
    Serial.println(soil_data[2], DEC);
    Serial.println(soil_data[3], DEC);
    _delay_ms(TRANS_TIME);
//    _delay_ms(1000);
  }
//  else {
//    driver.printBuffer("Nothing recieved:", buf, buflen);
//  }
}

