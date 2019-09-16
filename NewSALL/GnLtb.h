#pragma once

#ifndef GNLTBFILE 
#define GNLTBFILE
#include "Model.h"
class gnLoadLtb : public Model
{
public:
	gnLoadLtb(char file[]);
	gnLoadLtb(wchar_t file[]);
private:
	FILE* pFile;
	long m_fSize;
	/// funtion load data
	void _header_data();
	void _load_mesh();
	void _load_vertices(int num, int type, ltb_submesh sub);
	void _load_submesh(ltb_mesh);
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

void gnLoadLtb::_load_bone()
{
	for (int n = 0; n < m_nBone; n++)
	{
		int nchar = 0;
		fread(&nchar, 2, 1, pFile);
		memset(m_Bone[n].name, 0, sizeof(m_Bone[n].name));
		fread(m_Bone[n].name, nchar, 1, pFile);
		int isbone, num2;
		fread(&isbone, 1, 1, pFile);
		fread(&num2, 2, 1, pFile);
		m_Bone[n].isbone = isbone;
		m_Bone[n].num2 = num2;
		float aMatrix[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				float value;
				fread(&value, 4, 1, pFile);
				aMatrix[i][j] = value;
			}
		}
		m_Bone[n].matdata.parse(aMatrix);
		int nsubbone;
		fread(&nsubbone, 4, 1, pFile);
		m_Bone[n].nSubbone = nsubbone;
	}
}
void gnLoadLtb::_header_data()
{
	fsize();
	fseek(pFile, 0, 0);
	int nchar = 0;
	uint8  temp[128];
	fseek(pFile, 20, SEEK_CUR);
	fread(&m_version, sizeof(m_version), 1, pFile);
	fseek(pFile, 8, SEEK_CUR);
	fread(&m_nBone, sizeof(m_nBone), 1, pFile);
	fseek(pFile, 48, SEEK_CUR);
	fread(&nchar, 2, 1, pFile);
	fread(temp, nchar + 8, 1, pFile);
	fread(&m_nMesh, sizeof(m_nMesh), 1, pFile);
	m_Bone = new ltb_bone[m_nBone];
	m_Mesh = new ltb_mesh[m_nMesh];
	_load_mesh();
	_load_bone();
	___par();
	___toWeights();
	m_nAnim = 0;
	if (m_fSize - ftell(pFile) > 1024)
	{
		_load_anim();
	}
	fclose(pFile);
}

