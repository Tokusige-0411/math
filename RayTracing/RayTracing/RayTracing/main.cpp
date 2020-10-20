#include<DxLib.h>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

//ヒントになると思って、色々と関数を用意しておりますが
//別にこの関数を使わなければいけないわけでも、これに沿わなければいけないわけでも
//ありません。レイトレーシングができていれば構いません。

///レイ(光線)と球体の当たり判定
///@param ray (視点からスクリーンピクセルへのベクトル)
///@param sphere 球
///@hint レイは正規化しといたほうが使いやすいだろう
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp) {
	//レイが正規化済みである前提で…
	//
	//視点から球体中心へのベクトル(視線)を作ります
	Vector3 centerVec = sp.pos - eye;
	//
	//中心から視線への内積をとります＝＞ベクトル長
	auto dot = Dot(centerVec, ray);
	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	//このtmpがR(C・R)となる
	auto tmp = ray * dot;
	return ((centerVec - tmp).Magnitude() <= sp.radius);
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Sphere& sphere) {

	Vector3 ray;
	Position3 screenPos;
	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向
			//�@視点とスクリーン座標から視線ベクトルを作る
			screenPos = { static_cast<float>(x - (screen_width / 2)), static_cast<float>(y - (screen_height / 2)), 0.0f };
			ray = screenPos - eye;
			//�A正規化しとく
			ray.Normalize();
			//�BIsHitRay関数がTrueだったら白く塗りつぶす
			if (IsHitRayAndObject(eye, ray, sphere))
			{
				DrawPixel(x, y, 0xffffff);
			}
			//※塗りつぶしはDrawPixelという関数を使う。
		}
	}
}

int WINAPI WinMain(HINSTANCE , HINSTANCE,LPSTR,int ) {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("簡易版のレイトレでっせ"));
	DxLib_Init();

	auto eye = Vector3(0, 0, 300);
	auto sphere = Sphere(100, Position3(0, 0, -100));
	RayTracing(eye, sphere);

	WaitKey();
	DxLib_End();
}