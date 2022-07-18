#include "New_PlayerMgr.h"
#include"UsersInput.h"
#include"MyFunc.h"
#include"SoundEmitter.h"
#include"easing.h"

using namespace New_PlayerInfo;

int New_PlayerMgr::ARROW_GRAPH = 0;
//const float New_PlayerMgr::GRAVITY = 0.9;
//const float New_PlayerMgr::WIRE_EXTEND_RATE = 0.08;

void New_PlayerMgr::PullOpe()
{
	Vector2<float>V1 = GetVector2();
	Vector2<float>V2 = GetVec(V1);
	V2.x *= wireLength;
	V2.y *= wireLength;

	Vector2<float>V3 = V1 - V2;

	players[ope].PosFixed(V3);
}
void New_PlayerMgr::PullCpu()
{
	Vector2<float>V1 = GetVector();
	Vector2<float>V2 = GetVec(V1);
	V2.x *= wireLength;
	V2.y *= wireLength;

	Vector2<float>V3 = V1 - V2;

	players[cpu].PosFixed(V3);
}

New_PlayerMgr::New_PlayerMgr(CamAccessor* Cam, EffectMgr* EffectMgr) :actRecord(DELAY_FLAME_X, DELAY_FLAME_Y)
{
	players[0].Set(Cam, EffectMgr, &actRecord);
	players[1].Set(Cam, EffectMgr, &actRecord);
	cam = Cam;
	effect = EffectMgr;

	if (!ARROW_GRAPH)
	{
		ARROW_GRAPH = LoadGraph("_resource/graphics/arrow.png");
	}

	afImg.Set(Cam);
	afImg2.Set(Cam);

	//PLAYERS_MAX_DISTANCE = 200;
	//PLAYERS_MAX_DISTANCE_CHAIN = 200;
	HOLD_SPAN = 7;
	THROW_POWER = 32;
	DROP_SPEED = 70;
	GRAVITY = 0.9;
}



New_PlayerMgr::~New_PlayerMgr()
{
}

void New_PlayerMgr::Init(Vector2<float>CenterFloorPos)
{
	ope = 0;
	cpu = 1;
	Vector2<float>InitPos = { CenterFloorPos.x,CenterFloorPos.y };

	players[ope].Init(InitPos);
	players[cpu].Init(InitPos);
	actRecord.Init();
	//enemyKill = false;
	//canChain = true;
	chainTimer = NOW_CHAIN_JUDGE_SPAN;
	dropFlag = false;
	dropTimer = DROP_HOLED_TIME;
	dead = false;
	afImg.Init();
	afImg2.Init();
	spring.Init(cam->CamAffect(InitPos));
	deadTimer = 0;

	//maxDistance = PLAYERS_MAX_DISTANCE;
	wireLength = MIN_WIRE_LENGTH;
	targetWireLength = MIN_WIRE_LENGTH;

	chainArrow.Init();

	warpTimer = 0;
	oldOnGround = false;

	stamina.amount = stamina.MAX;
}

void New_PlayerMgr::Switch()
{
	int i = ope;
	ope = cpu;
	cpu = i;
}

void New_PlayerMgr::Chain(New_Map& Map)
{
	players[0].ResetMove();
	players[1].ResetMove();
	actRecord.Init();

	players[0].holdTimer = 0;
	players[1].holdTimer = 0;
	players[0].chain = false;
	players[1].chain = false;

	//players[cpu].Thrown(players[ope].GetTurnFlag());
	//players[cpu].Thrown2(GetPadVec(), HOLD_SPAN, (float)THROW_POWER/* * GetDistance() / wireLength*/);
	if (players[ope].onGround && players[cpu].onGround)
	{
		players[cpu].Jump(wireLength, (float)MIN_WIRE_LENGTH);
	}
	else
	{
		players[cpu].Thrown3(Map, GetPadVec(), players[ope].WorldPos(), MAX_WIRE_LENGTH, MIN_WIRE_LENGTH, wireLength, HOLD_SPAN);
	}
	players[cpu].StartChainFlash();

	if (players[cpu].OnGround() == true)
	{
		players[cpu].KickSmoke();
	}
	Switch();

	chainTimer = 0;

	chainArrow.Chained();

	if (targetWireLength < MAX_WIRE_LENGTH)
	{
		targetWireLength += ((float)(MAX_WIRE_LENGTH - MIN_WIRE_LENGTH) / stamina.MAX);
		if (MAX_WIRE_LENGTH < targetWireLength)targetWireLength = MAX_WIRE_LENGTH;
	}
}

