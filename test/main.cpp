#include "DelaunayMesh.h"
#include "InpIO.h"

int main() {
	gl::DelaunayMesh mesh;
	const std::vector<std::string> pathes{ "D:\\2020.12gmsh\\mymesh\\test\\pb1.inp" ,"D:\\2020.12gmsh\\mymesh\\test\\pb2.inp" };
	mesh.AddInp(pathes);
    //mesh.AddInp(
    //        "D:\\2020.12gmsh\\mymesh\\mesh-yc-master\\mesh-yc-"
    //        "master\\fTetWild\\build\\Release\\6.1.inp");
	mesh.ReMesh();
	mesh.OutportInp("D:\\2020.12gmsh\\mymesh\\test\\pb3.inp");
	//mesh.OutportMesh("D:\\2020.12gmsh\\mymesh\\test\\pb3.mesh");
    //mesh.Outport("D:\\2020.12gmsh\\mymesh\\test\\6.2.1.inp");

	return 0;
}