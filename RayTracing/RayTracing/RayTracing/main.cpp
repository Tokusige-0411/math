#include<DxLib.h>
#include<cmath>
#include<iostream>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;
const float albedo[3] = { 1.0f, 0.7f, 0.7f };

// 反射ベクトルを返す
Vector3 ReflectVector(const Vector3& invec, const Vector3& normal)
{
	// 反射ベクトルの式
	// R = I - 2*(N・I)N
	return (invec - normal * (2 * Dot(normal, invec)));
}


//ヒントになると思って、色々と関数を用意しておりますが
//別にこの関数を使わなければいけないわけでも、これに沿わなければいけないわけでも
//ありません。レイトレーシングができていれば構いません。

using Color = Vector3;

// 最大値最小値を求める
float Clamp(float in, const float maxValue = 1.0f, const float minValue = 0.0f)
{
	return max(min(maxValue, in), minValue);
}

Color Clamp(const Color& value, const float maxValue = 255.0f, const float minValue = 0.0f)
{
	return Color(Clamp(value.x, maxValue, minValue), Clamp(value.y, maxValue, minValue), Clamp(value.z, maxValue, minValue));
}

UINT32 GetUint32ColorFromVectorColor(const Color& col)
{
	return GetColor(col.x, col.y, col.z);
}

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
		return true;
	}
	else
	{
		return false;
	}
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Sphere& sphere, const Position3& light) {

	Position3 screenPos;
	Vector3 ray;
	// 光源から球へのベクトル
	Vector3 lightVec = sphere.pos - light;
	lightVec.Normalize();
	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向

			//①視点とスクリーン座標から視線ベクトルを作る
			screenPos = { static_cast<float>(x - (screen_width / 2)), static_cast<float>((screen_height / 2) - y), 0.0f };
			ray = screenPos - eye;
			//②正規化しとく
			ray.Normalize();
			float t = 0;
			//③IsHitRay関数がTrueだったら白く塗りつぶす
			if (IsHitRayAndObject(eye, ray, lightVec, sphere, t))
			{
				// tから交点Pを求める
				auto p = eye + (ray * t);
				// 球体の中心から交点Pへの法線ベクトルを求める
				auto n = (p - sphere.pos).Normalized();

				// 法線ベクトルと光線ベクトルの内積を求め明るさとして扱う
				auto diffuseB = Dot(-lightVec, n);
				diffuseB = Clamp(diffuseB);


				// ライト反射ベクトル
				auto r = ReflectVector(lightVec, n);
				// ライト反射ベクトルと視線逆ベクトルの内積を取りpow関数でn乗する
				auto specular = pow(Clamp(Dot(r, -ray)), 20);

				Color difColor(255, 128, 128);
				Color specColor(255, 255, 255);

				difColor *= diffuseB;
				specColor *= specular;

				DrawPixel(x, y, GetUint32ColorFromVectorColor(Clamp(difColor + specColor)));
			}
			else
			{
				// 仮に平法線ベクトルを(0, 1, 0)とする
				// 平面に当たる条件は視線と法線ベクトルが90度以上
				//※塗りつぶしはDrawPixelという関数を使う。
				Plane plane(Vector3(0, 1, 0), 100);
				auto n = Vector3(0.0f, 1.0f, 0.0f).Normalized();
				auto d = Dot(n, ray);
				if (d < 0.0f)
				{
					//地面に当たっている
					//平面の交点P=eye+ray*t
					//t=w/u(wは平面からの距離)
					//平面に近づく大きさ
					//w = eye・N, u = -ray・N
					//t=w/uとしたいが、平面にはoffset(d)がある
					//t = (w+d)/u
					//あとはここから交点Pを求める
					//多分tは４万ぐらい
					auto w = Dot(eye, plane.N);
					auto u = Dot(-ray, plane.N);
					auto T = (w + plane.d) / u;
					auto P = eye + ray * T;
					Color col(255, 255, 255);
					col *= max(1.0, -Clamp(T / 1000.0f));
					auto checker = ((int)(P.x / 20) + (int)(P.z / 20)) % 2 == 0 ? 1 : 0;
					checker += P.x < 0 ? 1 : 0;
					checker += P.z < 0 ? 1 : 0;
					if (checker % 2 == 0)
					{
						DrawPixel(x, y, GetUint32ColorFromVectorColor(col));
					}
				}
			}
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
	auto light = Vector3(-100, 200, 200);
	while (ProcessMessage() != -1)
	{
		ClsDrawScreen();
		RayTracing(eye, sphere, light);
		ScreenFlip();
	}

	WaitKey();
	DxLib_End();
}