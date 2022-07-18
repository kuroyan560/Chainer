#pragma once
#include "DxLib.h"

class Fps {
	int mStartTime;
	int mCount;
	float mFps;
	static const int N = 60;
	static const int FPS = 50;

public:
	static int GetBaseFPS() { return FPS; }

	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	void Update() {
		!mCount ? mStartTime = GetNowCount() : 0;
		if (mCount == N) {
			int t = GetNowCount();
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
		int waitTime = mCount * 1000 / FPS - (GetNowCount() - mStartTime);
		waitTime > 0 ? Sleep(waitTime) : void();
	}

	void Draw() {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", mFps);
	}
};