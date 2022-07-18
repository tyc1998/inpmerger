#ifndef GL_MESH_H
#define GL_MESH_H

#include "Types.h"
#include "Predicate.h"
#include "Algo.h"
#include "GeomAlgo.h"
#include "StlIO.h"
#include <vector>
#include <array>
#include <unordered_set>
#include <geogram/mesh/mesh.h>

namespace std {
	template<>
	struct hash<std::array<int, 3>> {
		size_t operator()(const std::array<int, 3>& self) const {
			return (long long)self[0] + (long long)self[1] * 3 + (long long)self[0] * 9;
		}
	};
}

namespace gl {
	class MeshVertex {
	public:
		MeshVertex(const Vector3& p) :pos_(p) {}
		MeshVertex() {}

		inline double& operator[](const int index) {
			assert(index >= 0 && index < 3);
			return pos_[index];
		}

		inline double operator[](const int index) const {
			assert(index >= 0 && index < 3);
			return pos_[index];
		}

		Vector3 pos_;
	};

	class MeshTet {
	public:
		MeshTet() {}
		MeshTet(const Vector4i& idx, int comp) : indices_(idx), comp_(comp) {}
		MeshTet(int v0, int v1, int v2, int v3, int comp) : indices_(v0, v1, v2, v3), comp_(comp) {}

		inline int& operator[](const int index) {
			assert(index >= 0 && index < 4);
			return indices_[index];
		}

		inline int operator[](const int index) const {
			assert(index >= 0 && index < 4);
			return indices_[index];
		}

		Vector4i indices_;
		int comp_;
		bool is_removed_ = false;
		double volume_ = 0;
		double mindis_ = 0;
	};

	class Mesh {
	public:
		std::vector<MeshVertex> tet_vertex_;
		std::vector<MeshTet> tets_;

		void CountVolume();

		void CountMinDis();

		int DeleteZero(double v, double dis);

		int FindBoundaryFace(std::vector<Vector3i>& boundary_faces);

		void DeleteDanglingNode();

		void FilterOutside(std::string stl_path);

		double GetQuality(const MeshTet& t);
		double GetQuality(int t_id);
		double GetQuality(const MeshVertex& v0, const MeshVertex& v1, const MeshVertex& v2, const MeshVertex& v3);
		double GetQuality(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3);
	};
}

#endif