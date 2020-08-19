#include<DxLib.h>
#include<cmath>
#include<random>
#include"Geometry.h"

using namespace std;

struct Rock {
	Circle circle;
	float angle;
	float speed;
	bool alive;
};

void DrawWood(const Capsule& cap, int handle) {


	auto v = cap.posB - cap.posA;
	auto angle = atan2(v.y, v.x) + DX_PI_F / 2.0f;
	auto w = cos(angle)*cap.radius;
	auto h = sin(angle)*cap.radius;

	DrawModiGraph(
		cap.posA.x - w, cap.posA.y - h,
		cap.posB.x - w, cap.posB.y - h,
		cap.posB.x + w, cap.posB.y + h,
		cap.posA.x + w, cap.posA.y + h, handle,true);
}


///とある点を中心に回転して、その回転後の座標を返します。
///@param center 回転中心点
///@param angle 回転角度
///@param pos 元の座標
///@return 変換後の座標
Matrix RotatePosition(const Position2& center, float angle) {
	//①中心を原点に平行移動して
	//②原点中心に回転して
	//③中心を元の座標へ戻す

	Matrix mat = IdentityMat();
	mat = MultipleMat(TranslateMat(center.x, center.y),
		MultipleMat(RotateMat(angle), TranslateMat(-center.x, -center.y)));
	return mat;
	//これを書き換えて、特定の点を中心に回転を行うようにしてください。
}

float Clamp(float vlate, float maxval = 1.0f, float minval = 0.0f) {
	return min(max(vlate, minval), maxval);
}

