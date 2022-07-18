#include "New_Player.h"
#include"UsersInput.h"
#include"Constants.h"
#include"easing.h"
#include"SoundEmitter.h"
#include"MyFunc.h"
#include "New_MapChip.h"
#include"New_ItemMgr.h"

using namespace New_PlayerInfo;

Image New_Player::LIGHT;

LoopImage New_Player::RUN_IMAGE[2];
LoopImage New_Player::JUMP_IMAGE[2];
LoopImage New_Player::DROP_IMAGE[2];
AnimationImage New_Player::GROUND_IMAGE[2];

LoopImage New_Player::RUN_OUT_LINE[2];
LoopImage New_Player::JUMP_OUT_LINE[2];

LoopImage New_Player::JUMP_SHADOW[2];
LoopImage New_Player::DROP_SHADOW[2];

bool New_Player::LOAD_GRAPHICS = false;

int New_Player::ID = 0;

const float New_Player::FOLLOW_EASING_SPEED = 0.1F;

const float New_Player::INPUT_REACT_RATE = 0.1F;


void New_Player::KickSmoke()
{
	if (id == YUA)
	{
		effect->EmitKickSmoke({ LocalX(), LocalY2() }, PURPLE);
	}
	else
	{
		effect->EmitKickSmoke({ LocalX(), LocalY2() }, GREEN);
	}
}

int New_Player::GetShadowGraph() const
{
	if (dropFlag)
	{
		return DROP_SHADOW[id].GetNowGraph();
	}
	return JUMP_SHADOW[id].GetNowGraph();
}

void New_Player::LerpToPos(const Vector2<int> Pos)
{
	onGround = false;
	Vector2<float> p = { (float)Pos.x,(float)Pos.y };
	pos.x = MyFunc::lerp(pos.x, p.x, 0.1f);
	pos.y = MyFunc::lerp(pos.y, p.y, 0.1f);

	if (GetDistance(pos, p) < 0.1)
	{
		pos = p;
	}
}

void New_Player::Jump(float NowLength, float MinLength)
{
	move.x = GetPadVec().x * 15;
	move.y = -JUMP_POWER + 4 - 4  * (NowLength / MinLength);
	//actQueue->PushAct('y', Act{ ActName::JUMP,move.y });	//「ジャンプ」を記録
	KickSmoke();
	PlaySE(SE_JUMP);
	//canJump = false;
	onGround = false;
}



New_Player::New_Player()
{
	size = { SIZE,SIZE };
	hitFixed = { 0,0 };
	followEaseRate = 1.0F;

	if (!LOAD_GRAPHICS)
	{
		LIGHT.Init("_resource/graphics/light.png", true);

		RUN_IMAGE[YUA].Init("_resource/graphics/yua_run.png", true, 9, 3, { 9,1 }, size);
		RUN_IMAGE[MAKI].Init("_resource/graphics/maki_run.png", true, 9, 3, { 9,1 }, size);
		RUN_OUT_LINE[YUA].Init("_resource/graphics/yua_run_out.png", true, 9, 3, { 9,1 }, { 70,70 });
		RUN_OUT_LINE[MAKI].Init("_resource/graphics/maki_run_out.png", true, 9, 3, { 9,1 }, { 70,70 });

		//JUMP_IMAGE[0].Init("_resource/graphics/yua_jump.png", true, 2, 4, { 2,1 }, size);
		//JUMP_IMAGE[1].Init("_resource/graphics/maki_jump.png", true, 2, 4, { 2,1 }, size);

		JUMP_IMAGE[YUA].Init("_resource/graphics/yua_jump.png", true, 4, 3, { 4,1 }, size);
		JUMP_IMAGE[MAKI].Init("_resource/graphics/maki_jump.png", true, 4, 3, { 4,1 }, size);
		JUMP_OUT_LINE[YUA].Init("_resource/graphics/yua_jump_out.png", true, 4, 3, { 4,1 }, { 70,70 });
		JUMP_OUT_LINE[MAKI].Init("_resource/graphics/maki_jump_out.png", true, 4, 3, { 4,1 }, { 70,70 });

		DROP_IMAGE[YUA].Init("_resource/graphics/yua_drop.png", true, 2, 4, { 2,1 }, size);
		DROP_IMAGE[MAKI].Init("_resource/graphics/maki_drop.png", true, 2, 4, { 2,1 }, size);

		GROUND_IMAGE[YUA].Init("_resource/graphics/yua_ground.png", true, 7, 2, { 7,1 }, size, 6, true);
		GROUND_IMAGE[MAKI].Init("_resource/graphics/maki_ground.png", true, 7, 2, { 7,1 }, size, 6, true);

		JUMP_SHADOW[YUA].Init("_resource/graphics/yua_jump_shadow.png", true, 4, 3, { 4,1 }, size);
		JUMP_SHADOW[MAKI].Init("_resource/graphics/maki_jump_shadow.png", true, 4, 3, { 4,1 }, size);
		DROP_SHADOW[YUA].Init("_resource/graphics/yua_drop_shadow.png", true, 2, 4, { 2,1 }, size);
		DROP_SHADOW[MAKI].Init("_resource/graphics/maki_drop_shadow.png", true, 2, 4, { 2,1 }, size);

		LOAD_GRAPHICS = true;
	}

	id = ID++;
	if (ID == 2)ID = 0;
	leftRightHitFlag = false;

	CreateEaseSeed(&chainMoveEaseID, Out, Circ, 0.01f, MAX_CHAIN_POWER, 0);
}

