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
	static const int PLAYER_RANGE = 16;	//�Q�v���C���[�Ԃ̋������ǂꂭ�炢��

	static const int AFTER_IMAGE_DRAW_SPAN = 12;

	static const int NOW_CHAIN_JUDGE_SPAN = 30;		//�`�F�C�����Ƃ���������Ƃ�X�p���A�G�Ƃ̓����蔻��Ŏg���i��������Ώ������قǌ������j
	//static const int CHAIN_SPAN = 40;				//�`�F�C����A���Ƀ`�F�C���ł���܂ł̃X�p��
	static const int CHAIN_CAN_INPUT = 10;			//�`�F�C����A���t���[���o��������͂��󂯕t����悤�ɖ邩
	//static const int PLAYERS_MAX_DISTANCE = 240;	//�Q�v���C���[�Ԃ̍ő勗��
	//static const int PLAYERS_MAX_DISTANCE_CHAIN = 160;	//�`�F�C����
	//static const int HOLD_SPAN = 8;	//�`�F���ō����B�_�Ńz�[���h���鎞��

	//static const int THROW_POWER = 32;	//�`�F�C���̂Ƃ��̓������

	//static const int DROP_SPEED = 70;	//�h���b�v����

	//static const float GRAVITY;

	//int PLAYERS_MAX_DISTANCE;
	//int PLAYERS_MAX_DISTANCE_CHAIN;
	int HOLD_SPAN;
	int THROW_POWER;
	int DROP_SPEED;
	float GRAVITY;

	//�Ђ��̒���
	static const int MAX_WIRE_LENGTH = 400;
	static const int MIN_WIRE_LENGTH = 150;
	//static const float WIRE_EXTEND_RATE;	//���`�F�C�����邲�Ƃɑ����钷���̊���
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
	//bool enemyKill;	//�`�F�C�����ɃG�l�~�[���L��������
	int chainTimer;	//�`�F�C�����Ă��玟�Ƀ`�F�C���\�ɂȂ�܂ł̎���

	bool dead;
	static const int REVIVE_SPAN = 40;
	int deadTimer;

	void Switch();
	void Chain(New_Map& Map);

	//�h���b�v
	static const int DROP_HOLED_TIME = 15;
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

	//�`�F�C���X�^�~�i
	New_PlayerMgr_Info::ChainStamina stamina;

	void PullOpe();
	void PullCpu();

	//���Ԃ����Ԃ����΂炭�������瑊�_�����[�v
	static const int WARP_SPAN = 10;
	int warpTimer;
	bool oldOnGround;	//����L���������n�����u�Ԃ��Ƃ邽��

	//�h���b�v���͎��̃G�t�F�N�g
	New_PlayerMgr_Info::DropEffect dropEffect;

public:
	New_PlayerMgr(CamAccessor* Cam, EffectMgr* EffectMgr);
	~New_PlayerMgr();

	//�}�b�v�����A���̍��W
	void Init(Vector2<float>CenterFloorPos);
	void Update(New_Map& Map,const New_Goal& Goal);
	void Draw(bool Goal);
	void Finalize();

	Vector2<float>GetVector();	//���_�L�������瑀��L�����܂ł̃x�N�g��
	Vector2<float>GetVector2();	//����L�������瑊�_�L����m�ł̃x�N�g��
	float GetDistance();	//�Q�L�����̋���


	bool GetOpeOnGround() { return players[ope].OnGround(); }
	bool NowChainHit();
	bool AfImgDraw();

	//����L����
	New_Player* GetOpePlayer() { return &players[ope]; }
	New_Player* GetCpuPlayer() { return &players[cpu]; }

	int GetOpeNum()const { return ope; }
	int GetCpuNum()const { return cpu; }

	//�G��|�����Ƃ��t���O�����Ă�
	void RaiseCanChain(int who);

	bool NowDrop() { return dropFlag; }

	//�X�^�[�g���o�̃h���b�v
	//void StartDrop();

	//�Q�L�����Ƃ��󒆂ɂ��邩
	bool TwoNotOnGround();

	//�}������
	bool FastFall() { return (30 < players[ope].GetMoveY()); }

	bool GetDead(){ return (REVIVE_SPAN <= deadTimer); }
	float GetZoomChangeRate();

	bool GetAlive() { return (players[cpu].GetAlive() && players[ope].GetAlive()); }
};

