bool interrupt = false; //Stop checking for buttons
bool startRightLiftMotorForward = true; //Start right lift motor forward for transmission

void jiggle() //Move motors attached to gears slowly to align gears with transitioning gears
{
	if(startRightLiftMotorForward	 == true) //Causes motors to change directions rapidly (see loop containing call for this function in the checkButtons task)
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
}

void moveTransmission(bool jiggleStart) //Move the actual transmission motor to change the function of the lift motors
{
	while(interrupt != true){ //Won't reloop if interrupt has been activated
		if(jiggleStart == false) //Checks to see if the jiggle function has been initially called, if not, it waits for the code to be executed in the task
		{
			sleep(25); //Using 'sleep' as per the RobotC help docs
		}else
		{
			while(interrupt != true) //breaks loop if interrupt has been activated
			{
				motor[Transmission] = 50; //moves transmission gear - speed needs to be low as well, but not as low since it is moving a larger distance
			}
			motor[Transmission] = 0; //Stops motor for transmission once the gears have been shifted
		}
	}
}

task controlTransmission() //Made a separate task for this so that the checkButtons task won't get held up on this
{
	bool jiggleRunning = false; //Initializes the variable for the jiggle function's state, but will not be set to true until the code for it has been executed
	moveTransmission(jiggleRunning); //Start the transmission function loops, which won't start until jiggle running has been activated in the next line (check to see here if it freezes on the function)
	while(interrupt == false) //Stops when interrupted
	{
		jiggle();
		jiggleRunning = true; //Sets boolean variable so that transmission motor can start transitioning
		if(startRightLiftMotorForward == true) //If-statement essentially just switches gears for jiggle as it loops, check here if that doesn't work
		{
			startRightLiftMotorForward = false;
		}else
		{
			startRightLiftMotorForward = true;
		}
		sleep(100); //wait so that gears have a chance to move during transition - adjust times as needed
	}
	jiggleRunning = false; //Shuts off boolean for jiggle when interrupted
}

task checkButtons() //continuous task meant to continuously monitor buttons
{
	while(true){ //continuously runs since it is a task, stopped through main file
		bool con8Rtoggle = false; //Controller button 8R pressed
		if(vexRT(btn8R) == true) //Check button state on controller
		{
			if(con8Rtoggle == false) //Checks to see if it has previously been toggled (transmission motors will be running, could check there instead to check for toggle)
			{
				con8Rtoggle = true; //Changes toggle state
				startTask(controlTransmission, 8);
			}else
			{
				con8Rtoggle = false; //Returns toggle to off state, begins to shut off actions for when it is toggled
				interrupt = true; //Should give controlTransmission task enough time to shut down correctly, add time if it doesn't work properly
				stopTask(controlTransmission);
			}
		}
	}
}
