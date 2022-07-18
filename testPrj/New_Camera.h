#pragma once
#include"CamAccessor.h"
#include"Vector2.h"
class New_Camera : public CamAccessor
{
	const Vector2<float>BASE_POS;	//��ʒ����̍��W
	Vector2<float>pos;
	const float MIN_ZOOM = 0.4f;
	const float MAX_ZOOM = 1.0f;
	const float GOAL_ZOOM = 6.0f;
	float zoom;	//�Y�[�����i�ʏ�1.0�j
	float toZoom;

	int mapWidth;
	int mapHeight;

	//�J�����V�F�C�N�p
	Vector2<float> shake;	//�J�����V�F�C�N��
	float fullPower;		//�ő�J�����V�F�C�N����
	float power;			//�J�����V�F�C�N�̋���
	int fullShakeFlame;		//�J�����V�F�C�N���鎞��
	int flameTimer;			//���ԑ���p�i�t���[���P�ʁj
	int span;				//�J�����V�F�C�N�X�V���鏈���i�t���[�����[�g�j

	void ShakeInit();
	void ShakeUpdate();

	int goalEaseID;
	const int GOAL_SPAN = 15;
	int goalTimer;
	bool goalMoveFlag;

public:
	New_Camera();
	~New_Camera() {};

	//��{�֐�
	void Init(int MapX, int MapY);
	void Update(const Vector2<float>& FollowPos, float FollowRate, float ZoomChangeRate);
	bool GoalUpdate(const Vector2<int>GoalPos,bool GoalTrigger);	//�g�劮��������true��Ԃ�

	void Debug();

	//���̑��J�����@�\
	//���W���J�����ɓK��������i�I�u�W�F�N�g�N���X����A�N�Z�X�\�j
	virtual float CamAffectX(const float WorldX)const override;
	virtual float CamAffectY(const float WorldY)const override;
	virtual Vector2<float>CamAffect(const Vector2<float> World)const override;

	virtual Vector2<float> CamZoomAffect(Vector2<float>Size)const { return { Size.x * zoom,Size.y * zoom }; }
	virtual float CamZoomAffect(float Size)const { return Size * zoom; }

	virtual void Shake(int Power, int Time, int Span)override;

	//�v���C���[���n�ʂɒ��n�����Ƃ�
	static const int DROP_GROUND_SHAKE_POWER = 20;
	static const int DROP_GROUND_SHAKE_TIME = 2 * 7;
	static const int DROP_GROUND_SHAKE_SPAN = 2;
	virtual void DropGroundShake()override;
};