#include "InpIO.h"

namespace gl {
	int InpIO::Import(const std::vector<std::string>& inps, Mesh& mesh)
	{
		int ret = 0;

		char buffer[1024];
		int state = 0;
		int offset = -1;
		int idx;
		for (auto& inp : inps) {
			FILE* p = fopen(inp.c_str(), "r");

			while (!feof(p)) {
				fgets(buffer, sizeof(buffer), p);
				if (strncmp(buffer, "*NODE", 5) == 0) {
					state = 1;
					continue;
				}
				else if (strncmp(buffer, "*ELEMENT,TYPE=C3D4", 8) == 0) {
					state = 2;
					++offset;
					continue;
				}
				else if (strncmp(buffer, "*****", 5) == 0)
					break;
				if (state == 1) {
					double a, b, c;
					sscanf(buffer, "%d,%lf,%lf,%lf", &idx, &a, &b, &c);
					mesh.tet_vertex_.emplace_back(gl::Vector3(a, b, c));
				}
				else if (state == 2) {
					int a, b, c, d;
					sscanf(buffer, "%d,%d,%d,%d,%d", &idx, &a, &b, &c, &d);
					mesh.tets_.emplace_back(MeshTet(Vector4i(a, b, c, d), offset));
				}
			}
			++ret;
		}

		return ret;
	}

	int InpIO::OutPortInp(const std::string path, Mesh& mesh)
	{
		mesh.DeleteDanglingNode();
		FILE* p = fopen(path.c_str(), "w");

		fprintf(p, "*NODE\n");
		int idx = 0;
		for (int i = 0; i < mesh.tet_vertex_.size(); ++i)
			fprintf(p, "%d,%lf,%lf,%lf\n", ++idx, mesh.tet_vertex_[i][0], mesh.tet_vertex_[i][1], mesh.tet_vertex_[i][2]);

		sort(mesh.tets_.begin(), mesh.tets_.end(), [](const MeshTet& lhs, const MeshTet& rhs) {
			return lhs.comp_ < rhs.comp_;
			});
		int offset = 0;
		for (int i = 0; i < mesh.tets_.size(); ++i) {
			int comp = mesh.tets_[i].comp_;
			fprintf(p, "*ELEMENT,TYPE=C3D4,ELSET=%d\n", comp);
			while (i < mesh.tets_.size() && mesh.tets_[i].comp_ == comp) {
				if (!mesh.tets_[i].is_removed_)
					fprintf(p, "%d,%d,%d,%d,%d\n", ++idx,
						mesh.tets_[i][0] + 1, mesh.tets_[i][1] + 1,
						mesh.tets_[i][2] + 1, mesh.tets_[i][3] + 1);
				++i;
			}
		}

		fprintf(p, "*****");
		fclose(p);

		return idx;
	}

	int InpIO::OutPortMesh(const std::string path, Mesh& mesh) {
		FILE* p = fopen(path.c_str(), "w");

		fprintf(p, "MeshVersionFormatted 1\nDimension 3\n");
		fprintf(p, "Vertices\n%u\n", mesh.tet_vertex_.size());
		for (int i = 0; i < mesh.tet_vertex_.size(); ++i)
			fprintf(p, "%lf %lf %lf 1\n", mesh.tet_vertex_[i][0], mesh.tet_vertex_[i][1], mesh.tet_vertex_[i][2]);

		std::vector<Vector3i> boundary_face;
		mesh.FindBoundaryFace(boundary_face);
		fprintf(p, "Triangles\n%u\n", boundary_face.size());
		for (int i = 0; i < boundary_face.size(); ++i)
			fprintf(p, "%d %d %d 1\n", boundary_face[i][0], boundary_face[i][1], boundary_face[i][2]);

		fprintf(p, "Tetrahedra\n%u\n", mesh.tets_.size());
		for (int i = 0; i < mesh.tets_.size(); ++i) {
			fprintf(p, "%d %d %d %d %d\n", mesh.tets_[i][0] + 1, mesh.tets_[i][1] + 1, mesh.tets_[i][2] + 1,
				mesh.tets_[i][3] + 1, 1);
		}

		fprintf(p, "End\n");
		fclose(p);

		return mesh.tets_.size();
	}
}