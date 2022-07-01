#ifndef GL_GEOMALGO_H
#define GL_GEOMALGO_H

#include <CGAL/Tetrahedron_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include "Mesh.h"

namespace gl {
	template<bool Exact>
	bool IsOverlap(const MeshTet& lhs, const MeshTet& rhs) {
		typedef std::conditional<Exact, CGAL::Exact_predicates_exact_constructions_kernel,
			CGAL::Exact_predicates_inexact_constructions_kernel> K;
		typedef CGAL::Tetrahedron_3<K> Tet;

		//TODO
		return true;
	}
}

#endif