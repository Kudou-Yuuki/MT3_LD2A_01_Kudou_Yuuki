#include "../DirectXGame/math/Vector3.h"
#include "Function.h"
#include <Novice.h>
#include <algorithm>
#include <corecrt_math.h>
#include <cstdint>
#include <imgui.h>

const char kWindowTitle[] = "LD2A_01_クドウユウキ_タイトル";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;

	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = {0};
	char preKeys[256] = {0};

	Ball ball = {
	    {1.2f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        2.0f, 0.2f, BLUE,
	};

	Vector3 cameraPosition = {0.0f, 1.9f, -10.0f};
	Vector3 cameraRotate = {0.26f, 0.0f, 0.0f};

	float deltaTime = 1.0f / 60.0f;
	float angle = 0.0f; // 円運動用角度
	bool isActive = false;

	int mouseX = 0, mouseY = 0, prevMouseX = 0, prevMouseY = 0;
	int mouseMoveX = 0, mouseMoveY = 0;
	const float sensitivity = 0.01f;

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);
		Novice::GetMousePosition(&mouseX, &mouseY);

		if (keys[DIK_SPACE]) {
			mouseMoveX = Novice::IsPressMouse(0) ? mouseX - prevMouseX : 0;
			mouseMoveY = Novice::IsPressMouse(1) ? mouseY - prevMouseY : 0;
		}
		cameraRotate.y += mouseMoveX * sensitivity;
		cameraRotate.x += mouseMoveY * sensitivity;

		Matrix4x4 cameraMatrix = MakeAffineMatrix({1, 1, 1}, cameraRotate, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projMatrix = MakePrespectiveMatrix(0.45f, float(kWindowWidth) / kWindowHeight, 0.1f, 100.0f);
		Matrix4x4 vpMatrix = Multiply(viewMatrix, projMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, kWindowWidth, kWindowHeight, 0.0f, 1.0f);

		// カメラ移動
		Vector3 moveInput = {0, 0, 0};
		if (keys[DIK_W])
			moveInput.z += 1.0f;
		if (keys[DIK_S])
			moveInput.z -= 1.0f;
		if (keys[DIK_A])
			moveInput.x -= 1.0f;
		if (keys[DIK_D])
			moveInput.x += 1.0f;
		float len = sqrtf(moveInput.x * moveInput.x + moveInput.z * moveInput.z);
		if (len > 0.0f) {
			moveInput.x /= len;
			moveInput.z /= len;
		}
		Vector3 forward = {sinf(cameraRotate.y), 0.0f, cosf(cameraRotate.y)};
		Vector3 right = {cosf(cameraRotate.y), 0.0f, -sinf(cameraRotate.y)};
		Vector3 camMove = {right.x * moveInput.x + forward.x * moveInput.z, 0.0f, right.z * moveInput.x + forward.z * moveInput.z};
		cameraPosition.x += camMove.x * 0.05f;
		cameraPosition.z += camMove.z * 0.05f;

		// 円運動
		if (isActive) {
			const float radius = 3.0f;
			const float angularSpeed = 1.0f; // rad/s
			angle += angularSpeed * deltaTime;
			ball.position.x = radius * cosf(angle);
			ball.position.y = radius * sinf(angle);
			ball.position.z = 0.0f;
		}

		// 描画
		DrawGrid(vpMatrix, viewportMatrix, cameraPosition);
		DrawSphere({ball.position, ball.radius}, vpMatrix, viewportMatrix, ball.color);

		// 中心線
		Vector3 screenOrigin = Transform(Transform({0, 0, 0}, vpMatrix), viewportMatrix);
		Vector3 screenBall = Transform(Transform(ball.position, vpMatrix), viewportMatrix);
		Novice::DrawLine(int(screenOrigin.x), int(screenOrigin.y), int(screenBall.x), int(screenBall.y), BLACK);

		ImGui::Begin("Window");
		if (ImGui::Button("Start")) {
			isActive = true;
		}
		ImGui::End();

		Novice::ScreenPrintf(0, 0, "Move : WASD");
		Novice::ScreenPrintf(0, 20, "SPACE & RIGHT CLICK : Rotate Y");
		Novice::ScreenPrintf(0, 40, "SPACE & LEFT  CLICK : Rotate X");

		Novice::EndFrame();
		prevMouseX = mouseX;
		prevMouseY = mouseY;


		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
			break;
	}

	Novice::Finalize();
	return 0;
}
