#include "New_ItemMgr.h"
#include"MyFunc.h"
#include"SoundEmitter.h"
int New_ItemMgr::itemGraph = 0;
int New_ItemMgr::getCount = 0;
std::vector<New_Item> New_ItemMgr::items;
CamAccessor* New_ItemMgr::camPtr;
EffectMgr* New_ItemMgr::effectMgr;

const float New_ItemMgr::MOVE_SPEED = DX_PI / 30;
float New_ItemMgr::radian = 0.0f;

void New_ItemMgr::Update()
{
	radian += MOVE_SPEED;
	if (radian == DX_PI * 2)
	{
		radian = 0;
	}

	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].InWin(camPtr) && !items[i].get)
			{
				items[i].effect.Update();
			}
		}
	}
}

void New_ItemMgr::Draw()
{
	if (!items.empty())
	{
		float X1, Y1, X2, Y2;

		float moveY = (float)MAX_MOVE * sin(radian);

		for (int i = 0; i < items.size(); i++)
		{
			if (items[i].InWin(camPtr) && !items[i].get)
			{
				X1 = camPtr->CamAffectX(items[i].pos.x);
				Y1 = camPtr->CamAffectY(items[i].pos.y + moveY);
				X2 = camPtr->CamAffectX(items[i].pos.x + New_Item::GRAPH_SIZE);
				Y2 = camPtr->CamAffectY(items[i].pos.y + New_Item::GRAPH_SIZE + moveY);
				DrawExtendGraph(X1, Y1, X2, Y2, itemGraph, true);

				items[i].effect.Draw(camPtr);
			}
		}
	}
}

void New_ItemMgr::AddItem(int X, int Y)
{
	items.push_back(New_Item({ (float)X,(float)Y }));
}

bool New_ItemMgr::HitCheckPoint(Vector2<float>Pos)
{
	bool flag = false;

	Vector2<float>p;

	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (!items[i].get && items[i].InWin(camPtr))
			{
				//p = camPtr->CamAffect(items[i].pos);
				p = items[i].pos;

				if (Pos.x < p.x || p.x + New_Item::GRAPH_SIZE < Pos.x
					|| Pos.y < p.y || p.y + New_Item::GRAPH_SIZE < Pos.y)
				{
					
				}
				else
				{
					items[i].get = true;
					getCount++;
					flag = true;
					effectMgr->EmitChainSquare(camPtr->CamAffect(items[i].pos), GREEN);
					PlaySE(SE_GLASS);
				}
			}
		}
	}

	return flag;
}

bool New_ItemMgr::HitCheckBox(Vector2<float> CenterPos, int Size)
{
	bool flag = false;
	float X1 = CenterPos.x - Size / 2;
	float Y1 = CenterPos.y - Size / 2;
	float X2 = CenterPos.x + Size / 2;
	float Y2 = CenterPos.y + Size / 2;

	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (!items[i].get && items[i].InWin(camPtr))
			{
				if (X2 < items[i].pos.x || items[i].pos.x + New_Item::GRAPH_SIZE < X1
					|| Y2 < items[i].pos.y || items[i].pos.y + New_Item::GRAPH_SIZE < Y1)
				{
					//“–‚½‚ç‚È‚©‚Á‚½
				}
				else
				{
					items[i].get = true;
					getCount++;
					flag = true;
					effectMgr->EmitChainSquare(camPtr->CamAffect(items[i].pos), GREEN);
					PlaySE(SE_GLASS);
				}
			}
		}
	}

	return flag;
}

bool New_Item::InWin(CamAccessor* Cam)
{
	float X1 = Cam->CamAffectX(this->pos.x - GRAPH_SIZE / 2);
	float Y1 = Cam->CamAffectY(this->pos.y - GRAPH_SIZE / 2);
	float X2 = X1 + GRAPH_SIZE * 2;
	float Y2 = Y1 + GRAPH_SIZE * 2;

	return MyFunc::InnerWin(X1,Y1,X2,Y2);
}

void New_ChargeItemEffect::Update()
{
	for (int i = 0; i < EFFECT08_MAX_ARRAY; i++)
	{
		if (effect08[i].flag == false)
		{
			effect08[i].speed = 0.8;
			effect08[i].angle = GetRand(359) / 180.0f * DX_PI_F;
			effect08[i].x = initPos.x - 3 / 2;
			effect08[i].y = initPos.y - 3 / 2;
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

void New_ChargeItemEffect::Draw(CamAccessor* Cam)
{
	for (int i = 0; i < EFFECT08_MAX_ARRAY; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, effect08[i].alpha);
		DrawBox(Cam->CamAffectX(effect08[i].x), Cam->CamAffectY(effect08[i].y),
			Cam->CamAffectX(effect08[i].x + 3), Cam->CamAffectY(effect08[i].y + 3), GetColor(58, 231, 67), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}