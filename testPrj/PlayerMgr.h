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
	static const int PLAYER_RANGE = 16;	//�Q�v���C���[�Ԃ̋������ǂꂭ�炢��

	static const int AFTER_IMAGE_DRAW_SPAN = 12;
	
	static const int NOW_CHAIN_JUDGE_SPAN = 30;		//�`�F�C�����Ƃ���������Ƃ�X�p���A�G�Ƃ̓����蔻��Ŏg���i��������Ώ������قǌ������j
	static const int CHAIN_SPAN = 40;				//�`�F�C����A���Ƀ`�F�C���ł���܂ł̃X�p��
	static const int CHAIN_CAN_INPUT = 10;			//�`�F�C����A���t���[���o��������͂��󂯕t����悤�ɖ邩
	static const int PLAYERS_MAX_DISTANCE = 180;	//�Q�v���C���[�Ԃ̍ő勗��
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
	bool enemyKill;	//�`�F�C�����ɃG�l�~�[���L��������
	int chainTimer;	//�`�F�C�����Ă��玟�Ƀ`�F�C���\�ɂȂ�܂ł̎���

	bool dead;
	static const int REVIVE_SPAN = 40;
	int deadTimer;

	int goalLine;

	void Switch();
	void Chain();

	//�h���b�v
	static const int DROP_HOLED_TIME = 5;
	static const int GROUND_HOLED_TIME = 10;
	bool shakeFlag;
	bool dropFlag;
	int dropTimer;
	int groundTimer;
	void Drop();
	void DropUpdate();

	EffectMgr* effect;

	//�`�F�C���ˏo�������
	New_PlayerMgr_Info::ChainArrow chainArrow;

public:
	PlayerMgr(CamAccessor* Cam, EffectMgr* EffectMgr);
	~PlayerMgr();

	void Init(Map* Map,Vector2<float>InitPos);
	void Update();
	void Draw();
	void Finalize();

	Vector2<float>GetVector();	//���_�L�������瑀��L�����܂ł̃x�N�g��
	Vector2<float>GetVector2();	//����L�������瑊�_�L����m�ł̃x�N�g��
	float GetDistance();	//�Q�L�����̋���


	bool GetOpeOnGround() { return players[ope].OnGround(); }
	//�`�F�C�������ǂ���
	bool NowChain();
	bool NowChainHit();
	bool AfImgDraw();
	//�S�[���������ǂ���
	bool Goaled();
	bool GetDead();

	//����L����
	Player* GetOpePlayer() { return &players[ope]; }
	Player* GetCpuPlayer() { return &players[cpu]; }

	int GetOpeNum()const { return ope; }
	int GetCpuNum()const { return cpu; }

	//�G��|�����Ƃ��t���O�����Ă�
	void RaiseCanChain(int who);

	bool NowDrop() { return dropFlag; }

	//�X�^�[�g���o�̃h���b�v
	void StartDrop();

	//�Q�L�����Ƃ��󒆂ɂ��邩
	bool TwoNotOnGround();
};