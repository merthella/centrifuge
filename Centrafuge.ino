#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD ekran ve Servo nesneleri oluşturuluyor
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

// Değişkenler tanımlanıyor
int motorDevri = 0;
int dakika = 0;
int dakikaKalan = 0;
int startButton = 6;
int arttirmaButton = 7;
int azaltmaButton = 8;
int buzzer = 9;

void setup() {
  // LCD ekranı başlatılıyor ve sistem hazırlanıyor mesajı gösteriliyor
  lcd.init();
  lcd.backlight();
  lcd.print("Hoş Geldiniz!");
  delay(1000);
  lcd.clear();
  lcd.print("Sistem kontrol ediliyor");
  delay(5000);
  lcd.clear();

  // Butonlar ve Servo için gerekli pinler tanımlanıyor
  pinMode(startButton, INPUT_PULLUP);
  pinMode(arttirmaButton, INPUT_PULLUP);
  pinMode(azaltmaButton, INPUT_PULLUP);
  myServo.attach(10); // Servo sinyali için kullanılan pin
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // Motor devri seçimi yapılıyor
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

  // Dakika seçimi yapılıyor
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

  // Başlatma butonu bekleniyor
  lcd.clear();
  lcd.print("Onayla: *Baslat");
  while (digitalRead(startButton) == HIGH) {
    if (digitalRead(arttirmaButton) == LOW) {
      // Motor hızı ayarlanıyor ve geri sayım başlatılıyor
      int servoValue = map(motorDevri, 0, 10000, 0, 180);
      myServo.write(servoValue); // Servo'yu belirtilen pozisyona getir
      dakikaKalan = dakika;
      while (dakikaKalan > 0) {
        lcd.setCursor(0, 0);
        lcd.print("Dakika: ");
        lcd.setCursor(8, 0);
        lcd.print(dakikaKalan);
        delay(60000);
        dakikaKalan--;
      }

      // Motor yavaşlatılıyor ve sesli uyarı veriliyor
      for (int i = 180; i >= 0; i -= 1) {
        int servoValue = map(i, 0, 180, 0, 180);
        myServo.write(servoValue); // Servo'yu yavaşlatarak durdur
        delay(10);
      }
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(500);
}
}

// Değişkenler sıfırlanıyor ve LCD ekran temizleniyor
motorDevri = 0;
dakika = 0;
lcd.clear();
}

