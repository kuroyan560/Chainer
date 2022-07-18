#include "TransMgr.h"

void TransMgr::Update()
{
	retryTrans.Update();
}

void TransMgr::Draw()
{
	retryTrans.Draw();
}

void TransMgr::StartRetryTrans()
{
	retryTrans.Start();
}

void TransMgr::StartSceneChangeTrans()
{
	retryTrans.Start();
}

bool TransMgr::FullScreen()
{
	if (retryTrans.FullScreen())
	{
		return true;
	}
	else return false;
}

bool TransMgr::NowTrans()
{
	if (retryTrans.NowTrans())
	{
		return true;
	}
	return false;
}
