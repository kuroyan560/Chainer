#include "Graphics.h"
#include"DxLib.h"
#include<math.h>

Flash::Flash(int Span, int Count)
{
	flashSpan = Span;
	timer = 0;
	countMax = Count * 2 + 1;
	counter = 0;
}

//点滅機能
void Flash::StartFlash()
{
	counter = countMax;
	timer = flashSpan;
}

//ループ＆透明度変更しない画像========================================
Image::Image(const char* FileName,bool Trans)
{
	handle = LoadGraph(FileName);
	transFlag = Trans;
}

void Image::Init(const char* FileName, bool Trans)
{
	handle = LoadGraph(FileName);
	transFlag = Trans;
}

void Image::Draw(Vector2<float> Position, Vector2<float> Size, bool Turn)
{
	if (!Turn)
	{
		//DrawGraph(Position.x, Position.y, handle, transFlag);
		DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
						Position.x + Size.x / 2, Position.y + Size.y / 2, handle, transFlag);
	}
	else
	{
		//DrawTurnGraph(Position.x, Position.y, handle, transFlag);
		DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
						Position.x - Size.x / 2, Position.y + Size.y / 2, handle, transFlag);
	}
}

//ループする画像======================================================
FlashImage::FlashImage(const char* FileName, bool Trans, int Span, int Count)
	:Image(FileName, Trans), Flash(Span, Count)
{
}

void FlashImage::Draw(Vector2<float> Position, Vector2<float> Size, bool Turn)
{
	if (0 < timer)
	{
		timer--;
	}
	else
	{
		if (0 < counter)
		{
			counter--;
			timer = flashSpan;
		}
	}

	if (counter % 2 == 0)
	{
		if (!Turn)
		{
			//DrawGraph(Position.x, Position.y, handle, transFlag);
			DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
							Position.x + Size.x / 2, Position.y + Size.y / 2, handle, transFlag);
		}
		else
		{
			//DrawTurnGraph(Position.x, Position.y, handle, transFlag);
			DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
				Position.x - Size.x / 2, Position.y + Size.y / 2, handle, transFlag);
		}
	}
	else
	{
		SetDrawBright(255, 255, 255);

		if (!Turn)
		{
			//DrawGraph(Position.x, Position.y, handle, transFlag);
			DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
				Position.x + Size.x / 2, Position.y + Size.y / 2, handle, transFlag);
		}
		else
		{
			//DrawTurnGraph(Position.x, Position.y, handle, transFlag);
			DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
				Position.x - Size.x / 2, Position.y + Size.y / 2, handle, transFlag);
		}

		SetDrawBright(0, 0, 0);
	}
}

int LoopImage::flame = 0;

void LoopImage::FlameUpdate()
{
	flame++;
}

LoopImage::LoopImage(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size)
{
	arrayPtr = new int[MaxFlame];
	LoadDivGraph(FileName, num.x * num.y, num.x, num.y, size.x, size.y, arrayPtr);
	maxFlame = MaxFlame;
	speed = Speed;
	transFlag = Trans;
}

LoopImage::~LoopImage()
{
	delete[] arrayPtr;
}

void LoopImage::Init(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size)
{
	arrayPtr = new int[MaxFlame];
	LoadDivGraph(FileName, num.x * num.y, num.x, num.y, size.x, size.y, arrayPtr);
	maxFlame = MaxFlame;
	speed = Speed;
	transFlag = Trans;
}

void LoopImage::Draw(Vector2<float> Position, Vector2<float> Size, bool Turn)
{
	if (!Turn)
	{
		//DrawGraph(Position.x, Position.y, arrayPtr[flame / speed % maxFlame], transFlag);
		DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
				Position.x + Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame / speed % maxFlame], transFlag);
	}
	else
	{
		//DrawTurnGraph(Position.x, Position.y, arrayPtr[flame / speed % maxFlame], transFlag);
		DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
			Position.x - Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame / speed % maxFlame], transFlag);
	}
}

int LoopImage::GetNowGraph()const
{
	return arrayPtr[flame / speed % maxFlame];
}

AnimationImage::AnimationImage(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size, int LastDrawFlame, bool ImageSwitchFlag, bool firstFlame) {
	arrayPtr = new int[MaxFlame];
	LoadDivGraph(FileName, num.x * num.y, num.x, num.y, size.x, size.y, arrayPtr);
	maxFlame = MaxFlame;
	speed = Speed;
	transFlag = Trans;
	lastDrawFlame = LastDrawFlame;
	flame = 0;
	animationFlag = false;
	imageSwitchFlag = ImageSwitchFlag;
	firstFlameDraw = firstFlame;
}

AnimationImage::~AnimationImage()
{
	delete[] arrayPtr;
}

