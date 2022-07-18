#include "ChargeItemMgr.h"

void ChargeItemMgr::Delete()
{
	if (items.empty() == false)
	{
		items.clear();
	}
}

ChargeItemMgr::ChargeItemMgr(CamAccessor* Cam)
{
	camPtr = Cam;
}

void ChargeItemMgr::Set(const int& ItemNum, Vector2<int>* InitPos)
{
	Delete();

	if (InitPos != nullptr)
	{
		for (int i = 0; i < ItemNum; i++)
		{
			items.push_back(ChargeItem());
			items.back().Set(camPtr, *(InitPos + i));
		}
	}
}

void ChargeItemMgr::Init()
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i].Init();
	}
}

void ChargeItemMgr::Update(PlayerMgr* PlayerMgr)
{
	ChargeItem::MoveUpdate();

	for (int i = 0; i < items.size(); i++)
	{
		items[i].Update();

		if (items[i].InScreenCheck() == true)
		{
			if (items[i].HitCheck(*PlayerMgr->GetOpePlayer()))
			{
				if(items[i].Catch() == true)
				PlayerMgr->RaiseCanChain(PlayerMgr->GetOpeNum());
			}
			else if (items[i].HitCheck(*PlayerMgr->GetCpuPlayer()))
			{
				if(items[i].Catch() == true)
				PlayerMgr->RaiseCanChain(PlayerMgr->GetCpuNum());
			}
		}
	}
}

void ChargeItemMgr::Draw()
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i].Draw();
	}
}

void ChargeItemMgr::Finalize()
{
}
