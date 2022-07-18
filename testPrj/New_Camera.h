#pragma once
#include"CamAccessor.h"
#include"Vector2.h"
class New_Camera : public CamAccessor
{
	const Vector2<float>BASE_POS;	//画面中央の座標
	Vector2<float>pos;
	const float MIN_ZOOM = 0.4f;
	const float MAX_ZOOM = 1.0f;
	const float GOAL_ZOOM = 6.0f;
	float zoom;	//ズーム率（通常1.0）
	float toZoom;

	int mapWidth;
	int mapHeight;

	//カメラシェイク用
	Vector2<float> shake;	//カメラシェイク量
	float fullPower;		//最大カメラシェイク強さ
	float power;			//カメラシェイクの強さ
	int fullShakeFlame;		//カメラシェイクする時間
	int flameTimer;			//時間測る用（フレーム単位）
	int span;				//カメラシェイク更新する処理（フレームレート）

	void ShakeInit();
	void ShakeUpdate();

	int goalEaseID;
	const int GOAL_SPAN = 15;
	int goalTimer;
	bool goalMoveFlag;

public:
	New_Camera();
	~New_Camera() {};

	//基本関数
	void Init(int MapX, int MapY);
	void Update(const Vector2<float>& FollowPos, float FollowRate, float ZoomChangeRate);
	bool GoalUpdate(const Vector2<int>GoalPos,bool GoalTrigger);	//拡大完了したらtrueを返す

	void Debug();

	//その他カメラ機能
	//座標をカメラに適応させる（オブジェクトクラスからアクセス可能）
	virtual float CamAffectX(const float WorldX)const override;
	virtual float CamAffectY(const float WorldY)const override;
	virtual Vector2<float>CamAffect(const Vector2<float> World)const override;

	virtual Vector2<float> CamZoomAffect(Vector2<float>Size)const { return { Size.x * zoom,Size.y * zoom }; }
	virtual float CamZoomAffect(float Size)const { return Size * zoom; }

	virtual void Shake(int Power, int Time, int Span)override;

	//プレイヤーが地面に着地したとき
	static const int DROP_GROUND_SHAKE_POWER = 20;
	static const int DROP_GROUND_SHAKE_TIME = 2 * 7;
	static const int DROP_GROUND_SHAKE_SPAN = 2;
	virtual void DropGroundShake()override;
};