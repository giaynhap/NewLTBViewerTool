
#ifndef GNLTBFILE 
#define GNLTBFILE
#include "Model.h"
#include <stdlib.h>
#include <stdio.h>
class LtbLoader : public Model
{
public:
	LtbLoader(const char file[]);
	LtbLoader(const wchar_t file[]);
	~LtbLoader();
private:
	FILE* pFile;
	long m_fSize;
	/// funtion load data
	void _header_data();
	void _load_mesh();
	void _load_vertices(int num, int type, struct ltb_submesh *sub);
	void _load_submesh(ltb_mesh_t *);
	void _load_bone();
	void ___par();
	int ___weight(int* num, float* size);
	void ___toWeights();
	void _load_anim();
	ltb_frame* tmpFrame;
	//////////////////////////////////
	void fsize(void)
	{
		int old = ftell(pFile);
		fseek(pFile, 0, SEEK_END);
		m_fSize = ftell(pFile);
		fseek(pFile, old, 0);
	}
};
#endif
