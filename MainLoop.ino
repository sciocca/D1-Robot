//pauses are arbitrary until testing
#include <SendOnlySoftwareSerial.h> 
#include <ArduinoInit.h>

unsigned int FSM;
byte homecolor, floorcolor, bumpercount;
unsigned int FloorThresh = 6274;
void setup(){ 
configArduino();

void Motors_Off ();
void Rotation_Left ();
void Rotation_Right ();
void Move_Forward (int time);
void Move_Backward ();
void TotalRotation();
void Bumper_Hit();
void minirotation();

attachInterrupt(0, Bumper_Hit, LOW);
attachInterrupt(1, Bumper_Hit, LOW);

byte homecolor = 0;
byte floorcolor = 0;
outputHigh(4);
pause(100);
FSM = readADC(0);
Serial.print ("Floor Sensor");
Serial.println (FSM);
if (FSM >= FloorThresh)
  {
    homecolor = 1;
  }
}


void loop(){

pause(100);
FSM = readADC(0);
Serial.print ("Floor Sensor");
Serial.println (FSM);
if (FSM >= FloorThresh)
{
  floorcolor = 1;
}

if (floorcolor == homecolor) //Navigation light
{
  int highestval = 1000000;
  int highest_rotate;
  for (int i = 1; 1<=16;i++) //16 rotations
  {
    minirotation();
    unsigned int sensorA = 0;
    sensorA = readADC(4);
    Serial.println(highestval);
    Serial.print("Nav Light"); //print is for testing
    if (sensorA < highestval) //more light is being read that previous highest
    {
      highestval = sensorA;
      highest_rotate = i;
    }
    pause(100);
    Serial.println(highest_rotate);
    Serial.println(i);
  }
  if(highest_rotate != 16)
  {
      motors('1','A',50);
      motors('2','B',50);
      pause((190/2)*highest_rotate); //rotates to the point with the highest light reading
  }
  motors('B','O',100);
  Move_Forward(5000);
}
else //Goal Search
{
 int notfinding = 0;
 while (notfinding < 10)
 { 
  if(readADC(1) < readADC(3) && readADC(5) < readADC(1)) //nav to goal sensor (outer reads less light than center
  {
    Move_Forward(10000);
  }
  else;
  {
    minirotation();
    notfinding = notfinding + 1;
  }
}
Move_Forward(5000);
}
return 1;

}//loop end

//subroutines
void Motors_Off()
{
  pause(5);
  motors('B','X',0);
  pause (2000);
  return; 
}

void Rotation_Left()
{
  pause(5);
  motors (1,'A',80);
  motors (2,'B',80);
  pause(240); // 90 degree turn (hopefully)
  return;
}

void Rotation_Right()
{
  pause(5);
  motors (1,'B',80);
  motors (2,'A',80);
  pause(240); //90 degrees
  return;
}

void Move_Forward(int time)
{
  pause(5);
  motors ('B','A',80);
  pause(time);
  return;
}

void Move_Backward()
{
  pause(5);
  motors ('B','B',80);
  pause(300);
  return;
}
void minirotation()
{
  motors('1','A',50);
  motors('2','B',50);
  pause(190/2);
  motors('B','O',100);
  pause(100);
 
}
void TotalRotation()
{
  pause(5);
  motors (1,'B',80);
  motors (2,'A',80);
  pause(240);
  //SECOND ROTATION MUST BE MULTIPLIED BY TWO
  motors (1,'A',80);
  motors (2,'B',80);
  pause(480); 
  return;
}

void Bumper_Hit()
{
  pause(5);
  if (readInput(2) == 1 && readInput(3) == 1){
    return;
  }
    if (readInput(2) == 0 &&  readInput(3) == 1) // left
    {
      Move_Backward();
      Rotation_Right(); 
      Move_Forward(2000); 
      return;
    }
    if (readInput(2) == 1 &&  readInput(3) == 0) // right
    {
      Move_Backward();
      Rotation_Left();
      Move_Forward(2000);
      return;
    }
    if (readInput(2) == 0 && readInput(3) == 0) // both
    {
      Move_Backward();
      Rotation_Left();
      Move_Forward(2000);
      return;
    }
    else{
      return;
    }
  }


