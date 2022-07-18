#pragma once
#include"BaseObject.h"
#include"ActRecord.h"
#include"Map.h"
#include"Graphics.h"
#include"EffectMgr.h"

enum PlayableChara { YUA, MAKI };

class Player :public Object
{
	static const int SIZE = 64;
	static const int HIT_FIXED = 0;
	static const int MAX_MOVE_SPEED = 9;
	static const int JUMP_POWER = 15;
	static const int RUN_SPEED = 9; //9
	static const int DROP_SPEED = 70;
	//チェイン
	static const int THROWN_POWER = 32;


	//グラフィックス
	static bool LOAD_GRAPHICS;

	static Image LIGHT;

	static LoopImage RUN_IMAGE[2];
	static LoopImage JUMP_IMAGE[2];
	static LoopImage DROP_IMAGE[2];
	static AnimationImage GROUND_IMAGE[2];

	//アウトライン
	static LoopImage RUN_OUT_LINE[2];
	static LoopImage JUMP_OUT_LINE[2];

	//影
	static LoopImage JUMP_SHADOW[2];
	static LoopImage DROP_SHADOW[2];

	//プレイヤー識別
	static int ID;
	int id;

	static const float INPUT_REACT_RATE;

	bool alive;

	Vector2<float>move;
	float targetMoveX;

	ActRecord* actQueue;

	EffectMgr* effect;

	//追従イージング
	static const float FOLLOW_EASING_SPEED;
	float followEaseRate;
	float startMove;
	bool oldNear;
	bool canJump;

	//Y軸方向マップの高さ
	int maxY;

	bool dropFlag;

	bool chain;
	
	static const int CHAIN_FLASH_SPAN = 20;
	int flashTimer;

	bool start;
	bool groundStart;

	bool leftRightHitFlag;

	//最高到達点でホールドする時間
	static const int HOLD_SPAN = 8;
	int holdTimer;

	bool turnFlag;

public:
	Player();
	~Player();

	void Set(CamAccessor* Camera,EffectMgr* EffectMgr, ActRecord* Queue);

	//基本関数
	void Init(Vector2<float>InitPos, int MapHeight);
	void LightDraw();
	void Draw(int Ope, bool CanChain,bool NotGoal);
	void Update(float Gravity);
	void AutoRun();
	void Back();
	void Finalize();

	void Thrown(bool OpeTurnFlag);
	void Thrown2(Vector2<float>StickVec);

	//操作キャラ
	void OpeUpdate();

	//非操作キャラ
	void CpuUpdate(const Player& OpePlayer, const int& Range);

	//移動量リセット（チェイン中前フレームの移動量が残らないように）
	void ResetMove() { move = { 0.0F,0.0F }; }
	void ResetMoveX() { move.x = 0.0F; }

	//ポジション修正
	void PosFixed(Vector2<float>Move);

	//今地面についているか
	bool OnGround() { return canJump; }

	//ドロップ
	void DropMove();

	//マップチップとの当たり判定
	void HitCheckMap(Map& Map,bool TwoNotOnGround);

	//ゴール後の床当たり判定
	void HitFloor();

	bool GetAlive() { return alive; }

	//着地アニメーション終わったか
	bool GetNowGroundAnimation()const { return GROUND_IMAGE[id].NowAnimation(); }

	void KickSmoke();

	bool NowDrop()const { return dropFlag; }

	int GetShadowGraph()const;

	void StartChainFlash() { flashTimer = CHAIN_FLASH_SPAN; }

	void RaiseStart() { start = true; }

	bool GetTurnFlag() { return turnFlag; }
};