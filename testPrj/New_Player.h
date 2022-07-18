#pragma once
#include"BaseObject.h"
#include"ActRecord.h"
#include"New_Map.h"
#include"Graphics.h"
#include"EffectMgr.h"

namespace New_PlayerInfo
{
	enum PlayableChara { YUA, MAKI };
}

class  New_Player :public Object
{
public:
	static const int SIZE = 64;
private:
	static const int HIT_FIXED = 0;
	static const int MAX_MOVE_SPEED = 9;
	static const int JUMP_POWER = 20;
	static const int RUN_SPEED = 9; //9


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

	bool dropFlag;

	static const int CHAIN_FLASH_SPAN = 20;
	int flashTimer;

	bool start;
	bool groundStart;

	bool leftRightHitFlag;

	//最高到達点でホールドする時間
	//static const int HOLD_SPAN = 8;
public:
	int holdTimer;
	bool onGround;
private:
	bool turnFlag;

public:
	bool chain;
private:
	//新チェイン
	const int MAX_CHAIN_POWER = 110;
	const int MIN_CHAIN_POWER = 35;
	//Vector2<float>from;
	//Vector2<float>to;
	//bool rightSpinFlag;
	//float chainPower;
	int chainMoveEaseID;
	Vector2<float>chainMove;
	//チェインフラグ降りてなかった時自動で下ろす（応急処置）
	static const int CHAIN_SPAN = 18;
	int chainTimer;	

	bool hogeA;
	bool hogeX;

	bool stickLeft;

	//訂正量
	Vector2<float>fixed;

public:
	float spin;
	New_Player();
	~New_Player();

	void Set(CamAccessor* Camera, EffectMgr* EffectMgr, ActRecord* Queue);

	//基本関数
	void Init(Vector2<float>InitPos);
	void LightDraw();
	void Draw(int Ope);
	void Update(float Gravity,Vector2<float>PartnerPos,bool Goal);
	//void AutoRun();
	//void Back();
	void Finalize();

	void Kill();

	//void Thrown(bool OpeTurnFlag);
	//void Thrown2(Vector2<float>StickVec,int HoldSpan,float Power);
	void Thrown3(New_Map& Map, Vector2<float>StickVec, Vector2<float>OpePos, int MaxLength, int MinLength, int NowLength, int HoldSpan);

	//操作キャラ
	void OpeUpdate();

	//非操作キャラ
	void CpuUpdate(const New_Player& OpePlayer, const int& Range);

	//移動量リセット（チェイン中前フレームの移動量が残らないように）
	void ResetMove() { move = { 0.0F,0.0F }; targetMoveX = 0.0f; }
	void ResetMoveX() { move.x = 0.0F; }
	void ResetMoveY() { move.y = 0.0F; }

	//ポジション修正
	void PosFixed(Vector2<float>Move);

	//今地面についているか
	bool OnGround() { return onGround; }

	//ドロップ
	void DropMove(int DropSpeed);

	//マップチップとの当たり判定
	//void HitCheckMap(Map& Map, bool TwoNotOnGround);
	void HitCheckMap(New_Map& Map);

	//ゴール後の床当たり判定
	//void HitFloor();

	bool GetAlive() { return alive; }

	//着地アニメーション終わったか
	bool GetNowGroundAnimation()const { return GROUND_IMAGE[id].NowAnimation(); }

	void KickSmoke();

	bool NowDrop()const { return dropFlag; }

	int GetShadowGraph()const;

	void StartChainFlash() { flashTimer = CHAIN_FLASH_SPAN; }

	void RaiseStart() { start = true; }

	bool GetTurnFlag() { return turnFlag; }

	float GetMoveY()const { return move.y; }

	void StopMove() {
		move = NONE_VECTOR2;
		chainMove = NONE_VECTOR2;
	}

	//近づけさせる
	void LerpToPos(const Vector2<int>Pos);

	void Jump(float NowLength, float MinLength);
};