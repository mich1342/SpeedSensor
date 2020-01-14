//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

  This program will test the LCD panel and the buttons
  Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


const int eeAddress = 0;
#include <EEPROM.h>
int Lenght;
int prev_Lenght = 0;
// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 200)  return btnUP;
  if (adc_key_in < 350)  return btnDOWN;
  if (adc_key_in < 500)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
  */


  return btnNONE;  // when all others fail, return this...
}

int delayButton = 300;
int state = 0;
int prev_state = 9999;

float dt1, dt2, T = 0;

int batas = 350;
#define pinA1 A1
void setup()
{
  pinMode(A1, INPUT);
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(5, 0);
  lcd.print("Group 4"); // print a simple message
  lcd.setCursor(7, 1);
  lcd.print("v1");
  delay(2000);
  Lenght = EEPROM.read(eeAddress);
}

void loop()
{
  lcd_key = read_LCD_buttons();

  switch (state) {
    case 0:
      if (state != prev_state) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Run");
        prev_state = state;
      }
      if ((lcd_key == btnRIGHT) || (lcd_key == btnLEFT)) {
        state = 1;
        delay(delayButton);
      }
      if (lcd_key == btnSELECT) {
        state = 100;
        delay(delayButton);
      }
      break;

    case 1:
      if (state != prev_state) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Settings");
        prev_state = state;
      }

      if ((lcd_key == btnRIGHT) || (lcd_key == btnLEFT)) {
        state = 0;
        delay(delayButton);
      }

      if (lcd_key == btnSELECT) {
        state = 2;
        delay(delayButton);
      }
      break;

    case 2:
      if (state != prev_state) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Settings");
        lcd.setCursor(0, 1);
        lcd.print("-> Length");
        prev_state = state;
      }
      if ((lcd_key == btnRIGHT) || (lcd_key == btnLEFT)) {
        state = 3;
        delay(delayButton);
      }
      if (lcd_key == btnSELECT) {
        state = 4;
        delay(delayButton);
      }
      break;

    case 3:
      if (state != prev_state) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Settings");
        lcd.setCursor(0, 1);
        lcd.print("-> Software");
        prev_state = state;
      }
      if (lcd_key == btnSELECT) {
        state = 5;
        delay(delayButton);
      }
      if ((lcd_key == btnRIGHT) || (lcd_key == btnLEFT)) {
        state = 2;
        delay(delayButton);
      }
      break;

    case 4:
      if ((state != prev_state) || (lcd_key == btnUP) || (lcd_key == btnDOWN) || (Lenght != prev_Lenght)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Lenght :");
        lcd.setCursor(1, 1);
        lcd.print(Lenght);
        lcd.setCursor(10, 1);
        lcd.print("mm");

        updateLenght();
        prev_state = state;
      }
      if (lcd_key == btnUP) {
        Lenght = Lenght + 1;
        delay(delayButton);
      }
      if ((lcd_key == btnDOWN) && (Lenght > 0)) {
        Lenght = Lenght - 1;

        delay(delayButton);
      }
      if (lcd_key == btnSELECT) {
        state = 0;

        delay(delayButton);
      }

      break;

    case 5:
     if (state != prev_state) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Software");
        lcd.setCursor(0, 1);
        lcd.print(" v1.0");
        prev_state = state;
      }
      if (lcd_key == btnSELECT) {
        state = 0;
        delay(delayButton);
      }
      break;

    case 100:
      lcd.clear();
      lcd.print("Waiting...");
      if (analogRead(pinA1) < 10) {
        dt1 = micros();
        state = 101;
      }
      break;

    case 101:
      if (analogRead(pinA1) > 10) {
        dt2 = micros();
        if (dt2 < dt1) {
          dt2 = dt2 + dt1;
        }
        state = 102;
      }
      break;

    case 102:
      if (state != prev_state) {
        dt1 = dt1 / 1000000;
        dt2 = dt2 / 1000000;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Result :");
        lcd.setCursor(0, 1);
        T = (Lenght) / ((dt2 - dt1) * 1000);
        lcd.print(T);
        lcd.setCursor(6, 1);
        lcd.print("m/s");
        prev_state = state;
      }

      if (lcd_key == btnSELECT) {
        state = 0;
        delay(delayButton);
      }
  }
}

void updateLenght() {
  if (Lenght != prev_Lenght) {
    if (Lenght <= 0) {
      Lenght = 0;
    }
    EEPROM.write(eeAddress, Lenght);
    prev_Lenght = Lenght;
  }
}
