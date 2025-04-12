
#include "../DirectXGame/math/Vector3.h"
#include "Function.h"
#include <Novice.h>
const char kWindowTitle[] = "LD2A_01_クドウユウキ_タイトル";

static const int kColumnWidth = 60;
static const int kRowHeight = 30;



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

		Vector3 translate{4.1f, 2.6f, 0.8f};
		Vector3 scale{1.5f, 5.2f, 7.3f};


		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

		Vector3 point{2.3f, 3.8f,1.4f};

		Matrix4x4 trasformMatrix = {
		// 列0
		 1.0f, 2.0f, 3.0f, 4.0f,
        // 列1
	     3.0f, 1.0f, 1.0f, 2.0f,
	    // 列2
	    1.0f, 4.0f, 2.0f, 3.0f,
		// 列3
		2.0f, 2.0f, 1.0f, 3.0f
		
		};

		Vector3 transformed = Transform(point, trasformMatrix);

		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		VectorScreenPrintf(0, 0, transformed, "transformed");
		MatrixScreenPrintf(0, kRowHeight*1, translateMatrix, "translateMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, scaleMatrix, "scaleMatrix");

		

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