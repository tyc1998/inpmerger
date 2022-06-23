#ifndef GL_INPIO_H
#define GL_INPIO_H

#define _CRT_SECURE_NO_WARNINGS 1

#include <string>
#include <vector>
#include <stdarg.h>
#include "Types.h"
#include "DelaunayMesh.h"
#include "Mesh.h"

namespace gl {
	class InpIO {
	private:
		typedef std::pair<std::string, bool> psb;

	public:
		static int Import(const std::vector<std::string>& inps, Mesh& mesh);

		static int OutPortInp(const std::string path, Mesh& mesh);

		static int OutPortMesh(const std::string path, Mesh& mesh);
	};
}

#endif