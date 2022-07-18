#pragma once
#include"New_Player.h"
#include"ActRecord.h"
#include"Graphics.h"
#include"EffectMgr.h"
#include"MuraEffect.h"
#include<list>
#include"Vector2.h"
#include"New_Map.h"
#include"New_SpringMgr.h"
#include"New_Goal.h"

namespace New_PlayerMgr_Info
{
	class ChainArrow
	{
		static int CHAIN_ARROW_GRAPH;
		static const Vector2<float>CHAIN_ARROW_GRAPH_SIZE;

		struct Info
		{
			Vector2<float>from;
			Vector2<float>to;
			int easeID_Pal;
			int pal;
			Info();
		};

		std::list<Info>info;
		std::list<Info>::iterator itr;
		static const int NUM = 6;
		static const int LENGTH = 170;

	public:
		ChainArrow();
		~ChainArrow() {};

		void Init();
		void Update(Vector2<float>CpuWorldPos, CamAccessor* Cam);
		void Draw(CamAccessor* Cam);

		void Chained();
	};

	struct ChainStamina
	{
		static const int MAX = 10;
		int amount;

		void Draw();

	private:
		const int POSY = 16;
		const int HEIGHT = 64;
		const int WIDTH = 48;
		const int SPACE = 16;
	};

	class DropEffect
	{
		const int SIZE = 128;
		int graph;
		int easeID_Pal;

	public:
		DropEffect();
		void Emit();

		void Init();
		void Draw(CamAccessor* Cam, Vector2<float>OpeLocalCenter, Vector2<float>CpuLocalCenter);
	};
}

class New_PlayerMgr
{
	static int ARROW_GRAPH;

	static const int DELAY_FLAME_X = 15;
	static const int DELAY_FLAME_Y = 4;
	static const int PLAYER_RANGE = 16;	//２プレイヤー間の距離がどれくらいか

	static const int AFTER_IMAGE_DRAW_SPAN = 12;

	static const int NOW_CHAIN_JUDGE_SPAN = 30;		//チェイン中という判定をとるスパン、敵との当たり判定で使う（小さければ小さいほど厳しい）
	//static const int CHAIN_SPAN = 40;				//チェイン後、次にチェインできるまでのスパン
	static const int CHAIN_CAN_INPUT = 10;			//チェイン後、何フレーム経ったら入力を受け付けるように鳴るか
	//static const int PLAYERS_MAX_DISTANCE = 240;	//２プレイヤー間の最大距離
	//static const int PLAYERS_MAX_DISTANCE_CHAIN = 160;	//チェイン中
	//static const int HOLD_SPAN = 8;	//チェン最高到達点でホールドする時間

	//static const int THROW_POWER = 32;	//チェインのときの投げる力

	//static const int DROP_SPEED = 70;	//ドロップ速さ

	//static const float GRAVITY;

	//int PLAYERS_MAX_DISTANCE;
	//int PLAYERS_MAX_DISTANCE_CHAIN;
	int HOLD_SPAN;
	int THROW_POWER;
	int DROP_SPEED;
	float GRAVITY;

	//ひもの長さ
	static const int MAX_WIRE_LENGTH = 400;
	static const int MIN_WIRE_LENGTH = 150;
	//static const float WIRE_EXTEND_RATE;	//一回チェインするごとに増える長さの割合
	float wireLength;
	float targetWireLength;

	New_Player players[2];
	ActRecord actRecord;
	CamAccessor* cam;

	AfterImage afImg;
	AfterImage afImg2;

	New_SpringMgr spring;

	int ope;
	int cpu;
	//bool canChain;
	//bool enemyKill;	//チェイン時にエネミーをキルしたか
	int chainTimer;	//チェインしてから次にチェイン可能になるまでの時間

	bool dead;
	static const int REVIVE_SPAN = 40;
	int deadTimer;

	void Switch();
	void Chain(New_Map& Map);

	//ドロップ
	static const int DROP_HOLED_TIME = 15;
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

	//チェインスタミナ
	New_PlayerMgr_Info::ChainStamina stamina;

	void PullOpe();
	void PullCpu();

	//宙ぶらりん状態がしばらく続いたら相棒がワープ
	static const int WARP_SPAN = 10;
	int warpTimer;
	bool oldOnGround;	//操作キャラが着地した瞬間をとるため

	//ドロップ入力時のエフェクト
	New_PlayerMgr_Info::DropEffect dropEffect;

public:
	New_PlayerMgr(CamAccessor* Cam, EffectMgr* EffectMgr);
	~New_PlayerMgr();

	//マップ中央、床の座標
	void Init(Vector2<float>CenterFloorPos);
	void Update(New_Map& Map,const New_Goal& Goal);
	void Draw(bool Goal);
	void Finalize();

	Vector2<float>GetVector();	//相棒キャラから操作キャラまでのベクトル
	Vector2<float>GetVector2();	//操作キャラから相棒キャラmでのベクトル
	float GetDistance();	//２キャラの距離


	bool GetOpeOnGround() { return players[ope].OnGround(); }
	bool NowChainHit();
	bool AfImgDraw();

	//操作キャラ
	New_Player* GetOpePlayer() { return &players[ope]; }
	New_Player* GetCpuPlayer() { return &players[cpu]; }

	int GetOpeNum()const { return ope; }
	int GetCpuNum()const { return cpu; }

	//敵を倒したときフラグをたてる
	void RaiseCanChain(int who);

	bool NowDrop() { return dropFlag; }

	//スタート演出のドロップ
	//void StartDrop();

	//２キャラとも空中にいるか
	bool TwoNotOnGround();

	//急落下中
	bool FastFall() { return (30 < players[ope].GetMoveY()); }

	bool GetDead(){ return (REVIVE_SPAN <= deadTimer); }
	float GetZoomChangeRate();

	bool GetAlive() { return (players[cpu].GetAlive() && players[ope].GetAlive()); }
};

