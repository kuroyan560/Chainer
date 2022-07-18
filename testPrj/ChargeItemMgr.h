#pragma once
#include"ChargeItem.h"
#include<vector>
#include"PlayerMgr.h"
class ChargeItemMgr
{
	std::vector<ChargeItem> items;
	CamAccessor* camPtr;

	void Delete();

public:
	ChargeItemMgr(CamAccessor* Cam);
	~ChargeItemMgr() {};

	void Set(const int& ItemNum, Vector2<int>* InitPos);
	void Init();
	void Update(PlayerMgr* PlayerMgr);
	void Draw();
	void Finalize();
};