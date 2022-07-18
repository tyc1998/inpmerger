#include "Mesh.h"

namespace gl {
	void Mesh::CountVolume() {
		double p = INT_MAX;
		for (int i = 0; i < tets_.size(); ++i) {
			tets_[i].volume_ = abs(Predicates::Orient3dVolume(tet_vertex_[tets_[i][0]].pos_, tet_vertex_[tets_[i][1]].pos_,
				tet_vertex_[tets_[i][2]].pos_, tet_vertex_[tets_[i][3]].pos_));
			p = std::min(p, tets_[i].volume_);
		}
		printf("min volume is %lf\n", p);
	}

	void Mesh::CountMinDis() {
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

	int Mesh::DeleteZero(double v, double dis) {
		int ret = 0;
		CountVolume();
		CountMinDis();
		for (int i = 0; i < tets_.size(); ++i) {
			if (tets_[i].volume_ < v && tets_[i].mindis_ < dis)
				tets_[i].is_removed_ = true, ++ret;
		}
		return ret;
	}

	int Mesh::FindBoundaryFace(std::vector<Vector3i>& boundary_faces) {
		std::vector<std::array<int, 2>> edges;
		std::vector<std::vector<int>> vertex_conn_tet(tet_vertex_.size(), std::vector<int>());
		std::vector<std::array<int, 3>> triangles;
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

	void Mesh::DeleteDanglingNode() {
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

	void Mesh::FilterOutside(std::string stl_path) {
		std::vector<Vector3> points;
		std::vector<Vector3i> faces;
		GEO::Mesh mesh;
		StlIO::ImPortSTL(stl_path, points, faces, mesh);

		Eigen::MatrixXd C(tets_.size(), 3);
		C.setZero();
		int index = 0;
		for (size_t i = 0; i < tets_.size(); ++i) {
			for (int j = 0; j < 4; ++j)
				C.row(index) += tet_vertex_[tets_[i][j]].pos_.cast<double>();
			C.row(index++) /= 4.0;
		}

		Eigen::Matrix<double, Eigen::Dynamic, 3> V(points.size(), 3);
		Eigen::Matrix<int, Eigen::Dynamic, 3> F(faces.size(), 3);

		for (int i = 0; i < points.size(); ++i)
			V.row(i) = points[i];
		for (int i = 0; i < faces.size(); ++i)
			F.row(i) = faces[i];

		Eigen::VectorXd W;
		FastWindingNumber(Eigen::MatrixXd(V.cast<double>()), Eigen::MatrixXi(F), C, W);

		index = 0;
		int n_tets = 0;
		std::vector<bool> old_flags(tets_.size());
		for (int t_id = 0; t_id < tets_.size(); ++t_id) {
			auto& t = tets_[t_id];
			old_flags[t_id] = t.is_removed_;

			if (W(index) <= 0.5)
				t.is_removed_ = true;
			else
				n_tets++;

			++index;
		}
	}

	double Mesh::GetQuality(const MeshTet& t) {
		std::array<double, 12> T;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				T[i * 3 + j] = tet_vertex_[t[i]].pos_[j];
		}

		return AMIPSenergy(T);
	}

	double Mesh::GetQuality(int t_id) {
		std::array<double, 12> T;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++)
				T[i * 3 + j] = tet_vertex_[tets_[t_id][i]].pos_[j];
		}
		return AMIPSenergy(T);
	}

	double Mesh::GetQuality(const MeshVertex& v0, const MeshVertex& v1, const MeshVertex& v2, const MeshVertex& v3) {
		std::array<double, 12> T = { {v0.pos_[0], v0.pos_[1], v0.pos_[2], v1.pos_[0], v1.pos_[1], v1.pos_[2],
										v2.pos_[0], v2.pos_[1], v2.pos_[2], v3.pos_[0], v3.pos_[1], v3.pos_[2]} };
		return AMIPSenergy(T);
	}

	double Mesh::GetQuality(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3) {
		std::array<double, 12> T = { {v0[0], v0[1], v0[2], v1[0], v1[1], v1[2],
										v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]} };
		return AMIPSenergy(T);
	}
}