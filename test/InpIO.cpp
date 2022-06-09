#include "InpIO.h"

namespace gl {
	InpIO::InpIO(const std::vector<std::string>& inps) {
		for (int i = 0; i < inps.size(); ++i)
			inps_.emplace_back(psb(inps[i], 0));
	}

	InpIO::InpIO(int count, ...){
		va_list arg_ptr;
		int n_arg_value = 0;
		va_start(arg_ptr, count);
		while (count--) {
			inps_.emplace_back(psb(va_arg(arg_ptr, std::string), 0));
		}
		va_end(arg_ptr);
	}

	void InpIO::AddInp(std::string inp){
		inps_.emplace_back(psb(inp, 0));
	}

	void InpIO::AddInp(const std::vector<std::string>& inps){
		for (int i = 0; i < inps.size(); ++i)
			inps_.emplace_back(psb(inps[i], 0));
	}

	void InpIO::AddInp(int count, ...){
		va_list arg_ptr;
		int n_arg_value = 0;
		va_start(arg_ptr, count);
		while (count--) {
			inps_.emplace_back(psb(va_arg(arg_ptr, std::string), 0));
		}
		va_end(arg_ptr);
	}

	int InpIO::Import()
	{
		int ret = 0;

		char buffer[1024];
		int state = 0;
		int offset = 0;
		int idx;
		for (auto& inp : inps_) {
			FILE* p = fopen(inp.first.c_str(), "r");

			while (!feof(p)) {
				fgets(buffer, sizeof(buffer), p);
				if (strncmp(buffer, "*NODE", 5) == 0) {
					state = 1;
					offset = 0;
					continue;
				}
				else if (strncmp(buffer, "*ELEMENT,TYPE=C3D4", 8) == 0) {
					pos_offset_.emplace_back(offset);
					state = 2;
					offset = 0;
					continue;
				}
				else if (strncmp(buffer, "*****", 5) == 0)
					break;
				if (state == 1) {
					double a, b, c;
					sscanf(buffer, "%d,%lf,%lf,%lf", &idx, &a, &b, &c);
					pos_.emplace_back(floatTetWild::Vector3(a, b, c));
				}
				else if (state == 2) {
					int a, b, c, d;
					sscanf(buffer, "%d,%d,%d,%d,%d", &idx, &a, &b, &c, &d);
					indices_.emplace_back(floatTetWild::Vector4i(a, b, c, d));
				}
			}
			indice_offset_.emplace_back(offset);
			++ret;
			inp.second = 1;
		}

		return ret;
	}

	bool InpIO::ImportOth(const std::vector<floatTetWild::Vector3>& pos, const std::vector<floatTetWild::Vector4i>& indices)
	{
		pos_ = pos;
		indices_ = indices;
		return true;
	}

	bool gl::InpIO::ImportOth(const std::vector<floatTetWild::Vector3>& pos, const std::vector<floatTetWild::Vector4i>& indices,
		std::vector<int>& pos_offset, std::vector<int>& indice_offset)
	{
		pos_ = pos;
		indices_ = indices;
		pos_offset_ = pos_offset;
		indice_offset_ = indice_offset;
		return true;
	}

	int InpIO::OutPort(const std::string path)
	{
		FILE* p = fopen(path.c_str(), "w");

		fprintf(p, "*NODE\n");
		int idx = 0;
		for (int i = 0; i < pos_.size(); ++i)
			fprintf(p, "%d,%lf,%lf,%lf\n", ++idx, pos_[i][0], pos_[i][1], pos_[i][2]);

		int offset = 0;
		for (int i = 0; i < indice_offset_.size(); ++i) {
			int comp_idx = 0;
			fprintf(p, "*ELEMENT,TYPE=C3D4,ELSET=%d\n", ++comp_idx);
			for (int k = 0; k < indices_.size(); ++k) {
				fprintf(p, "%d,%d,%d,%d,%d\n", ++idx, indices_[k + offset][0] + 1, indices_[k + offset][1] + 1, indices_[k + offset][2] + 1,
					indices_[k + offset][3] + 1);
			}
			offset += indice_offset_[i];
		}

		fprintf(p, "*****");

		return idx;
	}

	int InpIO::OutPutVd(std::vector<double>& vd)
	{
		vd.clear();
		int ret = 0;
        vd.reserve(pos_.size() * 3);
		for (int i = 0; i < pos_.size(); ++i) {
			for (int j = 0; j < 3; ++j)
				vd.emplace_back(pos_[i][j]);
			++ret;
		}
		return ret;
	}

	void InpIO::Clear() {
		inps_.clear();
		pos_.clear();
		indices_.clear();
		pos_offset_.clear();
		indice_offset_.clear();
	}

	void InpIO::ResetPoAndInc(const std::vector<floatTetWild::Vector3>& pos, const  std::vector<floatTetWild::Vector4i>& indices,
		const std::vector<int>& pos_offset, const std::vector<int>& indice_offset)
	{
		pos_ = pos;
		indices_ = indices;
		pos_offset_ = pos_offset;
		indice_offset_ = indice_offset;
	}

	void gl::InpIO::ResetPoAndInc(std::vector<floatTetWild::Vector3>&& pos, std::vector<floatTetWild::Vector4i>&& indices,
		std::vector<int>&& pos_offset, std::vector<int>&& indice_offset)
	{
		pos_ = pos;
		indices_ = indices;
		pos_offset_ = pos_offset;
		indice_offset_ = indice_offset;
	}

	void InpIO::ResetInc(const std::vector<floatTetWild::Vector4i>& indices, const std::vector<int>& indice_offset)
	{
		indices_ = indices;
		indice_offset_ = indice_offset;
	}

	void InpIO::ResetInc(std::vector<floatTetWild::Vector4i>&& indices, std::vector<int>&& indice_offset)
	{
		indices_ = indices;
		indice_offset_ = indice_offset;
	}

	std::vector<floatTetWild::Vector3>& gl::InpIO::GetPos()
	{
		return pos_;
	}
}