void New_PlayerMgr::Drop()
{
	PlaySE(SE_DROPEEFECT);
	dropEffect.Emit();
	dropFlag = true;
	dropTimer = 0;
	groundTimer = 0;
}

void New_PlayerMgr::DropUpdate()
{
	if (dropFlag)
	{
		if (dropTimer < DROP_HOLED_TIME)
		{
			players[ope].ResetMove();
			players[cpu].ResetMove();
		}
		else if (dropTimer == DROP_HOLED_TIME)
		{
			players[ope].DropMove(DROP_SPEED);
			players[cpu].DropMove(DROP_SPEED);
		}

		dropTimer++;

		if (players[ope].OnGround() == true)
		{
			if (groundTimer == 0)
			{
				cam->DropGroundShake();
			}
			if (groundTimer < GROUND_HOLED_TIME)
			{
				groundTimer++;
			}
			else
			{
				dropFlag = false;
			}
			if (players[ope].GetNowGroundAnimation() == false)
			{
				dropFlag = false;
			}
		}
	}
}

void New_PlayerMgr::Update(New_Map& Map, const New_Goal& Goal)
{
	bool hoge = false;
	if (!dropFlag)
	{
		if (!dead && !Goal.flag)
		{
			if (((JustInput(A) || JustInput(X)) && GetDistance()!= 0.0f) /*&& (canChain || enemyKill)*/)
			{
				/*canChain = false;
				enemyKill = false;*/
				if (stamina.amount)
				{
					Chain(Map);
					if (stamina.amount)
					{
						stamina.amount--;
					}
				}
			}

			if (!players[ope].chain && !players[cpu].chain && CHAIN_CAN_INPUT < chainTimer && !dropFlag)	//チェイン中じゃないとき入力可能
			{
				players[ope].OpeUpdate();
				players[cpu].CpuUpdate(players[ope], PLAYER_RANGE);
			}
		}
	}

	players[cpu].Update(GRAVITY, players[ope].WorldPos(), Goal.flag);
	players[ope].Update(GRAVITY, players[cpu].WorldPos(), Goal.flag);
	//spring.Update(maxDistance, players[ope].LocalPos(), players[cpu].LocalPos());

	//着地したら再びチェインできるように
	//if (!canChain)
	//{
	//	if (players[ope].OnGround() == true /*&& chainTimer == CHAIN_SPAN*/)
	//	{
	//		canChain = true;
	//	}
	//}

	//紐の長さの伸び縮み
	/*if (players[ope].OnGround())
	{
		maxDistance = MyFunc::lerp(maxDistance, PLAYERS_MAX_DISTANCE, 0.2F);
	}
	else
	{
		maxDistance = MyFunc::lerp(maxDistance, PLAYERS_MAX_DISTANCE_CHAIN, 0.2F);
	}*/

	spring.Update(wireLength, players[ope].WorldPos(), players[cpu].WorldPos());
	//元の長さに戻っていく
	if (players[ope].OnGround())
	{
		targetWireLength = MIN_WIRE_LENGTH;
	}

	if (targetWireLength == MIN_WIRE_LENGTH) //元の長さに
	{
		wireLength = MyFunc::lerp(wireLength, targetWireLength, 0.02F);
	}
	else //チェインで伸びる
	{
		wireLength = MyFunc::lerp(wireLength, targetWireLength, 0.03F);
	}

	//離れすぎていたら修正
	if (wireLength < GetDistance())
	{
		/*if (players[ope].OnGround() && players[ope].WorldY() < players[cpu].WorldY())
		{
			PullOpe();
		}
		else
		{*/
			PullCpu();
		//}
	}

	//宙ぶらりん状態回避
	if (warpTimer && players[ope].WorldY() < players[cpu].WorldY())
	{
		warpTimer--;
	}
	if (players[ope].OnGround())
	{
		if (!oldOnGround)
		{
			warpTimer = WARP_SPAN;
		}
		if (!warpTimer && !players[cpu].OnGround())
		{
			players[cpu].SetPos(players[ope].WorldPos());
			players[cpu].onGround = true;
		}

		//スタミナ回復
		if (stamina.amount != stamina.MAX && players[cpu].OnGround())
		{
			stamina.amount = stamina.MAX;
		}
	}
	oldOnGround = players[ope].OnGround();

	if (GetAlive() && !Goal.flag)
	{
		players[ope].HitCheckMap(Map);
		players[cpu].HitCheckMap(Map);
	}

	//ドロップ
	DropUpdate();
	if (JustInput(RT) == true && players[ope].OnGround() == false && !dropFlag && !dead)
	{
		Drop();
	}

	afImg.Update(players[ope].WorldPos(), players[ope].GetShadowGraph(), AfImgDraw());
	afImg2.Update(players[cpu].WorldPos(), players[cpu].GetShadowGraph(), (AfImgDraw() && dropFlag));

	//チェインしてからの次に使えるようになるまでのカウントダウン
	if (chainTimer < NOW_CHAIN_JUDGE_SPAN)
	{
		chainTimer++;
	}

	if (dead)
	{
		deadTimer++;
	}

	if (players[ope].GetAlive() == false || players[cpu].GetAlive() == false)
	{
		players[ope].ResetMoveX();
		players[cpu].ResetMoveX();
		dead = true;
	}

	chainArrow.Update(players[cpu].WorldPos(),cam);

	if (Goal.flag)
	{
		players[cpu].LerpToPos(Goal.GetCenter());
		players[ope].LerpToPos(Goal.GetCenter());
	}

	//デバッグ用
	if (KeyJustInput(KEY_INPUT_UP))
	{
		if (KeyInput(KEY_INPUT_LSHIFT))
		{
			DROP_SPEED++;
		}
		else
		{
			//PLAYERS_MAX_DISTANCE++;
		}
	}
	else if (KeyJustInput(KEY_INPUT_DOWN))
	{
		if (KeyInput(KEY_INPUT_LSHIFT))
		{
			DROP_SPEED--;
		}
		else
		{
			//PLAYERS_MAX_DISTANCE--;
		}
	}
	if (KeyJustInput(KEY_INPUT_RIGHT))
	{
		if (KeyInput(KEY_INPUT_LSHIFT))
		{
			GRAVITY += 0.1f;
		}
		else
		{
			//PLAYERS_MAX_DISTANCE_CHAIN++;
		}
	}
	else if (KeyJustInput(KEY_INPUT_LEFT))
	{
		if (KeyInput(KEY_INPUT_LSHIFT))
		{
			GRAVITY -= 0.1f;
		}
		else
		{
			//PLAYERS_MAX_DISTANCE_CHAIN--;
		}
	}
	if (KeyJustInput(KEY_INPUT_A))
	{
		HOLD_SPAN--;
	}
	else if (KeyJustInput(KEY_INPUT_D))
	{
		HOLD_SPAN++;
	}
	if (KeyJustInput(KEY_INPUT_W))
	{
		THROW_POWER++;
	}
	else if (KeyJustInput(KEY_INPUT_S))
	{
		THROW_POWER--;
	}
}

