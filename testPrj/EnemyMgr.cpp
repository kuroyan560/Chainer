#include "EnemyMgr.h"

void EnemyMgr::Delete()
{
	if (enemys.empty() == false)
	{
		for (int i = 0; i < enemys.size(); i++)
		{
			delete enemys[i];
		}
		enemys.clear();
	}
}

EnemyMgr::EnemyMgr(CamAccessor* Cam)
{
	camPtr = Cam;
}

EnemyMgr::~EnemyMgr()
{
	Delete();
}

void EnemyMgr::Set(const int& EnemyNum, EnemyInfo* Info)
{
	Delete();

	if (Info != nullptr)
	{
		for (int i = 0; i < EnemyNum; i++)
		{
			if ((Info + i)->id == ID_STOP)
			{
				enemys.push_back(new StopEnemy());
				enemys.back()->Set(camPtr, (Info + i)->initPos);
			}
			else if ((Info + i)->id == ID_JUMP)
			{
				enemys.push_back(new JumpEnemy());
				enemys.back()->Set(camPtr, (Info + i)->initPos);
			}
			else if ((Info + i)->id == ID_FLY)
			{
				enemys.push_back(new FlyEnemy());
				enemys.back()->Set(camPtr, (Info + i)->initPos);
			}
		}
	}
}

void EnemyMgr::Init()
{
	if (!enemys.empty())
	{
		for (int i = 0; i < enemys.size(); i++)
		{
			enemys[i]->Init();
		}
	}
}

void EnemyMgr::Update(PlayerMgr* PlayerMgr,Record* Rec)
{
	if (!enemys.empty())
	{
		for (int i = 0; i < enemys.size(); i++)
		{
			if (enemys[i]->GetAlive() == true)
			{
				enemys[i]->BaseUpdate();
				if (enemys[i]->GetUpdateFlag() == true)
				{
					enemys[i]->Update();
				}

				if ((PlayerMgr->NowChainHit() == true || PlayerMgr->GetOpePlayer()->NowDrop() == true) && enemys[i]->HitCheck(*PlayerMgr->GetOpePlayer()) == true)
				{
					WaitTimer(HIT_STOP);
					ShakeCam();

					enemys[i]->Dead();
					PlayerMgr->RaiseCanChain(PlayerMgr->GetOpeNum());
					Rec->EnemyKillScoreGet();
				}
			}
			if (enemys[i]->GetAlive() == true)
			{
				if ((PlayerMgr->NowChainHit() == true || PlayerMgr->GetCpuPlayer()->NowDrop() == true) && enemys[i]->HitCheck(*PlayerMgr->GetCpuPlayer()) == true)
				{
					WaitTimer(HIT_STOP);
					ShakeCam();
					enemys[i]->Dead();
					PlayerMgr->RaiseCanChain(PlayerMgr->GetCpuNum());
					Rec->EnemyKillScoreGet();
				}
			}
		}
	}
}

void EnemyMgr::Draw()
{
	if (!enemys.empty())
	{
		for (int i = 0; i < enemys.size(); i++)
		{
			enemys[i]->Draw();
		}
	}
}

void EnemyMgr::Finalize()
{
}

void EnemyMgr::ShakeCam()
{
	camPtr->Shake(SHAKE_POWER, SHAKE_TIME, SHAKE_SPAN);
}
