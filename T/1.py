import meshio

mesh=meshio.read("./T.1.mesh")

mesh.write("./T.1.inp")
