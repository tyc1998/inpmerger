#ifndef GL_STLIO_H
#define GL_STLIO_H

#include "Types.h"
#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_repair.h>
#include <geogram/mesh/mesh_reorder.h>
#include <vector>

namespace gl {
	class StlIO {
	public:
		static
			bool ImPortSTL(const std::string& path, std::vector<Vector3>& points, std::vector<Vector3i>& faces,
				GEO::Mesh& input) {
			input.clear(false, false);

			const bool ok = GEO::mesh_load(path, input);

			if (!ok)
				return false;

			if (!input.facets.are_simplices()) {
				mesh_repair(input,
					GEO::MeshRepairMode(GEO::MESH_REPAIR_TRIANGULATE | GEO::MESH_REPAIR_QUIET));
			}

			points.resize(input.vertices.nb());
			for (size_t i = 0; i < points.size(); i++)
				points[i] << (input.vertices.point(i))[0], (input.vertices.point(i))[1],
				(input.vertices.point(i))[2];

			faces.resize(input.facets.nb());
			for (size_t i = 0; i < faces.size(); i++)
				faces[i] << input.facets.vertex(i, 0), input.facets.vertex(i, 1), input.facets.vertex(i, 2);

			return ok;
		}
	};
}

#endif