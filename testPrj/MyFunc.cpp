#include "MyFunc.h"
#include"DxLib.h"
#include"Constants.h"
#include<fstream>

float MyFunc::lerp(float start, float end, float t)
{
	return (1 - t) * start + t * end;
}

void MyFunc::DrawExtendGraph2(Vector2<float> CenterPos, float Size, int GraphHandle, int Trans)
{
	DrawExtendGraph(CenterPos.x - Size / 2, CenterPos.y - Size / 2,
		CenterPos.x + Size / 2, CenterPos.y + Size / 2, GraphHandle, Trans);
}

void MyFunc::DrawExtendGraph3(Vector2<float> CenterPos, float Width, float Height, int GraphHandle, int Trans)
{
	DrawExtendGraph(CenterPos.x - Width / 2, CenterPos.y - Height / 2, 
		CenterPos.x + Width / 2,CenterPos.y + Height / 2, GraphHandle, Trans);
}

void MyFunc::DrawExtendGraph4(float LeftUpX, float LeftUpY, float Size, int GraphHandle, int Trans)
{
	DrawExtendGraph(LeftUpX, LeftUpY, LeftUpX + Size, LeftUpY + Size, GraphHandle, Trans);
}

void MyFunc::DrawExtendGraph5(float LeftUpX, float LeftUpY, float Width, float Height, int GraphHandle, int Trans)
{
	DrawExtendGraph(LeftUpX, LeftUpY, LeftUpX + Width, LeftUpY + Height, GraphHandle, Trans);
}

bool MyFunc::InnerWin(const float& LocalX1, const float& LocalY1, const float& LocalX2, const float& LocalY2)
{
	if (LocalX2 < 0 || Constants::WIN_WIDTH < LocalX1 || LocalY2 < 0 || Constants::WIN_HEIGHT < LocalY1)
	{
		return false;
	}
	return true;
}

bool MyFunc::InnerWin(const Vector2<float>& LocalPos, const float& Size)
{
	if (LocalPos.x + Size / 2 < 0 || Constants::WIN_WIDTH < LocalPos.x - Size / 2 || LocalPos.y + Size / 2 < 0 || Constants::WIN_HEIGHT < LocalPos.y - Size / 2)
	{
		return false;
	}
	return true;
}

int MyFunc::GetDigits(int Num)
{
	int digit = 1;
	while (Num /= 10)
	{
		digit++;
	}
	return digit;
}

//int MyFunc::SetNumArray(int* Ptr, int Num)
//{
//	if (Ptr == nullptr)
//	{
//		int digit = GetDigits(Num);
//		Ptr = new int[digit];
//		//‚»‚ê‚¼‚ê‚ÌŒ…‚Ì”‚ð‹‚ß‚é
//		for (int i = digit - 1; 0 <= i; i--)
//		{
//			Ptr[i] = Num % 10;
//			Num /= 10;
//		}
//		return digit;
//	}
//	return 0;
//}

void MyFunc::DrawLine2(const int& Graph, Vector2<float> GraphSize, Vector2<float> From, const Vector2<float> To, int Thickness)
{
	float distance = GetDistance(From, To);
	Vector2<float> Vec = GetVec2(From, To);

	Vector2<float>ExpRate = { distance / GraphSize.x,Thickness / GraphSize.y };
	Vector2<float>CenterPos = From + Vec * distance / 2;

	DrawRotaGraph3(CenterPos.x, CenterPos.y, GraphSize.x / 2, GraphSize.y / 2, ExpRate.x, ExpRate.y, GetRad(Vec), Graph, true);
}

bool MyFunc::CheckFileExist(const std::string& str)
{
	std::ifstream ifs(str);
	return ifs.is_open();
}

void MyFunc::SetNumArray(std::vector<int>* Array, const int& Num)
{
	if (Array != nullptr)
	{
		Array->clear();
		int n = Num;
		int digit = GetDigits(n);

		for (int i = 0; i < digit; i++)
		{
			Array->push_back(0);
		}

		for (int i = digit - 1; 0 <= i; i--)
		{
			Array->at(i) = n % 10;
			n /= 10;
		}
	}
}
