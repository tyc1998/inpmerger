import meshio

mesh=meshio.read("D:\\2020.12gmsh\\mymesh\\test\\pb3.1.mesh")

mesh.write("./pb3.1.inp")
