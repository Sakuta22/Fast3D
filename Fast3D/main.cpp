#include <iostream>
#include "Render/Render.h"
#include <Windows.h>
#include <vector>
using namespace std;
using namespace Console3D;

int main() {
	vector<Console3D::Polygon> cube = {
		Console3D::Polygon(vector<Vector> {Point(-0.5f, -0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f), Point(0.5f,  0.5f, -0.5f), Point(0.5f, -0.5f, -0.5f)}), //front
		Console3D::Polygon(vector<Vector> {Point(0.5f,  0.5f, -0.5f), Point(0.5f,  0.5f,  0.5f), Point(0.5f, -0.5f,  0.5f), Point(0.5f, -0.5f, -0.5f)}), //right
		Console3D::Polygon(vector<Vector> {Point(-0.5f, -0.5f,  0.5f), Point(0.5f, -0.5f,  0.5f), Point(0.5f,  0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //back
		Console3D::Polygon(vector<Vector> {Point(-0.5f,  0.5f, -0.5f), Point(-0.5f, -0.5f, -0.5f), Point(-0.5f, -0.5f,  0.5f), Point(-0.5f,  0.5f,  0.5f)}), //left
		Console3D::Polygon(vector<Vector> {Point(-0.5f,  0.5f,  0.5f), Point(0.5f,  0.5f,  0.5f), Point(0.5f,  0.5f, -0.5f), Point(-0.5f,  0.5f, -0.5f)}), //top
		Console3D::Polygon(vector<Vector> {Point(-0.5f, -0.5f, -0.5f), Point(0.5f, -0.5f, -0.5f), Point(0.5f, -0.5f,  0.5f), Point(-0.5f, -0.5f,  0.5f)}), //bottom
	};
	Object Cube(cube);

	std::vector<Console3D::Polygon> torus = generateTorus(1.5f, 0.33f, 36, 36);
	vector<Console3D::Polygon> sphereData = generateSphere(0.5f, 36, 36);
	vector<Console3D::Polygon> sphereData1 = generateSphere(0.50f, 6, 6);
	Scene MScene({ torus, sphereData, cube });
	//Scene MScene({Cube});

	Camera MCamera(Point(0.f, 0.f, -6.f), Vector(0.f, 0.f, 1.f), ViewPort());

	//Sleep(4000);
	Matrix mat, matx, maty, matz;
	Vector axis(Point(300.f, 1000.f, 650.f));
	axis.Normalize();
	Matrix::CreateRotateMatrix(mat, axis, 3.f);
	Matrix::CreateRotateMatrix(matx, Vector(0.f, 1.f, 0.f), 4.f);
	Matrix::CreateRotateMatrix(maty, Vector(1.f, 0.f, 0.f), 2.f);
	Matrix::CreateRotateMatrix(matz, Vector(1.f, 0.33f, 1.f).Normalized(), 2.f);
	Screen MScreen;
	Render render(MScene, MCamera);
	//render.settings.RenderInvisiblePolygons = true;
	Render::Settings::RenderInvisiblePolygons = false;
	render.camera.MoveToDiff(0.0f, 0.f, 2.5f);
	int q = 0, qq = 0, w = 0;
	while (true) {
		w++;
		if (qq == 1)
			q--;
		if (qq == 0)
			q++;
		if (q == 20)
			qq = 1;
		if (q == -20)
			qq = 0;

		//if (w % 20 == 0)
		MScreen.SetScreenNow();

		render.scene.data[0].Rotation(mat);
		//render.scene.scene[0].Rotation(matz);
		render.scene.data[1].Rotation(matx);
		render.scene.data[1].Rotation(matz);
		//render.scene.scene[1].Rotation(mat);
		//render.scene.scene[2].Rotation(matx);
		render.scene.data[2].Rotation(maty);
		render.scene.data[2].Rotation(matz);
		//render.camera.MoveToDiff((qq == 0 ? -0.05f : 0.05f), (qq == 1 ? -0.02f : 0.02f), (qq == 1 ? -0.05f : 0.05f));

		render.Start(MScreen.Buffers[1 - MScreen.ActiveBuffer]);
		MScreen.SwapBuffers();

		Sleep(17);
	}
}