void AnimationImage::Init()
{
	flame = 0;
	animationFlag = false;
}

void AnimationImage::Init(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size, int LastDrawFlame, bool ImageSwitchFlag, bool firstFlame) {
	arrayPtr = new int[MaxFlame];
	LoadDivGraph(FileName, num.x * num.y, num.x, num.y, size.x, size.y, arrayPtr);
	maxFlame = MaxFlame;
	speed = Speed;
	transFlag = Trans;
	lastDrawFlame = LastDrawFlame;
	flame = 0;
	animationFlag = false;
	imageSwitchFlag = ImageSwitchFlag;
	firstFlameDraw = firstFlame;
}

void AnimationImage::Update(){
	if (animationFlag == true) {
		timer++;

		if (timer % speed == 0 && flame < maxFlame - 1) {
			flame++;
			timer = 0;
		}

		if (flame == maxFlame - 1 && timer == lastDrawFlame) {
			if (imageSwitchFlag) {
				animationFlag = false;
			}
		}
	}
}

void AnimationImage::Draw(Vector2<float> Position, Vector2<float> Size, bool Turn){

	if (animationFlag) {
		if (!Turn) {
			DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
							Position.x + Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame], transFlag);
		}
		else {
			DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
							Position.x - Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame], transFlag);
		}
	}
	else {
		if (firstFlameDraw) {
			if (!Turn) {
				DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
								Position.x + Size.x / 2, Position.y + Size.y / 2, arrayPtr[0], transFlag);
			}
			else {
				DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
								Position.x - Size.x / 2, Position.y + Size.y / 2, arrayPtr[0], transFlag);
			}
		}
	}
}

void AnimationImage::AnimationStart(){
	if (!animationFlag) {
		animationFlag = true;
		flame = 0;
		timer = 0;
	}
}

bool AnimationImage::DrawComplete()
{
	if (flame == maxFlame - 1 && timer == lastDrawFlame) {
		return true;
	}
	else {
		return false;
	}
}

FlashLoopImage::FlashLoopImage(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size, int Span, int Count)
	:LoopImage(FileName, Trans, MaxFlame, Speed, num, size), Flash(Span, Count)
{
}

void FlashLoopImage::Draw(Vector2<float> Position, Vector2<float> Size, bool Turn)
{
	if (0 < timer)
	{
		timer--;
	}
	else
	{
		if (0 < counter)
		{
			counter--;
			timer = flashSpan;
		}
	}

	if (counter % 2 == 0)
	{
		if (!Turn)
		{
			//DrawGraph(Position.x, Position.y, arrayPtr[flame / speed % maxFlame], transFlag);
			DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
				Position.x + Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame / speed % maxFlame], transFlag);
		}
		else
		{
			//DrawTurnGraph(Position.x, Position.y, arrayPtr[flame / speed % maxFlame], transFlag);
			DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
				Position.x - Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame / speed % maxFlame], transFlag);
		}
	}
	else
	{
		SetDrawBright(255, 255, 255);

		if (!Turn)
		{
			//DrawGraph(Position.x, Position.y, arrayPtr[flame / speed % maxFlame], transFlag);
			DrawExtendGraph(Position.x - Size.x / 2, Position.y - Size.y / 2,
				Position.x + Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame / speed % maxFlame], transFlag);
		}
		else
		{
			//DrawTurnGraph(Position.x, Position.y, arrayPtr[flame / speed % maxFlame], transFlag);
			DrawExtendGraph(Position.x + Size.x / 2, Position.y - Size.y / 2,
				Position.x - Size.x / 2, Position.y + Size.y / 2, arrayPtr[flame / speed % maxFlame], transFlag);
		}

		SetDrawBright(0, 0, 0);
	}
}

float RotaAnimationImage::GetAngle(Vector2<float> Vec)
{
	float s;

	s = acos(Vec.x / sqrt(Vec.x * Vec.x + Vec.y * Vec.y)); /*角度θを求める*/

	if (Vec.y < 0) /* θ＞πの時 */
		s = 2 * DX_PI - s;

	return s;
}

void RotaAnimationImage::AnimationStart(Vector2<float> Position,Vector2<float> Vec)
{
	if (!animationFlag) {
		animationFlag = true;
		flame = 0;
		timer = 0;

		pos = Position;
		angle = GetAngle(Vec);
	}
}

void RotaAnimationImage::Draw()
{
	if (animationFlag) {
		DrawRotaGraph(pos.x, pos.y, 1.0F, angle, arrayPtr[flame], true);
	}
	else {
		if (firstFlameDraw) {
			DrawRotaGraph(pos.x, pos.y, 1.0F, angle, arrayPtr[0], true);
		}
	}
}
