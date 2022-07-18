#include "Player.h"
#include"UsersInput.h"
#include"Constants.h"
#include"easing.h"
#include"SoundEmitter.h"

static float lerp(float start, float end, float t)
{
	return (1 - t) * start + t * end;
}

Image Player::LIGHT;

LoopImage Player::RUN_IMAGE[2];
LoopImage Player::JUMP_IMAGE[2];
LoopImage Player::DROP_IMAGE[2];
AnimationImage Player::GROUND_IMAGE[2];

LoopImage Player::RUN_OUT_LINE[2];
LoopImage Player::JUMP_OUT_LINE[2];

LoopImage Player::JUMP_SHADOW[2];
LoopImage Player::DROP_SHADOW[2];

bool Player::LOAD_GRAPHICS = false;

int Player::ID = 0;

const float Player::FOLLOW_EASING_SPEED = 0.1F;

const float Player::INPUT_REACT_RATE = 0.1F;


void Player::KickSmoke()
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

int Player::GetShadowGraph() const
{
	if (dropFlag)
	{
		return DROP_SHADOW[id].GetNowGraph();
	}
	return JUMP_SHADOW[id].GetNowGraph();
}



Player::Player()
{
	size = { SIZE,SIZE };
	hitFixed = { HIT_FIXED,HIT_FIXED };
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
	leftRightHitFlag = false;
}

Player::~Player()
{
}

void Player::Set(CamAccessor* Camera,EffectMgr* EffectMgr, ActRecord* Queue)
{
	camPtr = Camera;
	effect = EffectMgr;
	actQueue = Queue;
}

void Player::Init(Vector2<float>InitPos,int MapHeight)
{
	pos = InitPos;
	move = { 0.0F,0.0F };
	canJump = false;
	alive = true;
	maxY = MapHeight - 32;
	dropFlag = false;
	chain = true;
	GROUND_IMAGE[0].Init();
	GROUND_IMAGE[1].Init();
	groundStart = false;

	holdTimer = 0;

	targetMoveX = 0.0F;

	turnFlag = false;
}

void Player::LightDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ADD, 20);
	LIGHT.Draw(LocalPos(), size * 7);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Player::Update(float Gravity)
{
	//更新

	pos += move;

	//上の制限
	if (!start && WorldY1() < 0)
	{
		pos.y = 0 + SIZE / 2;
		move.y *= -0.2;
	}

	if (WorldX1() < 0)
	{
		pos.x = 0 + SIZE / 2;
		move.x *= -0.2;
	}

	//後退はできない
	if (LocalX1() < 0)
	{
		pos.x += 0 - LocalX1();
	}

	move.y += Gravity;	//重力落下

	if (holdTimer && 0 < move.y && !dropFlag)
	{
		holdTimer--;
		move.y = 0;
	}

	GROUND_IMAGE[id].Update();

}

void Player::AutoRun()
{
	if (!chain && alive)
	{
		pos.x += RUN_SPEED;	//オートラン
	}
}

void Player::Back()
{
	if (!chain)
	{
		pos.x -= RUN_SPEED;	//オートラン
	}
}

void Player::Draw(int Ope, bool CanChain, bool NotGoal)
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
			if (canJump)	//地面に足がついているとき
			{
				RUN_IMAGE[id].Draw(LocalPos(), size,turnFlag);

				if (id == Ope && CanChain && NotGoal)
				{
					RUN_OUT_LINE[id].Draw(LocalPos(), { 70,70 },turnFlag);
				}
			}
			else if (dropFlag)
			{
				DROP_IMAGE[id].Draw(LocalPos(), size, turnFlag);
			}
			else
			{
				JUMP_IMAGE[id].Draw(LocalPos(), size, turnFlag);
				if (id == Ope && CanChain && NotGoal)
				{
					JUMP_OUT_LINE[id].Draw(LocalPos(), { 70,70 },turnFlag);
				}
			}
		}

		GROUND_IMAGE[id].Draw(LocalPos(), size, turnFlag);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Player::Finalize()
{
}

