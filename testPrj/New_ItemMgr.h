#pragma once
#include"DxLib.h"
#include<vector>
#include"Vector2.h"
#include"CamAccessor.h"
#include"EffectMgr.h"

class New_ChargeItemEffect
{
	static const int EFFECT08_MAX_ARRAY = 50;

	struct effect08
	{
		float x = 0;
		float y = 0;
		bool flag = false;
		float speed = 0;
		float angle = 0;
		int alpha = 0;

		void Init()
		{
			x = 0;
			y = 0;
			flag = false;
			speed = 0;
			angle = 0;
			alpha = 0;
		}
	};

	effect08 effect08[EFFECT08_MAX_ARRAY];

	Vector2<float> initPos;

public:
	New_ChargeItemEffect(Vector2<float> Pos) :initPos(Pos) {};
	void Update();
	void Draw(CamAccessor* Cam);
};

struct New_Item
{
	New_ChargeItemEffect effect;

	Vector2<float>pos;
	bool get = false;
	New_Item(Vector2<float>Pos) :pos(Pos), get(false), effect({ Pos.x + GRAPH_SIZE / 2,Pos.y + GRAPH_SIZE / 2 - 7 }) {};
	bool InWin(CamAccessor* Cam);

public:
	static const int GRAPH_SIZE = 64;
};

static class New_ItemMgr
{
	static int itemGraph;
	static std::vector<New_Item>items;
	static CamAccessor* camPtr;
	static EffectMgr* effectMgr;

	static const int MAX_MOVE = 9;
	static const float MOVE_SPEED;
	static float radian;

public:
	static int getCount;
	static void Init(CamAccessor* Cam, EffectMgr* Effect)
	{
		items.clear();
		getCount = 0;
		camPtr = Cam; 
		effectMgr = Effect;
	}
	static void AddItem(int X, int Y);
	static void Load() { itemGraph = LoadGraph("_resource/graphics/item.png"); }
	static void Update();
	static void Draw();

	static bool HitCheckPoint(Vector2<float>Pos);
	static bool HitCheckBox(Vector2<float>CenterPos,int Size);

	static int MaxItemNum() { return items.size(); }
};