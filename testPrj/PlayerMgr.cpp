#include"PlayerMgr.h"
#include"UsersInput.h"
#include"SoundEmitter.h"
#include"Constants.h"

int PlayerMgr::ARROW_GRAPH = 0;
const float PlayerMgr::GRAVITY = 0.9f;

static float lerp(float start, float end, float t)
{
	return (1 - t) * start + t * end;
}

PlayerMgr::PlayerMgr(CamAccessor* Cam,EffectMgr* EffectMgr) :actRecord(DELAY_FLAME_X,DELAY_FLAME_Y)
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
	
	spring.Set(Cam);
}

PlayerMgr::~PlayerMgr()
{
}

void PlayerMgr::Init(Map* Map, Vector2<float>InitPos)
{
	ope = 0;
	cpu = 1;
	players[ope].Init(InitPos, Map->GetMapY());
	players[cpu].Init(InitPos, Map->GetMapY());
	actRecord.Init();
	enemyKill = false;
	canChain = true;
	chainTimer = CHAIN_SPAN;
	goalLine = Map->GetMapX();
	//dropFlag = false;
	dropTimer = DROP_HOLED_TIME;
	mapPtr = Map;
	dead = false;
	afImg.Init();
	afImg2.Init();
	spring.Init(PLAYERS_MAX_DISTANCE, InitPos);
	deadTimer = 0;
	maxDistance = PLAYERS_MAX_DISTANCE;
	
	chainArrow.Init();
}

void PlayerMgr::Switch()
{
	int i = ope;
	ope = cpu;
	cpu = i;
}

void PlayerMgr::Chain()
{
	players[0].ResetMove();
	players[1].ResetMove();
	actRecord.Init();

	//players[cpu].Thrown(players[ope].GetTurnFlag());
	players[cpu].Thrown2(GetPadVec());
	players[cpu].StartChainFlash();
	if (players[cpu].OnGround() == true)
	{
		players[cpu].KickSmoke();
	}
	Switch();

	chainTimer = 0;
	chainArrow.Chained();
}

void PlayerMgr::Drop()
{
	dropFlag = true;
	dropTimer = 0;
	groundTimer = 0;
}

void PlayerMgr::DropUpdate()
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
			players[ope].DropMove();
			players[cpu].DropMove();
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
			/*else
			{
				dropFlag = false;
			}*/
			if (players[ope].GetNowGroundAnimation() == false)
			{
				dropFlag = false;
			}
		}
	}
}

void PlayerMgr::Update()
{
	if (!dropFlag)
	{
		if (players[ope].WorldPos().x < goalLine && !dead)
		{
			if (JustInput(X) == true && (canChain || enemyKill) && (GetPadVec() != Vector2<float>{0.0F, 0.0F}))
			{
				canChain = false;
				enemyKill = false;
				Chain();
			}

			if (CHAIN_CAN_INPUT < chainTimer)	//チェイン中じゃないとき入力可能
			{
				players[ope].OpeUpdate();
				players[cpu].CpuUpdate(players[ope], PLAYER_RANGE);
			}
		}

		if (Goaled())
		{
			players[ope].AutoRun();
			if (players[ope].WorldX1() < players[cpu].WorldX2())
			{
				players[cpu].Back();
			}
			players[cpu].AutoRun();
		}
	}


	players[ope].Update(GRAVITY);
	players[cpu].Update(GRAVITY);
	spring.Update(players[ope].LocalPos(), players[cpu].LocalPos());

	//ドロップ
	DropUpdate();
	if (JustInput(RT) == true && players[ope].OnGround() == false && !dropFlag)
	{
		Drop();
	}
		
	//着地したら再びチェインできるように
	if (!canChain)
	{
		if (players[ope].OnGround() == true /*&& chainTimer == CHAIN_SPAN*/)
		{
			canChain = true;
		}
	}

	if (players[ope].OnGround())
	{
		maxDistance = lerp(maxDistance, PLAYERS_MAX_DISTANCE, 0.2F);
	}
	else
	{
		maxDistance = lerp(maxDistance, PLAYERS_MAX_DISTANCE_CHAIN, 0.2F);
	}

	//離れすぎていたら修正
	if (maxDistance < GetDistance())
	{
		if (players[ope].OnGround() && !players[cpu].OnGround())
		{
			Vector2<float>V1 = GetVector2();
			Vector2<float>V2 = GetVec(V1);
			V2.x *= PLAYERS_MAX_DISTANCE;
			V2.y *= PLAYERS_MAX_DISTANCE;

			Vector2<float>V3 = V1 - V2;

			players[ope].PosFixed(V3);
		}
		else
		{
			Vector2<float>V1 = GetVector();
			Vector2<float>V2 = GetVec(V1);
			V2.x *= maxDistance;
			V2.y *= maxDistance;

			Vector2<float>V3 = V1 - V2;

			players[cpu].PosFixed(V3);
		}
	}

	players[ope].HitCheckMap(*mapPtr,TwoNotOnGround());
	players[cpu].HitCheckMap(*mapPtr,TwoNotOnGround());

	if (goalLine <= players[ope].WorldPos().x)
	{
		if (players[ope].OnGround() == true)
		{
			players[ope].ResetMoveX();
		}
		players[ope].HitFloor();
	}
	if (goalLine <= players[cpu].WorldPos().x)
	{
		if (players[cpu].OnGround() == true)
		{
			players[cpu].ResetMoveX();
		}
		players[cpu].HitFloor();
	}

	afImg.Update(players[ope].WorldPos(), players[ope].GetShadowGraph(), AfImgDraw());
	afImg2.Update(players[cpu].WorldPos(), players[cpu].GetShadowGraph(), (AfImgDraw() && dropFlag));

	//チェインしてからの次に使えるようになるまでのカウントダウン
	if (chainTimer < CHAIN_SPAN)
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

	chainArrow.Update(players[cpu].WorldPos(), cam);
}

