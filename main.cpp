#include "../External/KamataEngine/include/math/Vector3.h"
#include "Function.h"
#include <Novice.h>
const char kWindowTitle[] = "LD2A_01_クドウユウキ_タイトル";

static const int kColumnWidth = 60;
static const int kColumnHeight = 30;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		Vector3 v1{1.0f, 3.0f, -5.0f};
		Vector3 v2{4.0f, -1.0f, 2.0f};
		float k = {4.0f};

		Vector3 resultAdd = Add(v1, v2);
		Vector3 resultSubtract = Subtract(v1, v2);
		Vector3 resultMultiply = Multiply(k, v1);
		float resultDot = Dot(v1, v2);
		float resultLength = Length(v1);
		Vector3 resultNormalize = Normalize(v2);
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, resultAdd, " : Add");
		VectorScreenPrintf(0, kColumnHeight, resultSubtract, " : Subtract");
		VectorScreenPrintf(0, 2 * kColumnHeight, resultMultiply, " : Multiply");
		Novice::ScreenPrintf(0, 3 * kColumnHeight, "%0.2f : Dot", resultDot);
		Novice::ScreenPrintf(0, 4 * kColumnHeight, "%0.2f : Length", resultLength);
		VectorScreenPrintf(0, 5 * kColumnHeight, resultNormalize, " : Normalize");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
