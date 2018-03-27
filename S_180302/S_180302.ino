#include <Wire.h>										//go to exp
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <iarduino_RTC.h>
const uint8_t PIN_CS = 10;								// номер вывода CS адаптера SD ++
const uint8_t btn_watering = 3;							// номер входа, подключенный к кнопке "полив"
const uint8_t btn_view = 4;								// номер входа, подключенный к кнопке "показать"
int btn_state_watering = 0;								// переменная для хранения состояния кнопки "полив"
int btn_state_view = 0;									// переменная для хранения состояния кнопки "показать"
int time_m = 0;
int tm = 3;
File myFile;											// создаём переменную myFile
iarduino_RTC time(RTC_DS3231);
LiquidCrystal_I2C lcd(0x3F, 16, 2);						// Устанавливаем дисплей
void setup()
{
	time.begin();
	Wire.begin();
	pinMode(btn_watering, INPUT);						// инициализируем пин, подключенный к кнопке, как вход
	pinMode(btn_view, INPUT);							// инициализируем пин, подключенный к кнопке, как вход
	if (!SD.begin(PIN_CS)) myFile.println("SD-card not found");	// инициализация SD карты с указанием номера вывода CS  
	lcd.init();
	lcd.noBacklight();
}
void loop()
{
	myFile = SD.open("iarduino.txt", FILE_WRITE);
	btn_state_watering = digitalRead(btn_watering);		// считываем значения с входа кнопки "полив"
	if (btn_state_watering == HIGH)						// проверяем нажата ли кнопка
	{
		myFile.print(time.gettime("d-m-Y;H:i:s"));		// выводим время   
		myFile.println(";;; Watering;");				// пишем "Полив"
		delay(500);
		lcd.backlight();
		lcd.print(" -- Watering --");
		delay(3000);
		lcd.clear();
		lcd.noBacklight();
	}
		btn_state_view = digitalRead(btn_view);				// считываем значения с входа кнопки "показать"
	if (btn_state_view == HIGH)							// проверяем нажата ли кнопка
	{
		int measur_arr[3];
		int k;
		for (k = 0; k < 3; k = k + 1)
		{
			measur_arr[k] = measurement(k);
			lcd.backlight();
			lcd.print(measur_arr[k]);
			lcd.print("%;  ");
		}
		delay(3000);
		lcd.clear();
		lcd.noBacklight();
	}
	time.gettime();
	time_m = time.Hours;
	if ((time_m == 07 || time_m == 19) && time_m!= tm)									// если прошла 60 секунда
	{
		myFile.print(time.gettime("d-m-Y;H:i:s"));		// выводим время
		myFile.print(";");
		int measur_arr[3];
		int k;
		for (k = 0; k < 3; k = k + 1)
		{
			measur_arr[k] = measurement(k);
			myFile.print(measur_arr[k]);
			myFile.print(";");
		}
			myFile.println(";");
			tm = time_m;
	}
	myFile.close();									// закрываем файл
}
 int measurement(int i) {
	 int power[3] = { 5, 6, 7 };
	 int sensorPin[3] = { 2, 1, 0 };
	 int sensorValue;
	 pinMode(power[i], OUTPUT);
	 digitalWrite(power[i], HIGH);
	 delay(100);
	 sensorValue = analogRead(sensorPin[i]);
	 sensorValue = (1024 - sensorValue) / 10;
	 digitalWrite(power[i], LOW);
	 return sensorValue;
 }