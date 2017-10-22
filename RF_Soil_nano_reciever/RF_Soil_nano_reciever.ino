#include <RH_ASK.h>
//#include <SPI.h>

#define LED_TIME 50
#define TRANS_TIME 200 // blink time for the LED
#define LED_RECIEVING 4

struct {
  byte soil_value;
  byte soil_concentration;
  byte soil_state;
  unsigned long counter;
} soil_data;
byte zize = sizeof(soil_data);
byte rx_buf[sizeof(soil_data)] = {0};
#define RH_BUF_LEN sizeof(soil_data)
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
      soil_data.soil_concentration = 0;
    soil_data.soil_state = 0;
    soil_data.soil_value = 0;
}

void loop() {
  digitalWrite(LED_RECIEVING, LOW);
  if (driver.recv(rx_buf, &zize)) {
    driver.printBuffer("Received:", rx_buf, zize);
    memcpy(&soil_data, rx_buf, zize );
    digitalWrite(LED_RECIEVING, HIGH);

    //    _delay_ms(1000);
    Serial.println(soil_data.soil_concentration, DEC);
    Serial.println(soil_data.soil_value, DEC);
    Serial.println(soil_data.soil_state, DEC);
    Serial.println(soil_data.counter, DEC);
    _delay_ms(TRANS_TIME);
    //    _delay_ms(1000);
  }
  //  else {
  //    driver.printBuffer("Nothing recieved:", buf, buflen);
  //  }
}

