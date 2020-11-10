#include<DxLib.h>
#include<cmath>
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
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Vector3& light,const Sphere& sp,float& t) {
	//レイが正規化済みである前提で…
	//
	//視点から球体中心へのベクトル(視線)を作ります
	Vector3 centerVec = sp.pos - eye;
	//
	//中心から視線への内積をとります＝＞ベクトル長
	auto dot = Dot(centerVec, ray);
	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	//このtmpがR(C・R)となる
	auto V = centerVec - ray * dot;
	if (V.Magnitude() <= sp.radius)
	{
		// 表面の交点tを求める
		auto w = sqrt((sp.radius * sp.radius) - (V.Magnitude() * V.Magnitude()));
		t = dot - w;
		// 交点tの法線ベクトルを求める

		return true;
	}
	else
	{
		return false;
	}
}

// 最大値最小値を求める
float Clamp(float in, const float maxValue = 1.0f, const float minValue = 0.0f)
{
	return max(min(maxValue, in), minValue);
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Sphere& sphere, const Position3& light) {

	Position3 screenPos;
	Vector3 ray;
	// 光源から球へのベクトル
	Vector3 lightVec = light - sphere.pos;
	lightVec.Normalize();
	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向
			//�@視点とスクリーン座標から視線ベクトルを作る
			screenPos = { static_cast<float>(x - (screen_width / 2)), static_cast<float>((screen_height / 2) - y), 0.0f };
			ray = screenPos - eye;
			//�A正規化しとく
			ray.Normalize();
			float t = 0;
			//�BIsHitRay関数がTrueだったら白く塗りつぶす
			if (IsHitRayAndObject(eye, ray, lightVec, sphere, t))
			{
				// tから交点Pを求める
				auto p = eye + (ray * t);
				// 球体の中心から交点Pへの法線ベクトルを求める
				auto n = (p - sphere.pos).Normalized();
				// 法線ベクトルと交戦ベクトルの内積を求め明るさとして扱う
				auto d = Dot(lightVec, n);
				d = Clamp(d);
				DrawPixel(x, y, GetColor(255 * d, 255 * d, 255 * d));
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
	auto light = Vector3(100, 500, 200);
	while (ProcessMessage() != -1)
	{
		ClsDrawScreen();
		RayTracing(eye, sphere, light);
		ScreenFlip();
	}

	WaitKey();
	DxLib_End();
}