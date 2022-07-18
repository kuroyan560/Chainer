#pragma once
#include"New_Spring.h"
#include"Vector2.h"
#include"CamAccessor.h"
class New_SpringMgr
{
	const float gravity = 4.0f;
	const float mass = 5.0f;
	const float k = 0.8;
	const float damp = 4;
	const float len = 3;

	static const int SPRING_NUM = 12;
	New_Spring s[SPRING_NUM];

	Vector2<float> opePos;
	Vector2<float> cpuPos;

	bool fullStretch;
	float maxLen;

public:
	New_SpringMgr() {}
	~New_SpringMgr() {}
	void Init(Vector2<float> pos);
	void Update(float _maxLen, Vector2<float> _opePos, Vector2<float> _cpuPos);
	void Draw(CamAccessor* Cam);
	void Finalize();
};