void New_PlayerMgr::Draw(bool Goal)
{
	//デバッグ用
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "\n\n\nA：ジャンプ\nX：チェイン（左入力で左上へ）\n（空中で）RT：ドロップ");
	//DrawFormatString(0, 0, GetColor(255, 0, 0), "\n\n\n\n操作キャラ：%s", ope ? "マキ" : "ユア");
	//DrawFormatString(0, 0, GetColor(255, 0, 0), "\n\n\n\n\n操作キャラ座標　X：%.1f Y：%.1f", players[ope].WorldPos().x, players[ope].WorldPos().y);
	//DrawFormatString(0, 0, GetColor(255, 0, 0), "\n\n\n\n\n\nチェイン可能：%s　チェインタイマー：%d", canChain ? "true" : "false", chainTimer);

	/*if (goalLine <= players[ope].WorldPos().x)
	{
		DrawString(0, 18 * 5, "ゴール！", GetColor(255, 0, 0));
	}*/

	//チェイン先矢印描画
	//Vector2<float>From = players[cpu].LocalPos();
	//Vector2<float> To = From + GetPadVec() * 200;
	//MyFunc::DrawLine2(CHAIN_ARROW_GRAPH, CHAIN_ARROW_GRAPH_SIZE, From, To, 64);
	if (!dead)
	{
		afImg.Draw();
		afImg2.Draw();

		players[0].LightDraw();
		players[1].LightDraw();

		if (!Goal)
		{
			//chainArrow.Draw(cam);
		}
		spring.Draw(cam);

		DrawGraph(players[ope].LocalX() - 8, players[ope].LocalY1() - 16, ARROW_GRAPH, true);
	}
	players[0].Draw(ope);
	players[1].Draw(ope);

	if(!dead)
	dropEffect.Draw(cam, players[ope].LocalPos(), players[cpu].LocalPos());

	stamina.Draw();

	//デバッグ用
	//DrawString(0, 0, "A：下回り、X：上回り", GetColor(255, 255, 255));
	//DrawFormatString(0, 0, (MAX_WIRE_LENGTH <= wireLength) ? (GetColor(255, 0, 0)) : (GetColor(255, 255, 255)), "\n現在の紐の長さ：%.1f\n二人の距離：%.1f", wireLength,GetDistance());
	/*DrawFormatString(0, 0, GetColor(255, 255, 255),
		"\n\n\n\nチェイン最高到達点ホールドフレーム(ADキーで変更)：%d\nドロップスピード(LSHIFT + 上下キーで変更)：%d\n重力(LSHIFT + 左右キーで変更)：%.1f",
		HOLD_SPAN, DROP_SPEED, GRAVITY);*/
	//int x = mapPtr->GetChipNumX(players[ope].WorldX());
	//int y = mapPtr->GetChipNumY(players[ope].WorldY());
	//DrawFormatString(0, Constants::WIN_HEIGHT-18 * 2, GetColor(255, 255, 255), "マップチップ番号 X：%d\n　　　　　　　　 Y：%d", x, y);
}

