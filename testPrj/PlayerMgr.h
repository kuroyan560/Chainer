#pragma once
#include"Player.h"
#include"ActRecord.h"
#include"Map.h"
#include"Graphics.h"
#include"EffectMgr.h"
#include"MuraEffect.h"
#include"New_PlayerMgr.h"

class PlayerMgr
{
	static int ARROW_GRAPH;

	static const int DELAY_FLAME_X = 15;
	static const int DELAY_FLAME_Y = 4;
	static const int PLAYER_RANGE = 16;	//２プレイヤー間の距離がどれくらいか

	static const int AFTER_IMAGE_DRAW_SPAN = 12;
	
	static const int NOW_CHAIN_JUDGE_SPAN = 30;		//チェイン中という判定をとるスパン、敵との当たり判定で使う（小さければ小さいほど厳しい）
	static const int CHAIN_SPAN = 40;				//チェイン後、次にチェインできるまでのスパン
	static const int CHAIN_CAN_INPUT = 10;			//チェイン後、何フレーム経ったら入力を受け付けるように鳴るか
	static const int PLAYERS_MAX_DISTANCE = 180;	//２プレイヤー間の最大距離
	static const int PLAYERS_MAX_DISTANCE_CHAIN = 145;

	static const float GRAVITY;

	float maxDistance;

	Player players[2];
	ActRecord actRecord;
	CamAccessor* cam;

	AfterImage afImg;
	AfterImage afImg2;

	SpringMgr spring;

	Map* mapPtr;

	int ope;
	int cpu;
	bool canChain;
	bool enemyKill;	//チェイン時にエネミーをキルしたか
	int chainTimer;	//チェインしてから次にチェイン可能になるまでの時間

	bool dead;
	static const int REVIVE_SPAN = 40;
	int deadTimer;

	int goalLine;

	void Switch();
	void Chain();

	//ドロップ
	static const int DROP_HOLED_TIME = 5;
	static const int GROUND_HOLED_TIME = 10;
	bool shakeFlag;
	bool dropFlag;
	int dropTimer;
	int groundTimer;
	void Drop();
	void DropUpdate();

	EffectMgr* effect;

	//チェイン射出方向矢印
	New_PlayerMgr_Info::ChainArrow chainArrow;

public:
	PlayerMgr(CamAccessor* Cam, EffectMgr* EffectMgr);
	~PlayerMgr();

	void Init(Map* Map,Vector2<float>InitPos);
	void Update();
	void Draw();
	void Finalize();

	Vector2<float>GetVector();	//相棒キャラから操作キャラまでのベクトル
	Vector2<float>GetVector2();	//操作キャラから相棒キャラmでのベクトル
	float GetDistance();	//２キャラの距離


	bool GetOpeOnGround() { return players[ope].OnGround(); }
	//チェイン中かどうか
	bool NowChain();
	bool NowChainHit();
	bool AfImgDraw();
	//ゴールしたかどうか
	bool Goaled();
	bool GetDead();

	//操作キャラ
	Player* GetOpePlayer() { return &players[ope]; }
	Player* GetCpuPlayer() { return &players[cpu]; }

	int GetOpeNum()const { return ope; }
	int GetCpuNum()const { return cpu; }

	//敵を倒したときフラグをたてる
	void RaiseCanChain(int who);

	bool NowDrop() { return dropFlag; }

	//スタート演出のドロップ
	void StartDrop();

	//２キャラとも空中にいるか
	bool TwoNotOnGround();
};