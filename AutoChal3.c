#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in8,    Pontentiometer, sensorPotentiometer)
#pragma config(Sensor, dgtl4,  Bumper,         sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           Right,         tmotorVex393_HBridge, openLoop, reversed, driveRight, encoderPort, I2C_1)
#pragma config(Motor,  port6,           Claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           Arm,           tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          Left,          tmotorVex393_HBridge, openLoop, reversed, driveLeft, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

float currPos1;
float currPos2;
float inches;
float rotate;
float sum1;
float sum2;
float lastError1;
float lastError2;
float error1;
float error2;
float dif1;
float dif2;
float motorOutput1;

float motorOutput2;

float error;
float setpoint;
float currPos;
float Kp;
float motorOutput;
float sum;
float Ki;
float lastError;
float dif;
float Kd;
float stopSum;
bool trash1;

bool stopdrive;
bool armdone;
bool trash2;


task armPID(){
Kp = 0.1;
/*0.01 too low, 0.1 works but does have oscillation*/
	Ki = 0.000005;
	/*Good Ki: 0.000005; 7 D in a W, try I super low? 0.00002 = old*/
	Kd = 5;
	sum = 0;
	lastError = 0;
	stopSum = 0;
	/*
	Agressive: 10
	Too low (going up): 0.05
	*/
	armdone = false;
	while(stopSum < 1000){
	currPos = SensorValue[in8];
	error = setpoint - currPos;
	dif = error - lastError;

	if(error < 400 && error > -400){
	sum+=error;
		}

	motorOutput = Kp*error + Ki*sum + Kd*dif;

	if (motorOutput > 127){
		motorOutput = 127;
		}

	else if(motorOutput < -127){
		motorOutput = -127;
		}

	motor[Arm] = motorOutput;
	lastError = error;

	if(error < 10 && error > -10){
	stopSum += 1;
}

	if(trash1 == true){
		if(currPos <setpoint){
	stopSum =1001;
}
}
if(stopdrive == false){
wait1Msec(1);
}

	}

	if(stopSum > 999){
		armdone = true;
		if(trash1 == true){
				motor[Arm] = -5;
	}
	wait10Msec(20);
}
}

void close(){
	motor[Claw] = 67;
	wait1Msec(100);
	motor[Claw] = 20;
	wait1Msec(3000);
}

void open(){
	motor[Claw] = -67;
	wait1Msec(300);
	motor[Claw] = 0;
}




task drive()
{

currPos1 = 0;
currPos2 = 0;
/*1: comment out first motor output equals lines = nothing
//2: see why we switch motor Output1 and 2 = nothing
//3: and try just using one to see if that works also*/

while (stopdrive == false){

currPos1 = -SensorValue[I2C_1];
currPos2 = -SensorValue[I2C_1];

rotate = 52.52713179;

setpoint = rotate * inches;
	Kp = 0.05;
Ki = 0.00002;
	/*Good Ki: 0.000005; */
	Kd = 0.05;
	lastError1 = 0;
		lastError2 = 0;

	error1 = setpoint - currPos1;
	error2 = setpoint - currPos2;

	dif1 = error1 - lastError1;
	dif2 = error2 - lastError2;

		sum1+=error1;
			sum2+=error2;

	motorOutput1 = Kp*error1 + Ki*sum1 + Kd*dif1;
	motorOutput2 = Kp*error2 + Ki*sum2 + Kd*dif2;

	if (motorOutput1 > 120){
		motorOutput1 = 120;
		}
			else if(motorOutput1 < -127){
		motorOutput1 = -127;
		}
if(motorOutput2 > 127){
		motorOutput2 = 127;
	}
	else if(motorOutput2 < -127){
		motorOutput2 = -127;
		}


	motor[Right] = motorOutput1;
	motor[Left] = motorOutput2;

	lastError1 = error1;
	lastError2 = error2;

	if(trash2 == false){
	if(currPos1 > setpoint-70 && currPos1 < setpoint+70){
		stopdrive = true;
		motor[Right] = 0;
		motor[Left] = 0;
	}
}
	if(trash2 == true){
	if(currPos1 > setpoint-100 && currPos1 < setpoint+100){
		stopdrive = true;
		motor[Right] = 0;
		motor[Left] = 0;
	}
}

wait1Msec(1);

}



}

task main(){
	currPos1 = 0;
	currPos2= 0;
	/*inch and a half roughly either side driving*/
		trash1 = true;
		stopdrive = false;
	inches = 30;
	trash2 = false;
	startTask(drive);
	while(stopdrive == false){
		wait1Msec(1);
}

	setpoint = 3975;
	trash1 = false;
	startTask(armPID);
	while(armdone == false){
		wait1Msec(1);
}
	close();
		inches = 0;
		stopdrive = false;
		trash2 = true;
	startTask(drive);
	while(stopdrive == false){
		wait1Msec(1);
}
	setpoint = 800;
	trash1 = true;
	stopSum = 0;
	startTask(armPID);
	while(armdone == false){
		wait1Msec(1);
}
	open();


}
