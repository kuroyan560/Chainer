#include"ActRecord.h"
const Vector2<float> ActRecord::NONE = { 0.0F,0.0F };

ActRecord::ActRecord(int DelayFlameX, int DelayFlameY)
{
	numX = DelayFlameX;
	numY = DelayFlameY;
}

void ActRecord::Init()
{
	while(!recordX.empty())
	{
		recordX.pop();
	}
	while (!recordY.empty())
	{
		recordY.pop();
	}

	for (int i = 0; i < numX; i++)
	{
		recordX.push(Act{ NONEACT,0.0F });
	}
	for (int i = 0; i < numY; i++)
	{
		recordY.push(Act{ NONEACT,0.0F });
	}
}

void ActRecord::PushAct(char which, Act act)
{
	if (which == 'x')
	{
		recordX.push(act);
	}
	else if(which == 'y')
	{
		recordY.push(act);
	}
}

Act ActRecord::GetAct(char which)
{
	if (which == 'x')
	{
		return recordX.front();
	}
	else if (which == 'y')
	{
		return recordY.front();
	}
	else
	{
		return Act{ ActName::NONEACT,0.0F };
	}
}

void ActRecord::PopAct()
{
	recordX.pop();
	recordY.pop();
}
