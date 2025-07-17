#include "../DirectXGame/math/Vector3.h"
#include "Function.h"
#include <Novice.h>
#include <algorithm> // Ensure this header is included for std::max
#include <corecrt_math.h>
#include <cstdint>
#include <imgui.h>

const char kWindowTitle[] = "LD2A_01_クドウユウキ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	static const int kColumnWidth = 60;
	static const int kRowHeight = 30;

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

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
	    {-0.7f, 0.3f,  0.0f},
        {2.0f,  -0.5f, 0.0f}
    };

	Triangle triangle = {
	    {{-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}
    };

	AABB aabb1 = {
	    {-0.5f, -0.5f, -0.5f},
        {0.5f,  0.5f,  0.5f }
    };

	Spring spring = {
		{0.0f, 0.0f, 0.0f}, 
		{1.0f}, 
		{100},
        {2.0f},
	};

	Ball ball = {
	    {1.2f, 0.0f, 0.0f}, // position
	    {0.0f, 0.0f, 0.0f}, // velocity ← 初期は 0 でもOK
	    {0.0f, 0.0f, 0.0f}, // acceleration
	    2.0f, // mass
	    0.2f, // radius ← この値が小さすぎても描画で見えません
	    BLUE, // color
	};
	bool springActive = false; // Startボタンを押したらtrueになる

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
	Vector3 translate = {0.0f, 0.0f, 3.0f};

	Vector3 ScreenVertices[3];

	Vector3 controlPoint[3] = {
	    {0.2f, 0.5f, 0.0f}, // 肩
	    {0.4f, 0.0f, 0.0f}, // ひじ
	    {0.3f, -0.5f, 0.0f}, // 手
	};

	Sphere SpherecontrolPoint[3] = {
	    {
         {controlPoint[0].x, controlPoint[0].y, controlPoint[0].z},
         0.01f, },
	    {
         {controlPoint[1].x, controlPoint[1].y, controlPoint[1].z},
         0.01f, },
	    {
         {controlPoint[2].x, controlPoint[2].y, controlPoint[2].z},
         0.01f, },
	};

	// 各関節のローカル平行移動（親基準）
	Vector3 translates[3] = {
	    {0.2f, 0.5f, 0.0f}, // 肩
	    {0.4f, 0.0f, 0.0f}, // ひじ
	    {0.3f, 0.0f, 0.0f}, // 手
	};

	// 各関節のローカル回転（ラジアン）
	Vector3 rotates[3] = {
	    {0.0f, 0.0f, -6.8f},
	    {0.0f, 0.0f, -1.4f},
	    {0.0f, 0.0f, 0.0f },
	};

	// 各関節のスケール（基本1）
	Vector3 scales[3] = {
	    {1.0f, 1.0f, 1.0f},
	    {1.0f, 1.0f, 1.0f},
	    {1.0f, 1.0f, 1.0f},
	};


	Vector3 a{0.2f, 1.0f, 0.0f};
	Vector3 b{2.4f, 3.1f, 1.2f};
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;

	Vector3 rotate{0.4f, 1.43f, -0.8f};
	float deltaTime = 1.0f / 60.0f;
	
	

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
		cameraRotate.x += mouseMoveY * sensitivity; // X軸回転を更新

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

		if (isCollision(aabb1, segment)) {
			color = RED;
		} else {
			color = WHITE;
		}

		for (int i = 0; i < 3; ++i) {
			SpherecontrolPoint[i] = {
			    {controlPoint[i].x, controlPoint[i].y, controlPoint[i].z},
			    0.11f,
			};
		}

		// 階層的なローカルマトリクスを作成してワールド行列を計算
		Matrix4x4 shoulderLocal = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		Matrix4x4 elbowLocal = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		Matrix4x4 handLocal = MakeAffineMatrix(scales[2], rotates[2], translates[2]);

		Matrix4x4 shoulderWorld = shoulderLocal;                    // 肩はワールド基準
		Matrix4x4 elbowWorld = Multiply(elbowLocal, shoulderWorld); // ひじは肩の子
		Matrix4x4 handWorld = Multiply(handLocal, elbowWorld);      // 手はひじの子

		// ワールド行列で球の位置を更新
		SpherecontrolPoint[0].center = Transform(controlPoint[0], shoulderWorld);
		SpherecontrolPoint[1].center = Transform(controlPoint[1], elbowWorld);
		SpherecontrolPoint[2].center = Transform(controlPoint[2], handWorld);

		prevMouseX = mouseX;
		prevMouseY = mouseY;

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

		if (springActive) {
			Vector3 diff = ball.position - spring.position;
			float length = Length(diff);
			if (length != 0.0f) {
				Vector3 direction = Normalize(diff);
				Vector3 restPosition = spring.position + direction * spring.naturalLength;
				Vector3 displacement = ball.position - restPosition;
				Vector3 restoringForce = -spring.stiffness * displacement;
				Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
				Vector3 totalForce = restoringForce + dampingForce;

				ball.acceleration = totalForce / ball.mass;
			}
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;
		}

		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
	

		float angularVelocity = 0.0f;
		float angle = 0.0f;

		angle += angularVelocity * deltaTime;
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(Multiply(viewMatrix, projectionMatrix), viewportMatrix, cameraPosition);
		DrawSphere({ball.position, ball.radius}, Multiply(viewMatrix, projectionMatrix), viewportMatrix, ball.color);
		// 原点をワールド→スクリーン変換
		Vector3 screenOrigin = Transform(Transform({0.0f, 0.0f, 0.0f}, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);

		// 球の中心もワールド→スクリーン変換
		Vector3 screenBall = Transform(Transform(ball.position, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);

		// 線を描画（原点 → ボール位置）
		Novice::DrawLine(int(screenOrigin.x), int(screenOrigin.y), int(screenBall.x), int(screenBall.y), BLACK);
		ImGui::Begin("Window");

		if (ImGui::Button("Start")) {
			springActive = true;
		}
		ImGui::End();


		Novice::ScreenPrintf(0, 0, "Move : WASD");
		Novice::ScreenPrintf(0, 20, "SPACE & RGIHT CLICK : LotateY");
		Novice::ScreenPrintf(0, 40, "SPACE & LEFT  CLICK : LotateZ");

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
