#include <iostream>
#include <iomanip>
#include "Render/Render.h"
#include <Windows.h>
#include <vector>
using namespace std;
using namespace Console3D;

int main() {
	vector<Console3D::Polygon> cube = {
		Console3D::Polygon(vector<Vector> {Point(-0.5f, -0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f), Point( 0.5f,  0.5f, -0.5f), Point( 0.5f, -0.5f, -0.5f)}), //front
		Console3D::Polygon(vector<Vector> {Point( 0.5f,  0.5f, -0.5f), Point( 0.5f,  0.5f,  0.5f), Point( 0.5f, -0.5f,  0.5f), Point( 0.5f, -0.5f, -0.5f)}), //right
		Console3D::Polygon(vector<Vector> {Point(-0.5f, -0.5f,  0.5f), Point( 0.5f, -0.5f,  0.5f), Point( 0.5f,  0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //back
		Console3D::Polygon(vector<Vector> {Point(-0.5f,  0.5f, -0.5f), Point(-0.5f, -0.5f, -0.5f), Point(-0.5f, -0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //left
		Console3D::Polygon(vector<Vector> {Point(-0.5f,  0.5f,  0.5f), Point( 0.5f,  0.5f,  0.5f), Point( 0.5f,  0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f)}), //top
		Console3D::Polygon(vector<Vector> {Point(-0.5f, -0.5f, -0.5f), Point( 0.5f, -0.5f, -0.5f), Point( 0.5f, -0.5f,  0.5f), Point(-0.5f, -0.5f,  0.5f)}), //bottom
	};
	Object Cube(cube, L'*', L'2');
	vector<Console3D::Polygon> torus = generateTorus(1.5f, 0.33f, 36, 36);
	vector<Console3D::Polygon> sphere = generateSphere(0.65f, 36, 36);
	Scene MScene({ {torus, L'*', L'@'}, Cube, {sphere, L'*', L'`'} });

	Camera MCamera(Point(0.f, 0.f, -2.5f), Vector(0.f, 0.f, 1.f), ViewPort());

	Screen MScreen;
	
	Matrix mat, matx, maty, matz;
	Matrix::CreateRotateMatrix(mat, Vector(300.f, 1000.f, 650.f).Normalize(), 2.f);
	Matrix::CreateRotateMatrix(matx, Vector(0.f, 1.f, 0.f), 3.f);
	Matrix::CreateRotateMatrix(maty, Vector(1.f, 0.f, 0.f), 1.f);
	Matrix::CreateRotateMatrix(matz, Vector(1.f, 0.33f, 1.f).Normalized(), 1.f);

	Render render(MScene, MCamera);
	TickSystem tickSystem(1.f / 30.f);
	render.settings.cullMode = Render::CullMode::Back;
	//render.settings.windingOrder = Render::WindingOrder::CounterClockwise;
	float w = 0.f;

	float titletime = 0.f;

	float avgfps = 0.f, cntframe = 0.f;
	tickSystem.Restart();
	while (true) {
		float deltatime = tickSystem.Update();

		avgfps += (cntframe == 0 ? 40 : 1.f / deltatime);
		cntframe++;

		char debugMsg[256];
		sprintf_s(debugMsg, "FPS: %.5f  |  avg: %.5f  |  dt: %.5f\n", (1.0f / deltatime), avgfps / cntframe, deltatime);
		OutputDebugStringA(debugMsg);

		titletime += deltatime;
		if (titletime >= 0.1f) {
			string title = "FPS: " + std::to_string((int)(1.0f / deltatime)) + " | avg: " + std::to_string(avgfps / cntframe) + " | dt: " + std::to_string(deltatime);
			SetConsoleTitleA(title.c_str());
			titletime = 0.f;
		}

		while (tickSystem.NeedATick()) {

			render.scene.data[0].Rotation(mat);
			render.scene.data[1].Rotation(matx);
			render.scene.data[1].Rotation(matz);
			render.scene.data[2].Rotation(maty);
			render.scene.data[2].Rotation(matz);


			render.camera.MoveToDiff(0.f, 0.f, -sinf(w) * cosf(w) / 5.f);
			w += 0.1f;
			render.camera.MoveToDiff(0.f, 0.f, sinf(w) * -cosf(w) / 5.f);
		}
		MScreen.SetScreenNow();
		render.SetScreen({ MScreen.screen, MScreen.width, MScreen.height });
		render.screen.UpdateRatio();
		render.zBuffer.SetZBuffer(&render.screen);

		render.Start();
		MScreen.FillBuffer();
		MScreen.SwapBuffers();
	}
}