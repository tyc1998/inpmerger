#ifndef GL_INPIO_H
#define GL_INPIO_H

#define _CRT_SECURE_NO_WARNINGS 1

#include <string>
#include <vector>
#include <stdarg.h>
#include <floattetwild/Types.hpp>
#include <floattetwild/Mesh.hpp>

namespace gl {
	class InpIO {
	private:
		typedef std::pair<std::string, bool> psb;

	public:
		InpIO() {}

		InpIO(const std::vector<std::string>& inps);

		InpIO(int count, ...);

		void AddInp(std::string inp);

		void AddInp(const std::vector<std::string>& inps);

		void AddInp(int count, ...);

		int Import();

		bool ImportOth(const std::vector<floatTetWild::Vector3>& pos, const std::vector<floatTetWild::Vector4i>& indices);

		bool ImportOth(const std::vector<floatTetWild::Vector3>& pos, const std::vector<floatTetWild::Vector4i>& indices,
			std::vector<int>& pos_offset, std::vector<int>& indice_offset);

		int OutPort(const std::string path);

		int OutPutVd(std::vector<double>& vd);

		void Clear();

		void ResetPoAndInc(const std::vector<floatTetWild::Vector3>& pos,
			const std::vector<floatTetWild::Vector4i>& indices, const std::vector<int>& pos_offset,
			const std::vector<int>& indice_offset);

		void ResetPoAndInc(std::vector<floatTetWild::Vector3>&& pos,
			std::vector<floatTetWild::Vector4i>&& indices, std::vector<int>&& pos_offset,
			std::vector<int>&& indice_offset);

		void ResetInc(const std::vector<floatTetWild::Vector4i>& indices, const std::vector<int>& indice_offset);

		void ResetInc(std::vector<floatTetWild::Vector4i>&& indices, std::vector<int>&& indice_offset);

		std::vector<floatTetWild::Vector3>& GetPos();

	private:
		std::vector<psb> inps_;
		std::vector<floatTetWild::Vector3> pos_;
		std::vector<floatTetWild::Vector4i> indices_;
		std::vector<int> pos_offset_;
		std::vector<int> indice_offset_;
	};
}

#endif