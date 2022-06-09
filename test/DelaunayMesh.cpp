#include "DelaunayMesh.h"


namespace gl {
	void DelaunayMesh::AddInp(const std::vector<std::string>& pathes)
	{
		inpio_.AddInp(pathes);
	}

	void DelaunayMesh::AddInp(const std::string& path)
	{
		inpio_.AddInp(path);
	}

	bool DelaunayMesh::Mesh() {
		inpio_.Import();
		
		std::vector<floatTetWild::Vector3>& pos = inpio_.GetPos();
		//std::sort(pos.begin(), pos.end(), [](const floatTetWild::Vector3& lhs, const floatTetWild::Vector3& rhs) {
		//	return lhs[0] < rhs[0] && lhs[1] < rhs[1] && lhs[2] < rhs[2];
		//	});
		//pos.erase(std::unique(pos.begin(), pos.end()), pos.end());

		std::vector<int> v(pos.size());
		std::iota(v.begin(), v.end(), 0);
		std::vector<Point> points;
		points.reserve(pos.size());
		for (int i = 0; i < pos.size(); ++i) {
			points.emplace_back(Point(pos[i][0], pos[i][1], pos[i][2]));
		}

		CDT cdt;
		cdt.insert(boost::make_zip_iterator(boost::make_tuple(points.begin(), v.begin())),
			boost::make_zip_iterator(boost::make_tuple(points.end(), v.end())));
		assert(cdt.is_valid());
		std::vector<floatTetWild::Vector4i> newtets;
		newtets.reserve(cdt.finite_cell_handles().size());
		for (CDT::Cell_handle c : cdt.finite_cell_handles()) {
			floatTetWild::Vector4i tet;
			for (int i = 0; i < 4; ++i)
				tet[i] = c->vertex(i)->info();
			newtets.emplace_back(tet);
		}
		assert(cdt.finite_cell_handles().size() == newtets.size());

		//pos.resize(cdt.finite_vertex_handles().size());
		//{
		//	int i = 0;
		//	for (CDT::Vertex_handle v : cdt.finite_vertex_handles()) {
		//		pos[i][0] = v->point().x();
		//		pos[i][1] = v->point().y();
		//		pos[i][2] = v->point().z();
		//	}
		//}
		inpio_.ResetInc(std::move(newtets), std::vector<int>{static_cast<int>(newtets.size())});

		return true;
	}

	void DelaunayMesh::Outport(const std::string path)
	{
		inpio_.OutPort(path);
	}
}