void gnLoadLtb::_load_anim()
{
	int iData = ftell(pFile);
	//printf("  == %i \n",iData);
	int nskipdata = 0;
	fread(&nskipdata, 4, 1, pFile);
	long skipzie = 0; ;
	for (int k = 0; k < nskipdata; k++)
	{
		int nchar = 0;
		fread(&nchar, 2, 1, pFile);
		fseek(pFile, nchar, SEEK_CUR);

		fread(&skipzie, 4, 1, pFile);
		fseek(pFile, skipzie * 4, SEEK_CUR);
	}
	short CompAnim = 0;
	short CompAnim2 = 0;
	fread(&CompAnim, 2, 1, pFile);
	fread(&CompAnim2, 2, 1, pFile);
	fread(&m_nAnim, 2, 1, pFile);
	m_Anim = new ltb_anim[m_nAnim];
	fseek(pFile, 2, SEEK_CUR);
	for (int i = 0; i < m_nAnim; i++)
	{
		fread(m_Anim[i].Dim, 12, 1, pFile);
		short gnchar = 0;
		fread(&gnchar, 2, 1, pFile);
		memset(m_Anim[i].name, 0, sizeof(m_Anim[i].name));
		fread(m_Anim[i].name, gnchar, 1, pFile);
		fseek(pFile, 4, SEEK_CUR);

		fread(&m_Anim[i].interp_time, 4, 1, pFile);
		fread(&m_Anim[i].nkeyframe, 4, 1, pFile);

		m_Anim[i].listkeyframe = new int[m_Anim[i].nkeyframe];
		m_Anim[i].frame = new ltb_frame[m_nBone];

		for (int j = 0; j < m_Anim[i].nkeyframe; j++)
		{
			int temp = 0;
			fread(&temp, 4, 1, pFile);
			m_Anim[i].listkeyframe[j] = temp;

			int nchar = 0;
			fread(&nchar, 2, 1, pFile);
			fseek(pFile, nchar, SEEK_CUR);
		}
		short nsup = 0;
		fread(&nsup, 2, 1, pFile);
		bool first = false;
		for (int k = 0; k < m_nBone; k++)
		{
			m_Anim[i].frame[k].pos = new gnVector[m_Anim[i].nkeyframe];
			m_Anim[i].frame[k].quats = new gnVector[m_Anim[i].nkeyframe];

			float p[3];
			float q[4];

			if (nsup != 0)
			{

				if (first == false)
				{
					first = true;
					fseek(pFile, ftell(pFile) - 2, 0);
				}
				unsigned short gframe_2;
				unsigned short gframe_1;
				fread(&gframe_1, 2, 1, pFile);
				fseek(pFile, 2, SEEK_CUR);
				for (int j = 0; j < gframe_1; j++)
				{
					//p.clear();
					short a[3] = { 0,0,0 };
					fread(a, 6, 1, pFile);
					p[0] = (UnpackFromInt16(a[0]));
					p[1] = (UnpackFromInt16(a[1]));
					p[2] = (UnpackFromInt16(a[2]));

					m_Anim[i].frame[k].pos[j].parse(p);

				}
				if (gframe_1 < m_Anim[i].nkeyframe)
					for (int j = gframe_1; j < m_Anim[i].nkeyframe; j++)
					{
						m_Anim[i].frame[k].pos[j].parse(p);
					}

				fread(&gframe_2, 2, 1, pFile);
				fseek(pFile, 2, SEEK_CUR);

				for (int j = 0; j < gframe_2; j++)
				{
					short a[4] = { 0,0,0,0 };
					fread(a, 8, 1, pFile);
					q[0] = (a[0] / 32767.0);
					q[1] = (a[1] / 32767.0);
					q[2] = (a[2] / 32767.0);
					q[3] = (a[3] / 32767.0);

					m_Anim[i].frame[k].quats[j].parse(q);
				}

				if (gframe_2 < m_Anim[i].nkeyframe)
					for (int j = gframe_2; j < m_Anim[i].nkeyframe; j++)
					{
						m_Anim[i].frame[k].quats[j].parse(q);
					}
			}
			else
			{
				if (k >= 2)
				{
					fseek(pFile, 1, SEEK_CUR);
				}

				for (int j = 0; j < m_Anim[i].nkeyframe; j++)
				{
					if (k == 0)
					{
						short a[6];
						fread(a, 12, 1, pFile);
						p[0] = (UnpackFromInt16(a[0]));
						p[1] = (UnpackFromInt16(a[2]));
						p[2] = (UnpackFromInt16(a[4]));
					}
					else
					{
						float a[3];
						fread(a, 12, 1, pFile);
						p[0] = (a[0]);
						p[1] = (a[1]);
						p[2] = (a[2]);
					}
					m_Anim[i].frame[k].pos[j].parse(p);
				}
				for (int j = 0; j < m_Anim[i].nkeyframe; j++)
				{
					if (k == 0)
					{
						short a[8];
						fread(a, 16, 1, pFile);
						q[0] = (a[0] / 32767.0f);
						q[1] = (a[2] / 32767.0f);
						q[2] = (a[4] / 32767.0f);
						q[3] = (a[6] / 32767.0f);
					}
					else
					{
						float a[4];
						fread(a, 16, 1, pFile);
						q[0] = (a[0]);
						q[1] = (a[1]);
						q[2] = (a[2]);
						q[3] = (a[3]);
					}

					m_Anim[i].frame[k].quats[j].parse(q);

				}

			}
		}
	}

}
void gnLoadLtb::_load_mesh()
{
	for (int i = 0; i < m_nMesh; i++)
	{
		int nchar = 0;
		fread(&nchar, 2, 1, pFile);
		memset(m_Mesh[i].name, 0, sizeof(m_Mesh[i].name));
		fread(m_Mesh[i].name, nchar, 1, pFile);
		fread(&m_Mesh[i].m_nSubMesh, sizeof(m_Mesh[i].m_nSubMesh), 1, pFile);
		fseek(pFile, m_Mesh[i].m_nSubMesh * 4, SEEK_CUR);
		fseek(pFile, 8, SEEK_CUR);
		m_Mesh[i].SubMesh = new ltb_submesh[m_Mesh[i].m_nSubMesh];
		_load_submesh(m_Mesh[i]);
	}
}