New_Player::~New_Player()
{
}

void New_Player::Set(CamAccessor* Camera, EffectMgr* EffectMgr, ActRecord* Queue)
{
	camPtr = Camera;
	effect = EffectMgr;
	actQueue = Queue;
}

void New_Player::Init(Vector2<float>InitPos)
{
	pos = InitPos;
	move = NONE_VECTOR2;
	onGround = false;
	alive = true;
	dropFlag = false;
	//chain = true;
	GROUND_IMAGE[0].Init();
	GROUND_IMAGE[1].Init();
	groundStart = false;

	holdTimer = 0;

	targetMoveX = 0.0F;

	turnFlag = false;

	//chainMove = NONE_VECTOR2;
	//新チェイン
	chain = false;
	//rightSpinFlag = true;
	EaseInit(chainMoveEaseID);
	chainMove = NONE_VECTOR2;

	fixed = NONE_VECTOR2;

	stickLeft = false;
}

void New_Player::LightDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 20);
	LIGHT.Draw(LocalPos(), size * 7);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void New_Player::Update(float Gravity, Vector2<float>PartnerPos, bool Goal)
{
	fixed = NONE_VECTOR2;

	if (chain)
	{
		chainTimer--;
		//bool Closs = (0 <= GetCross(to, this->WorldPos() - from));
		if (/*Closs != rightSpinFlag || */!chainTimer)
		{
			chain = false;
		}

		//float spin = (!rightSpinFlag ? DX_PI / 2 : -DX_PI / 2);
		if (hogeX)
		{
			spin = DX_PI / 2;
		}
		else if (hogeA)
		{
			spin = -DX_PI / 2;
		}

		chainMove = SpinVector2(CreateVector(PartnerPos, this->WorldPos()), spin);
		float power = GetEaseUpdate(chainMoveEaseID);
		chainMove.x *= power;
		chainMove.y *= power * (hogeX ? 0.8 : 1.0);
	}
	else
	{
		chainMove.x *= 0.8;
		chainMove.y *= 0.8;

		if (fabs(chainMove.x) < 0.1)chainMove.x = 0.0f;
		if (fabs(chainMove.y) < 0.1)chainMove.y = 0.0f;
	}

	//更新
	if (!Goal)
	{
		pos += chainMove;
		pos += move;
	}

	move.y += Gravity;	//重力落下

	if (holdTimer/* && 0 < move.y */&& !dropFlag)
	{
		//move.y = 0;
		move.y -= Gravity;
	}
	if (!chain && holdTimer)
	{
		holdTimer--;
	}

	GROUND_IMAGE[id].Update();
}

