#ifndef GL_DELAUNAY_MESH_H
#define GL_DELAUNAY_MESH_H

#include "InpIO.h"
#include "Mesh.h"
#include "Predicate.h"

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Delaunay_triangulation_cell_base_3.h>
#include <boost/iterator/zip_iterator.hpp>

namespace gl {
	class DelaunayMesh {
	private:
		typedef CGAL::Exact_predicates_exact_constructions_kernel K;

		// Vertex base
		typedef CGAL::Triangulation_vertex_base_3<K> Vb;
		typedef CGAL::Triangulation_vertex_base_with_info_3<int, K, Vb> VbI;

		// Cell base
		typedef CGAL::Triangulation_cell_base_3<K> Cb;

		typedef CGAL::Triangulation_cell_base_with_info_3<int, K, Cb> CbI;

		// data structure
		typedef CGAL::Triangulation_data_structure_3<VbI, CbI> cdTds;

		typedef CGAL::Delaunay_triangulation_3<K, cdTds> CDT;
		typedef CGAL::Triangulation_3<K> Triangulation;

		typedef CDT::Point_3 Point;

	public:
		DelaunayMesh(){}

		void AddInp(const std::vector<std::string>& pathes);

		void AddInp(const std::string& path);

		bool ReMesh();

		void OutportInp(const std::string path);

		void OutportMesh(const std::string path);

		int DeleteZero();

	private:
		Mesh mesh_;
	};
}


#endif