void New_PlayerMgr::Finalize()
{
	players[0].Finalize();
	players[1].Finalize();
}


//相棒キャラから操作キャラまでのベクトル
Vector2<float> New_PlayerMgr::GetVector()
{
	Vector2<float>V;

	V.x = players[ope].WorldPos().x - players[cpu].WorldPos().x;
	V.y = players[ope].WorldPos().y - players[cpu].WorldPos().y;

	return V;
}

Vector2<float> New_PlayerMgr::GetVector2()
{
	Vector2<float>V;

	V.x = players[cpu].WorldPos().x - players[ope].WorldPos().x;
	V.y = players[cpu].WorldPos().y - players[ope].WorldPos().y;

	return V;
}

float New_PlayerMgr::GetDistance()
{
	return GetVecSize(GetVector());
}

bool New_PlayerMgr::NowChainHit()
{
	if (chainTimer < NOW_CHAIN_JUDGE_SPAN)
	{
		return true;
	}
	return false;
}

bool New_PlayerMgr::AfImgDraw()
{
	if (chainTimer < AFTER_IMAGE_DRAW_SPAN || (dropFlag && !groundTimer))
	{
		return true;
	}
	return false;
}

void New_PlayerMgr::RaiseCanChain(int who)
{
	//enemyKill = true;
	if (who == YUA)
	{
		//if (!dropFlag)
		//{
		effect->EmitChainSquare(players[who].LocalPos(), PURPLE);
		//}
		//else
		//{
		//	effect->EmitSander(players[who].LocalPos(), PURPLE);
		//}
	}
	else
	{
		/*if (!dropFlag)
		{*/
		effect->EmitChainSquare(players[who].LocalPos(), GREEN);
		/*}
		else
		{
			effect->EmitSander(players[who].LocalPos(), GREEN);
		}*/
	}
	PlaySE(SE_GLASS);
}

//void New_PlayerMgr::StartDrop()
//{
//	dropFlag = true;
//	groundTimer = 0;
//	Vector2<float> Pos = { Constants::GetWinCenter().x, -128 };
//	players[ope].SetPos(Pos);
//	players[cpu].SetPos(Pos);
//	players[ope].DropMove(DROP_SPEED);
//	players[cpu].DropMove(DROP_SPEED);
//	players[ope].RaiseStart();
//	players[cpu].RaiseStart();
//}

bool New_PlayerMgr::TwoNotOnGround()
{
	if (!players[cpu].OnGround() && !players[ope].OnGround())
	{
		return true;
	}
	return false;
}

float New_PlayerMgr::GetZoomChangeRate()
{
	return 1.0f - ((targetWireLength - (float)MIN_WIRE_LENGTH) / (float)(MAX_WIRE_LENGTH - MIN_WIRE_LENGTH));
}

