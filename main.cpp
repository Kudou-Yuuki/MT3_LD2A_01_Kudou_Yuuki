
#include "../DirectXGame/math/Vector3.h"
#include "Function.h"
#include <Novice.h>
#include <cstdint>
#include <imgui.h>
const char kWindowTitle[] = "LD2A_01_クドウユウキ_タイトル";

static const int kColumnWidth = 60;
static const int kRowHeight = 30;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Sphere sphere = {
	    {0.0f, 1.0f, 0.0f},
        1.0f
    };

	Vector3 cameraPosition = {0.0f, 0.0f, -10.0f};
	Vector3 cameraRotate = {0.26f, 0.0f, 0.0f};

	Vector3 v1 = {1.2f, -3.9f, 2.5f};
	Vector3 v2 = {2.8f, 0.4f, -1.3f};
	Vector3 cross = Cross(v1, v2);

	Vector3 rotate{0.0f, 0.0f, 0.0f};
	Vector3 translate = {0.0f, 0.0f, 3.0f};

	Vector3 ScreenVertices[3];

	static const Vector3 kLocalVertices[3] = {
	    {0.0f,  0.5f,  0.0f}, // 頂点1
	    {0.5f,  -0.5f, 0.0f}, // 頂点2
	    {-0.5f, -0.5f, 0.0f}  // 頂点3
	};

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

		VectorScreenPrintf(0, 0, cross, "cross");

		Matrix4x4 worldMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePrespectiveMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 WorldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 1.0f);

		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], WorldViewProjectionMatrix);
			ScreenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}
		if (keys[DIK_W]) {
			translate.z -= 0.1f;
		}

		if (keys[DIK_S]) {
			translate.z += 0.1f;
		}
		if (keys[DIK_A]) {
			translate.x += 0.1f;
		}
		if (keys[DIK_D]) {
			translate.x -= 0.1f;
		}

		rotate.y += 0.01f;
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		///

		Novice::DrawTriangle(
		    int(ScreenVertices[0].x), int(ScreenVertices[0].y), int(ScreenVertices[1].x), int(ScreenVertices[1].y), int(ScreenVertices[2].x), int(ScreenVertices[2].y), RED, kFillModeSolid);


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