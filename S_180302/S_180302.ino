#include <Wire.h>									//��������� spi ��� 
#include <iarduino_RTC.h>
#include <SPI.h>
#include <SD.h>
const uint8_t PIN_CS = 10;                            // ����� ������ CS �������� SD ++
const int btn_Watering = 3;                              // ����� �����, ������������ � ������ "�����"
int buttonState = 0;                                   // ���������� ��� �������� ��������� ������ "�����"
int sensorPin_A = 0;
int sensorValue_A = 0;
int sensorPin_B = 1;
int sensorValue_B = 0;
int sensorPin_C = 2;
int sensorValue_C = 0;
int power_A = 5;
int power_B = 6;
int power_C = 7;
int time_r = 0;
File myFile;                                          // ������ ���������� myFile
iarduino_RTC time(RTC_DS3231);
void setup()
{
	time.begin();
	Wire.begin();
	pinMode(btn_Watering, INPUT);                          // �������������� ���, ������������ � ������, ��� ����
	pinMode(power_A, OUTPUT);
	pinMode(power_B, OUTPUT);
	pinMode(power_C, OUTPUT);
	
	Serial.begin(9600);                               // ��������� ����
	while (!Serial) { ; }
	Serial.println("open my port");
	if (!SD.begin(PIN_CS))                             // ������������� SD ����� � ��������� ������ ������ CS  
	{
		myFile.println("SD-card not found");
		Serial.println("SD-card not found");			// ������ �������������. ����� �� ���������� ��� �� ��������� (����������� ���������) ������� ���� MicroSD
	}					 
}

void loop()
{
	myFile = SD.open("iarduino.txt", FILE_WRITE);
	buttonState = digitalRead(btn_Watering);                  // ��������� �������� � ����� ������ "�����"
	if (buttonState == HIGH)                                // ��������� ������ �� ������
	{
		Serial.println(";;; Watering;");
		myFile.print(time.gettime("d-m-Y; H:i:s"));          // ������� �����   
		myFile.println(";;; Watering;");                       // ����� "�����"   
		delay(500);
	}

	Serial.println(time.gettime("s"));
	delay(500);
	time.gettime();
	time_r = time.seconds;
		
	if (time_r == 00)								// ���� ������ 60 �������
	{ 
		Serial.println("ok time");

		digitalWrite(power_A, HIGH);                       // �������� ������� �� ��������
		digitalWrite(power_B, HIGH);
		digitalWrite(power_C, HIGH);
		delay(100);
		sensorValue_A = analogRead(sensorPin_A);
		sensorValue_B = analogRead(sensorPin_B);
		sensorValue_C = analogRead(sensorPin_C);

		Serial.println(time.gettime("d-m-Y; H:i:s"));
		Serial.println(sensorValue_A);


		myFile.print(time.gettime("d-m-Y; H:i:s"));			// ������� �����
		myFile.print("; ");
		myFile.print(sensorValue_A);
		myFile.print("; ");
		myFile.print(sensorValue_B);
		myFile.print("; ");
		myFile.print(sensorValue_C);
		myFile.println("; ");
		digitalWrite(power_A, LOW);                        // ��������� ������� �� ��������
		digitalWrite(power_B, LOW);
		digitalWrite(power_C, LOW);
	}
		myFile.close();													// ��������� ����
}