void gnLoadLtb::_load_submesh(ltb_mesh Mesh)
{
	for (int i = 0; i < Mesh.m_nSubMesh; i++)
	{
		int matnum = 0, unk1 = 0, sectionSize = 0;
		fseek(pFile, 4, SEEK_CUR);
		fread(&matnum, sizeof(matnum), 1, pFile);
		fseek(pFile, 17, SEEK_CUR);
		fread(&unk1, sizeof(unk1), 1, pFile);
		fread(&sectionSize, sizeof(sectionSize), 1, pFile);
		if (sectionSize != 0)
		{
			int numVerts, numIdx, meshType, a = 0;
			fread(&numVerts, sizeof(numVerts), 1, pFile);
			fread(&numIdx, sizeof(numIdx), 1, pFile);
			fread(&meshType, sizeof(meshType), 1, pFile);
			Mesh.SubMesh[i].nvertices = numVerts;
			Mesh.SubMesh[i].nIdx = numIdx;
			Mesh.SubMesh[i].type = meshType;
			Mesh.SubMesh[i].vertices = new gnVector[numVerts];
			Mesh.SubMesh[i].weights = new fweight[numVerts];
			Mesh.SubMesh[i].normal = new gnVector[numVerts];
			Mesh.SubMesh[i].uv = new gnVector[numVerts];
			Mesh.SubMesh[i].triangles = new short3_t[numIdx];
			fseek(pFile, 20, SEEK_CUR);
			Mesh.SubMesh[i].unk1 = unk1;
			if (unk1 == 4)
			{
				fread(&a, sizeof(a), 1, pFile);
				Mesh.SubMesh[i].weightsets = new iweights[1];
				Mesh.SubMesh[i].weightsets[0][0] = a;

			}
			else if (unk1 == 5)
				fread(&a, 2, 1, pFile);
			_load_vertices(numVerts, meshType, Mesh.SubMesh[i]);

			for (int j = 0; j < numIdx; j++)
			{
				short3_t indx;
				fread(indx, sizeof(short3_t), 1, pFile);
				Mesh.SubMesh[i].triangles[j][0] = indx[0]; Mesh.SubMesh[i].triangles[j][1] = indx[1]; Mesh.SubMesh[i].triangles[j][2] = indx[2];
			}

			if (unk1 == 5)
			{
				int numWeight = 0;
				fread(&numWeight, sizeof(numWeight), 1, pFile);

				Mesh.SubMesh[i].weightsets = new iweights[numWeight];
				Mesh.SubMesh[i].nWeight = numWeight;
				for (long j = 0; j < numWeight; j++)
				{
					short weightsets[2];
					byte weightsets2[4];
					int weightsets3;
					fread(&weightsets[0], 2, 1, pFile);
					fread(&weightsets[1], 2, 1, pFile);
					fread(&weightsets2[0], 1, 1, pFile);
					fread(&weightsets2[1], 1, 1, pFile);
					fread(&weightsets2[2], 1, 1, pFile);
					fread(&weightsets2[3], 1, 1, pFile);
					fread(&weightsets3, 4, 1, pFile);
					Mesh.SubMesh[i].weightsets[j][0] = weightsets[0];
					Mesh.SubMesh[i].weightsets[j][1] = weightsets[1];
					Mesh.SubMesh[i].weightsets[j][2] = weightsets2[0];
					Mesh.SubMesh[i].weightsets[j][3] = weightsets2[1];
					Mesh.SubMesh[i].weightsets[j][4] = weightsets2[2];
					Mesh.SubMesh[i].weightsets[j][5] = weightsets2[3];
					Mesh.SubMesh[i].weightsets[j][6] = weightsets3;
				}
			}
			int unk2 = 0;
			fread(&unk2, 1, 1, pFile);
			fseek(pFile, unk2, SEEK_CUR);
		}
	}
}
void gnLoadLtb::_load_vertices(int numVerts, int meshType, ltb_submesh Sub) {
	if (meshType == 3)
		meshType = LTB_MESHTYPE_TWOEXTRAFLOAT;
	bool IncludeWeights = false;
	if (meshType == LTB_MESHTYPE_NOTSKINNED)
		IncludeWeights = false;
	else
		IncludeWeights = true;
	for (int i = 0; i < numVerts; i++)
	{
		float vert[3];
		float normal[3];
		fread(vert, sizeof(vert), 1, pFile);

		Sub.vertices[i].parse(vert);

		if (IncludeWeights)
		{
			float f1 = 0.0f, f2 = 0.0f, f3 = 0.0f, f4 = 0.0f;
			fread(&f1, sizeof(f1), 1, pFile);
			if (meshType != LTB_MESHTYPE_EXTRAFLOAT)  fread(&f2, sizeof(f2), 1, pFile);
			else f2 = 1.0f - f1;
			if (meshType != LTB_MESHTYPE_TWOEXTRAFLOAT && meshType != LTB_MESHTYPE_EXTRAFLOAT)  fread(&f3, sizeof(f3), 1, pFile);
			else f3 = 1.0f - (f2 + f1);
			f4 = 1.0f - (f1 + f2 + f3);
			Sub.weights[i][0] = f1;	Sub.weights[i][1] = f2; Sub.weights[i][2] = f3;
			Sub.weights[i][3] = f4;
		}
		fread(normal, sizeof(normal), 1, pFile);
		Sub.normal[i].parse(normal);
		float uv[2];
		fread(uv, sizeof(uv), 1, pFile);
		//  uv[1]=1.0f-uv[1];
		if (uv[0] > 1.0f) uv[0] -= 1.0f;
		Sub.uv[i].parse(uv);
	}
}
gnLoadLtb::gnLoadLtb(char file[])
{
	pFile = fopen(file, "rb");
	_header_data();
	m_iAnim = 0;
	m_iTime = 0;
	m_iFrame = 0;
};
gnLoadLtb::gnLoadLtb(wchar_t file[])
{
	pFile = _wfopen(file, L"rb");
	_header_data();
	m_iAnim = 0;
	m_iTime = 0;
	m_iFrame = 0;
};

