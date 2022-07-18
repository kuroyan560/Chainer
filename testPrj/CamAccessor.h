#pragma once
#include"Vector2.h"
//カメラのオブジェクトとの窓口
class CamAccessor
{
public:
	virtual ~CamAccessor() {};
	//座標をカメラに適応させたものを返す
	virtual float CamAffectX(const float WorldX)const = 0;
	virtual float CamAffectY(const float WorldY)const = 0;
	virtual Vector2<float>CamAffect(const Vector2<float> World)const = 0;
	virtual Vector2<float> CamZoomAffect(Vector2<float>Size)const = 0;
	virtual float CamZoomAffect(float Size)const = 0;
	virtual void DropGroundShake() = 0;
	virtual void Shake(int Power, int Time, int Span) = 0;
};