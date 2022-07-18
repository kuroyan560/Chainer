#pragma once
#include"BaseObject.h"
#include"CamAccessor.h"
#include"Vector2.h"
#include"Map.h"

class FloorAndWall
{
protected:
	static int FLOOR_GRAPH;
	static int WALL_GRAPH;

	CamAccessor* camPtr;

	Vector2<float> floorPos[2];
	bool floorDraw;

	Vector2<float>wallPos[2];
	bool wallDraw;

	float LocalX1(const int num);
	float LocalX2(const int num);

public:
	FloorAndWall(CamAccessor* Cam);
	void Init(Map* map);
	void Update();
	virtual void Draw();
};

class Floor:public FloorAndWall
{
public:
	Floor(CamAccessor* Cam) :FloorAndWall(Cam) {};
	void Init();
	virtual void Draw()override;

};