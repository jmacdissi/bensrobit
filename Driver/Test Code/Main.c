#pragma config(Motor,  port1,           Transmission,  tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           Conveyor,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           LLaunch,       tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port4,           RLaunch,       tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port5,           RLift1,        tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port6,           RLift2,        tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port7,           LLift1,        tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port8,           LLift2,        tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port9,           RDrive,        tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port10,          LDrive,        tmotorVex393_HBridge, openLoop, driveLeft)
#pragma DebuggerWindows ("Motors")

bool interrupt = false; //Stops functions or tasks that have this in their code
bool con8Rtoggle = false; //Controller button 8R pressed
bool jiggleForwardIfTrue = true; //Start lift motors forward for transmission
bool jiggleRunning = false; //Initializes the variable for the jiggle function's state, but will not be set to true until the code for it has been executed

void jiggle() //Move motors attached to gears slowly to align gears with transitioning gears
{
	jiggleRunning = true; //Sets boolean variable so that transmission motor can start transitioning
	while(interrupt == false)
	{
		if(jiggleForwardIfTrue	 == true) //Causes motors to change directions rapidly
		{
			motor[RLift1] = 10; //Keep speeds very low, the gears only need to move a small amount
			motor[RLift2] = 10;
			motor[LLift1] = 10;
			motor[LLift2] = 10;
		}else
		{
			motor[RLift1] = -10;
			motor[RLift2] = -10;
			motor[LLift1] = -10;
			motor[LLift2] = -10;
		}

		if(jiggleForwardIfTrue == true) //If-statement essentially just switches gears for jiggle as it loops, check here if that doesn't work
		{
			jiggleForwardIfTrue = false;
		}else
		{
			jiggleForwardIfTrue = true;
		}
	}
	jiggleRunning = false;
}

void moveTransmission() //Move the actual transmission motor to change the function of the lift motors
{
	while(interrupt == false) //breaks loop if interrupt has been activated
	{
		motor[Transmission] = 50; //moves transmission gear - speed needs to be low as well, but not as low since it is moving a larger distance
	}
	motor[Transmission] = 0; //Stops motor for transmission once the gears have been shifted
}

task control()
{
	const int maxIterations = 5; //Controls how many times each program loops before breaking, change this number as needed
	while(interrupt == false)
	{
		for (int initial = 0; interrupt == true || initial == maxIterations; initial++)
		{
			jiggle();
		}

		for (int initial = 0; interrupt == true || initial == maxIterations; initial++)
		{
			moveTransmission();
		}
	}
}

task main()
{
	while(true)
	{
		if(vexRT(btn8R) == true) //Check button state on controller
		{
			if(con8Rtoggle == false) //Checks to see if it has previously been toggled
			{
				con8Rtoggle = true; //Changes toggle state
				startTask(control);
			}else
			{
				con8Rtoggle = false; //Returns toggle to off state, begins to shut off actions for when it is toggled
				interrupt = true;
				sleep(25); //Sleep to allow everything to shut off containing the interrupt variable
				stopAllTasks();
			}
		}
	}
}
