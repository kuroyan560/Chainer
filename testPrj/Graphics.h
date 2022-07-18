#pragma once
#include"Vector2.h"
//���N���X
class Graphics
{
protected:
	bool transFlag;

public:
	virtual ~Graphics() {};
	virtual void Draw(Vector2<float> Position, Vector2<float> Size, bool Turn = false) = 0;	//������W
};

//�_�ŋ@�\
class Flash
{
protected:
	Flash(int Span,int Count);
	~Flash() {};

	int flashSpan;
	int timer;
	int countMax;
	int counter;

	void StartFlash();	//�_�ŃX�^�[�g
};

//���[�v�������x�ύX���Ȃ��摜========================================
class Image :public Graphics
{
protected:
	int handle;

public:
	Image(const char* FileName,bool Trans);
	Image() {};
	~Image() {};

	void Init(const char* FileName, bool Trans);
	virtual void Draw(Vector2<float> Position, Vector2<float> Size, bool Turn = false)override;
};
//�_�ŋ@�\��
class FlashImage :public Image,Flash
{
public:
	FlashImage(const char* FileName, bool Trans, int Span, int Count);
	~FlashImage() {};

	virtual void Draw(Vector2<float> Position, Vector2<float> Size, bool Turn = false)override;
};

//���[�v����摜======================================================
class LoopImage :public Graphics
{
protected:
	static int flame;
	int maxFlame;		//�摜�̖���
	int speed;
	int* arrayPtr;

public:
	static void FlameUpdate();

	LoopImage(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size);
	LoopImage() {};
	~LoopImage();

	void Init(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, Vector2<float> size);
	virtual void Draw(Vector2<float> Position, Vector2<float> Size, bool Turn = false)override;
	int GetNowGraph()const;
};

//�A�j���[�V��������摜
class AnimationImage : public Graphics
{
protected:
	int maxFlame;		//�摜�̖���
	int* arrayPtr;
	int flame;
	int lastDrawFlame;
	int speed;
	bool animationFlag;
	int timer;
	bool imageSwitchFlag;
	bool firstFlameDraw;

public:
	/// <summary>
	/// ���[�v���Ȃ��A�j���[�V����
	/// </summary>
	/// <param name="FileName">�摜�̃t�@�C���p�X</param>
	/// <param name="Trans">���߃t���O</param>
	/// <param name="MaxFlame">�摜�̖���</param>
	/// <param name="Speed">�A�j���[�V�����X�s�[�h</param>
	/// <param name="num">���ꂼ��x��y�̉摜��</param>
	/// <param name="size">�摜�T�C�Y</param>
	/// <param name="LastDrawFlame">�A�j���[�V�����㉽�t���[���ҋ@���邩</param>
	/// <param name="ImageSwitchFlag">�A�j���[�V�������I������瑼�̃C���[�W�ɕύX���邩</param>
	/// <param name="firstFlame">��ԍŏ��̉摜����ɕ\�����Ă������i�ꎞ�I�ɃA�j���[�V��������摜�p�j</param>
	AnimationImage(const char* FileName, bool Trans, int MaxFlame, int Speed,Vector2<int> num, 
				   Vector2<float> size, int LastDrawFlame, bool ImageSwitchFlag, bool firstFlame = false);
	AnimationImage() {};
	~AnimationImage();

	void Init();
	void Init(const char* FileName, bool Trans, int MaxFlame, int Speed, Vector2<int> num, 
			  Vector2<float> size, int LastDrawFlame, bool ImageSwitchFlag, bool firstFlame = false);
	void Update();
	virtual void Draw(Vector2<float> Position, Vector2<float> Size, bool Turn = false)override;

	void AnimationStart();
	bool NowAnimation()const { return animationFlag; }
	bool DrawComplete();
};

class RotaAnimationImage :public AnimationImage
{
private:
	Vector2<float> pos;
	float angle;

	float GetAngle(Vector2<float> Vec);

public:
	RotaAnimationImage() {};
	~RotaAnimationImage() {};

	void AnimationStart(Vector2<float> Position,Vector2<float> Vec);
	void Draw();
};

//�_�ŋ@�\��
class FlashLoopImage :public LoopImage,Flash
{
public:
	FlashLoopImage(const char* FileName, bool Trans, int MaxFlame, int Speed, 
						Vector2<int> num, Vector2<float> size, int Span, int Count);
	~FlashLoopImage() {};
	
	virtual void Draw(Vector2<float> Position, Vector2<float> Size, bool Turn = false)override;
};