//void New_Player::AutoRun()
//{
//	if (!chain && alive)
//	{
//		pos.x += RUN_SPEED;	//オートラン
//	}
//}

//void New_Player::Back()
//{
//	if (!chain)
//	{
//		pos.x -= RUN_SPEED;	//オートラン
//	}
//}

void New_Player::Draw(int Ope)
{
	if (groundStart)
	{
		effect->EmitStartEffect({ LocalX(),LocalY2() });
		groundStart = false;
	}
	//DrawBox(LocalX1(), LocalY1(), LocalX2(), LocalY2(), GetColor(255,0,0), true);
	if (alive)
	{
		if (flashTimer)
		{
			float pal = 255.0F * ((float)flashTimer / (float)CHAIN_FLASH_SPAN);
			if (pal < 128)pal = 128;
			SetDrawBlendMode(DX_BLENDMODE_ADD, pal);
			flashTimer--;
		}
		else if (dropFlag)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD, 230);
		}

		if (GROUND_IMAGE[id].NowAnimation() == false)
		{
			if (onGround)	//地面に足がついているとき
			{
				RUN_IMAGE[id].Draw(LocalPos(), camPtr->CamZoomAffect(size), turnFlag);

				if (id == Ope)
				{
					RUN_OUT_LINE[id].Draw(LocalPos(), camPtr->CamZoomAffect({ 70,70 }), turnFlag);
				}
			}
			else if (dropFlag)
			{
				DROP_IMAGE[id].Draw(LocalPos(), camPtr->CamZoomAffect(size), turnFlag);
			}
			else
			{
				JUMP_IMAGE[id].Draw(LocalPos(), camPtr->CamZoomAffect(size), turnFlag);
				if (id == Ope)
				{
					JUMP_OUT_LINE[id].Draw(LocalPos(), camPtr->CamZoomAffect({ 70,70 }), turnFlag);
				}
			}
		}

		GROUND_IMAGE[id].Draw(LocalPos(), camPtr->CamZoomAffect(size), turnFlag);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//デバッグ用
		//DrawFormatString(0, id*150, (id == Ope)?GetColor(255,0,0):GetColor(255, 255, 255), "ChainMoveX：%.1f\nChainMoveY：%.1f\nrightSpinFlag：%s\nCross：%s",
			//chainMove.x, chainMove.y, rightSpinFlag ? "true" : "false", hoge ? "true" : "false");
		//DrawFormatString(0, id * 200, GetColor(255, 255, 255), "moveX : %.3f\nmoveY : %.3f\n\nchainMoveX : %.3f\nchainMoveY : %.3f",
			//move.x, move.y, chainMove.x, chainMove.y);
	}
}

void New_Player::Finalize()
{
}

void New_Player::Kill()
{
	effect->EmitExplosion(LocalPos());
	PlaySE(SE_DEAD);
	alive = false;
}

//void New_Player::Thrown2(Vector2<float> StickVec, int HoldSpan, float Power)
//{
//	move.y = 0.0F;
//	//move.x = 0.0F;
//
//	if (StickVec.x < 0)
//	{
//		turnFlag = true;
//	}
//	else
//	{
//		turnFlag = false;
//	}
//	float rad = GetRad(StickVec);
//	if (DX_PI / 4 + DX_PI < rad && rad < DX_PI / 4 * 3 + DX_PI)
//	{
//		StickVec.y = -sin(DX_PI / 4);
//	}
//	move += StickVec * Power;
//	if (onGround)
//	{
//		KickSmoke();
//	}
//	PlaySE(SE_CHAIN);
//	onGround = false;
//	chain = true;
//
//	holdTimer = HoldSpan;
//
//	targetMoveX = 0.0f;
//}

