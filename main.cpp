#include <Novice.h>
#include <MyMath.h>
#include <imgui.h>

const char kWindowTitle[] = "LE2A_08_スヤマハナ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Plane plane{ {0,1,0}, 1};

	Sphere sphere1{ {0,0,0}, 2 };

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Vector3 rotate{};
	Vector3 translate{};

	float kWindowWidth = 1280;
	float kWindowHeight = 720;

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
		
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		ImGui::Begin("Sphere1");
		ImGui::SliderFloat3("Center", reinterpret_cast<float*>(&sphere1.center), -2, 2);
		ImGui::SliderFloat("Radius", reinterpret_cast<float*>(&sphere1.radius), 0, 2);
		ImGui::End();

		ImGui::Begin("plane");
		ImGui::SliderFloat3("Center", reinterpret_cast<float*>(&plane.normal), -2, 2);
		ImGui::SliderFloat("Distance", reinterpret_cast<float*>(&plane.distsnce), 0, 2);
		ImGui::End();
		plane.normal = Normalize(plane.normal);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, WHITE);

		if (IsCollision(sphere1, plane)) {
			DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, RED);
		} else {
			DrawSphere(sphere1, worldViewProjectionMatrix, viewportMatrix, WHITE);
		}

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
