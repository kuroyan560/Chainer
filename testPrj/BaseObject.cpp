#include "BaseObject.h"
#include"Constants.h"
//四角いオブジェクト抽象クラス===========================================

float Object::LocalX()const
{
	return camPtr->CamAffectX(pos.x);
}

float Object::LocalY()const
{
	return camPtr->CamAffectY(pos.y);
}

float Object::LocalX1()const
{
	return camPtr->CamAffectX(WorldX1());
}

float Object::LocalY1()const
{
	return camPtr->CamAffectY(WorldY1());
}

float Object::LocalX2()const
{
	return camPtr->CamAffectX(WorldX2());
}

float Object::LocalY2()const
{
	return camPtr->CamAffectY(WorldY2());
}

float Object::HitX1()const
{
	return WorldX1() - hitFixed.x;
}

float Object::HitY1()const
{
	return WorldY1() - hitFixed.y;
}

float Object::HitX2()const
{
	return WorldX2() + hitFixed.x;
}

float Object::HitY2()const
{
	return WorldY2() + hitFixed.y;
}

bool Object::HitCheck(const Object& obj)
{
	if (this->HitX1() <= obj.HitX2() && obj.HitX1() <= this->HitX2()
		&& this->HitY1() <= obj.HitY2() && obj.HitY1() <= this->HitY2())
	{
		return true;
	}
	
	return false;
}

bool Object::NearCheck(const Object& obj, int Range)
{
	if (this->HitX1() - Range <= obj.HitX2() && obj.HitX1() <= this->HitX2() + Range
		&& this->HitY1() - Range <= obj.HitY2() && obj.HitY1() <= this->HitY2() + Range)
	{
		return true;
	}

	return false;
}

bool Object::InScreenCheck()
{
	if ((((0 < LocalX1() && LocalX1() < Constants::WIN_WIDTH) || (0 < LocalX2() && LocalX2() < Constants::WIN_WIDTH))
		 && (0 < LocalY1() && LocalY1() < Constants::WIN_HEIGHT))
		|| (((0 < LocalX1() && LocalX1() < Constants::WIN_WIDTH) || (0 < LocalX2() && LocalX2() < Constants::WIN_WIDTH))
			&& (0 < LocalY2() && LocalY2() < Constants::WIN_HEIGHT)))
	{
		return true;
	}
	return false;
}
