#pragma once
#include"BaseObject.h"
#include"Graphics.h"
#include"Vector2.h"
#include"PlayerMgr.h"
#include"Record.h"

struct CollectInfo
{
	Vector2<int> initPos;
	bool get = false;
};

class CollectItem :public Object
{
	static const int SIZE = 64;
	static bool LOAD;
	static int IMAGE[5];
	static LoopImage GRAPH;
	static Image GRAPH_GET;

	int which;
	bool exist;

	bool* get;
	bool got;

	Vector2<float>initPos;
	CollectInfo* info;

public:
	CollectItem(CamAccessor* Cam);
	~CollectItem() {};

	void Init(Vector2<int> Pos,bool* Get);
	void Update(PlayerMgr* PlayerMgr, Record* Rec);
	void Draw();
	void Finalize();


	CollectItem();
	void Set(CamAccessor* Cam, CollectInfo* Info);
	void Init();
	void Update2(PlayerMgr* PlayerMgr,Record* Rec);

};