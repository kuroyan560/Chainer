#pragma once
#include"Vector2.h"
#include"CamAccessor.h"
#include"PlayerMgr.h"
#include"Constants.h"

class BaseCamera : public CamAccessor
{
protected:
	static const int NORMAL_ZOOM = 1;
	static const float SET_SPEED_RATE;

	const Vector2<float>BASE_POS;	//��ʒ����̍��W
	Vector2<float>floorPos;
	float zoom;	//�Y�[�����i�ʏ�1.0�j
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

public:
	BaseCamera() :BASE_POS{ (float)Constants::WIN_WIDTH / 2,(float)Constants::WIN_HEIGHT / 2 } {};
	BaseCamera(Vector2<float> Base) :BASE_POS{ Base } {};
	~BaseCamera() {};
	//��{�֐�
	void Init();
	void Init(Vector2<float>InitPos, float InitZoom,int MapHeight);

	virtual void Update();

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
	static const int DROP_GROUND_SHAKE_TIME = 2*7;
	static const int DROP_GROUND_SHAKE_SPAN = 2;
	virtual void DropGroundShake()override;
};

//���X�N�J����
class Camera_SideScroll :public BaseCamera
{
	PlayerMgr* player;

public:
	Camera_SideScroll(PlayerMgr* Player) :player{ Player }, BaseCamera({ (float)Constants::WIN_WIDTH * 2/5, (float)Constants::WIN_HEIGHT / 2 }) {};
	~Camera_SideScroll() {};
	void Update();
};

//�c�X�N�J����
class Camera_VerticalScroll :public BaseCamera
{
	PlayerMgr* player;

public:
	Camera_VerticalScroll(PlayerMgr* Player) :player{ Player }, BaseCamera({ (float)Constants::WIN_WIDTH / 2, (float)Constants::WIN_HEIGHT * 3 / 4 }) {};
	~Camera_VerticalScroll() {};
	void Update();
};