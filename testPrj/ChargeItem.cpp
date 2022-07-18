#include "ChargeItem.h"
#include"DxLib.h"
#include<math.h>

int ChargeItem::GRAPH[2] = { 0 };

const float ChargeItem::MOVE_SPEED = DX_PI / 30;
float ChargeItem::moveY = 0.0F;
float ChargeItem::radian = 0.0F;

ChargeItem::ChargeItem()
{
	if (!GRAPH[0])
	{
		LoadDivGraph("_resource/graphics/chargeItem.png", 2, 2, 1, SIZE, SIZE, GRAPH);
	}
	size = { SIZE,SIZE };
	hitFixed = { 0,0 };
}

void ChargeItem::Set(CamAccessor* Cam, Vector2<int> InitPos)
{
	camPtr = Cam;
	initPos = { (float)(InitPos.x * 32) + SIZE / 2,(float)(InitPos.y * 32) + SIZE / 2 };
	effect.Set(Cam, { initPos.x,initPos.y-7 });
}

void ChargeItem::Init()
{
	pos = initPos;
	timer = 0;
	effect.Init();
}

void ChargeItem::MoveUpdate()
{
	radian += MOVE_SPEED;
	if (radian == DX_PI * 2)
	{
		radian = 0;
	}
	moveY = (float)MAX_MOVE * sin(radian);
}

void ChargeItem::Update()
{
	if (timer)
	{
		timer--;
		if (timer == 0)
		{
			effect.Init();
		}
	}
	else
	{
		effect.Update();
	}
}

void ChargeItem::Draw()
{
	//“üŽè•s‰Â”\
	if (timer)
	{
		DrawExtendGraph(LocalX1(), LocalY1(), LocalX2(), LocalY2(), GRAPH[0], true);
		//DrawBox(LocalX1(), LocalY1(), LocalX2(), LocalY2(), GetColor(0, 0, 255), false);
	}
	//“üŽè‰Â”\
	else
	{
		DrawExtendGraph(LocalX1(), LocalY1() + moveY, LocalX2(), LocalY2() + moveY, GRAPH[1], true);
		//DrawBox(LocalX1(), LocalY1(), LocalX2(), LocalY2(), GetColor(0, 0, 255), true);
		effect.Draw();
	}
}

bool ChargeItem::Catch()
{
	if (!timer)
	{
		timer = SPAN;
		return true;
	}

	return false;
}

void ChargeItemEffect::Init()
{
	for (int i = 0; i < EFFECT08_MAX_ARRAY; i++)
	{
		effect08[i].Init();
	}
}

void ChargeItemEffect::Update()
{
	for (int i = 0; i < EFFECT08_MAX_ARRAY; i++)
	{
		if (effect08[i].flag == false)
		{
			effect08[i].speed = 0.8;
			effect08[i].angle = GetRand(359) / 180.0f * DX_PI_F;
			effect08[i].x = initPos.x - 3/2;
			effect08[i].y = initPos.y - 3/2;
			effect08[i].alpha = 240;
			effect08[i].flag = true;
			break;
		}
	}

	for (int i = 0; i < EFFECT08_MAX_ARRAY; i++)
	{
		if (effect08[i].flag == true)
		{
			effect08[i].alpha -= 6;
			effect08[i].x += effect08[i].speed * cos(effect08[i].angle);
			effect08[i].y += effect08[i].speed * sin(effect08[i].angle);
			if (effect08[i].alpha <= 0)
				effect08[i].flag = false;
		}
	}
}

void ChargeItemEffect::Draw()
{
	for (int i = 0; i < EFFECT08_MAX_ARRAY; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, effect08[i].alpha);
		DrawBox(cam->CamAffectX(effect08[i].x), cam->CamAffectY(effect08[i].y), 
				cam->CamAffectX(effect08[i].x + 3), cam->CamAffectY(effect08[i].y + 3), GetColor(166, 39, 229), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
