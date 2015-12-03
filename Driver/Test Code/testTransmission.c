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

task moveTransmission()
{
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

task checkButtons()
{

}
