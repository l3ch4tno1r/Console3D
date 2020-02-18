#ifdef MAIN

#include <iostream>
#include <thread>

#include "Console.h"

#include "Matrix2D\Transform2D.h"
#include "Matrix2D\Vector2D.h"

#include "Utilities\TimeMeasurement.h"

using namespace std;

int main()
{
	Console& console = Console::Get();

	float aspeed = 30.0f;			// 1 tour / min
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0.0f;			// Angle

	float radius = 35;

	const chrono::milliseconds interval = 16ms;

	chrono::high_resolution_clock::time_point start, next;

	//// Model
	Vector2D pt1( 20.0f,  20.0f);
	Vector2D pt2(-20.0f,  20.0f);
	Vector2D pt3(-20.0f, -20.0f);
	Vector2D pt4( 20.0f, -20.0f);
	
	Transform2D transform(60.0f, 40.0f, a);

	std::string msg;

	while (true)
	{
		start = chrono::high_resolution_clock::now();
		next  = start + interval;

		{
			if (GetAsyncKeyState((unsigned short)27) & 0x8000)
				break;

			Timer2 timer(msg);

			console.Clear();

			console.DisplayMessage(msg);

			Vector2D _pt1 = transform.mat * pt1.mat;
			Vector2D _pt2 = transform.mat * pt2.mat;
			Vector2D _pt3 = transform.mat * pt3.mat;
			Vector2D _pt4 = transform.mat * pt4.mat;
			
			console.DrawLine(_pt1.x, _pt1.y, _pt2.x, _pt2.y);
			console.DrawLine(_pt2.x, _pt2.y, _pt3.x, _pt3.y);
			console.DrawLine(_pt3.x, _pt3.y, _pt1.x, _pt1.y);
			console.DrawLine(_pt3.x, _pt3.y, _pt4.x, _pt4.y);
			console.DrawLine(_pt4.x, _pt4.y, _pt1.x, _pt1.y);

			a += aspeed * dt;

			transform.SetRotationAngle(a);

			console.HeartBeat();

			console.Render();
		}

		this_thread::sleep_until(next);
	}

	return 0;
}

#endif // MAIN