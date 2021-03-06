#include <DxLib.h>
#include <cmath>
#include "Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)
	{
		return 0;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	int catH = LoadGraph("image/arrowcat.png");

	Vector2f targetPos;
	Vector2f centorPos = { 320.0f, 240.0f };
	float speed = 1.0f;

	Vector2f bulPos;
	Vector2f bulVec;
	int frame = 0;

	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		int x, y;
		GetMousePoint(&x, &y);
		targetPos = { (float)x, (float)y };

		auto diff = targetPos - centorPos;
		auto angle = atan2(diff.y, diff.x);

		if (frame % 120 == 0)
		{
			bulPos = centorPos;
			bulVec = Vector2f(diff.x, diff.y);
			bulVec.Normalize();
			bulVec = bulVec * 5.0f;
		}

		bulPos = bulPos + bulVec;

		ClsDrawScreen();

		SetDrawMode(DX_DRAWMODE_BILINEAR);

		DrawRotaGraph(centorPos.x, centorPos.y, 1.0f,
			angle,
			catH, true, false);

		DrawCircle(bulPos.x, bulPos.y, 3, 0xffffff, true);

		ScreenFlip();

		++frame;
	}
	DxLib_End();
	return 0;
}
