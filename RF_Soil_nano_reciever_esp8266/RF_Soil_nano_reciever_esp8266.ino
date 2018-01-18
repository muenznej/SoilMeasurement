#include <RH_ASK.h>

#define LED_TIME 50
#define TRANS_TIME 200 // blink time for the LED
#define LED_RECIEVING LED_BUILTIN
#define RX_PIN D1
#define TX_PIN D2
#define SOIL_SENSOR_POL1 D8
#define SOIL_SENSOR_POL2 D7
#define SOIL_SENSOR_READ A0

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

RH_ASK driver(2000, RX_PIN, TX_PIN);


void setup() {
  pinMode(SOIL_SENSOR_POL1, OUTPUT);
  pinMode(SOIL_SENSOR_POL2, OUTPUT);
  digitalWrite(SOIL_SENSOR_POL1, HIGH);
  digitalWrite(SOIL_SENSOR_POL2, HIGH);
  pinMode(SOIL_SENSOR_READ, INPUT);

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
  digitalWrite(SOIL_SENSOR_POL1, HIGH);
  digitalWrite(SOIL_SENSOR_POL2, LOW);
  //Serial.print("Moisture: ");
  // //Serial.println(measureSoil(), DEC);
  //Serial.println( analogRead( SOIL_SENSOR_READ ), DEC);
  //Serial.print("");

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
  delay(TRANS_TIME);
}

float measureSoil() {
  byte numIter = 10;
  float value = 0;
  for (byte i = 1; i <= numIter; i++) {
    //   if ( i%2 ) {
    pinMode(SOIL_SENSOR_POL2, OUTPUT);
    digitalWrite(SOIL_SENSOR_POL2, HIGH);
    delay(5);
    value += analogRead( SOIL_SENSOR_READ );
    //    }
    //    else {
    //      pinMode(SOIL_SENSOR_POL2, INPUT);
    //      pinMode(SOIL_SENSOR_POL1, OUTPUT);
    //      delay(5);
    //      value += analogRead( SOIL_SENSOR_POL2 );
  }
  value /= numIter;
  //value /= 4;
  return value;
}