void New_Player::Thrown3(New_Map& Map,Vector2<float> StickVec, Vector2<float> OpePos, int MaxLength,int MinLength, int NowLength, int HoldSpan)
{
	bool oldStickLeft = stickLeft;

	hogeA = JustInput(A);
	hogeX = JustInput(X);

	if (StickVec.x < 0)
	{
		hogeA = !hogeA;
		hogeX = !hogeX;

		stickLeft = true;
	}

	if (oldStickLeft != stickLeft)
	{
		move.x = 0.0f;
		chainMove.x = 0.0f;
	}

	if (0 < StickVec.x)
	{
		move.x = 4;
	}

	move.y = 0.0F;
	chain = true;

	//from = OpePos;
	//to = StickVec * NowLength;
	//if (0 <= GetCross(to, this->WorldPos() - from))
	//{
	//	rightSpinFlag = true;
	//	if (OpePos.x < this->pos.x && onGround && 0 < StickVec.x)
	//	{
	//		rightSpinFlag = !rightSpinFlag;
	//	}
	//}
	//else
	//{
	//	rightSpinFlag = false;
	//	/*if (this->pos.x < OpePos.x && onGround && StickVec.x < 0)
	//	{
	//		rightSpinFlag = !rightSpinFlag;
	//	}*/
	//	/*if ((Map.CheckHitWall(OpePos + CreateVector(DX_PI * 3 / 2) * NowLength) != NONE_VECTOR2) 
	//		|| (Map.CheckHitDamage(OpePos + CreateVector(DX_PI * 3 / 2) * NowLength)))
	//	{
	//		rightSpinFlag = !rightSpinFlag;
	//	}*/
	//}

	float rate = (float)(NowLength-MinLength) / (float)(MaxLength - MinLength);
	float power = MIN_CHAIN_POWER + (float)(MAX_CHAIN_POWER - MIN_CHAIN_POWER) * rate;
	if (onGround)
	{
		power = MIN_CHAIN_POWER;
	}
	ReSetEaseStart(chainMoveEaseID, power);

	EaseInit(chainMoveEaseID);
	PlayEase_Trigger(chainMoveEaseID);

	if (onGround)
	{
		KickSmoke();
		onGround = false;
	}
	PlaySE(SE_CHAIN);
	holdTimer = HoldSpan;

	targetMoveX = 0.0f;
	chainTimer = CHAIN_SPAN;
}

void New_Player::OpeUpdate()
{
	move.x = 0.0F;

	//操作の反映（地面についていないときだけX軸の入力受け付ける）
	if ((move.x = MAX_MOVE_SPEED * -Input(InputName::LEFT)) != 0)
	{
		if (chain)
		{
			move.x *= INPUT_REACT_RATE;
		}
		turnFlag = true;
		actQueue->PushAct('x', Act{ ActName::LEFT_ACT, move.x });	//「左に移動」、そのときの移動量を記録
	}
	else if ((move.x = MAX_MOVE_SPEED * Input(InputName::RIGHT)) != 0)
	{
		if (chain)
		{
			move.x *= INPUT_REACT_RATE;
		}
		turnFlag = false;
		actQueue->PushAct('x', Act{ ActName::RIGHT_ACT, move.x });	//「右に移動」、そのときの移動量を記録
	}

	if (move.x == 0.0F)
	{
		actQueue->PushAct('x', Act{ ActName::NONEACT,0.0F });
	}

	//if (JustInput(InputName::A) && canJump)
	//{
	//	move.y = -JUMP_POWER;
	//	actQueue->PushAct('y', Act{ ActName::JUMP,move.y });	//「ジャンプ」を記録
	//	KickSmoke();
	//	PlaySE(SE_JUMP);
	//	canJump = false;
	//}
	//else
	//{
	actQueue->PushAct('y', Act{ ActName::NONEACT,0.0F });
	//}
}

