#pragma once
#include<vector>

enum type
{
	In,Out,InOut
};
enum ease
{
	Quad, Cubic, Quart, Quint, Sine, Exp, Circ, Back, SoftBack, Elastic, Bounce, Linear
};

namespace EasingFunction
{
	double InQuad(double t);
	double InCubic(double t);
	double InQuart(double t);
	double InQuint(double t);
	double InSine(double t);
	double InExp(double t);
	double InCirc(double t);
	double InBack(double t);
	double InSoftBack(double t);
	double InElastic(double t);
	double InBounce(double t);
	double InLinear(double t);

	class EaseSeed
	{
		float easeRate;

		type typeName;
		type nowType;
		ease easeName;
		float changeRate;
		float start;
		float end;
		bool play;
		bool playBack;

	public:
		EaseSeed(type Type, ease Ease, float ChangeRate, float Start, float End)
			:typeName(Type), easeName(Ease), changeRate(ChangeRate), start(Start), end(End)
		{
			play = playBack = false;
			nowType = Type;
			easeRate = 1.0F;
		};
		void Play_Trigger();
		void PlayBack_Trigger();
		float GetEase_Update();
		bool GetComplete() { return (1.0f <= easeRate); }
		void ReSetStart(float Start) { start = Start; }
		void ReSetEnd(float End) { end = End; }
		void Init() { easeRate = 1.0f; }
	};

	class EaseSeed2
	{
		float easeRate;

		type typeName;
		type nowType;
		ease easeName;
		float changeRate;
		bool play;
		bool playBack;

	public:
		EaseSeed2(type Type, ease Ease, float ChangeRate)
			:typeName(Type), easeName(Ease), changeRate(ChangeRate)
		{
			play = playBack = false;
			nowType = Type;
			easeRate = 1.0F;
		}

		void Play_Trigger();
		void PlayBack_Trigger();
		void Update();
		float GetEaseRate() { return easeRate; }
		bool GetComplete() { return (1.0f <= easeRate); }
		void Init() { easeRate = 1.0f; }
	};

	class EaseSeedMgr
	{
		static std::vector<EaseSeed>seed;
		static std::vector<EaseSeed2>seed2;

		static int TransIDtoIndex(int ID);

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="Id">IDを格納するための変数のポインタ</param>
		/// <param name="Type">In,Out,InOut</param>
		/// <param name="Ease">Ease</param>
		/// <param name="ChangeRate">イージングの速さ</param>
		/// <param name="Start">スタート値</param>
		/// <param name="End">エンド値</param>
		static void PushEaseSeed(int* Id,type Type, ease Ease, float ChangeRate, float Start, float End);
		static void PushEaseSeed(int* Id, type Type, ease Ease, float ChangeRate);
		static int Play_Trigger(int id);
		static int PlayBackTrigger(int id);
		static float GetEaseUpdate(int id);
		static float GetEaseAffect(int id, float Start, float End);
		static void EaseUpdate(int id);
		static bool CompleteEase(int id);
		static int ReSetStart(int id, float start);
		static int ReSetEnd(int id, float end);
		static void EaseInit(int id);
	};
}

double Ease(type Type, ease Ease, double t);

//返り値　０…成功　-1…エラー
int CreateEaseSeed(int* Id, type Type, ease Ease, float ChangeRate, float Start, float End);
int CreateEaseSeed(int* Id, type Type, ease Ease, float ChangeRate);
int PlayEase_Trigger(const int& EaseId);
int PlayBackEase_Trigger(const int& EaseId);
float GetEaseUpdate(const int& EaseId);	//シード１用（IDは正）
float GetEaseAffect(const int& EaseID, const float& Start, const float& End);//シード２用（IDは負）
void EaseUpdate(const int& EaseId);//シード２用（IDは負）
bool CompleteEase(const int& EaseId);
int ReSetEaseStart(const int& EaseId, const float& Start);
int ReSetEaseEnd(const int& EaseId, const float& End);
void EaseInit(const int& EaseId);