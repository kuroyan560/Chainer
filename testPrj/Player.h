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
	//�`�F�C��
	static const int THROWN_POWER = 32;


	//�O���t�B�b�N�X
	static bool LOAD_GRAPHICS;

	static Image LIGHT;

	static LoopImage RUN_IMAGE[2];
	static LoopImage JUMP_IMAGE[2];
	static LoopImage DROP_IMAGE[2];
	static AnimationImage GROUND_IMAGE[2];

	//�A�E�g���C��
	static LoopImage RUN_OUT_LINE[2];
	static LoopImage JUMP_OUT_LINE[2];

	//�e
	static LoopImage JUMP_SHADOW[2];
	static LoopImage DROP_SHADOW[2];

	//�v���C���[����
	static int ID;
	int id;

	static const float INPUT_REACT_RATE;

	bool alive;

	Vector2<float>move;
	float targetMoveX;

	ActRecord* actQueue;

	EffectMgr* effect;

	//�Ǐ]�C�[�W���O
	static const float FOLLOW_EASING_SPEED;
	float followEaseRate;
	float startMove;
	bool oldNear;
	bool canJump;

	//Y�������}�b�v�̍���
	int maxY;

	bool dropFlag;

	bool chain;
	
	static const int CHAIN_FLASH_SPAN = 20;
	int flashTimer;

	bool start;
	bool groundStart;

	bool leftRightHitFlag;

	//�ō����B�_�Ńz�[���h���鎞��
	static const int HOLD_SPAN = 8;
	int holdTimer;

	bool turnFlag;

public:
	Player();
	~Player();

	void Set(CamAccessor* Camera,EffectMgr* EffectMgr, ActRecord* Queue);

	//��{�֐�
	void Init(Vector2<float>InitPos, int MapHeight);
	void LightDraw();
	void Draw(int Ope, bool CanChain,bool NotGoal);
	void Update(float Gravity);
	void AutoRun();
	void Back();
	void Finalize();

	void Thrown(bool OpeTurnFlag);
	void Thrown2(Vector2<float>StickVec);

	//����L����
	void OpeUpdate();

	//�񑀍�L����
	void CpuUpdate(const Player& OpePlayer, const int& Range);

	//�ړ��ʃ��Z�b�g�i�`�F�C�����O�t���[���̈ړ��ʂ��c��Ȃ��悤�Ɂj
	void ResetMove() { move = { 0.0F,0.0F }; }
	void ResetMoveX() { move.x = 0.0F; }

	//�|�W�V�����C��
	void PosFixed(Vector2<float>Move);

	//���n�ʂɂ��Ă��邩
	bool OnGround() { return canJump; }

	//�h���b�v
	void DropMove();

	//�}�b�v�`�b�v�Ƃ̓����蔻��
	void HitCheckMap(Map& Map,bool TwoNotOnGround);

	//�S�[����̏������蔻��
	void HitFloor();

	bool GetAlive() { return alive; }

	//���n�A�j���[�V�����I�������
	bool GetNowGroundAnimation()const { return GROUND_IMAGE[id].NowAnimation(); }

	void KickSmoke();

	bool NowDrop()const { return dropFlag; }

	int GetShadowGraph()const;

	void StartChainFlash() { flashTimer = CHAIN_FLASH_SPAN; }

	void RaiseStart() { start = true; }

	bool GetTurnFlag() { return turnFlag; }
};