void gnLoadLtb::___par()
{
	int nsubone[512];
	nsubone[0] = m_Bone[0].nSubbone;
	m_Bone[0].par = -1;
	for (int i = 1; i < m_nBone; i++)
	{
		nsubone[i] = m_Bone[i].nSubbone;
		for (int j = i - 1; j >= 0; j--)
			if (nsubone[j] > 0)
			{
				nsubone[j] -= 1;
				m_Bone[i].par = j;
				break;
			}
	}
}
int gnLoadLtb::___weight(int* num, float* size)
{
	float max = size[0];
	int boneWeight = num[1];
	if (num[0] > 4) num[0] = 4;
	for (int i = 1; i < num[0]; i++)
	{
		if (max < size[i])
		{
			max = size[i];
			boneWeight = num[i + 1];
		}
	}
	return boneWeight;
}
void gnLoadLtb::___toWeights()
{
	for (int i = 0; i < m_nMesh; i++)
	{
		for (int x = 0; x < m_Mesh[i].m_nSubMesh; x++)
		{
			m_Mesh[i].SubMesh[x].weights_out = new int[m_Mesh[i].SubMesh[x].nvertices];
			if (m_Mesh[i].SubMesh[x].unk1 != 4)
			{
				int pWeightset = 0;
				for (int j = 0; j < m_Mesh[i].SubMesh[x].nWeight; j++)
				{
					int intWeightSet[5];
					int num = 0;
					for (int n = 0; n < 4; n++)
					{
						if (m_Mesh[i].SubMesh[x].weightsets[j][2 + n] > -1)
						{
							intWeightSet[n + 1] = m_Mesh[i].SubMesh[x].weightsets[j][2 + n];
							num += 1;
						}
						else break;
					}
					intWeightSet[0] = num;
					char a[32];
					for (int k = 0; k < m_Mesh[i].SubMesh[x].weightsets[j][1]; k++)
					{
						float WeightsetSize[7];
						for (int h = 0; h < 5; h++)
							WeightsetSize[h] = m_Mesh[i].SubMesh[x].weights[pWeightset][h];
						int outw = ___weight(intWeightSet, WeightsetSize);
						m_Mesh[i].SubMesh[x].weights_out[pWeightset] = outw;
						pWeightset += 1;
					}
				}
			}
			else
			{
				for (int j = 0; j < m_Mesh[i].SubMesh[x].nvertices; j++)
					m_Mesh[i].SubMesh[x].weights_out[j] = m_Mesh[i].SubMesh[x].weightsets[0][0];
			}
		}
	}
}
int gnLoadLtb::m_iFPS = TIMESTEP;
