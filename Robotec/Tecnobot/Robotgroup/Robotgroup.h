#ifndef __ROBOTGROUP_H__
#define __ROBOTGROUP_H__

#include "Arduino.h"

class Robotgroup
{
private:

  int enA;
  int in1;
  int in2;
  int enB;
  int in3;
  int in4;
  int button;
  
  bool isSetup;
  bool isRun;
  void setup(void);
  void lazySetup();
  
  void AvanceMotor(int MotorNumber, int Speed);
  void RetrocesoMotor(int MotorNumber, int Speed);
  void DetenerMotor(int MotorNumber);

  public:

  Robotgroup(void);

  void Motor(int MotorNumber, int Speed);
  void BotonRun();
};


#endif