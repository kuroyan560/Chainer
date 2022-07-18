#include "UsersInput.h"

static const int KEY_NUM = 256;
static char keys[KEY_NUM];
static char oldkeys[KEY_NUM];

static TransMgr* trans;

void InputSet(TransMgr* TransMgr)
{
	trans = TransMgr;
}

void KeyUpdate()
{
	//最新のキーボード情報だったものは１フレーム前のキーボード情報として保存
	memcpy(oldkeys, keys, KEY_NUM);

	//最新のキーボード情報を取得
	GetHitKeyStateAll(keys);
}

bool KeyJustInput(int KeyCode)
{
	if (keys[KeyCode] && !oldkeys[KeyCode]) {
		return true;
	}
	else return false;
}

bool KeyInput(int KeyCode)
{
	return keys[KeyCode];
}

static XINPUT_STATE oldInput = { 0 };
static XINPUT_STATE input = { 0 };

static const float STICK_MAX = 32767;
static const float STICK_MIN = -32768;

static const float INPUT_RATE = 7;	//大きければ大きいほど入力判定が甘くなる

void PadUpdate()
{
	oldInput = input;
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &input);
}

bool JustInput(InputName what)
{
	if (trans->NowTrans() == true)
	{
		return false;
	}

	if (what == LEFT)
	{
		if (input.ThumbLX == STICK_MIN && oldInput.ThumbLX != STICK_MIN)
		{
			return true;
		}
	}
	else if (what == RIGHT)
	{
		if (input.ThumbLX == STICK_MAX && oldInput.ThumbLX != STICK_MAX)
		{
			return true;
		}
	}
	else if (what == UP)
	{
		if (input.ThumbLY == STICK_MAX && oldInput.ThumbLY != STICK_MAX)
		{
			return true;
		}
	}
	else if (what == DOWN)
	{
		if (input.ThumbLY == STICK_MIN && oldInput.ThumbLY != STICK_MIN)
		{
			return true;
		}
	}
	else if (what == A && input.Buttons[XINPUT_BUTTON_A] && !oldInput.Buttons[XINPUT_BUTTON_A])
	{
		return true;
	}
	else if (what == B && input.Buttons[XINPUT_BUTTON_B] && !oldInput.Buttons[XINPUT_BUTTON_B])
	{
		return true;
	}
	else if (what == X && input.Buttons[XINPUT_BUTTON_X] && !oldInput.Buttons[XINPUT_BUTTON_X])
	{
		return true;
	}
	else if (what == Y && input.Buttons[XINPUT_BUTTON_Y] && !oldInput.Buttons[XINPUT_BUTTON_Y])
	{
		return true;
	}
	else if (what == LT && input.LeftTrigger == 255 && oldInput.LeftTrigger != 255)
	{
		return true;
	}
	else if (what == RT && input.RightTrigger == 255 && oldInput.RightTrigger != 255)
	{
		return true;
	}
	else if (what == BACK && input.Buttons[XINPUT_BUTTON_BACK] && !oldInput.Buttons[XINPUT_BUTTON_BACK])
	{
		return true;
	}
	else if (what == START && input.Buttons[XINPUT_BUTTON_START] && !oldInput.Buttons[XINPUT_BUTTON_START])
	{
		return true;
	}
	return false;
}

float Input(InputName what)
{
	if (trans->NowTrans() == true)
	{
		return 0.0F;
	}

	if (what == LEFT)
	{
		if (input.ThumbLX <= (STICK_MIN / INPUT_RATE))
		{
			return input.ThumbLX / STICK_MIN;
		}
	}
	else if (what == RIGHT)
	{
		if ((STICK_MAX / INPUT_RATE) <= input.ThumbLX)
		{
			return input.ThumbLX / STICK_MAX;
		}
	}
	else if (what == UP)
	{
		if (STICK_MAX / INPUT_RATE <= input.ThumbLY)
		{
			return input.ThumbLY / STICK_MAX;
		}
	}
	else if (what == DOWN)
	{
		if (input.ThumbLY <= (STICK_MIN / INPUT_RATE))
		{
			return input.ThumbLY / STICK_MIN;
		}
	}

	return 0.0F;
}

Vector2<float> GetPadVec()
{
	if ((Input(LEFT) || Input(RIGHT)) || (Input(UP) || Input(DOWN)))
	{
		return GetVec(Vector2<float>{(float)input.ThumbLX, (float)-input.ThumbLY});
	}
	else
	{
		return { 0.0F,0.0F };
	}
}

Vector2<float> GetChainVec()
{
	if (Input(LEFT))
	{
		return { -1,-1 };	//左上
	}
	else
	{
		return { 1,-1 };	//右上
	}

	return { 0,0 };
}

bool GetPadStickInput()
{
	if (GetPadVec().x == 0.0F && GetPadVec().y == 0.0F)
	{
		return false;
	}

	return true;
}
