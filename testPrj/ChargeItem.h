#pragma once
#include"BaseObject.h"
#include"Graphics.h"

class ChargeItemEffect
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

	CamAccessor* cam;
	Vector2<float> initPos;

public:
	void Set(CamAccessor* Cam, Vector2<float> Pos) { cam = Cam; initPos = Pos; }
	void Init();
	void Update();
	void Draw();
};

class ChargeItem : public Object
{
	static int GRAPH[2];

	static const int SPAN = 60;	//‚Ü‚½Žg‚¦‚é‚æ‚¤‚É‚È‚é‚Ü‚Å‚ÌƒXƒpƒ“
	static const int SIZE = 48;
	static const int HIT_FIXED = +6;	//“–‚½‚è”»’è‚Ì‘å‚«‚³

	static const int MAX_MOVE = 9;
	static const float MOVE_SPEED;
	static float moveY;
	static float radian;

	int timer;
	Vector2<float>initPos;

	ChargeItemEffect effect;

public:
	ChargeItem();
	~ChargeItem() {};

	void Set(CamAccessor* Cam, Vector2<int> InitPos);
	void Init();
	static void MoveUpdate();
	void Update();
	void Draw();
	bool Catch();
};