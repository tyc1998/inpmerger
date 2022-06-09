#include "DelaunayMesh.h"

int main() {
	gl::DelaunayMesh mesh;
	mesh.AddInp("D:\\2020.12gmsh\\mymesh\\test\\pb1.inp");
	mesh.AddInp("D:\\2020.12gmsh\\mymesh\\test\\pb2.inp");
    //mesh.AddInp(
    //        "D:\\2020.12gmsh\\mymesh\\mesh-yc-master\\mesh-yc-"
    //        "master\\fTetWild\\build\\Release\\6.1.inp");
	mesh.Mesh();
	mesh.Outport("D:\\2020.12gmsh\\mymesh\\test\\pb3.inp");
    //mesh.Outport("D:\\2020.12gmsh\\mymesh\\test\\6.2.1.inp");

	return 0;
}