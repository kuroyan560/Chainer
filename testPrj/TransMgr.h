#pragma once
#include"HashiTrans.h"
class TransMgr
{
	HashiTrans retryTrans;

public:
	void Update();
	void Draw();

	void StartRetryTrans();
	void StartSceneChangeTrans();

	bool FullScreen();
	bool NowTrans();
};