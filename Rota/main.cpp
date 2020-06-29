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

	Vector2 targetPos;
	Vector2 centorPos = { 320, 240 };

	while (!ProcessMessage() || CheckHitKey(KEY_INPUT_ESCAPE))
	{
		GetMousePoint(&targetPos.x, &targetPos.y);
		auto diff = targetPos - centorPos;
		auto angle = atan2(diff.y, diff.x);

		ClsDrawScreen();

		DrawRotaGraph(centorPos.x, centorPos.y, 1.0f,
			angle,
			catH, true, false);

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
