#define RH_SPEED 2000 // RadioHead bitrate in bit/s
#define TX_PIN 3 // PB3
#define RX_PIN 4 // PB4 // not needed
#define BUTTON_PIN PB2
#define LED_TIME 50 // blink time for the LED
#define TRANS_TIME 200 // blink time for the LED
#define BUZZER_PIN 1 // PB1
#define SOIL_SENSOR_POL1 A2 //PB3
#define SOIL_SENSOR_POL2 A1 //PB4 // Hardwareinterupt0

#include <RH_ASK.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>

// some mcs (e.g atmega328) have WDTCSR instead of WTDCR
#if !defined WDTCR and defined WDTCSR
#define WDTCR WDTCSR
#endif
// watchdog intervals
// sleep bit patterns for WDTCSR
enum
{
  WDT_16_MS  =  0b000000,
  WDT_32_MS  =  0b000001,
  WDT_64_MS  =  0b000010,
  WDT_128_MS =  0b000011,
  WDT_256_MS =  0b000100,
  WDT_512_MS =  0b000101,
  WDT_1_SEC  =  0b000110,
  WDT_2_SEC  =  0b000111,
  WDT_4_SEC  =  0b100000,
  WDT_8_SEC  =  0b100001,
};

struct {
  byte soil_value = 255;
  byte soil_concentration = 100;
  byte soil_reference = 255;
  boolean soil_wet = true;
  unsigned long counter = 1;
} soil_data;

#define WATER_THRESHOLD 30 // 30%

byte zize = sizeof(soil_data);
byte tx_buf[sizeof(soil_data)] = {0};
#define RH_BUF_LEN sizeof(soil_data)
#define RH_ASK_MAX_MESSAGE_LEN RH_BUF_LEN
RH_ASK driver(RH_SPEED, RX_PIN, TX_PIN);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  //pinMode(RX_PIN, INPUT);

  driver.init();
  causeNoise();
  //setReference();
}

void setReference() {
  //soil_data.soil_reference = measureSoil();
  soil_data.soil_reference = 0; // for testing only
  causeNoise();
}

void causeNoise() { // some random noise, not optimized for resonance frequency
  for (byte i = 1; i < 127; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    _delay_us(500);
    digitalWrite(BUZZER_PIN, LOW);
    _delay_us(500);
  }
}
void processSensor() {
  soil_data.soil_value = measureSoil();
  soil_data.soil_concentration = (byte)(soil_data.soil_value / soil_data.soil_reference);
  soil_data.counter++;
  if ( soil_data.soil_concentration <= WATER_THRESHOLD ) {
    soil_data.soil_wet = false;
  }
  else {
    soil_data.soil_wet = true;
  }
}
void loop() {
  static boolean CheckBeep = false;
  soil_data.soil_value = measureSoil();

  soil_data.counter++;
  //processSensor();
  if ( !(soil_data.counter %= 10) ) {
    soil_data.soil_wet = false;
    CheckBeep = true;
  }


  memcpy(tx_buf, &soil_data, sizeof(soil_data) );
  pinMode(TX_PIN, OUTPUT); // needed because TX_PIN is also SOIL_SENSOR_POL1
  if (driver.send((uint8_t *)tx_buf, zize)) {
    driver.waitPacketSent();
  }

  _delay_ms(TRANS_TIME); // not needed?

  for (int i = 1; i <= 1; i++) {
    myWatchdogEnable (WDT_4_SEC);
  }

  if ( soil_data.soil_wet != true ) {
    if ( CheckBeep ) {
      causeNoise();
      CheckBeep = false;
      soil_data.soil_wet = true;
    }
  }
}
ISR(WDT_vect) {
  wdt_disable();
}

void myWatchdogEnable(const byte interval) {
  //attachInterrupt(0, setReference, FALLING); // use interrupt 0 (pin PB2) and run
  attachInterrupt(digitalPinToInterrupt(2), setReference, LOW ); // use interrupt 0 (pin PB2) and run
  noInterrupts();
  wdt_reset();

  MCUSR = 0;                          // reset various flags
  WDTCR |= 0b00011000;               // see docs, set WDCE, WDE
  WDTCR =  0b01000000 | interval;    // set WDIE, and appropriate delay

  ADCSRA &= ~_BV(ADEN);

  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_bod_disable();
  interrupts();
  sleep_mode();

}

byte measureSoil() {
  byte numIter = 10;
  unsigned short value = 0;
  //noInterrupts();
  detachInterrupt(0);
  for (byte i = 1; i <= numIter; i++) {
    if ( i%2 ) {
      pinMode(SOIL_SENSOR_POL1, INPUT);
      pinMode(SOIL_SENSOR_POL2, OUTPUT);
      _delay_ms(5);
      analogWrite( SOIL_SENSOR_POL2, 127 );
      value += analogRead( SOIL_SENSOR_POL1 );
    }
    else {
      pinMode(SOIL_SENSOR_POL2, INPUT);
      pinMode(SOIL_SENSOR_POL1, OUTPUT);
      _delay_ms(5);
      analogWrite( SOIL_SENSOR_POL1, 127 );
      value += analogRead( SOIL_SENSOR_POL2 );        
    }
     
    //interrupts();
    //      pinMode(SOIL_SENSOR_POL1, OUTPUT);
    //      pinMode(SOIL_SENSOR_POL2, INPUT); // use pull/push resistors
    //      _delay_ms(5);
    //      digitalWrite( SOIL_SENSOR_POL1, HIGH );
    //      value += analogRead( SOIL_SENSOR_POL2 );
    //    }
    //    else {
    //      pinMode(SOIL_SENSOR_POL1, INPUT); // use pull/push resistors
    //      pinMode(SOIL_SENSOR_POL2, OUTPUT);
    //      _delay_ms(5);
    //      digitalWrite( SOIL_SENSOR_POL2, HIGH );
    //      value += analogRead( SOIL_SENSOR_POL1 );
    //    }
  }
  
  pinMode(SOIL_SENSOR_POL1, OUTPUT);
  pinMode(SOIL_SENSOR_POL1, LOW);
  pinMode(SOIL_SENSOR_POL2, OUTPUT);
  pinMode(SOIL_SENSOR_POL2, LOW);
  
  value /= numIter;
  value /= 4; // roughly remap 1024 DAC values to a 255 (byte)
  return (byte)value;
}
