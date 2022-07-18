#include "CollectItem.h"
#include"SoundEmitter.h"

bool CollectItem::LOAD = false;
int CollectItem::IMAGE[5] = { 0 };
LoopImage CollectItem::GRAPH;
Image CollectItem::GRAPH_GET;

CollectItem::CollectItem()
{
	size = { SIZE,SIZE };
	hitFixed = { 0,0 };

	if (!LOAD)
	{
		GRAPH.Init("_resource/graphics/collectItem.png", true, 5, 7, { 5,1 }, size);
		GRAPH_GET.Init("_resource/graphics/collectItem_get.png", true);
		LoadDivGraph("_resource/graphics/collectItem.png", 5, 5, 1, size.x, size.y, IMAGE);
		LOAD = true;
	}
}

CollectItem::CollectItem(CamAccessor* Cam)
{
	camPtr = Cam;
	size = { SIZE,SIZE };
	hitFixed = { 0,0 };

	if (!LOAD)
	{
		GRAPH.Init("_resource/graphics/collectItem.png", true, 5, 7, { 5,1 }, size);
		GRAPH_GET.Init("_resource/graphics/collectItem_get.png", true);
		LoadDivGraph("_resource/graphics/collectItem.png", 5, 5, 1, size.x, size.y, IMAGE);
		LOAD = true;
	}
}

void CollectItem::Init(Vector2<int> Pos,bool* Get)
{
	if (Get != nullptr)
	{
		exist = true;

		if (!*Get)
		{
			which = 0;
		}
		else
		{
			which = 1;
		}

		pos.x = Pos.x * 32 + SIZE / 2;
		pos.y = Pos.y * 32 + SIZE / 2;

		get = Get;
		got = false;
	}
	else
	{
		exist = false;
	}
}

void CollectItem::Update(PlayerMgr* PlayerMgr, Record* Rec)
{
	if (exist)
	{
		if (this->HitCheck(*PlayerMgr->GetCpuPlayer()) || this->HitCheck(*PlayerMgr->GetOpePlayer()))
		{
			exist = false;
			PlaySE(SE_GET);
			Rec->SingleCollectItemGet();
		}
	}
}

void CollectItem::Draw()
{
	if (exist)
	{
		if (!which)
		{
			GRAPH.Draw(LocalPos(), size);
		}
		else
		{
			GRAPH_GET.Draw(LocalPos(), size);
		}
	}
}

void CollectItem::Finalize()
{
	if (got && info != nullptr)
	{
		info->get = true;
	}
}

void CollectItem::Set(CamAccessor* Cam, CollectInfo* Info)
{
	info = Info;
	camPtr = Cam;
	initPos = { (float)(info->initPos.x * 32) + SIZE / 2,(float)(info->initPos.y * 32) + SIZE / 2 };
}

void CollectItem::Init()
{
	pos = initPos;
	exist = true;
	got = false;

	if (info->get)
	{
		which = 1;
	}
	else
	{
		which = 0;
	}
}

void CollectItem::Update2(PlayerMgr* PlayerMgr, Record* Rec)
{
	if (exist)
	{
		if (this->HitCheck(*PlayerMgr->GetCpuPlayer()) || this->HitCheck(*PlayerMgr->GetOpePlayer()))
		{
			exist = false;
			got = true;
			PlaySE(SE_GET);
			Rec->CollectItem();
		}
	}
}
