#ifndef GL_PREDICATE_H
#define GL_PREDICATE_H

#include "Types.h"


namespace gl {
	class Predicates {
	public:
		static const int ORI_POSITIVE = 1;
		static const int ORI_ZERO = 0;
		static const int ORI_NEGATIVE = -1;
		static const int ORI_UNKNOWN = INT_MAX;

		bool IsDegenerate(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3);

		static int Orient3d(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
		static int Orient3dTolerance(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
		static double Orient3dVolume(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

		static int Orient2d(const Vector2& p1, const Vector2& p2, const Vector2& p3);

		static double OrientDis(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
	};
}

#endif