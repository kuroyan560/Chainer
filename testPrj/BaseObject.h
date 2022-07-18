#pragma once
#include"Vector2.h"
#include"CamAccessor.h"

//オブジェクト抽象クラス===========================================
class Object
{
protected:
	Vector2<float>pos;		//中心ワールド座標
	Vector2<float>size;		//x…横幅、y…縦幅

	//当たり判定とる範囲調整用（当たり判定を広く取ったり狭く取ったり）
	Vector2<int>hitFixed;

	//カメラへの窓口
	CamAccessor* camPtr;

public:
	virtual ~Object() {};

//ゲッタ
	//ワールド座標（中央）
	Vector2<float> WorldPos()const { return pos; }
	float WorldX()const { return pos.x; }
	float WorldY()const { return pos.y; }

	//ワールド座標（当たり判定で使う）
	float WorldX1()const { return pos.x - size.x / 2; }
	float WorldY1()const { return pos.y - size.y / 2; }
	float WorldX2()const { return pos.x + size.x / 2; }
	float WorldY2()const { return pos.y + size.y / 2; }

	//ローカル座標（中央）
	float LocalX()const;
	float LocalY()const;
	Vector2<float> LocalPos()const { return { LocalX(),LocalY() }; }

	//ローカル座標(描画などで使う）
	float LocalX1()const;
	float LocalY1()const;
	float LocalX2()const;
	float LocalY2()const;

	//当たり判定用の座標（仮想関数にしているのは、当たり判定を取る範囲の位置が変わる可能性があるため）
	virtual float HitX1()const;
	virtual float HitY1()const;
	virtual float HitX2()const;
	virtual float HitY2()const;

//セッタ
	void SetPos(Vector2<float> SetPos) { pos = SetPos; }
	void SetX(float initPosX) { pos.x = initPosX; }
	void SetY(float initPosY) { pos.y = initPosY; }

//当たり判定
	bool HitCheck(const Object& obj);

//近くにいるかどうか
	bool NearCheck(const Object &obj,int Range);

//画面内に映っているか
	bool InScreenCheck();
};