void PlayerMgr::Draw()
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

	afImg.Draw();
	afImg2.Draw();

	players[0].LightDraw();
	players[1].LightDraw();

	chainArrow.Draw(cam);

	spring.Draw();

	if (!dead)
	{
		DrawGraph(players[ope].LocalX() - 8, players[ope].LocalY1() - 16, ARROW_GRAPH, true);
	}
		players[0].Draw(ope, (canChain || enemyKill), (players[ope].WorldPos().x < goalLine));
		players[1].Draw(ope, (canChain || enemyKill), (players[ope].WorldPos().x < goalLine));

	//デバッグ用
	//int x = mapPtr->GetChipNumX(players[ope].WorldX());
	//int y = mapPtr->GetChipNumY(players[ope].WorldY());
	//DrawFormatString(0, GetWinSize().y-18 * 2, GetColor(255, 255, 255), "マップチップ番号 X：%d\n　　　　　　　　 Y：%d", x, y);
}

void PlayerMgr::Finalize()
{
	players[0].Finalize();
	players[1].Finalize();
}


//相棒キャラから操作キャラまでのベクトル
Vector2<float> PlayerMgr::GetVector()
{
	Vector2<float>V;

	V.x = players[ope].WorldPos().x - players[cpu].WorldPos().x;
	V.y = players[ope].WorldPos().y - players[cpu].WorldPos().y;

	return V;
}

Vector2<float> PlayerMgr::GetVector2()
{
	Vector2<float>V;

	V.x = players[cpu].WorldPos().x - players[ope].WorldPos().x;
	V.y = players[cpu].WorldPos().y - players[ope].WorldPos().y;

	return V;
}

float PlayerMgr::GetDistance()
{
	return GetVecSize(GetVector());
}

bool PlayerMgr::NowChain()
{
	if (chainTimer < CHAIN_SPAN)
	{
		return true;
	}

	return false;
}

bool PlayerMgr::NowChainHit()
{
	if (chainTimer < NOW_CHAIN_JUDGE_SPAN)
	{
		return true;
	}
	return false;
}

bool PlayerMgr::AfImgDraw()
{
	if (chainTimer < AFTER_IMAGE_DRAW_SPAN || (dropFlag && !groundTimer))
	{
		return true;
	}
	return false;
}

bool PlayerMgr::Goaled()
{
	if (players[ope].WorldPos().x < goalLine)
	{
		return false;
	}
	else 
	{
		if (players[ope].OnGround() == true)
		{
			return true;
		}

		return false;
	}
}

bool PlayerMgr::GetDead()
{
	if (REVIVE_SPAN <= deadTimer)
	{
		return true;
	}
	return false;
}

void PlayerMgr::RaiseCanChain(int who)
{
	enemyKill = true;
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

void PlayerMgr::StartDrop()
{
	dropFlag = true;
	groundTimer = 0;
	Vector2<float> Pos = { 128,mapPtr->GetMapY() - Constants::WIN_HEIGHT * 9 - 128 };
	players[ope].SetPos(Pos);
	players[cpu].SetPos(Pos);
	players[ope].DropMove();
	players[cpu].DropMove();
	players[ope].RaiseStart();
	players[cpu].RaiseStart();
}

bool PlayerMgr::TwoNotOnGround()
{
	if (!players[cpu].OnGround() && !players[ope].OnGround())
	{
		return true;
	}
	return false;
}