void New_Player::CpuUpdate(const New_Player& OpeNew_Player, const int& Range)
{
	if (OpeNew_Player.WorldX() < this->WorldX())
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (NearCheck(OpeNew_Player, Range) == false)
	{
		//CPUの動き
		Act act = actQueue->GetAct('x');

		if (act.name == ActName::LEFT_ACT || act.name == ActName::RIGHT_ACT)
		{
			targetMoveX = act.move;
		}

		move.x = MyFunc::lerp(move.x, targetMoveX, 0.05F);

		oldNear = false;
	}
	else
	{
		//move.x = 0.0F;

		if (followEaseRate < 1.0F)
		{
			followEaseRate += FOLLOW_EASING_SPEED;
		}

		if (!oldNear)
		{
			followEaseRate = 0.0F;
			startMove = move.x;
			oldNear = true;
		}

		move.x = (0.0F - startMove) * Ease(In, Cubic, followEaseRate) + startMove;
	}

	Act actY = actQueue->GetAct('y');
	if (actY.name == ActName::JUMP /*&& canJump*/)
	{
		//ジャンプをするかしないかの判定
		move.y = actY.move;
		onGround = false;
		KickSmoke();
	}

	actQueue->PopAct();
}

void New_Player::PosFixed(Vector2<float> Move)
{
	pos += Move;
	fixed = Move;
}

void New_Player::DropMove(int DropSpeed)
{
	if (!onGround)
	{
		move.x = 0.0f;
		chainMove.x = 0.0f;
		move.y += DropSpeed;
		chainMove = NONE_VECTOR2;
		chain = false;
		dropFlag = true;
	}
}