void Player::Thrown(bool OpeTurnFlag)
{
	move.y = 0.0F;
	move.x = 0.0F;
	Vector2<float>ThrowVec = { 0.0f,0.0f };
	if (!OpeTurnFlag)
	{
		ThrowVec = { 1,-1 };
		turnFlag = false;
	}
	else
	{
		ThrowVec = { -1,-1 };
		turnFlag = true;
	}
	move += ThrowVec * THROWN_POWER;
	if (canJump)
	{
		KickSmoke();
	}
	PlaySE(SE_CHAIN);
	canJump = false;
	chain = true;

	holdTimer = HOLD_SPAN;
}

void Player::Thrown2(Vector2<float> StickVec)
{
	move.y = 0.0F;
	//move.x = 0.0F;
	if (0 < StickVec.y)
	{
		StickVec.y = 0;
	}

	if (StickVec.x < 0)
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}
	float rad = GetRad(StickVec);
	if (DX_PI / 4 + DX_PI < rad && rad < DX_PI / 4 * 3 + DX_PI)
	{
		StickVec.y = -sin(DX_PI / 4);
	}
	move += StickVec * THROWN_POWER;
	if (canJump)
	{
		KickSmoke();
	}
	PlaySE(SE_CHAIN);
	canJump = false;
	chain = true;

	holdTimer = HOLD_SPAN;

	targetMoveX = 0.0f;
}

void Player::OpeUpdate()
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

	if (JustInput(InputName::A) && canJump)
	{
		move.y = -JUMP_POWER;
		actQueue->PushAct('y', Act{ ActName::JUMP,move.y });	//「ジャンプ」を記録
		KickSmoke();
		PlaySE(SE_JUMP);
		canJump = false;
	}
	else
	{
		actQueue->PushAct('y', Act{ ActName::NONEACT,0.0F });
	}
}

void Player::CpuUpdate(const Player& OpePlayer, const int& Range)
{
	if (OpePlayer.WorldX() < this->WorldX())
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (NearCheck(OpePlayer, Range) == false)
	{
		//CPUの動き
		Act act = actQueue->GetAct('x');

		if (act.name == ActName::LEFT_ACT || act.name == ActName::RIGHT_ACT)
		{
			targetMoveX = act.move;
		}

		move.x = lerp(move.x, targetMoveX, 0.05F);

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
		canJump = false;
		KickSmoke();
	}

	actQueue->PopAct();
}

void Player::PosFixed(Vector2<float> Move)
{
	pos += Move;
}

void Player::DropMove()
{
	if (!canJump)
	{
		move.y += DROP_SPEED;
		dropFlag = true;
	}
}

