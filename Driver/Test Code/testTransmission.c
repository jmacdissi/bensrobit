void jiggle()
{
	if(startRightLiftMotorForward  == true)
	{
		motor[RLift1] = 5;
		motor[RLift2] = 5;
	}else
	{
		motor[LLift1] = 5;
		motor[LLift2] = 5;
	}
}

void moveTransmission()
{
	while(true){
		if(jiggleRunning == false)
		{
			sleep(25);
		}else
		{
			while(interrupt != true)
			{
				motor[Transmission] = 50;
			}
			motor[Transmission] = 0;
		}
	}
}

task checkButtons()
{
	while(true){
		if(vexRT(btn8R) == true)
		{
			if(con8Dtoggle == false)
			{
				con8Dtoggle = true;
				moveTransmission();
				while(interrupt == false)
				{
					jiggle();
					sleep(200);
				}
				jiggleRunning = false;
			}else
			{
				con8Dtoggle = false;
				interrupt = true;
			}
		}
	}
}
