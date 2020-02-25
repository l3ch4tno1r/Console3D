#ifdef MAIN

#include <iostream>
#include <thread>

#include "Devices\PaceMaker.h"
#include "Devices\Console.h"

#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry2D\Vector2D.h"
#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry3D\Vector3D.h"

#include "Graphics\OBJReader.h"

#include "Utilities\TimeMeasurement.h"
#include "Utilities\Angles.h"

using namespace std;

int main()
{
	PaceMaker& pacemaker = PaceMaker::Get();
	Console&   console   = Console::Get();

	float aspeed = 30.0f;			// 1 tour / min
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0.0f;			// Angle

	float radius = 35;

	Model3D model = OBJReader().ReadFile("Ressource/cube.obj");

	Transform3D ObjFromR0;

	// Set up Camera transform
	Vector3D tempz = { -5.0f, -7.0f, -2.5f };
	Vector3D tempx = Vector3D::Z() ^ tempz;
	Vector3D tempy = tempz ^ tempx;

	float normx = tempx.mat.Norm();
	float normy = tempy.mat.Norm();
	float normz = tempz.mat.Norm();

	Transform3D CamFromR0;

	CamFromR0.Rux = tempx.x / normx;
	CamFromR0.Ruy = tempx.y / normx;
	CamFromR0.Ruz = tempx.z / normx;
	CamFromR0.Rvx = tempy.x / normy;
	CamFromR0.Rvy = tempy.y / normy;
	CamFromR0.Rvz = tempy.z / normy;
	CamFromR0.Rwx = tempz.x / normz;
	CamFromR0.Rwy = tempz.y / normz;
	CamFromR0.Rwz = tempz.z / normz;
	CamFromR0.Tx  = 5.0f;
	CamFromR0.Ty  = 7.0f;
	CamFromR0.Tz  = 4.0f;

	float focal = 180.0f;

	const float tab[3][4] = {
		focal, 0.0f,  0.0f,  0.0f,
		0.0f,  focal, 0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
	};

	Matrix::StaticMatrix::Matrix<float, 3, 4> Projection(tab);

	Transform2D CamFromImg(90.0f, 60.0f, 180.0f);

	while (true)
	{
		pacemaker.Wait();

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
		{
			pacemaker.Stop();
			break;
		}

		console.Clear();

		Transform3D R0FromCAM   = CamFromR0.mat.Invert();
		Transform3D CubeFromCam = R0FromCAM.mat * ObjFromR0.mat;

		Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = CamFromImg.mat * Projection * CubeFromCam.mat;

		for (const Model3D::Face& face : model.Faces())
		{
			Vector3D _v1    = CubeFromCam.mat * model.Vertices()[face.v1].mat;
			Vector3D _nface = CubeFromCam.mat * model.Normals()[face.vn1].mat;

			if ((_v1 | _nface) > 0)
				continue;

			Vector2D _pt1 = _Proj * model.Vertices()[face.v1].mat;
			Vector2D _pt2 = _Proj * model.Vertices()[face.v2].mat;
			Vector2D _pt3 = _Proj * model.Vertices()[face.v3].mat;

			console.DrawLine(_pt1.PX(), _pt1.PY(), _pt2.PX(), _pt2.PY());
			console.DrawLine(_pt2.PX(), _pt2.PY(), _pt3.PX(), _pt3.PY());
			console.DrawLine(_pt3.PX(), _pt3.PY(), _pt1.PX(), _pt1.PY());
		}
		
		console.HeartBeat();

		console.Render();

		a += aspeed * dt;

		ObjFromR0.Rux =  std::cos(TORAD(a));
		ObjFromR0.Ruy =  std::sin(TORAD(a));
		ObjFromR0.Rvx = -std::sin(TORAD(a));
		ObjFromR0.Rvy =  std::cos(TORAD(a));
	}

	return 0;
}

#endif // MAIN