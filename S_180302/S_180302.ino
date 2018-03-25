#include <Wire.h>										//��������� spi 180318 - �������� ������ ������
#include <iarduino_RTC.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
const uint8_t PIN_CS = 10;								// ����� ������ CS �������� SD ++
const uint8_t btn_watering = 3;							// ����� �����, ������������ � ������ "�����"
const uint8_t btn_view = 4;								// ����� �����, ������������ � ������ "��������"

int btn_state_watering = 0;								// ���������� ��� �������� ��������� ������ "�����"
int btn_state_view = 0;									// ���������� ��� �������� ��������� ������ "��������"
int time_s = 0;
int tm = 0;

File myFile;											// ������ ���������� myFile
iarduino_RTC time(RTC_DS3231);
LiquidCrystal_I2C lcd(0x3F, 16, 2);						// ������������� �������

void setup()
{
	time.begin();
	Wire.begin();
	pinMode(btn_watering, INPUT);						// �������������� ���, ������������ � ������, ��� ����
	pinMode(btn_view, INPUT);							// �������������� ���, ������������ � ������, ��� ����
	
	Serial.begin(9600);									// ��������� ����
	while (!Serial) { ; }
	Serial.println("open my port");
	if (!SD.begin(PIN_CS))								// ������������� SD ����� � ��������� ������ ������ CS  
	{
		myFile.println("SD-card not found");			// ������ ������������� �����
		Serial.println("SD-card not found");
	}					 
			lcd.init();
			lcd.noBacklight();
}

void loop()
{
	myFile = SD.open("iarduino.txt", FILE_WRITE);

	btn_state_watering = digitalRead(btn_watering);		// ��������� �������� � ����� ������ "�����"
	if (btn_state_watering == HIGH)						// ��������� ������ �� ������
	{
		Serial.println(";;; Watering;");
		myFile.print(time.gettime("d-m-Y; H:i:s"));		// ������� �����   
		myFile.println(";;; Watering;");				// ����� "�����"
		delay(500);
	}

	btn_state_view = digitalRead(btn_view);				// ��������� �������� � ����� ������ "��������"
	if (btn_state_view == HIGH)							// ��������� ������ �� ������
	{
		int measur_arr[3];
		int k;
		for (k = 0; k < 3; k = k + 1) {
			measur_arr[k] = measurement(k);
			Serial.print(measur_arr[k]);
			Serial.print("%; ");
			lcd.backlight();
			lcd.print(measur_arr[k]);
			lcd.print("%; ");
		}
			delay(5000);
			lcd.clear();
			lcd.noBacklight();
	}
				
		Serial.println(time.gettime("s"));
		delay(500);
		time.gettime();
		time_s = time.seconds;
		
		if (time_s == 00 && tm!= time_s)									// ���� ������ 60 �������
		{
			tm = time_s;
			Serial.println("ok time");
			myFile.print(time.gettime("d-m-Y; H:i:s"));		// ������� �����
			Serial.println(time.gettime("d-m-Y; H:i:s"));

			int measur_arr[3];
			int k;
			for (k = 0; k < 3; k = k + 1) {
				measur_arr[k] = measurement(k);
				Serial.print(measur_arr[k]);
				Serial.print("; ");
				myFile.print(measur_arr[k]);
				myFile.print("; ");
			}
		}
		myFile.close();									// ��������� ����
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

