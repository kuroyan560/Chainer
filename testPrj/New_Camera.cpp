#include "New_Camera.h"
#include"MyFunc.h"
#include"easing.h"
#include"DxLib.h"
#include"Constants.h"
#include"easing.h"
#include"SoundEmitter.h"

//デバッグ用
#include"UsersInput.h"

void New_Camera::ShakeInit()
{
	shake = { 0,0 };
	fullShakeFlame = 0;
	flameTimer = -1;
	fullPower = 0;
	power = 0;
	span = 0;
}

void New_Camera::ShakeUpdate()
{
	if (0 <= flameTimer)
	{
		if (flameTimer % span == 0)
		{
			double rate = (float)flameTimer / (float)fullShakeFlame;
			power = Ease(In, Quart, rate) * fullPower;
			float p = power * (1 + 1 - zoom);
			shake.x = GetRand(p * 2) - p;
			shake.y = GetRand(p * 2) - p;
		}
		flameTimer--;
	}
}

New_Camera::New_Camera()
	:BASE_POS(Constants::GetWinCenter())
{
	CreateEaseSeed(&goalEaseID, In, Exp, 0.05f,0.0f,GOAL_ZOOM);
}

void New_Camera::Init(int MapX, int MapY)
{
	zoom = MIN_ZOOM + (MAX_ZOOM - MIN_ZOOM) / 2;
	toZoom = MAX_ZOOM;
	pos = BASE_POS;
	ShakeInit();

	mapWidth = MapX;
	mapHeight = MapY;

	EaseInit(goalEaseID);

	goalTimer = 0;
	goalMoveFlag = false;
}

void New_Camera::Update(const Vector2<float>& FollowPos, float FollowRate, float ZoomChangeRate)
{
	ShakeUpdate();
	if (FollowRate != 1.0)
	{
		pos.x = MyFunc::lerp(pos.x, FollowPos.x, FollowRate);
		pos.y = MyFunc::lerp(pos.y, FollowPos.y, FollowRate);
	}
	else
	{
		pos = FollowPos;
	}
	if (GetDistance(pos, FollowPos) < 0.2)
	{
		pos = FollowPos;
	}

	if (pos.x < Constants::GetWinCenter().x)
	{
		pos.x = Constants::GetWinCenter().x;
	}
	else if (mapWidth - Constants::GetWinCenter().x < pos.x)
	{
		pos.x = mapWidth - Constants::GetWinCenter().x;
	}
	if (pos.y < Constants::GetWinCenter().y)
	{
		pos.y = Constants::GetWinCenter().y;
	}
	else if (mapHeight - Constants::GetWinCenter().y < pos.y)
	{
		pos.y = mapHeight - Constants::GetWinCenter().y;
	}

	toZoom = MIN_ZOOM + (MAX_ZOOM - MIN_ZOOM) * ZoomChangeRate;

	if (MAX_ZOOM < toZoom)
	{
		toZoom = MAX_ZOOM;
	}
	else if (toZoom < MIN_ZOOM)
	{
		toZoom = MIN_ZOOM;
	}

	zoom = MyFunc::lerp(zoom, toZoom, 0.05);
	if (fabs(toZoom - zoom) < 0.005)
	{
		zoom = toZoom;
	}
}

bool New_Camera::GoalUpdate(const Vector2<int> GoalPos,bool GoalTrigger)
{
	pos.x = MyFunc::lerp(pos.x, GoalPos.x, 0.1f);
	pos.y = MyFunc::lerp(pos.y, GoalPos.y, 0.1f);

	if (goalMoveFlag && !CompleteEase(goalEaseID))
	{
		zoom = GetEaseUpdate(goalEaseID);
	}

	if (GoalTrigger)
	{
		goalTimer = GOAL_SPAN;
	}

	if (goalTimer)
	{
		goalTimer--;
	}
	else
	{
		if (!goalMoveFlag)
		{
			goalMoveFlag = true;
			PlayEase_Trigger(goalEaseID);
			ReSetEaseStart(goalEaseID, zoom);
			PlaySE(SE_GOAL);
		}
	}

	if (goalMoveFlag && CompleteEase(goalEaseID))
	{
		pos = Constants::GetWinCenter();
		zoom = MyFunc::lerp(zoom, 1.0, 0.3f);
		if (fabs(1.0f - zoom) < 0.005)
		{
			zoom = 1.0f;
		}
		return true;
	}
	return false;
}

void New_Camera::Debug()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "ズーム率：%.2f", zoom);
}

float New_Camera::CamAffectX(const float WorldX) const
{
	return WorldX * zoom - ((pos.x + shake.x) - BASE_POS.x) - ((pos.x + shake.x) * zoom - (pos.x + shake.x));
}

float New_Camera::CamAffectY(const float WorldY) const
{
	return WorldY * zoom - ((pos.y + shake.y) - BASE_POS.y) - ((pos.y + shake.y) * zoom - (pos.y + shake.y));
}

Vector2<float> New_Camera::CamAffect(const Vector2<float> World) const
{
	float X = CamAffectX(World.x);
	float Y = CamAffectY(World.y);
	return {X, Y};
}

void New_Camera::Shake(int Power, int Time, int Span)
{
	fullPower = Power;
	power = Power;
	fullShakeFlame = Time;
	flameTimer = Time;
	span = Span;
}

void New_Camera::DropGroundShake()
{
	Shake(DROP_GROUND_SHAKE_POWER, DROP_GROUND_SHAKE_TIME, DROP_GROUND_SHAKE_SPAN);
}
