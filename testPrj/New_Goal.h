#pragma once
#include"Vector2.h"
#include"CamAccessor.h"
struct New_Goal
{
	const int SIZE = 128;
	Vector2<int>pos;
	bool flag;
	int graph;

	New_Goal();
	void HitCheck(Vector2<float>OpeWorldPos,Vector2<float>CpuWorldPos);
	void Draw(CamAccessor* Cam);

	Vector2<int>GetCenter()const
	{
		return { pos.x + SIZE / 2,pos.y + SIZE / 2 };
	}
};