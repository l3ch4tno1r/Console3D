#include <iostream>
#include <limits>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

namespace LCN
{
	template<typename T, size_t Dim>
	void ComputeCollision2(const AABB<T, Dim>& aabb, const Line<T, Dim>& line, AABBVSLine<T, Dim>& result)
	{
		const auto& origin    = line.Origin();
		const auto& direction = line.Direction();
		const auto& min       = aabb.Min();
		const auto& max       = aabb.Max();

		T tmaxmin = -std::numeric_limits<T>::infinity();
		T tminmax =  std::numeric_limits<T>::infinity();

		for (size_t i = 0; i < Dim; ++i)
		{
			if (std::abs(direction[i]) < FUZZ_FACTOR &&
				min[i] <= origin[i] && origin[i] <= max[i])
				continue;

			T t1 = (min[i] - origin[i]) / direction[i];
			T t2 = (max[i] - origin[i]) / direction[i];

			T tmin = std::min(t1, t2);
			T tmax = std::max(t1, t2);

			tmaxmin = std::max(tmaxmin, tmin);
			tminmax = std::min(tminmax, tmax);
		}

		if (!(result.m_Collide = (tmaxmin < tminmax)))
			return;

		result.m_Intersections[0].Distance = tmaxmin;
		result.m_Intersections[1].Distance = tminmax;

		result.m_Intersections[0].Point = tmaxmin * direction + origin;
		result.m_Intersections[1].Point = tminmax * direction + origin;
	}
}

int main()
{
	LCN::AABB2Df aabb{ { 4, 2 }, { 10,  6 } };
	LCN::Line2Df lines[] = {
		{ { -1,  7 }, {  2, -1 } }, // Collision
		{ {  0,  4 }, {  1,  0 } }, // Collision
		{ { 14,  5 }, { -3, -1 } }, // Collision
		{ { 11,  0 }, {  0,  1 } }, // No Collision
		{ {  5, -1 }, {  1,  4 } }  // Collision
	};
	
	for (const auto& line : lines)
	{
		LCN::AABBVSLine2Df result1;

		LCN::ComputeCollision(aabb, line, result1);

		std::cout << result1 << ", " << result1[0].Distance << ", " << result1[1].Distance << std::endl;

		LCN::AABBVSLine2Df result2;

		LCN::ComputeCollision2(aabb, line, result2);

		std::cout << result2 << ", " << result2[0].Distance << ", " << result2[1].Distance << std::endl;

		std::cout << "----------------------" << std::endl;
	}

	std::cin.get();
}