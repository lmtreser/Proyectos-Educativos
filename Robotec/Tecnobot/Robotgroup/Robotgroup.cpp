#include "Robotgroup.h"

Robotgroup::Robotgroup(void)
{
  enA = 3;
  in1 = 2;
  in2 = 4;
  enB = 6;
  in3 = 5;
  in4 = 7;
  button = 8;
  
  isSetup = false;
  isRun = false;
}

void Robotgroup::lazySetup()
{
  if (!isSetup)
  {
    setup();
    isSetup = true;
  }
}

void Robotgroup::setup(void)
{
  TCCR2B = TCCR2B & B11111000 | B00000011; // set timer 2 divisor to 32 for PWM frequency of 980.39Hz
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}


void Robotgroup::Motor(int MotorNumber, int Speed)
{
  lazySetup();
  if(Speed == 0)
  {
	DetenerMotor(MotorNumber);
  }
  else if(Speed < 0)
  {
	Speed = abs(Speed);
	RetrocesoMotor(MotorNumber,Speed);
  }  
  else
  {
	AvanceMotor(MotorNumber,Speed);
  }
}

void Robotgroup::AvanceMotor(int MotorNumber, int Speed)
{
  lazySetup();
  if(MotorNumber == 1)
  {
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	analogWrite(enA, Speed * 2.55);
  }
  else if(MotorNumber == 0)
  {
	digitalWrite(in4, HIGH);
	digitalWrite(in3, LOW);
	analogWrite(enB, Speed * 2.55);
  }  
}

void Robotgroup::RetrocesoMotor(int MotorNumber, int Speed)
{
  lazySetup();
  if(MotorNumber == 1)
  {
	digitalWrite(in2, HIGH);
	digitalWrite(in1, LOW);
	analogWrite(enA, Speed * 2.55);
  }
  else if(MotorNumber == 0)
  {
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	analogWrite(enB, Speed * 2.55);
  }  
}

void Robotgroup::DetenerMotor(int MotorNumber)
{
  lazySetup();
  if(MotorNumber == 1)
  {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(enA, LOW);
  }
  else if(MotorNumber == 0)
  {
	digitalWrite(in4, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(enB, LOW);
  }  
}

void Robotgroup::BotonRun()
{
	if(!isRun)
	{
		isRun = true;
		pinMode(button, INPUT);
		int buttonState = 1;
  
		while(1)
		{
			buttonState = digitalRead(button);
			if(!buttonState)
			{
				delay(50);
				if(!buttonState)
				{
					break;
				}
			}
		}
	}
}