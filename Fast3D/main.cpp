#include <iostream>
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
	Object Cube(cube);

	vector<Console3D::Polygon> torus = generateTorus(1.5f, 0.33f, 36, 36);
	vector<Console3D::Polygon> sphere = generateSphere(0.5f, 36, 36);
	Scene MScene({ torus, sphere, cube });

	Camera MCamera(Point(0.f, 0.f, -3.5f), Vector(0.f, 0.f, 1.f), ViewPort());

	Screen MScreen;
	
	Matrix mat, matx, maty, matz;
	Matrix::CreateRotateMatrix(mat, Vector(300.f, 1000.f, 650.f).Normalize(), 3.f);
	Matrix::CreateRotateMatrix(matx, Vector(0.f, 1.f, 0.f), 4.f);
	Matrix::CreateRotateMatrix(maty, Vector(1.f, 0.f, 0.f), 2.f);
	Matrix::CreateRotateMatrix(matz, Vector(1.f, 0.33f, 1.f).Normalized(), 2.f);

	Render render(MScene, MCamera);
	//render.settings.windingOrder = Render::WindingOrder::CounterClockwise;
	while (true) {
		MScreen.SetScreenNow();

		render.scene.data[0].Rotation(mat);

		render.scene.data[1].Rotation(matx);
		render.scene.data[1].Rotation(matz);

		render.scene.data[2].Rotation(maty);
		render.scene.data[2].Rotation(matz);

		render.Start(MScreen.Buffers[1 - MScreen.ActiveBuffer]);
		MScreen.SwapBuffers();

		Sleep(17);
	}
}