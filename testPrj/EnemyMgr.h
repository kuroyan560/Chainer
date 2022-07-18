#pragma once
#include"Enemy.h"
#include"StopEnemy.h"
#include"FlyEnemy.h"
#include"JumpEnemy.h"
#include<vector>
#include"PlayerMgr.h"
#include"Record.h"

class EnemyMgr
{
	std::vector<Enemy*>enemys;
	CamAccessor* camPtr;

	static const int SHAKE_POWER = 15;
	static const int SHAKE_TIME = 30;
	static const int SHAKE_SPAN = 3;
	static const int HIT_STOP = 20;

	void Delete();

public:
	EnemyMgr(CamAccessor* Cam);
	~EnemyMgr();

	void Set(const int& EnemyNum, EnemyInfo* Info);
	void Init();
	void Update(PlayerMgr* PlayerMgr,Record* Rec);
	void Draw();
	void Finalize();

	void ShakeCam();
};