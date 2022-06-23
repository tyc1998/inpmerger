import meshio

mesh=meshio.read("D:\\2020.12gmsh\\mymesh\\test\\pb3.inp")

mesh.write("./pb3.mesh")
