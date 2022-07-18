#pragma once
#include "Vector2.h"
#include <vector>
#include "DxLib.h"
#include "BaseObject.h"
#include "Result.h"

class AfterImage : public Object
{
	std::vector<Vector2<float>> oldPos;
	std::vector<int> image;
	std::vector<float> alpha;
	int count;
	bool nowChain;

	static const int INTERVAL = 2;
	static const int SUB_ALPHA = 5;
public:
	void Set(CamAccessor* Camera);
	void Init();
	void Draw();
	void Update(Vector2<float> OpePos, int Image, bool NowChain);
	void finalize();
};

class TimeStop
{
	int mTimer;
	bool mTimeStart;
	bool mTimeStop;
	int nowStage;

	static const int STOP_TIME = 45;
public:
	TimeStop() {}
	~TimeStop() {}
	void Init(int NowStage);
	void Update();
	void Draw();
	void Finalize();
	void TimerStart();
	void Restart();
	bool GetMTimeStop() const { return mTimeStop; }
};

class Spring
{
private:
	float x, y;
	float vx, vy;
	float ax, ay;
	float len;
	float disp;
	float k;
	float gravity;
	float mass;
	float damp;
	bool isClicked;
public:
	Spring() {}
	~Spring() {}
	void Init(float _x, float _y, float _len, float _k, float _gravity, float _mass, float _damp);
	void Update(float parentX, float parentY, float childX, float childY, int childNum);
	void Draw(float parentX, float parentY, int color);
	void Move(float targetX, float targetY);
	float GetX() { return x; }
	float GetY() { return y; }
};

class SpringMgr : public Object
{
	const float gravity = 4.0f;
	const float mass = 5.0f;
	const float k = 0.8;
	const float damp = 4;
	const float len = 3;

	static const int SPRING_NUM = 12;
	Spring s[SPRING_NUM];

	Vector2<float> opePos;
	Vector2<float> cpuPos;

	int maxLen;
	bool fullStretch;

public:
	SpringMgr() {}
	~SpringMgr() {}
	void Set(CamAccessor* Camera);
	void Init(float _maxLen, Vector2<float> pos);
	void Update(Vector2<float> _opePos, Vector2<float> _cpuPos);
	void Draw();
	void Finalize();
};

class MTimer
{
	int startTime;
	int endTime;
	int nowTime;
	Timer result;
public:
	MTimer() {}
	~MTimer() {}
	void Reset();
	void Start();
	void End();
	void Update();
	void Draw();
	void Finalize();
	/*int GetNowTime() { return nowTime - startTime; }
	int GetStartTime() { return startTime; }
	int GetEndTime() { return endTime; }*/
	Timer GetResultTime() { return result; }
};