int New_PlayerMgr_Info::ChainArrow::CHAIN_ARROW_GRAPH = 0;
const Vector2<float>New_PlayerMgr_Info::ChainArrow::CHAIN_ARROW_GRAPH_SIZE = { 100,64 };

New_PlayerMgr_Info::ChainArrow::ChainArrow()
{
	if (!CHAIN_ARROW_GRAPH)
	{
		CHAIN_ARROW_GRAPH = LoadGraph("_resource/graphics/chain_arrow.png");
	}
	for (int i = 0; i < NUM; i++)
	{
		info.push_back(Info());
	}

}
void New_PlayerMgr_Info::ChainArrow::Init()
{
	for (auto i = info.begin(); i != info.end(); i++)
	{
		EaseInit(i->easeID_Pal);
		i->pal = 0;
	}
	itr = info.begin();
}

void New_PlayerMgr_Info::ChainArrow::Update(Vector2<float> CpuWorldPos, CamAccessor* Cam)
{
	itr->from = CpuWorldPos;
	itr->to = CpuWorldPos + GetPadVec() * LENGTH * (2 - Cam->CamZoomAffect(1));

	for (auto i = info.begin(); i != info.end(); i++)
	{
		if (!CompleteEase(i->easeID_Pal))
		{
			i->pal = GetEaseUpdate(i->easeID_Pal);
		}
	}
}

void New_PlayerMgr_Info::ChainArrow::Draw(CamAccessor* Cam)
{
	for (auto i = info.begin(); i != info.end(); i++)
	{
		if (!CompleteEase(i->easeID_Pal))
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, i->pal);
			MyFunc::DrawLine2(CHAIN_ARROW_GRAPH, CHAIN_ARROW_GRAPH_SIZE, Cam->CamAffect(i->from), Cam->CamAffect(i->to), 64);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
	MyFunc::DrawLine2(CHAIN_ARROW_GRAPH, CHAIN_ARROW_GRAPH_SIZE, Cam->CamAffect(itr->from), Cam->CamAffect(itr->to), 64);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void New_PlayerMgr_Info::ChainArrow::Chained()
{
	PlayEase_Trigger(itr->easeID_Pal);
	if (++itr == info.end())
	{
		itr = info.begin();
	}
}

New_PlayerMgr_Info::ChainArrow::Info::Info()
{
	CreateEaseSeed(&easeID_Pal, In, Cubic, 0.05, 160, 0);
}

void New_PlayerMgr_Info::ChainStamina::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	float x = Constants::GetWinCenter().x - SPACE / 2 - (SPACE + WIDTH) * MAX / 2;
	for (int i = 0; i < amount; i++)
	{
		int pX = x + i * (WIDTH + SPACE);
		DrawBox(pX, POSY, pX + WIDTH, POSY + HEIGHT, GetColor(167, 78, 225), true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "スタミナ残り：%d", amount);
}

New_PlayerMgr_Info::DropEffect::DropEffect()
{
	graph = LoadGraph("_resource/graphics/dropEffect.png");

	CreateEaseSeed(&easeID_Pal, In, Exp, 0.08);
}

void New_PlayerMgr_Info::DropEffect::Emit()
{
	PlayEase_Trigger(easeID_Pal);
}

void New_PlayerMgr_Info::DropEffect::Init()
{
	EaseInit(easeID_Pal);
}

void New_PlayerMgr_Info::DropEffect::Draw(CamAccessor* Cam,Vector2<float> OpeLocalCenter, Vector2<float> CpuLocalCenter)
{
	if (!CompleteEase(easeID_Pal))
	{
		EaseUpdate(easeID_Pal);

		float size = Cam->CamZoomAffect(GetEaseAffect(easeID_Pal, 0, SIZE * 4));
		float X1 = OpeLocalCenter.x - size / 2;
		float Y1 = OpeLocalCenter.y - size / 2;
		float X2 = OpeLocalCenter.x + size / 2;
		float Y2 = OpeLocalCenter.y + size / 2;

		int pal = GetEaseAffect(easeID_Pal, 200, 0);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, pal);

		DrawExtendGraph(X1, Y1, X2, Y2, graph, true);

		X1 = CpuLocalCenter.x - size / 2;
		Y1 = CpuLocalCenter.y - size / 2;
		X2 = CpuLocalCenter.x + size / 2;
		Y2 = CpuLocalCenter.y + size / 2;

		DrawExtendGraph(X1, Y1, X2, Y2, graph, true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}