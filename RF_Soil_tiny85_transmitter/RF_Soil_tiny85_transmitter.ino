#define RH_SPEED 2000 // RadioHead bitrate in bit/s
#define TX_PIN 3
#define RX_PIN 4

#define LED_TIME 50 // blink time for the LED
#define TRANS_TIME 200 // blink time for the LED
#define BUZZER_PIN 1

#include <RH_ASK.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>

// some mcs (e.g atmega328) have WDTCSR instead of WTDCR
#if !defined WDTCR and defined WDTCSR
#define WDTCR WDTCSR
#endif

struct {
  byte soil_value;
  byte soil_concentration;
  byte soil_state;
  unsigned long counter;
} soil_data;
byte zize = sizeof(soil_data);
byte tx_buf[sizeof(soil_data)] = {0};
#define RH_BUF_LEN sizeof(soil_data)
#define RH_ASK_MAX_MESSAGE_LEN RH_BUF_LEN
RH_ASK driver(RH_SPEED, RX_PIN, TX_PIN);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(TX_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);
  
  driver.init();
  soil_data.soil_concentration = 100;
  soil_data.soil_state = 1;
  soil_data.soil_value = 255;
  soil_data.counter = 1;

}

void loop() {
  for (byte i=1;i<255;i++){
  digitalWrite(BUZZER_PIN,HIGH);
  _delay_us(500);
   digitalWrite(BUZZER_PIN,LOW);
   _delay_us(500);
  }
  
  memcpy(tx_buf, &soil_data, sizeof(soil_data) );
  if (driver.send((uint8_t *)tx_buf, zize)) {
    driver.waitPacketSent();
  }
  soil_data.counter++;
  _delay_ms(TRANS_TIME);
  // sleep bit patterns:
  //  1 second:  0b000110
  //  2 seconds: 0b000111
  //  4 seconds: 0b100000
  //  8 seconds: 0b100001
 
  for (int i = 1; i <= 2; i++) {
    myWatchdogEnable (0b000111);  // 8x2 second
  }

}

ISR(WDT_vect) {
  wdt_disable();
}

void myWatchdogEnable(const byte interval) {
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
