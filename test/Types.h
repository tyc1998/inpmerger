#ifndef GL_TYPES_H

#include <eigen3/Eigen/Dense>

namespace gl {

#define DOUBLE_ZERO 1e-8
#define DOUBLE_ZERO_2 1e-16
#define DOUBLE_ZERO_3 1e-24
	typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> MatrixXs;

	typedef Eigen::Matrix<double, 3, 3> Matrix3;

	typedef Eigen::Matrix<double, 3, 1> Vector3;
	typedef Eigen::Matrix<double, 2, 1> Vector2;

	typedef Eigen::Matrix<int, 4, 1> Vector4i;
	typedef Eigen::Matrix<int, 3, 1> Vector3i;
	typedef Eigen::Matrix<int, 2, 1> Vector2i;
}  // namespace gl

#endif