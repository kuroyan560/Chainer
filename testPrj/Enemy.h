#pragma once
#include"BaseObject.h"
#include"Vector2.h"
#include "easing.h"
#include"DxLib.h"
#include"PlayerMgr.h"

enum EnemyID { ID_STOP, ID_JUMP, ID_FLY };

struct EnemyInfo
{
	EnemyID id;
	Vector2<int> initPos;
};

class Enemy : public Object
{
	static const int SIZE = 64;
	bool updateFlag;

protected:
	bool aliveFlag;
	Vector2<float>initPos;
	virtual void ParaInit();

public:
	Enemy();
	~Enemy() {};

	//基本関数
	void Init();
	void Set(CamAccessor* Cam,Vector2<int> InitPos);
	void BaseUpdate();	//一度画面内に入って以降更新処理をするフラグをたてる
	virtual void Update() = 0;
	virtual void Draw();
	void Finalize();
	void Dead() { aliveFlag = false; }

	bool GetAlive()const { return aliveFlag; }
	bool GetUpdateFlag()const { return updateFlag; }
};