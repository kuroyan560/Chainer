#include "MuraEffect.h"

void AfterImage::Set(CamAccessor* Camera)
{
	camPtr = Camera;
}

void AfterImage::Init()
{
	oldPos.resize(0);
	image.resize(0);
	alpha.resize(0);
	count = 0;
}

void AfterImage::Draw()
{
	for (int i = oldPos.size() - 1; i > 0; i--)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, alpha[i]);
		DrawRotaGraph(camPtr->CamAffectX(oldPos[i].x), camPtr->CamAffectY(oldPos[i].y), camPtr->CamZoomAffect(1), 0, image[i], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void AfterImage::Update(Vector2<float> OpePos, int Image, bool NowChain)
{
	bool oldNowChain = nowChain;
	nowChain = NowChain;
	
	if (nowChain)
	{
		if (!oldNowChain || count >= INTERVAL)
		{
			count = 0;
		}

		count++;

		if (count == 1)
		{
			oldPos.push_back(OpePos);
			image.push_back(Image);
			alpha.push_back(90);
		}
	}

	for (int i{ 0 }; i < oldPos.size(); i++)
	{
		alpha[i] -= SUB_ALPHA;

		if (alpha[i] <= 0)
		{
			oldPos.erase(oldPos.begin() + i);
			image.erase(image.begin() + i);
			alpha.erase(alpha.begin() + i);
		}
	}
}

void AfterImage::finalize()
{
}


void TimeStop::Init(int NowStage)
{
	nowStage = NowStage;
	mTimer = 0;
	mTimeStart = false;
	mTimeStop = false;
}

void TimeStop::Update()
{
	if (!nowStage && mTimeStart && mTimer < STOP_TIME)
	{
		mTimer++;
		if (mTimer >= STOP_TIME)
		{
			mTimeStop = true;
		}
	}
}

void TimeStop::Draw()
{
}

void TimeStop::Finalize()
{
}

void TimeStop::TimerStart()
{
	mTimeStart = true;
}

void TimeStop::Restart()
{
	Init(nowStage);
}


void Spring::Init(float _x, float _y, float _len, float _k, float _gravity, float _mass, float _damp)
{
	x = _x;
	y = _y;
	vx = 0;
	vy = 0;
	ax = 0;
	ay = 0;
	len = _len;
	disp = 0;
	k = _k;
	gravity = _gravity;
	mass = _mass;
	damp = _damp;
}

void Spring::Update(float parentX, float parentY, float childX, float childY, int childNum)
{
	//親側に引っ張られる力を求める
	float pRad = atan2(parentY - y, parentX - x);
	float pLen = sqrtf(powf(parentX - x, 2.0f) + powf(parentY - y, 2.0f));
	float pFx = (parentX - x) * k;
	float pFy = (parentY - y - len * sin(pRad)) * k;

	//子側に引っ張られる力を求める
	float cRad = atan2(childY - y, childX - x);
	float cLen = sqrtf(powf(childX - x, 2.0f) + powf(childY - y, 2.0f));
	float cFx = (childX - x) * k;
	float cFy = (childY - y - len * sin(cRad)) * k;

	//力を合成して合力を求める
	float fx = (pFx + cFx) * damp;
	float fy = (pFy + cFy) * damp + gravity;

	//加速度を求める
	ax = fx / mass;
	ay = fy / mass;

	//速度を求める
	vx = ax;
	vy = ay;

	//位置を求める
	if (childNum)
	{
		x += vx;
		y += vy;
	}
	else
	{
		x = childX;
		y = childY;
	}
}

void Spring::Draw(float parentX, float parentY, int color)
{
	//DrawCircle(x, y, 15, color);
	DrawLine(x, y, parentX, parentY, color, 2);
}

void Spring::Move(float targetX, float targetY)
{
	x += targetX;
	y += targetY;
}


void SpringMgr::Set(CamAccessor* Camera)
{
	camPtr = Camera;
}

void SpringMgr::Init(float _maxLen, Vector2<float> pos)
{
	for (auto i{ 0 }; i < SPRING_NUM; i++)
	{
		s[i].Init(pos.x, pos.y, len, k, gravity, mass, damp);
	}
	maxLen = _maxLen;
	fullStretch = false;
}

void SpringMgr::Update(Vector2<float> _opePos, Vector2<float> _cpuPos)
{
	opePos = _opePos;
	cpuPos = _cpuPos;

	if (powf(opePos.x - cpuPos.x, 2.0f) + powf(opePos.y - cpuPos.y, 2.0f) > powf(maxLen, 2.0f))
	{
		float rad = atan2(cpuPos.y - opePos.y, cpuPos.x - opePos.x);
		float corX = cos(rad) * maxLen;
		float corY = sin(rad) * maxLen;

		s[0].Update(opePos.x + cos(rad) * maxLen, opePos.y + sin(rad) * maxLen,
			s[1].GetX(), s[1].GetY(), SPRING_NUM - 1);

		for (auto i = 1; i < SPRING_NUM - 1; i++)
		{
			float dispX = (opePos.x + corX * (SPRING_NUM - i) / SPRING_NUM) - s[i].GetX();
			float dispY = (opePos.y + corY * (SPRING_NUM - i) / SPRING_NUM) - s[i].GetY();
			s[i].Move(dispX / 2, dispY / 2);
		}

		if ((int)s[SPRING_NUM / 2].GetX() == (int)(cpuPos.x + corX / 2) ||
			(int)s[SPRING_NUM / 2].GetY() == (int)(cpuPos.y + corY / 2))
		{
			//fullStretch = true;
		}
	}
	else
	{
		fullStretch = false;

		s[0].Update(cpuPos.x, cpuPos.y, s[1].GetX(), s[1].GetY(), SPRING_NUM - 1);

		for (auto i = 1; i < SPRING_NUM - 1; i++)
		{
			s[i].Update(s[i - 1].GetX(), s[i - 1].GetY(), s[i + 1].GetX(), s[i + 1].GetY(), SPRING_NUM - 1 - i);
		}
	}

	s[SPRING_NUM - 1].Update(s[SPRING_NUM - 2].GetX(), s[SPRING_NUM - 2].GetY(), opePos.x, opePos.y, 0);
}

void SpringMgr::Draw()
{
	int color = GetColor(138, 80, 175);

	if (powf(cpuPos.x - opePos.x, 2.0f) + powf(cpuPos.y - opePos.y, 2.0f) > powf(maxLen, 2.0f))
	{
		float rad = atan2(cpuPos.y - opePos.y, cpuPos.x - opePos.x);
		float corX = cos(rad) * maxLen;
		float corY = sin(rad) * maxLen;

		if (fullStretch)
		{
			DrawLine(opePos.x, opePos.y, corX + opePos.x, corY + opePos.y, color, 5);
		}
		else
		{
			s[0].Draw(opePos.x + cos(rad) * maxLen, opePos.y + sin(rad) * maxLen, color);
		}
	}
	else
	{
		s[0].Draw(cpuPos.x, cpuPos.y, color);
	}

	if (!fullStretch)
	{
		for (auto i = 1; i < SPRING_NUM - 1; i++)
		{
			s[i].Draw(s[i - 1].GetX(), s[i - 1].GetY(), color);
		}
		s[SPRING_NUM - 1].Draw(s[SPRING_NUM - 2].GetX(), s[SPRING_NUM - 2].GetY(), color);
	}
}

void SpringMgr::Finalize()
{
}

void MTimer::Reset()
{
	result.Reset();
}

void MTimer::Start()
{
	startTime = GetNowCount();
	nowTime = startTime;
	endTime = 0;
}

void MTimer::End()
{
	endTime = nowTime;
}

void MTimer::Update()
{
	nowTime = GetNowCount() - startTime;
	result.m = nowTime / (60 * 1000);
	result.f = nowTime % 1000 / 10;
	result.s = (nowTime % (60 * 1000) - result.f) / 1000;
}

void MTimer::Draw()
{
}

void MTimer::Finalize()
{
}
