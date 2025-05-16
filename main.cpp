
#include "../DirectXGame/math/Vector3.h"
#include "Function.h"
#include <Novice.h>
#include <cstdint>
#include <imgui.h>
#include <corecrt_math.h>
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
	    {0.0f, 0.0f, 0.0f},
        1.0f
    };
	Plane plane = {
	    {0.2f, 0.2f, 0.2f},
        1.0f


    };
	Segment segment = {
	    {-2.0f, -1.0f, 0.0f},
        {3.0f,2.0f,2.0f,}
    };

	Vector3 point{-1.5f, 0.6f, 0.6f};
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);



	Sphere pointSphere = {point, 1.0f};

	Sphere closestPointSphere = {closestPoint, 0.01f};

	Vector3 cameraPosition = {0.0f, 1.9f, -10.0f};
	Vector3 cameraRotate = {0.26f, 0.0f, 0.0f};

	Vector3 v1 = {1.2f, -3.9f, 2.5f};
	Vector3 v2 = {2.8f, 0.4f, -1.3f};
	Vector3 cross = Cross(v1, v2);

	Vector3 rotate{0.0f, 0.0f, 0.0f};
	Vector3 translate = {0.0f, 0.0f, 3.0f};

	Vector3 ScreenVertices[3];



	int color = RED;

	static const Vector3 kLocalVertices[3] = {
	    {0.0f,  0.5f,  0.0f}, // 頂点1
	    {0.5f,  -0.5f, 0.0f}, // 頂点2
	    {-0.5f, -0.5f, 0.0f}  // 頂点3
	};
	// 前回のマウス位置を保持する変数
	static int prevMouseX = 0;
	static int prevMouseY = 0;

	// 更新処理部分
	// マウスの移動量を計算
	int mouseX = 0;
	int mouseY = 0;
	int mouseMoveX = 0;
	int mouseMoveY = 0;
	
	// マウスの移動量に基づいてカメラの回転角度を変更
	const float sensitivity = 0.01f; // 感度を調整
		
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
		Novice::GetMousePosition(&mouseX, &mouseY);
		if (keys[DIK_SPACE]) {

			if (Novice::IsPressMouse(0) == 1) {
				mouseMoveX = mouseX - prevMouseX;
			} else {
				mouseMoveX = 0;
			}
			if (Novice::IsPressMouse(1) == 1) {
				mouseMoveY = mouseY - prevMouseY;
			} else {
				mouseMoveY = 0;
			}
		}
		VectorScreenPrintf(0, 0, cross, "cross");
		cameraRotate.y += mouseMoveX * sensitivity; // Y軸回転を更新
		cameraRotate.x += mouseMoveY * sensitivity; // Y軸回転を更新


		Matrix4x4 worldMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePrespectiveMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 WorldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 1.0f);

		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], WorldViewProjectionMatrix);
			ScreenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}
		Vector3 localMove = {0.0f, 0.0f, 0.0f};
		if (Novice::CheckHitKey(DIK_W))
			localMove.z += 1.0f; // 前
		if (Novice::CheckHitKey(DIK_S))
			localMove.z -= 1.0f; // 後
		if (Novice::CheckHitKey(DIK_A))
			localMove.x -= 1.0f; // 左
		if (Novice::CheckHitKey(DIK_D))
			localMove.x += 1.0f; // 右

		// 正規化（斜め移動調整）
		float len = sqrtf(localMove.x * localMove.x + localMove.z * localMove.z);
		if (len > 0.0f) {
			localMove.x /= len;
			localMove.z /= len;
		}

		// カメラの向きに合わせたワールド座標変換
		Vector3 forward = {sinf(cameraRotate.y), 0.0f, cosf(cameraRotate.y)};
		Vector3 right = {cosf(cameraRotate.y), 0.0f, -sinf(cameraRotate.y)};

		// カメラ向きに応じて移動
		Vector3 move = {right.x * localMove.x + forward.x * localMove.z, 0.0f, right.z * localMove.x + forward.z * localMove.z};

		// 実際に移動
		cameraPosition.x += move.x * 0.05f;
		cameraPosition.z += move.z * 0.05f;

		Vector3 start = Transform(Transform(segment.origin, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), Multiply(viewMatrix, projectionMatrix)), viewportMatrix);
		plane.normal = Normalize(plane.normal);

		if (IsCollision(segment, plane)) {
			color = RED;
		} else {
			color = WHITE;
		}

		
	
	
		prevMouseX = mouseX;
		prevMouseY = mouseY;

		//if (cameraRotate.x > 1) {
		//	cameraRotate.x = 1;
		//} else if (cameraRotate.x <= -1) {
		//	cameraRotate.x = -1;
		//}
		//if (cameraRotate.y > 1) {
		//	cameraRotate.y = 1;
		//} else if (cameraRotate.y <= -1) {
		//	cameraRotate.y = -1;
		//}
		
		///
		/// ↑更新処理ここまで
		///

		///Multiply
		/// ↓描画処理ここから
		///
		///

		DrawGrid(Multiply(viewMatrix, projectionMatrix), viewportMatrix, cameraPosition);
		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color);
		DrawPlane(plane, Multiply(viewMatrix, projectionMatrix), viewportMatrix, WHITE);
	
		ImGui::Begin("Hello, world!");
		ImGui::DragFloat3("Planee.Normal", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Plaqqne.Normal", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("Planeermal", &cameraPosition.x, 0.01f);
		ImGui::DragFloat3("Plae.Normal", &cameraRotate.x, 0.01f);
	
		ImGui::DragFloat3("pointSphere", &pointSphere.center.x, 0.1f);
		ImGui::DragFloat3("closestPointSphere", &closestPointSphere.center.x, 0.1f);
		ImGui::DragFloat("SphereRadius", &pointSphere.radius, 0.1f);
		ImGui::DragFloat("SphereRadius", &closestPointSphere.radius, 0.1f);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		
		ImGui::End();
		
		Novice::ScreenPrintf(0, 0, "Move : WASD");
		Novice::ScreenPrintf(0, 20, "RGIHT CLICK : LotateY");
		Novice::ScreenPrintf(0, 40, "LEFT  CLICK : LotateZ");



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
