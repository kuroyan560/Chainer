#pragma once
#include<queue>
#include"Vector2.h"

enum ActName { NONEACT, JUMP, LEFT_ACT, RIGHT_ACT };

struct Act
{
	ActName name;
	float move;
};

//行動キュー
class ActRecord
{
private:
	static const Vector2<float> NONE;
	std::queue<Act>recordX;
	std::queue<Act>recordY;
	int numX;
	int numY;

public:
	ActRecord(int DelayFlameX,int DelayFlameY);	//非操作キャラの行動反映遅延フレーム
	~ActRecord() {};

	//全部空に
	void Init();

	//行動を記録
	void PushAct(char which,Act act);
	//先頭の行動記録を参照
	Act GetAct(char which);
	//行動記録を排出
	void PopAct();
};