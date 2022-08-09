#include <Wire.h>  //libraries untuk pengaksesan i2c
#include <Adafruit_BME280.h> //libraries yang baru saja diinstall seperti cara diatas
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define SEALEVELPRESSURE_HPA (1013.25) //nilai awal untuk pressure
Adafruit_BME280 bme; //penggunaan I2C
File myFile;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  
  if (!bme.begin(0x76)) {
    Serial.print("No BME280!");
    lcd.setCursor(0, 0);
    lcd.print("No BME280!");
    while (1);
  }
  while (!SD.begin(10)) {
    Serial.print("No SD Card!");
    lcd.setCursor(0, 0);
    lcd.print("No SD Card!");
    // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("initialization done.");
  
  myFile = SD.open("test.txt", FILE_WRITE);
}

void loop() {
  //pembacaan data temperature atau suhu
  Serial.print("Suhu =");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.setCursor(0, 1);
  lcd.print(bme.readTemperature());

  delay(1000);

  //pembacaan data Kelembaban
  Serial.print("Kelembaban = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  lcd.setCursor(0, 0);
  lcd.print("Kelembapan: ");
  lcd.setCursor(0, 1);
  lcd.print(bme.readHumidity());

  delay(1000);

  //pembacaan data tekanan atmosfer
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  lcd.setCursor(0, 0);
  lcd.print("Kelembapan: ");
  lcd.setCursor(0, 1);
  lcd.print(bme.readPressure() / 100.0F);

  delay(1000);

  //pembacaan data ketinggian berdasarkan permukaan laut
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  lcd.setCursor(0, 0);
  lcd.print("Kelembapan: ");
  lcd.setCursor(0, 1);
  lcd.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  delay(1000);

  Serial.println();
  delay(1000);

  if (myFile) {
    myFile.close();
    myFile = SD.open("data.csv", FILE_WRITE);
    myFile.print(bme.readTemperature());
    myFile.print(',');
    myFile.print(bme.readHumidity());
    myFile.print(',');
    myFile.print(bme.readPressure() / 100.0F);
    myFile.print(',');
    myFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    myFile.println();
    myFile.close();
  } else {
    myFile = SD.open("data.csv", FILE_WRITE);
    myFile.print("t,h,p,alt");
    myFile.close();
  }
}
