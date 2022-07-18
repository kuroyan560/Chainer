#pragma once
#include"Vector2.h"
#include<string>
#include<vector>
class MyFunc
{
public:
	static float lerp(float start, float end, float t);
	static void DrawExtendGraph2(Vector2<float>CenterPos, float Size, int GraphHandle, int Trans);
	static void DrawExtendGraph3(Vector2<float>CenterPos, float Width, float Height, int GraphHandle, int Trans);
	static void DrawExtendGraph4(float LeftUpX,float LeftUpY, float Size, int GraphHandle, int Trans);
	static void DrawExtendGraph5(float LeftUpX, float LeftUpY, float Width, float Height, int GraphHandle, int Trans);
	static bool InnerWin(const float& LocalX1, const float& LocalY1, const float& LocalX2, const float& LocalY2);
	static bool InnerWin(const Vector2<float>& LocalPos, const float& Size);

	static int GetDigits(int Num);
	//static int SetNumArray(int* Ptr, int Num);	//受け取ったポインタに配列を生成、引数の数の各桁の数字を格納、返り値は桁数（配列の要素数）

	//横長の画像用意
	static void DrawLine2(const int& Graph, Vector2<float> GraphSize, Vector2<float> From, const Vector2<float> To, int Thickness);

	static int ctoi(char c) {
		if (c >= '0' && c <= '9') {
			return c - '0';
		}
		return 0;
	}

	static bool CheckFileExist(const std::string& str);

	static void SetNumArray(std::vector<int>* Array, const int& Num);
};