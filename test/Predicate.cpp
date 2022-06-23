#include "Predicate.h"

extern "C" double orient3d(const double * pa, const double * pb, const double * pc, const double * pd);

extern "C" double orient2d(const double * pa, const double * pb, const double * pc);

namespace gl {
	const int Predicates::ORI_POSITIVE;
	const int Predicates::ORI_ZERO;
	const int Predicates::ORI_NEGATIVE;
	const int Predicates::ORI_UNKNOWN;

	int Predicates::Orient3d(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4) {
		const double result = orient3d(p1.data(), p2.data(), p3.data(), p4.data());

		if (result > 0)
			return ORI_POSITIVE;
		else if (result < 0)
			return ORI_NEGATIVE;
		else
			return ORI_ZERO;
	}

	int Predicates::Orient3dTolerance(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p) {
		const double result = orient3d(p1.data(), p2.data(), p3.data(), p.data());
		if (result == 0)
			return ORI_ZERO;

		Vector3 n = ((p2 - p3).cross(p1 - p3)).normalized();
		double d = std::abs(n.dot(p - p1));
		if (d <= DOUBLE_ZERO)
			return Predicates::ORI_ZERO;

		if (result > 0)
			return ORI_POSITIVE;
		else
			return ORI_NEGATIVE;
	}

	double Predicates::Orient3dVolume(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4) {
		const double ori = orient3d(p1.data(), p2.data(), p3.data(), p4.data());
		if (ori <= 0)
			return ori;
		else
			return (p1 - p4).dot((p2 - p4).cross(p3 - p4)) / 6;
	}

	int Predicates::Orient2d(const Vector2& p1, const Vector2& p2, const Vector2& p3) {
		const double result = orient2d(p1.data(), p2.data(), p3.data());
		if (result > 0)
			return ORI_POSITIVE;
		else if (result < 0)
			return ORI_NEGATIVE;
		else
			return ORI_ZERO;
	}

	double Predicates::OrientDis(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p)
	{
		Vector3 n = ((p2 - p3).cross(p1 - p3)).normalized();
		double d = std::abs(n.dot(p - p1));
		return d;
	}
}