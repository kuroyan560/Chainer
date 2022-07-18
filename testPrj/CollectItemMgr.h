#pragma once
#include"CollectItem.h"
#include<vector>
#include"Record.h"

class CollectItemMgr
{
	std::vector<CollectItem>items;
	CamAccessor* camPtr;

	CollectInfo* collectInfo;
	int num;

	void Delete();

public:
	CollectItemMgr(CamAccessor* Cam);
	~CollectItemMgr() {};

	void Set(const int& ItemNum, CollectInfo* CollectInfo);
	void Init();
	void Update(PlayerMgr* PlayerMgr,Record* Rec);
	void Draw();

	void GoalUpdate();
};