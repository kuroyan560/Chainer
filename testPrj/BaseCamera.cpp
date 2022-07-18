#include "BaseCamera.h"
#include<stdlib.h>
#include"easing.h"
#include"DxLib.h"

const float BaseCamera::SET_SPEED_RATE = 0.1F;

static float lerp(float start, float end, float t)
{
	return (1 - t) * start + t * end;
}

void BaseCamera::ShakeInit()
{
	shake = { 0,0 };
	fullShakeFlame = 0;
	flameTimer = -1;
	fullPower = 0;
	power = 0;
	span = 0;
}

void BaseCamera::ShakeUpdate()
{
	if (0 <= flameTimer)
	{
		if (flameTimer % span == 0)
		{
			double rate = (float)flameTimer / (float)fullShakeFlame;
			power = Ease(In, Quart, rate) * fullPower;
			shake.x = GetRand(power * 2) - power;
			shake.y = GetRand(power * 2) - power;
		}
		flameTimer--;
	}
}

void BaseCamera::Init()
{
	floorPos = BASE_POS;
	zoom = NORMAL_ZOOM;
	ShakeInit();
}

void BaseCamera::Init(Vector2<float> InitPos, float InitZoom,int MapHeight)
{
	floorPos = InitPos;
	zoom = InitZoom;
	ShakeInit();
	mapHeight = MapHeight;
}

void BaseCamera::Update()
{
	ShakeUpdate();
}

float BaseCamera::CamAffectX(const float WorldX) const
{
	return WorldX * zoom - ((floorPos.x + shake.x) - BASE_POS.x) - ((floorPos.x + shake.x) * zoom - (floorPos.x + shake.x));
}

float BaseCamera::CamAffectY(const float WorldY) const
{
	return WorldY * zoom - ((floorPos.y + shake.y) - BASE_POS.y) - ((floorPos.y + shake.y) * zoom - (floorPos.y + shake.y));
}

Vector2<float> BaseCamera::CamAffect(const Vector2<float> World) const
{
	float x = CamAffectX(World.x);
	float y = CamAffectY(World.y);
	return Vector2<float>{x, y};
}

void BaseCamera::DropGroundShake()
{
	Shake(DROP_GROUND_SHAKE_POWER, DROP_GROUND_SHAKE_TIME, DROP_GROUND_SHAKE_SPAN);
}

void BaseCamera::Shake(int Power, int Time, int Span)
{
	fullPower = Power;
	power = Power;
	fullShakeFlame = Time;
	flameTimer = Time;
	span = Span;
}

void Camera_SideScroll::Update()
{
	ShakeUpdate();

	if (floorPos.x < player->GetOpePlayer()->WorldX())
	{
		floorPos.x = lerp(floorPos.x, player->GetOpePlayer()->WorldX(), SET_SPEED_RATE);
	}

	if (player->NowDrop() == false)
	{
		floorPos.y = lerp(floorPos.y, player->GetOpePlayer()->WorldY(), 0.3F);
	}
	else
	{
		floorPos.y = lerp(floorPos.y, player->GetOpePlayer()->WorldY(), 0.9F);
	}

	if (floorPos.y < Constants::WIN_HEIGHT / 2)
	{
		floorPos.y = Constants::WIN_HEIGHT / 2;
	}
	if (mapHeight - Constants::WIN_HEIGHT / 2 < floorPos.y)
	{
		floorPos.y = mapHeight - Constants::WIN_HEIGHT / 2;
	}
}

void Camera_VerticalScroll::Update()
{
	ShakeUpdate();

	floorPos.y = lerp(floorPos.y, player->GetOpePlayer()->WorldY(), SET_SPEED_RATE);
}