void Player::HitCheckMap(Map& Map, bool TwoNotOnGround)
{
	if (alive)
	{
		bool oldLeftRightHitFlag = leftRightHitFlag;
		leftRightHitFlag = false;
		int X = 0;
		canJump = false;

		//左側
		if (Map.GetHitCheckWall({ WorldX1() - 1,WorldY1() - move.y + 1 }) == true
			|| Map.GetHitCheckWall({ WorldX1() - 1,WorldY2() - move.y - 2 }) == true)
		{
			//プレイヤー左側のマップチップのX2
			X = Map.WorldX2(Map.GetChipNumX(WorldX1() - 1));

			//プレイヤーが左側の壁を超えないようにする
			if (this->WorldX1() < X)
			{
				pos.x = X + SIZE / 2;
				if (chain)
				{
					move.x *= -0.8;
					leftRightHitFlag = true;
				}
				else
				{
					move.x = 0.0F;
				}
			}
		}
		//右側
		else if (Map.GetHitCheckWall({ WorldX2() + 1,WorldY1() - move.y + 1 }) == true
				 || Map.GetHitCheckWall({ WorldX2() + 1, WorldY2() - move.y - 2 }) == true)
		{
			//プレイヤーの右側のマップチップのX1
			X = Map.WorldX1(Map.GetChipNumX(WorldX2() + 1));

			//プレイヤーが右側の壁を超えないようにする
			if (X < this->WorldX2())
			{
				pos.x = X - SIZE / 2;
				if (chain)
				{
					move.x *= -0.8;
					leftRightHitFlag = true;
				}
				else
				{
					move.x = 0.0F;
				}
			}
		}

		if (leftRightHitFlag && !oldLeftRightHitFlag && TwoNotOnGround)
		{
			camPtr->DropGroundShake();
		}

		int Y = 0;

		//上側
		if (Map.GetHitCheckWall({ WorldX1() + 1,WorldY1() - 1 }) == true
			|| Map.GetHitCheckWall({ WorldX2() - 2,WorldY1() - 1 }) == true)
		{
			//プレイヤーの上側のマップチップのY1
			Y = Map.WorldY2(Map.GetChipNumY(WorldY1() - 1));

			//プレイヤーが上側の壁を超えないようにする
			if (this->WorldY1() < Y)
			{
				pos.y = Y + SIZE / 2;
				move.y *= -0.2;
			}
		}
		//下側
		if (Map.GetHitCheckWall({ WorldX1() + 1,WorldY2() + 1 }) == true
			|| Map.GetHitCheckWall({ WorldX2() - 2,WorldY2() + 1 }) == true || maxY < WorldY2())
		{
			int yy = 0;
			bool nonBlock = false;

			while (maxY < WorldY2() + 1 + yy)
			{
				yy--;
			}
			while (Map.GetHitCheckWall({ WorldX1() + 1,WorldY2() + 1 + yy }) == true)
			{
				yy--;
			}
			while (Map.GetHitCheckWall({ WorldX2() - 2,WorldY2() + 1 + yy }) == true)
			{
				yy--;
			}

			yy++;

			if (Map.GetHitCheckWall({ WorldX1() + 1,WorldY2() + 1 + yy }) == false
				&& Map.GetHitCheckWall({ WorldX2() - 2,WorldY2() + 1 + yy }) == false)
			{
				nonBlock = true;
			}

			if (!nonBlock)
			{
				//プレイヤーの下側のマップチップのY2
				Y = Map.WorldY1(Map.GetChipNumY(WorldY2() + 1 + yy));

				//プレイヤーが下側の壁を超えないようにする
				if (Y < this->WorldY2())
				{
					pos.y = Y - SIZE / 2 + 1;
					canJump = true;
					chain = false;
					move.y = 0.0F;
					if (dropFlag)
					{
						GROUND_IMAGE[id].AnimationStart();
						PlaySE(SE_GROUND);
						KickSmoke();
						//effect->EmitSander({ LocalX(),LocalY2() }, id ? PURPLE : GREEN);
						dropFlag = false;
					}
					if (start)
					{
						if (id == 0)
						{
							groundStart = true;
						}
						start = false;
					}
				}
			}
		}

		//斜め判定（上下修正優先）
		if (X == 0 && Y == 0)
		{
			//上
			if (move.y < 0)
			{
				if ((move.x < 0 && Map.GetHitCheckWall({ WorldX1() - 1,WorldY1() - 1 }) == true)	//左上
					|| (0 < move.x && Map.GetHitCheckWall({ WorldX2() + 1,WorldY1() - 1 }) == true))	//右上
				{
					//プレイヤーの上側のマップチップのY1
					Y = Map.WorldY2(Map.GetChipNumY(WorldY1() - 1));

					//プレイヤーが上側の壁を超えないようにする
					if (this->WorldY1() < Y)
					{
						pos.y = Y + SIZE / 2;
						move.y *= -0.2;
					}
				}
			}
			//下
			if (0 < move.y)
			{
				if ((move.x < 0 && Map.GetHitCheckWall({ WorldX1() - 1,WorldY2() + 1 }) == true) //左下
					|| (0 < move.x && Map.GetHitCheckWall({ WorldX2() + 1,WorldY2() + 1 }) == true))	//右下
				{
					//プレイヤーの下側のマップチップのY2
					Y = Map.WorldY1(Map.GetChipNumY(WorldY2() + 1));

					//プレイヤーが下側の壁を超えないようにする
					if (Y < this->WorldY2())
					{
						pos.y = Y - SIZE / 2 + 1;
						canJump = true;
						chain = false;
						move.y = 0.0F;
					}
				}
			}
		}

		//見えなくなったら死
		if (Map.GetMapY() < pos.y)
		{
			effect->EmitExplosion(LocalPos());
			PlaySE(SE_DEAD);
			alive = false;
		}
	}
}

void Player::HitFloor()
{
	if (0 < move.y && maxY-32 < this->WorldY2())
	{
		canJump = true;
		chain = false;
		if (dropFlag)
		{
			GROUND_IMAGE[id].AnimationStart();
			PlaySE(SE_GROUND);
			KickSmoke();
			dropFlag = false;
		}
		pos.y = maxY - 32 - SIZE / 2 + 1;
		move.x = 0.0F;
		move.y = 0.0F;
	}
}
