#include <RH_ASK.h>

#define LED_TIME 50
#define TRANS_TIME 200 // blink time for the LED
#define LED_RECIEVING 4

struct {
  byte soil_value = 255;
  byte soil_concentration = 100;
  byte soil_reference = 255;
  boolean soil_wet = true;
  unsigned long counter = 1;
} soil_data;

byte zize = sizeof(soil_data);
byte rx_buf[sizeof(soil_data)] = {0};
#define RH_BUF_LEN sizeof(soil_data)
#define RH_ASK_MAX_MESSAGE_LEN RH_BUF_LEN

RH_ASK driver(2000, 11, 12);


void setup() {
  pinMode(LED_RECIEVING, OUTPUT);
  Serial.begin(2400);
  if (!driver.init()) {
    Serial.println("init failed");
  }
  else {
    Serial.println("init successful");
  }
}

void loop() {

  digitalWrite(LED_RECIEVING, LOW);
  if (driver.recv(rx_buf, &zize)) {
    driver.printBuffer("Received:", rx_buf, zize);
    memcpy(&soil_data, rx_buf, zize );
    digitalWrite(LED_RECIEVING, HIGH);
    byte soil_value = 255;
    byte soil_concentration = 100;
    byte soil_reference = 255;
    boolean soil_wet = true;
    unsigned long counter = 1;
    Serial.print("Moisture: ");
    Serial.println(soil_data.soil_concentration, DEC);
    Serial.print("ADC value: ");
    Serial.println(soil_data.soil_value, DEC);
    Serial.print("ADC Reference: ");
    Serial.println(soil_data.soil_reference, DEC);
    Serial.print("Counter: ");
    Serial.println(soil_data.counter, DEC);
    Serial.print("Wet?: ");
    Serial.println(soil_data.soil_wet, DEC);
    Serial.println("");
  }
  _delay_ms(TRANS_TIME);
}

