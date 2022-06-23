#ifndef GL_MESH_H
#define GL_MESH_H

#include "Types.h"
#include "Predicate.h"
#include <vector>

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

		void CountVolume() {
			double p = INT_MAX;
			for (int i = 0; i < tets_.size(); ++i) {
				tets_[i].volume_ = abs(Predicates::Orient3dVolume(tet_vertex_[tets_[i][0]].pos_, tet_vertex_[tets_[i][1]].pos_,
					tet_vertex_[tets_[i][2]].pos_, tet_vertex_[tets_[i][3]].pos_));
				p = std::min(p, tets_[i].volume_);
			}
			printf("min volume is %lf\n", p);
		}

		void CountMinDis() {
			double p = INT_MAX;
			for (int i = 0; i < tets_.size(); ++i) {
				double ret = INT_MAX;
				for (int j = 0; j < 4; ++j) {
					ret = std::min(ret, Predicates::OrientDis(tet_vertex_[tets_[i][j % 4]].pos_, tet_vertex_[tets_[i][(j + 1) % 4]].pos_,
						tet_vertex_[tets_[i][(j + 2) % 4]].pos_, tet_vertex_[tets_[i][(j + 3) % 4]].pos_));
				}
				tets_[i].mindis_ = ret;
				p = std::min(p, ret);
			}
			printf("min dis is %lf\n", p);
		}

		int DeleteZero(double v, double dis) {
			int ret = 0;
			CountVolume();
			CountMinDis();
			for (int i = 0; i < tets_.size(); ++i) {
				if (tets_[i].volume_ < v && tets_[i].mindis_ < dis)
					tets_[i].is_removed_ = true, ++ret;
			}
			return ret;
		}
	};

}

#endif