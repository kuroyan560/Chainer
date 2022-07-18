#include "CollectItemMgr.h"

void CollectItemMgr::Delete()
{
	if (items.empty() == false)
	{
		items.clear();
	}
}

CollectItemMgr::CollectItemMgr(CamAccessor* Cam)
{
	camPtr = Cam;
}

void CollectItemMgr::Set(const int& ItemNum, CollectInfo* CollectInfo)
{
	Delete();

	if (CollectInfo != nullptr)
	{
		for (int i = 0; i < ItemNum; i++)
		{
			items.push_back(CollectItem());
			items.back().Set(camPtr, CollectInfo+i);
		}
	}
	collectInfo = CollectInfo;
}

void CollectItemMgr::Init()
{
	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			items[i].Init();
		}
	}
}

void CollectItemMgr::Update(PlayerMgr* PlayerMgr, Record* Rec)
{
	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			items[i].Update2(PlayerMgr,Rec);
		}
	}
}

void CollectItemMgr::Draw()
{
	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			items[i].Draw();
		}
	}
}

void CollectItemMgr::GoalUpdate()
{
	if (!items.empty())
	{
		for (int i = 0; i < items.size(); i++)
		{
			items[i].Finalize();
		}
	}
}
