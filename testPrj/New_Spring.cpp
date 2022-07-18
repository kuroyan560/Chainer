#include "New_Spring.h"
#include<math.h>
#include"DxLib.h"
#include"New_ItemMgr.h"

void New_Spring::Init(float _x, float _y, float _len, float _k, float _gravity, float _mass, float _damp)
{
	x = _x;
	y = _y;
	vx = 0;
	vy = 0;
	ax = 0;
	ay = 0;
	len = _len;
	disp = 0;
	k = _k;
	gravity = _gravity;
	mass = _mass;
	damp = _damp;
}

void New_Spring::Update(float parentX, float parentY, float childX, float childY, int childNum)
{
	//親側に引っ張られる力を求める
	float pRad = atan2(parentY - y, parentX - x);
	float pLen = sqrtf(powf(parentX - x, 2.0f) + powf(parentY - y, 2.0f));
	float pFx = (parentX - x) * k;
	float pFy = (parentY - y - len * sin(pRad)) * k;

	//子側に引っ張られる力を求める
	float cRad = atan2(childY - y, childX - x);
	float cLen = sqrtf(powf(childX - x, 2.0f) + powf(childY - y, 2.0f));
	float cFx = (childX - x) * k;
	float cFy = (childY - y - len * sin(cRad)) * k;

	//力を合成して合力を求める
	float fx = (pFx + cFx) * damp;
	float fy = (pFy + cFy) * damp + gravity;

	//加速度を求める
	ax = fx / mass;
	ay = fy / mass;

	//速度を求める
	vx = ax;
	vy = ay;

	//位置を求める
	if (childNum)
	{
		x += vx;
		y += vy;
	}
	else
	{
		x = childX;
		y = childY;
	}
}

void New_Spring::HitCheckItem()
{
	New_ItemMgr::HitCheckPoint({ x,y });
}

void New_Spring::Draw(CamAccessor* Cam,float parentX, float parentY, int color)
{
	int size = ceil(Cam->CamZoomAffect(2));
	DrawLine(Cam->CamAffectX(x), Cam->CamAffectY(y), Cam->CamAffectX(parentX), Cam->CamAffectY(parentY), color, size);
}

void New_Spring::Move(float targetX, float targetY)
{
	x += targetX;
	y += targetY;
}
