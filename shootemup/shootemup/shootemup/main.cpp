#include<DxLib.h>
#include<cmath>
#include<random>
#include"Geometry.h"

///“–‚½‚è”»’èŠÖ”
///@param posA A‚ÌÀ•W
///@param radiusA A‚Ì”¼Œa
///@param posB B‚ÌÀ•W
///@param radiusB B‚Ì”¼Œa
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//“–‚½‚è”»’è‚ğÀ‘•‚µ‚Ä‚­‚¾‚³‚¢
	Vector2 vec = { posA.x - posB.x, posA.y - posB.y };
	return (radiusA + radiusB) > vec.Magnitude();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("1916027_“¿dŒÕ‘å˜N");
	if (DxLib_Init() != 0) {
		return -1;
	}

	//”wŒi—p
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH=LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	struct Bullet {
		Position2 pos;//À•W
		Vector2 vel;//‘¬“x
		bool isActive = false;//¶‚«‚Ä‚é‚©`H
		bool state = false;// “r’†‚Å•Ï‚í‚é‚©‚Ç‚¤‚©
		int count = 0;
	};

	//’e‚Ì”¼Œa
	float bulletRadius = 5.0f;

	//©‹@‚Ì”¼Œa
	float playerRadius = 10.0f;

	//“K“–‚É256ŒÂ‚­‚ç‚¢ì‚Á‚Æ‚­
	Bullet bullets[256];

	Position2 enemypos(320,25);//“GÀ•W
	Position2 playerpos(320, 400);//©‹@À•W

	unsigned int frame = 0;//ƒtƒŒ[ƒ€ŠÇ——p
	unsigned int bulFrame = 0;

	char keystate[256];
	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;

	std::mt19937 mt;
	std::uniform_real_distribution<float> angleRange(-DX_PI / 8, DX_PI / 8);
	std::uniform_real_distribution<float> angleRange2(-DX_PI / 6, DX_PI / 6);
	std::uniform_real_distribution<float> vecRange(2.0f, 6.0f);

	int shakeScreen = MakeScreen(DEFAULT_SCREEN_SIZE_X, DEFAULT_SCREEN_SIZE_Y, true);
	int shakeCount = 0;
	Vector2 shakePos = { 0, 0 };
	std::uniform_real_distribution<float> shakeRange(-5, 5);


	while (ProcessMessage() == 0) {
		SetDrawScreen(shakeScreen);
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//”wŒi
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//ƒvƒŒƒCƒ„[
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//©‹@‚Ì–{‘Ì(“–‚½‚è”»’è)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		//’e”­Ë
		if (frame % 25 == 0) {
			if (bulFrame / 300 % 5 == 0) {
				// ©‹@‘_‚¢3way
				auto v = playerpos - enemypos;
				float baseAngle = atan2(v.y, v.x);
				float wayAngle = DX_PI_F / 9;
				for (int i = 0; i < 3; i++) {
					float angle = baseAngle + wayAngle * (float)(i - 1);
					for (auto& b : bullets) {
						if (!b.isActive) {
							b.pos = enemypos;
							b.vel = Vector2(cosf(angle), sinf(angle)) * 5.0f;
							b.isActive = true;
							break;
						}
					}
				}
			}
			else if (bulFrame / 300 % 5 == 1) {
				// ŠgU’e
				float diffAngle = (DX_PI_F * 2) / 16;
				float angle = 0.0f;
				for (int i = 0; i < 16; i++) {
					for (auto& b : bullets) {
						if (!b.isActive) {
							b.pos = enemypos;
							b.vel = Vector2(cosf(angle), sinf(angle)) * 5.0f;
							b.isActive = true;
							break;
						}
					}
					angle += diffAngle;
				}
			}
			else if (bulFrame / 300 % 5 == 2) {
				// ‚Î‚ç‚Ü‚«’e
				float diffAngle = (DX_PI_F * 2) / 24;
				float angle = 0.0f;
				for (int i = 0; i < 24; i++) {
					for (auto& b : bullets) {
						if (!b.isActive) {
							b.pos = enemypos;
							b.vel = Vector2(cosf(angle), sinf(angle)) * vecRange(mt);
							b.isActive = true;
							break;
						}
					}
					angle += diffAngle + angleRange(mt);
				}
			}
			else if (bulFrame / 300 % 5 == 3) {
				// ŠgU’e2
				float diffAngle = (DX_PI_F * 2) / 16;
				float angle = (diffAngle / 2) * (frame / 25 % 2);
				for (int i = 0; i < 16; i++) {
					for (auto& b : bullets) {
						if (!b.isActive) {
							b.pos = enemypos;
							b.vel = Vector2(cosf(angle), sinf(angle)) * 4.0f;
							b.isActive = true;
							break;
						}
					}
					angle += diffAngle;
				}
			}
			else if (bulFrame / 300 % 5 == 4) {
				// ŠgU’e2
				auto v = playerpos - enemypos;
				float angle = atan2(v.y, v.x) + angleRange2(mt);
				for (int i = 0; i < 16; i++) {
					for (auto& b : bullets) {
						if (!b.isActive) {
							b.pos = enemypos;
							b.vel = Vector2(cosf(angle), sinf(angle)) * 3.0f;
							b.count = 0;
							b.isActive = true;
							b.state = true;
							break;
						}
					}
				}
			}
		}

		float diffAngle = (DX_PI_F * 2) / 16;
		float angle = 0.0f;
		//’e‚ÌXV‚¨‚æ‚Ñ•\¦
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			// “r’†‚Å•ÏX‚³‚ê‚é’e–‹‚ÌXV
			if (b.state)
			{
				if (b.count >= 60)
				{
					b.state = false;
					b.vel = Vector2(cosf(angle), sinf(angle)) * 5.0f;
					angle += diffAngle;
				}
			}
			b.count++;

			//’e‚ÌŒ»İÀ•W‚É’e‚ÌŒ»İ‘¬“x‚ğ‰ÁZ‚µ‚Ä‚­‚¾‚³‚¢
			b.pos += b.vel;
			
			float angle = 0.0f;
			//’e‚ÌŠp“x‚ğatan2‚ÅŒvZ‚µ‚Ä‚­‚¾‚³‚¢Bangle‚É’l‚ğ“ü‚ê‚é‚ñ‚¾‚æƒIƒD
			angle = atan2(b.vel.y, b.vel.x);
			
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//’e‚Ì–{‘Ì(“–‚½‚è”»’è)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}
			//’e‚ğE‚·
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//‚ ‚½‚èI
			//«‚ÌIsHit‚ÍÀ‘•‚ğ‘‚¢‚Ä‚Ü‚¹‚ñB©•ª‚Å‘‚¢‚Ä‚­‚¾‚³‚¢B
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//“–‚½‚Á‚½”½‰‚ğ‘‚¢‚Ä‚­‚¾‚³‚¢B
				b.isActive = false;
				shakeCount = 3;
			}
		}

		// ‰æ–Ê¼ª²¸
		if (shakeCount > 0)
		{
			shakePos = { shakeRange(mt), shakeRange(mt) };
			shakeCount--;
		}
		else
		{
			shakePos = { 0, 0 };
		}

		//“G‚Ì•\¦
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//“G‚Ì–{‘Ì(“–‚½‚è”»’è)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(shakePos.x, shakePos.y, shakeScreen, true);

		++frame;
		++bulFrame;
		ScreenFlip();
	}

	DxLib_End();

	return 0;
}