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

	const Vector2<float>BASE_POS;	//画面中央の座標
	Vector2<float>floorPos;
	float zoom;	//ズーム率（通常1.0）
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

public:
	BaseCamera() :BASE_POS{ (float)Constants::WIN_WIDTH / 2,(float)Constants::WIN_HEIGHT / 2 } {};
	BaseCamera(Vector2<float> Base) :BASE_POS{ Base } {};
	~BaseCamera() {};
	//基本関数
	void Init();
	void Init(Vector2<float>InitPos, float InitZoom,int MapHeight);

	virtual void Update();

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
	static const int DROP_GROUND_SHAKE_TIME = 2*7;
	static const int DROP_GROUND_SHAKE_SPAN = 2;
	virtual void DropGroundShake()override;
};

//横スクカメラ
class Camera_SideScroll :public BaseCamera
{
	PlayerMgr* player;

public:
	Camera_SideScroll(PlayerMgr* Player) :player{ Player }, BaseCamera({ (float)Constants::WIN_WIDTH * 2/5, (float)Constants::WIN_HEIGHT / 2 }) {};
	~Camera_SideScroll() {};
	void Update();
};

//縦スクカメラ
class Camera_VerticalScroll :public BaseCamera
{
	PlayerMgr* player;

public:
	Camera_VerticalScroll(PlayerMgr* Player) :player{ Player }, BaseCamera({ (float)Constants::WIN_WIDTH / 2, (float)Constants::WIN_HEIGHT * 3 / 4 }) {};
	~Camera_VerticalScroll() {};
	void Update();
};