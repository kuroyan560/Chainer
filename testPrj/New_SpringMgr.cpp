#include "New_SpringMgr.h"

void New_SpringMgr::Init(Vector2<float> pos)
{
	for (auto i{ 0 }; i < SPRING_NUM; i++)
	{
		s[i].Init(pos.x, pos.y, len, k, gravity, mass, damp);
	}
	fullStretch = false;
}

void New_SpringMgr::Update(float _maxLen, Vector2<float> _opePos, Vector2<float> _cpuPos)
{
	maxLen = _maxLen;
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

	for (int i = 0; i < SPRING_NUM; i++)
	{
		s[i].HitCheckItem();
	}
}

void New_SpringMgr::Draw(CamAccessor* Cam)
{
	int color = GetColor(138, 80, 175);

	if (powf(cpuPos.x - opePos.x, 2.0f) + powf(cpuPos.y - opePos.y, 2.0f) > powf(maxLen, 2.0f))
	{
		float rad = atan2(cpuPos.y - opePos.y, cpuPos.x - opePos.x);
		float corX = cos(rad) * maxLen;
		float corY = sin(rad) * maxLen;

		if (fullStretch)
		{
			DrawLine(opePos.x, opePos.y, corX + opePos.x, corY + opePos.y, color, 2);
		}
		else
		{
			s[0].Draw(Cam,opePos.x + cos(rad) * maxLen, opePos.y + sin(rad) * maxLen, color);
		}
	}
	else
	{
		s[0].Draw(Cam,cpuPos.x, cpuPos.y, color);
	}

	if (!fullStretch)
	{
		for (auto i = 1; i < SPRING_NUM - 1; i++)
		{
			s[i].Draw(Cam,s[i - 1].GetX(), s[i - 1].GetY(), color);
		}
		s[SPRING_NUM - 1].Draw(Cam,s[SPRING_NUM - 2].GetX(), s[SPRING_NUM - 2].GetY(), color);
	}
}

void New_SpringMgr::Finalize()
{
}
