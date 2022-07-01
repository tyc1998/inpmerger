#ifndef GL_MESH_H
#define GL_MESH_H

#include "Types.h"
#include "Predicate.h"
#include "Algo.h"
#include <vector>
#include <array>
#include <unordered_set>

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

		int FindBoundaryFace(std::vector<Vector3i>& boundary_faces) {
			std::vector<std::array<int, 2>> edges;
			std::vector<std::vector<int>> vertex_conn_tet(tet_vertex_.size(),std::vector<int>());
			std::vector<std::array<int,3>> triangles;
			for (int i = 0; i < tets_.size(); ++i) {
				if (tets_[i].is_removed_ == true)
					continue;
				for (int j = 0; j < 4; ++j) {
					vertex_conn_tet[tets_[i][j]].emplace_back(i);
					for (int k = j + 1; k < 4; ++k) {
						std::array<int, 2> edge{ tets_[i][j], tets_[i][k] };
						if (edge[0] > edge[1])
							std::swap(edge[0], edge[1]);
						edges.emplace_back(std::move(edge));
						for (int l = k + 1; l < 4; ++l) {
							std::array<int, 3> triangle{ tets_[i][j], tets_[i][k],tets_[i][l] };
							std::sort(triangle.begin(), triangle.end(), std::less<int>());
							triangles.emplace_back(std::move(triangle));
						}
					}
				}
			}
			VectorUnique(edges);
			//VectorUnique(triangles);

			std::vector<std::array<int, 2>> boundary_edges;
			for (int i = 0; i < edges.size(); ++i) {
				if (CountVectorIntersection(vertex_conn_tet[edges[i][0]], vertex_conn_tet[edges[i][1]]) == 2)
					boundary_edges.emplace_back(edges[i]);
			}

			//for (int i = 0; i < triangles.size(); ++i) {
			//	bool flag = true;
			//	for (int j = 0; j < 3; ++j) {
			//		int cnt = CountVectorIntersection(vertex_conn_tet[triangles[i][j]], vertex_conn_tet[triangles[i][(j + 1) % 3]]);
			//		//if (cnt <= 2)
			//		//	printf("%d\n", cnt);
			//		if (cnt != 2) {
			//			flag = false;
			//			break;
			//		}
			//	}
			//	if (flag)
			//		boundary_faces.emplace_back(Vector3i(triangles[i][0], triangles[i][1], triangles[i][2]));
			//}

			std::unordered_set<std::array<int, 3>> us;
			for (int i = 0; i < triangles.size(); ++i) {
				std::unordered_set<std::array<int, 3>>::iterator it = us.find(triangles[i]);
				if (it == us.end())
					us.insert(triangles[i]);
				else
					us.erase(it);
			}
			boundary_faces.reserve(us.size());
			for (auto it = us.begin(); it != us.end(); ++it) {
				boundary_faces.emplace_back(Vector3i(it->data()[0], it->data()[1], it->data()[2]));
			}

			return boundary_faces.size();
		}

		void DeleteDanglingNode() {
			std::vector<bool> is_in_tet(tet_vertex_.size(), false);
			for (MeshTet tet : tets_) {
				for (int i = 0; i < 4; ++i)
					is_in_tet[tet[i]] = true;
			}
			std::vector<int> old_2_new_vertex(is_in_tet.size(), -1);
			{
				int idx = 0;
				for (int i = 0; i < is_in_tet.size(); ++i) {
					if (is_in_tet[i])
						old_2_new_vertex[i] = idx++;
				}
			}
			
			std::vector<MeshVertex> new_tet_vertex;
			new_tet_vertex.reserve(tet_vertex_.size());
			for (int i = 0; i < tet_vertex_.size(); ++i) {
				if (is_in_tet[i])
					new_tet_vertex.emplace_back(std::move(tet_vertex_[i]));
			}
			tet_vertex_ = std::move(new_tet_vertex);

			for (MeshTet& tet : tets_) {
				if (tet.is_removed_)
					continue;
				for (int i = 0; i < 4; ++i)
					tet[i] = old_2_new_vertex[tet[i]];
			}
		}
	};
}

#endif