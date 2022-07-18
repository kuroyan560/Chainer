#pragma once
#include"DxLib.h"
#include"Vector2.h"
#include"TransMgr.h"

void InputSet(TransMgr* TransMgr);
void KeyUpdate();
bool KeyJustInput(int KeyCode);
bool KeyInput(int KeyCode);

void PadUpdate();

static enum InputName { UP, DOWN, LEFT, RIGHT, A, B, X, Y, LT, RT, BACK, START };

bool JustInput(InputName what);
float Input(InputName what);	//“ü—Í—¦‚ð•Ô‚·

Vector2<float>GetPadVec();
Vector2<float>GetChainVec();
bool GetPadStickInput();