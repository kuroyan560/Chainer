#pragma once
#include"CamAccessor.h"
class New_Spring
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
	New_Spring() {}
	~New_Spring() {}
	void Init(float _x, float _y, float _len, float _k, float _gravity, float _mass, float _damp);
	void Update(float parentX, float parentY, float childX, float childY, int childNum);
	void HitCheckItem();
	void Draw(CamAccessor* Cam,float parentX, float parentY, int color);
	void Move(float targetX, float targetY);
	float GetX() { return x; }
	float GetY() { return y; }
};

