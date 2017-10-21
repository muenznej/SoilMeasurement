#include <RH_ASK.h>
//#include <SPI.h>

#define LED_TIME 50
#define TRANS_TIME 200
#define LED_RECIEVING 4

struct dataStruct {
  uint8_t soil_value ; // 1 Byte
//  uint8_t soil_state; // 1 Byte
//  uint8_t soil_conc; // 1 Byte
//  uint16_t counter; // 2 Byte
} soil_data;

uint8_t tx_buf[sizeof(soil_data)] = {0};

#define RH_BUF_LEN 1 // 1+1+1+2
#define RH_ASK_MAX_MESSAGE_LEN RH_BUF_LEN
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
 // uint8_t buflen = sizeof(buf);
uint8_t buflen = sizeof(buf);
  //if (driver.recv(buf, &buflen)) {
    driver.printBuffer("Received:", buf, buflen);
    memcpy(&soil_data, buf, sizeof(buf) );
      _delay_ms(200);
    digitalWrite(LED_RECIEVING, HIGH);
    Serial.println(soil_data.soil_conc);
    _delay_ms(200);
//    Serial.println("Soil-Data:");
//    Serial.print("#");
//    Serial.print(soil_data.counter);
//    Serial.println("-----------");
//    Serial.println("Soil ADC Value");
//    Serial.println(soil_data.soil_value);
//    Serial.println("Soil State:");
//    Serial.println(soil_data.soil_state);
//    Serial.println("Water-Concentration:");
//    Serial.println(soil_data.soil_conc);
//    Serial.println("");
//    Serial.println("");
  

}

