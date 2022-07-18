#pragma once
#include"Vector2.h"
//�J�����̃I�u�W�F�N�g�Ƃ̑���
class CamAccessor
{
public:
	virtual ~CamAccessor() {};
	//���W���J�����ɓK�����������̂�Ԃ�
	virtual float CamAffectX(const float WorldX)const = 0;
	virtual float CamAffectY(const float WorldY)const = 0;
	virtual Vector2<float>CamAffect(const Vector2<float> World)const = 0;
	virtual Vector2<float> CamZoomAffect(Vector2<float>Size)const = 0;
	virtual float CamZoomAffect(float Size)const = 0;
	virtual void DropGroundShake() = 0;
	virtual void Shake(int Power, int Time, int Span) = 0;
};