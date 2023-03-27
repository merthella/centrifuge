#include <LiquidCrystal_I2C.h>
#include <ESC.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
ESC myESC;

int motorDevri = 0;
int dakika = 0;
int dakikaKalan = 0;
int startButton = 6;
int arttirmaButton = 7;
int azaltmaButton = 8;
int buzzer = 9;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Hoş Geldiniz!");
  delay(1000);
  lcd.clear();
  lcd.print("Sistem kontrol ediliyor");
  delay(5000);
  lcd.clear();
  pinMode(startButton, INPUT_PULLUP);
  pinMode(arttirmaButton, INPUT_PULLUP);
  pinMode(azaltmaButton, INPUT_PULLUP);
  myESC.attach(10); // Opto-ESC sinyali için kullanılan pin
  pinMode(buzzer, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Motor Devri:");
  lcd.setCursor(0, 1);
  lcd.print(motorDevri);
  while (digitalRead(startButton) == HIGH) {
    if (digitalRead(arttirmaButton) == LOW) {
      motorDevri += 100;
      if (motorDevri > 10000) {
        motorDevri = 10000;
      }
      lcd.setCursor(0, 1);
      lcd.print(motorDevri);
      delay(500);
    }
    if (digitalRead(azaltmaButton) == LOW) {
      motorDevri -= 100;
      if (motorDevri < 0) {
        motorDevri = 0;
      }
      lcd.setCursor(0, 1);
      lcd.print(motorDevri);
      delay(500);
    }
  }
  lcd.clear();
  lcd.print("Dakika:");
  lcd.setCursor(0, 1);
  lcd.print(dakika);
  while (digitalRead(startButton) == HIGH) {
    if (digitalRead(arttirmaButton) == LOW) {
      dakika += 1;
      if (dakika > 60) {
        dakika = 60;
      }
      lcd.setCursor(0, 1);
      lcd.print(dakika);
      delay(500);
    }
    if (digitalRead(azaltmaButton) == LOW) {
      dakika -= 1;
      if (dakika < 0) {
        dakika = 0;
      }
      lcd.setCursor(0, 1);
      lcd.print(dakika);
      delay(500);
    }
  }
  lcd.clear();
  lcd.print("Onayla: *Baslat");
  while (digitalRead(startButton) == HIGH) {
    if (digitalRead(arttirmaButton) == LOW) {
      int pwm = map(motorDevri, 0, 10000, 1000, 2000);
      myESC.writeMicroseconds(pwm); // Opto-ESC'yi belirtilen hızda çalıştır
      dakikaKalan = dakika;
      while (dakikaKalan > 0) {
        lcd.setCursor(0, 0);
        lcd.print("Dakika: ");
        lcd.setCursor(8, 0);
        lcd.print(dakikaKalan);
        delay(60000);
        dakikaKalan--;
      }
      for (int i = 2000; i >= 1000; i -= 10) {
        myESC.writeMicroseconds(i); // Opto-ESC'yi yavaşlatarak durdur
        delay(10);
      }
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(500);
    }
  }
  motorDevri = 0;
  dakika = 0;
  lcd.clear();
}