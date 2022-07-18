#ifndef GL_GEOMALGO_H
#define GL_GEOMALGO_H

#include <CGAL/Tetrahedron_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <array>
#include <igl/parallel_for.h>
#include <eigen3/Eigen/Dense>
#include <igl/winding_number.h>
#include "Types.h"

namespace gl {
	double AMIPSenergy(const std::array<double, 12>& T);
	double AMIPSenergyAux(const std::array<double, 12>& T);

	void FastWindingNumber(const Eigen::MatrixXd& v, const Eigen::MatrixXi& F, const Eigen::MatrixXd& P,
		Eigen::VectorXd& W);
}

#endif