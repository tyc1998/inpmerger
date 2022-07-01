#include "DelaunayMesh.h"


namespace gl {
	void DelaunayMesh::AddInp(const std::vector<std::string>& pathes)
	{
		InpIO::Import(pathes, mesh_);
	}

	void DelaunayMesh::AddInp(const std::string& path)
	{
		InpIO::Import(std::vector<std::string>{path}, mesh_);
	}

	bool DelaunayMesh::ReMesh() {		
		//std::sort(pos.begin(), pos.end(), [](const gl::Vector3& lhs, const gl::Vector3& rhs) {
		//	return lhs[0] < rhs[0] && lhs[1] < rhs[1] && lhs[2] < rhs[2];
		//	});
		//pos.erase(std::unique(pos.begin(), pos.end()), pos.end());

		std::vector<int> v(mesh_.tet_vertex_.size());
		std::iota(v.begin(), v.end(), 0);
		std::vector<Point> points;
		points.reserve(mesh_.tet_vertex_.size());
		for (int i = 0; i < mesh_.tet_vertex_.size(); ++i) {
			points.emplace_back(Point(mesh_.tet_vertex_[i][0], mesh_.tet_vertex_[i][1], mesh_.tet_vertex_[i][2]));
		}

		CDT cdt;
		cdt.insert(boost::make_zip_iterator(boost::make_tuple(points.begin(), v.begin())),
			boost::make_zip_iterator(boost::make_tuple(points.end(), v.end())));
		assert(cdt.is_valid());
		mesh_.tets_.clear();
		for (CDT::Cell_handle c : cdt.finite_cell_handles()) {
			gl::Vector4i tet;
			for (int i = 0; i < 4; ++i)
				tet[i] = c->vertex(i)->info();
			mesh_.tets_.emplace_back(tet, 1);
		}
		assert(cdt.finite_cell_handles().size() == mesh_.tets_.size());

		printf("success delete %d tets\n", DeleteZero());

		return true;
	}

	int DelaunayMesh::DeleteZero() {
		int ret = 0;
		ret = mesh_.DeleteZero(5e-1, 1e-2);
		return ret;
	}

	void DelaunayMesh::OutportInp(const std::string path)
	{
		InpIO::OutPortInp(path, mesh_);
	}

	void DelaunayMesh::OutportMesh(const std::string path) {
		InpIO::OutPortMesh(path, mesh_);
	}
}