//カプセルと円が当たったか？
bool IsHit(const Capsule& cap, const Circle& cc) {
	auto vp = cap.posA - cc.pos;
	auto v = cap.posA - cap.posB;
	auto dot = Dot(vp, v);
	auto vlate = dot / (v.Magnitude() * v.Magnitude());
	vlate = Clamp(vlate);
	v *= vlate;
	auto vq = vp - v;
	return (vq.Magnitude() <= (cap.radius + cc.radius));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(512, 800, 32);
	SetMainWindowText("1916027_徳重虎大朗");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	int sw, sh;//スクリーン幅、高さ
	GetDrawScreenSize(&sw, &sh);

	auto woodH = LoadGraph("img/wood.png");
	int wdW, wdH;
	GetGraphSize(woodH, &wdW, &wdH);
	wdW = 200;

	auto cascadeH = LoadGraph("img/cascade_chip.png");
	auto chipH = LoadGraph("img/atlas0.png");
	auto rockH = LoadGraph("img/rock.png");

 	Capsule cap(20,Position2((sw-wdW)/2,sh-100),Position2((sw - wdW) / 2+wdW,sh-100));
	bool plAlive = true;
	int playerLH = LoadGraph("img/mario.png");
	int playerRH = LoadGraph("img/luigi.png");
	float plFallSpeed = 0.2f;

	Rock rocks[5];
	for (int i = 0; i < 5; i++)
	{
		rocks[i].alive = false;
		rocks[i].circle.radius = 12.0f;
	}
	std::mt19937 mt;
	std::uniform_real_distribution<float> rockPosRange(42.0f, static_cast<float>(sw - 42));
	std::uniform_real_distribution<float> rockSpeedRange(1.0f, 3.0f);
	
	int blastH[12];
	LoadDivGraph("img/explosion.png", 12, 12, 1, 128, 128, blastH);
	int blastCnt = 0;
	int blastFlag = false;
	Vector2 blastPos = { 0.0f, 0.0f };

	char keystate[256];
	
	float angle = 0.0f;

	int frame = 0;
	bool isLeft = false;

	float wallL = 32.0f;
	float wallR = sw - 32.0f;

	int clearH = LoadGraph("img/clear.png");
	float clearLine = 32.0;
	bool clearFlag = false;


	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);

		DrawBox(0, 0, sw, sh, 0xaaffff, true);

		if (frame %  300 == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				if (!rocks[i].alive)
				{
					rocks[i].alive = true;
					rocks[i].circle.pos = { rockPosRange(mt), 0.0f };
					rocks[i].speed = rockSpeedRange(mt);
					rocks[i].angle = 0.0f;
					break;
				}
			}
		}

		for (int i = 0; i < 5; i++)
		{
			if (rocks[i].alive)
			{
				rocks[i].circle.pos.y += rocks[i].speed;
				rocks[i].angle += 0.01f;
			}
		}

		int mx = 0, my = 0;

		if (clearFlag)
		{
			if (keystate[KEY_INPUT_SPACE])
			{
				clearFlag = false;
				angle = 0.0f;
				cap.posA = { static_cast<float>((sw - wdW) / 2), static_cast<float>(sh - 100) };
				cap.posB = { static_cast<float>((sw - wdW) / 2 + wdW), static_cast<float>(sh - 100) };
			}
		}
		else
		{
			// ｸﾘｱ判定
			if ((cap.posA.y < clearLine) && (cap.posB.y < clearLine))
			{
				clearFlag = true;		
				for (int i = 0; i < 5; i++)
				{
					rocks[i].alive = false;
				}
			}

			if (plAlive)
			{
				cap.posA.y += plFallSpeed;
				cap.posB.y += plFallSpeed;

				if (keystate[KEY_INPUT_LEFT]) {
					isLeft = true;
				}
				else if (keystate[KEY_INPUT_RIGHT]) {
					isLeft = false;
				}
				if (isLeft) {
					mx = cap.posA.x;
					my = cap.posA.y;
				}
				else {
					mx = cap.posB.x;
					my = cap.posB.y;
				}
				if (keystate[KEY_INPUT_Z]) {

					angle = -0.05f;
				}
				else if (keystate[KEY_INPUT_X]) {

					angle = 0.05f;
				}
				else {
					angle = 0.0f;
				}

				// 死亡判定をここに書いていく
				// 当たり判定を完成させて当たったときの反応を書いてください
				for (int i = 0; i < 5; i++)
				{
					if (rocks[i].alive)
					{
						if (IsHit(cap, rocks[i].circle)) {
							rocks[i].alive = false;
							plAlive = false;
							blastFlag = true;
							blastPos = { (cap.posA.x + cap.posB.x) / 2, (cap.posA.y + cap.posB.y) / 2 };
						}
					}
				}
				// 180度以上
				if (cap.posB.x < cap.posA.x)
				{
					plAlive = false;
				}
				// 壁に当たる
				if (cap.posA.x < wallL ||
					cap.posB.x > wallR ||
					(cap.posA.y > sh && cap.posB.y > sh))
				{
					plAlive = false;
				}
				

				//カプセル回転
				Matrix rotMat = RotatePosition(Position2(mx, my), angle);
				cap.posA = MultipleVec(rotMat, cap.posA);
				cap.posB = MultipleVec(rotMat, cap.posB);
			}
		}

		//背景の描画
		//滝
		int chipIdx = (frame/4) % 3;
		constexpr int dest_chip_size = 32;
		int destY = 48;
		while (destY < sh) {
			int destX = 0;
			while (destX < sw) {
				DrawRectExtendGraph(
					destX, destY, destX + dest_chip_size, destY+dest_chip_size,
					chipIdx * 16, 0,
					16, 16,
					cascadeH, false);
				destX += dest_chip_size;
			}
			destY += dest_chip_size;
		}

		DrawRectGraph(0, 0, 96, 0, 32, 32, chipH, true);
		DrawRectGraph(sw - 32, 0, 32, 0, 32, 32, chipH, true);
		DrawRectGraph(0, 32, 96, 32, 32, 32, chipH, true);
		DrawRectGraph(sw-32, 32, 32, 32, 32, 32, chipH, true);
		destY = 64;
		while (destY < sh) {
			DrawRectGraph(0, destY, 96, 64, 32, 32, chipH, true);
			DrawRectGraph(sw - 32, destY, 32, 64, 32, 32, chipH, true);
			destY += dest_chip_size;
		}

		// ﾌﾟﾚｲﾔｰの描画と死亡演出と死亡処理
		if (plAlive)
		{
			DrawWood(cap, woodH);
			if(!clearFlag)
			{
				DrawCircle(mx, my, 30, 0xff0000, false, 3);
			}
		}
		else
		{
			// 爆発処理
			if (blastFlag)
			{
				DrawRotaGraph(blastPos.x, blastPos.y, 1.0, 0.0, blastH[blastCnt / 2], true);
				blastCnt++;
				if (blastCnt >= 24)
				{
					blastFlag = false;
					blastCnt = 0;
				}
			}
			DrawRotaGraph(cap.posA.x, cap.posA.y, 1.8, 0.0, playerLH, true);
			DrawRotaGraph(cap.posB.x, cap.posB.y, 1.8, 0.0, playerRH, true);
			cap.posA.y += plFallSpeed;
			cap.posB.y += plFallSpeed;
			plFallSpeed += 0.5f;
			if ((cap.posA.y + 30.0f > sh) && (cap.posB.y + 30.0f > sh))
			{
				plAlive = true;
				angle = 0.0f;
				cap.posA = { static_cast<float>((sw - wdW) / 2), static_cast<float>(sh - 100) };
				cap.posB = { static_cast<float>((sw - wdW) / 2 + wdW), static_cast<float>(sh - 100) };
				plFallSpeed = 0.2f;
			}
		}

		for (int i = 0; i < 5; i++)
		{
			if (rocks[i].alive)
			{
				DrawRotaGraph(rocks[i].circle.pos.x, rocks[i].circle.pos.y, 1.0, rocks[i].angle, rockH, true);
			}
		}

		if (clearFlag)
		{
			DrawRotaGraph(sw / 2, sh / 2, 1.0, 0.0, clearH, true);
		}

		++frame;
		
		ScreenFlip();
	}

	DxLib_End();


}