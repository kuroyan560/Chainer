#include "Enemy.h"
#include"Constants.h"

Enemy::Enemy()
{
	size = { SIZE,SIZE };
	hitFixed = { 0,0 };
}

void Enemy::ParaInit()
{
}

void Enemy::Init()
{
	aliveFlag = true;
	updateFlag = false;
	pos = initPos;
	ParaInit();
}

void Enemy::Set(CamAccessor* Cam,Vector2<int> InitPos)
{
	camPtr = Cam;
	initPos = { (float)(InitPos.x * 32),(float)(InitPos.y * 32) };
}

void Enemy::BaseUpdate()
{
	if (!updateFlag)
	{
		if (InScreenCheck() == true)
		{
			updateFlag = true;
		}
	}
}

void Enemy::Draw()
{
	if (aliveFlag && InScreenCheck() == true)
	{
		DrawBox(LocalX1(), LocalY1(), LocalX2(), LocalY2(), GetColor(255, 0, 0), true);
	}
}

void Enemy::Finalize()
{
}
