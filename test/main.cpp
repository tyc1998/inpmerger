#include "DelaunayMesh.h"
#include "InpIO.h"
#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>

int main() {
	GEO::initialize();
	GEO::CmdLine::import_arg_group("standard");
	GEO::CmdLine::import_arg_group("pre");
	GEO::CmdLine::import_arg_group("algo");

	gl::DelaunayMesh mesh;
	const std::vector<std::string> pathes{ "D:\\2020.12gmsh\\mymesh\\test\\baoshijixie\\le.inp" ,
		"D:\\2020.12gmsh\\mymesh\\test\\baoshijixie\\ri.inp" };
	mesh.AddInp(pathes);
    //mesh.AddInp(
    //        "D:\\2020.12gmsh\\mymesh\\mesh-yc-master\\mesh-yc-"
    //        "master\\fTetWild\\build\\Release\\6.1.inp");
	mesh.ReMesh();
	std::string stl_path = "D:\\2020.12gmsh\\mymesh\\test\\baoshijixie\\111.stl";
	mesh.FilterOutside(stl_path);
	mesh.OutportInp("D:\\2020.12gmsh\\mymesh\\test\\baoshijixie\\222.inp");
	//mesh.OutportMesh("D:\\2020.12gmsh\\mymesh\\test\\T\\T.mesh");
	//mesh.OutportNode("D:\\2020.12gmsh\\mymesh\\test\\T\\T.node");
	//mesh.OutportEle("D:\\2020.12gmsh\\mymesh\\test\\T\\T.ele");
    //mesh.Outport("D:\\2020.12gmsh\\mymesh\\test\\6.2.1.inp");

	return 0;
}