#pragma once
#include"Vector2.h"
#include"CamAccessor.h"

//�I�u�W�F�N�g���ۃN���X===========================================
class Object
{
protected:
	Vector2<float>pos;		//���S���[���h���W
	Vector2<float>size;		//x�c�����Ay�c�c��

	//�����蔻��Ƃ�͈͒����p�i�����蔻����L��������苷���������j
	Vector2<int>hitFixed;

	//�J�����ւ̑���
	CamAccessor* camPtr;

public:
	virtual ~Object() {};

//�Q�b�^
	//���[���h���W�i�����j
	Vector2<float> WorldPos()const { return pos; }
	float WorldX()const { return pos.x; }
	float WorldY()const { return pos.y; }

	//���[���h���W�i�����蔻��Ŏg���j
	float WorldX1()const { return pos.x - size.x / 2; }
	float WorldY1()const { return pos.y - size.y / 2; }
	float WorldX2()const { return pos.x + size.x / 2; }
	float WorldY2()const { return pos.y + size.y / 2; }

	//���[�J�����W�i�����j
	float LocalX()const;
	float LocalY()const;
	Vector2<float> LocalPos()const { return { LocalX(),LocalY() }; }

	//���[�J�����W(�`��ȂǂŎg���j
	float LocalX1()const;
	float LocalY1()const;
	float LocalX2()const;
	float LocalY2()const;

	//�����蔻��p�̍��W�i���z�֐��ɂ��Ă���̂́A�����蔻������͈͂̈ʒu���ς��\�������邽�߁j
	virtual float HitX1()const;
	virtual float HitY1()const;
	virtual float HitX2()const;
	virtual float HitY2()const;

//�Z�b�^
	void SetPos(Vector2<float> SetPos) { pos = SetPos; }
	void SetX(float initPosX) { pos.x = initPosX; }
	void SetY(float initPosY) { pos.y = initPosY; }

//�����蔻��
	bool HitCheck(const Object& obj);

//�߂��ɂ��邩�ǂ���
	bool NearCheck(const Object &obj,int Range);

//��ʓ��ɉf���Ă��邩
	bool InScreenCheck();
};