void New_Player::HitCheckMap(New_Map& Map)
{
	if (alive)
	{
		bool oldLeftRightHitFlag = leftRightHitFlag;
		leftRightHitFlag = false;
		onGround = false;

		Vector2<float>oldPos = pos - move - chainMove - fixed;
		Vector2<float>Move = move + chainMove + fixed;
		Vector2<float>chipPos;	//ある一点のマップチップ左上座標格納用変数

		int X = 0;
		bool left = false;
		bool right = false;
		//左側
		if (pos.x < oldPos.x)
		{
			if (-CHIP_SIZE + 1 < Move.x)
			{
				if ((chipPos = Map.CheckHitWall({ HitX1() - 1,HitY1() - Move.y + 1 })) != NONE_VECTOR2
					|| (chipPos = Map.CheckHitWall({ HitX1() - 1,HitY2() - Move.y - 1 })) != NONE_VECTOR2)
				{
					//プレイヤー左側のマップチップのX2
					X = chipPos.x + CHIP_SIZE;
					left = true;
				}
			}
			else
			{
				for (float x1 = oldPos.x - size.x / 2; pos.x - size.x / 2 < x1; x1 -= CHIP_SIZE / 2)
				{
					if ((chipPos = Map.CheckHitWall({ x1 - 1,HitY1() - Move.y + 1 })) != NONE_VECTOR2
						|| (chipPos = Map.CheckHitWall({ x1 - 1,HitY2() - Move.y - 1 })) != NONE_VECTOR2)
					{
						//プレイヤーの左側のマップチップX2
						X = chipPos.x + CHIP_SIZE;
						left = true;
						break;
					}
				}
			}

			//プレイヤーが左側の壁を超えないようにする
			if (X && left && this->HitX1() < X)
			{
				pos.x = X + SIZE / 2 - HIT_FIXED;

				if (chain)
				{
					move.x = 0.0;
					chainMove.x = 0.0;
					leftRightHitFlag = true;
					chain = false;
				}
				/*else
				{
					move.x = 0.0F;
					chainMove.x = 0.0f;
				}*/
			}
		}
		//右側
		else if (oldPos.x < pos.x)
		{
			if (Move.x < CHIP_SIZE - 1)
			{
				if ((chipPos = Map.CheckHitWall({ HitX2() + 1,HitY1() - Move.y + 1 })) != NONE_VECTOR2
					|| (chipPos = Map.CheckHitWall({ HitX2() + 1,HitY2() - Move.y - 1 })) != NONE_VECTOR2)
				{
					//プレイヤーの右側のマップチップのX1
					X = chipPos.x;
					right = true;
				}
			}
			else
			{
				for (float x2 = oldPos.x + size.x / 2; x2 < pos.x + size.x / 2; x2 += CHIP_SIZE / 2)
				{
					if ((chipPos = Map.CheckHitWall({ x2 + 1,HitY1() - Move.y + 1 })) != NONE_VECTOR2
						|| (chipPos = Map.CheckHitWall({ x2 + 1,HitY2() - Move.y - 1 })) != NONE_VECTOR2)
					{
						//プレイヤーの右側のマップチップX1
						X = chipPos.x;
						right = true;
						break;
					}
				}
			}

			//プレイヤーが→側の壁を超えないようにする
			if (X && right && X < this->HitX2())
			{
				pos.x = X - SIZE / 2 + HIT_FIXED;

				if (chain)
				{
					move.x = 0.0;
					chainMove.x = 0.0;
					leftRightHitFlag = true;
					chain = false;
				}
				//else
				//{
				//	move.x = 0.0F;
				//	chainMove.x = 0.0f;
				//}
			}
		}

		//壁と衝突した瞬間画面揺れ
		if (leftRightHitFlag && !oldLeftRightHitFlag)
		{
			//camPtr->DropGroundShake();
		}

		int Y = 0;
		//上側
		if (pos.y < oldPos.y)
		{
			if (-CHIP_SIZE + 1 < Move.y)
			{
				if ((chipPos = Map.CheckHitWall({ HitX1() + 1,HitY1() - 1 })) != NONE_VECTOR2
					|| (chipPos = Map.CheckHitWall({ HitX2() - 1,HitY1() - 1 })) != NONE_VECTOR2)
				{
					//プレイヤーの上側のマップチップのY2
					Y = chipPos.y + CHIP_SIZE;
				}
			}
			else
			{
				for (float y1 = oldPos.y - size.y / 2; pos.y - size.y / 2 < y1; y1 -= CHIP_SIZE / 2)
				{
					if ((chipPos = Map.CheckHitWall({ HitX1() + 1,y1 - 1 })) != NONE_VECTOR2
						|| (chipPos = Map.CheckHitWall({ HitX2() - 1,y1 - 1 })) != NONE_VECTOR2)
					{
						//プレイヤーの上側のマップチップY2
						Y = chipPos.y + CHIP_SIZE;
						break;
					}
				}
			}

			if (Y)
			{
				//プレイヤーが上側の壁を超えないようにする
				if (this->HitY1() < Y)
				{
					pos.y = Y + SIZE / 2 - HIT_FIXED;
					move.y = 0;
					chainMove.y = 0;
					chain = false;
				}
			}
		}
		//下側
		else if (oldPos.y < pos.y)
		{
			if (Move.y < CHIP_SIZE - 1)	//MoveYがチップサイズより小さいとき
			{
				if ((chipPos = Map.CheckHitWall({ HitX1() + 1,HitY2() + 1 })) != NONE_VECTOR2
					|| (chipPos = Map.CheckHitWall({ HitX2() - 1,HitY2() + 1 })) != NONE_VECTOR2)
				{
					//プレイヤーの下側のマップチップのY1
					Y = chipPos.y;
				}
			}
			else
			{
				for (float y2 = oldPos.y + size.y / 2; y2 < pos.y + size.y / 2; y2 += CHIP_SIZE / 2)	//移動軌道に当たり判定用の疑似レイを放出
				{
					if ((chipPos = Map.CheckHitWall({ HitX1() + 1,y2 + 1 })) != NONE_VECTOR2
						|| (chipPos = Map.CheckHitWall({ HitX2() - 1,y2 + 1 })) != NONE_VECTOR2)
					{
						//プレイヤーの下側のマップチップのY1
						Y = chipPos.y;
						break;
					}
				}
			}

			if (Y )	//床との当たり判定があった
			{
				//プレイヤーが下側の壁をこえないようにする
				if (Y < this->HitY2())
				{
					pos.y = Y - SIZE / 2 + HIT_FIXED;
				}
				move.y = 0.0f;
				chainMove = NONE_VECTOR2;
				if (!onGround)
				{
					int i = 0;
				}
				onGround = true;
				chain = false;
				if (dropFlag)
				{
					GROUND_IMAGE[id].AnimationStart();
					PlaySE(SE_GROUND);
					KickSmoke();
					//effect->EmitSander({ LocalX(),LocalY2() }, id ? PURPLE : GREEN);
					dropFlag = false;
				}
			}
		}

		//斜め判定（上下修正優先）
		if (X == 0 && Y == 0)
		{
			//上
			if (Move.y < 0)
			{
				if ((Move.x < 0 && (chipPos = Map.CheckHitWall({ HitX1() - 1,HitY1() - 1 })) != NONE_VECTOR2)	//左上
					|| (0 < Move.x && (chipPos = Map.CheckHitWall({ HitX2() + 1,HitY1() - 1 })) != NONE_VECTOR2))	//右上
				{
					//プレイヤーの上側のマップチップのY2
					Y = chipPos.y + CHIP_SIZE;

					//プレイヤーが上側の壁を超えないようにする
					if (this->HitY1() < Y)
					{
						pos.y = Y + SIZE / 2 - HIT_FIXED;
						move.y = 0;
						chainMove.y = 0;
						chain = false;
					}
				}
			}
			//下
			else if (0 < Move.y)
			{
				if (Move.y < CHIP_SIZE - 1)	//MoveYがチップサイズより小さいとき
				{
					if ((Move.x < 0 && (chipPos = Map.CheckHitWall({ HitX1() - 1,HitY2() + 1 })) != NONE_VECTOR2)	//左下
						|| (0 < Move.x && (chipPos = Map.CheckHitWall({ HitX2() + 1,HitY2() + 1 })) != NONE_VECTOR2))	//右下
					{
						//プレイヤーの下側のマップチップのY1
						Y = chipPos.y;
					}
				}
				else
				{
					for (float y2 = oldPos.y + size.y / 2; y2 < pos.y + size.y / 2; y2 += CHIP_SIZE / 2)	//移動軌道に当たり判定用の疑似レイを放出
					{
						if ((Move.x < 0 && (chipPos = Map.CheckHitWall({ HitX1() - 1,y2 + 1 })) != NONE_VECTOR2)
							|| (0 < Move.x && (chipPos = Map.CheckHitWall({ HitX2() + 1,y2 + 1 })) != NONE_VECTOR2))
						{
							//プレイヤーの下側のマップチップのY1
							Y = chipPos.y;
							break;
						}
					}
				}
				if (Y)	//床との当たり判定があった
				{
					//プレイヤーが下側の壁をこえないようにする
					if (Y < this->HitY2())
					{
						pos.y = Y - SIZE / 2 + HIT_FIXED;
					}
					move.y = 0.0f;
					chainMove = NONE_VECTOR2;
					if (!onGround)
					{
						int i = 0;
					}
					onGround = true;
					chain = false;
					if (dropFlag)
					{
						GROUND_IMAGE[id].AnimationStart();
						PlaySE(SE_GROUND);
						KickSmoke();
						//effect->EmitSander({ LocalX(),LocalY2() }, id ? PURPLE : GREEN);
						dropFlag = false;
					}
				}
			}
		}

		//アイテムとの判定
		New_ItemMgr::HitCheckBox(this->WorldPos(), SIZE/2);

		//見えなくなったら死
		if (Map.GetHeight() < pos.y || 
			Map.CheckHitDamage({WorldX1(),WorldY1()})
			|| Map.CheckHitDamage({ WorldX2(),WorldY1() })
			|| Map.CheckHitDamage({ WorldX1(),WorldY2() })
			|| Map.CheckHitDamage({ WorldX2(),WorldY2() }))
		{
			Kill();
		}
	}
}
