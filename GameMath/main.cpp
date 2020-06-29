#include <DxLib.h>
#include <math.h>
#include "Geometry.h"

bool Init();
Rect rcA, rcB;
char keyState[256];
int speed;
int image;
Vector2 pos;
int angle;

//typedef Vector2 hypot;
Vector2 diff;

bool IsHit(const Rect& a, const Rect& b)
{
	return !(min(a.Right(), b.Right()) < max(a.Left(), b.Left()) ||
		min(a.Bottom(), b.Bottom()) < max(a.Top(), b.Top()));
}

bool IsHit(const Circle& a, const Circle& b)
{
	diff = a.center - b.center;
	return ((a.rad + b.rad) > (hypot(diff.x, diff.y)));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return 0;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	Init();

	Circle ciA, ciB;
	ciA = { {100, 100}, 50 };
	ciB = { {300, 300}, 40 };

	image = LoadGraph("image/Ryazirusi.png");
	pos = { 350, 250 };
	angle = 0;

	while(!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		int vax = 0;
		int	vay = 0;
		GetHitKeyStateAll(keyState);
		if (keyState[KEY_INPUT_LEFT])
		{
			vax -= speed;
		}
		if (keyState[KEY_INPUT_RIGHT])
		{
			vax += speed;
		}
		if (keyState[KEY_INPUT_UP])
		{
			vay -= speed;
		}
		if (keyState[KEY_INPUT_DOWN])
		{
			vay += speed;
		}

		//rcA.x += vax;
		//rcA.y += vay;

		ciA.center.x += vax;
		ciA.center.y += vay;

		int color = 0xffffff;
		if (IsHit(ciA, ciB))
		{
			color = 0xffaaaa;
			//if ()
			//{

			//}
			//if ()
			//{

			//}
			//if ()
			//{

			//}
			//if ()
			//{

			//}
		}

		ClsDrawScreen();

		//DrawBox(rcA.x, rcA.y, rcA.Right(), rcA.Bottom(), color, false);
		//DrawBox(rcB.x, rcB.y, rcB.Right(), rcB.Bottom(), color, false);

		//// ‰~A
		//DrawCircle(ciA.center.x, ciA.center.y, (int)ciA.rad, color);
		//// ‰~B
		//DrawCircle(ciB.center.x, ciB.center.y, (int)ciB.rad, color);

		DrawRotaGraph(pos.x, pos.y, 1.0, angle, image, true, false);

		ScreenFlip();
		//Draw();
	}

	DxLib_End();
	return 0;
}

bool Init()
{
	rcA = { 0, 0, 100, 100 };
	rcB = { 300, 300, 80, 80 };
	speed = 4;
	return true;
}
