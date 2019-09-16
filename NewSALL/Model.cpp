#include <windows.h>
#include "gl/GL.h"
#include "GL/freeglut.h"

#include "Model.h"

void Model::prepareMesh() {
	tmpFrame = new ltb_frame[m_nBone];
	BoneTrans = new matt4x4_t[m_nBone];
	for (int i = 0; i < m_nBone; i++)
	{
		tmpFrame[i].pos = new vec3_t[1];
		tmpFrame[i].quats = new vec4_t[1];
		memcpy(BoneTrans[i], m_Bone[i].matdata,sizeof(matt4x4_t));
	}

	if (m_nAnim <= 0) return;

	for (int i = 0; i < m_nMesh; i++)
	{
		for (int x = 0; x < m_Mesh[i].m_nSubMesh; x++)
		{
			m_Mesh[i].SubMesh[x].drvertices = new vec3_t[m_Mesh[i].SubMesh[x].nvertices];
			m_Mesh[i].SubMesh[x].drnormal = new vec3_t[m_Mesh[i].SubMesh[x].nvertices];

			for (int j = 0; j < m_Mesh[i].SubMesh[x].nvertices; j++)
			{
				int indexBone = m_Mesh[i].SubMesh[x].weights_out[j];

				matt4x4_t MatBoneI;
				InverseMat(m_Bone[indexBone].matdata,MatBoneI);
				
				VectorTransform(m_Mesh[i].SubMesh[x].vertices[j], MatBoneI, m_Mesh[i].SubMesh[x].drvertices[j]);
				VectorRotate(m_Mesh[i].SubMesh[x].normal[j], MatBoneI, m_Mesh[i].SubMesh[x].drnormal[j]);
			}
		}
	}
}

void Model::drawBone()
{

	float size;
	size = 10.0f;
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glPointSize(4.0f);

	for (int i = 0; i < m_nBone; i++)
	{
		glBegin(GL_POINTS);
		glVertex3f(BoneTrans[i][0][3], BoneTrans[i][1][3], BoneTrans[i][2][3] );
		glEnd();

		if (m_Bone[i].par > -1)
		{
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 0.0f);
			glVertex3f(BoneTrans[i][0][3], BoneTrans[i][1][3], BoneTrans[i][2][3]);
			glVertex3f(BoneTrans[m_Bone[i].par][0][3], BoneTrans[m_Bone[i].par][1][3], BoneTrans[m_Bone[i].par][2][3]);
			glEnd();
		}
	}
	glPointSize(1.0f);
}


void Model::drawMesh(int type)
{

	for (int x = 0; x < m_nMesh; x++)

		for (int j = 0; j < m_Mesh[x].m_nSubMesh; j++)
		{
			glPushMatrix();
			if (type == 0 || type == 1)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (type == 1 || type == 2)
				glDisable(GL_TEXTURE_2D);
			else
			{
				if (this->m_Mesh[x].SubMesh[j].textindex > 0)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, this->m_Mesh[x].SubMesh[j].textindex);
				}
				else glDisable(GL_TEXTURE_2D);
			}
			for (int i = 0; i < m_Mesh[x].SubMesh[j].nIdx; i++)
			{
				if (i == 0) glBegin(GL_TRIANGLE_FAN);
				else
					glBegin(GL_TRIANGLE_STRIP);
				int gtr[3] = { m_Mesh[x].SubMesh[j].triangles[i][0], m_Mesh[x].SubMesh[j].triangles[i][1], m_Mesh[x].SubMesh[j].triangles[i][2] };
				vec3_t normal;
				vec3_t newvert2;
				for (int k = 0; k < 3; k++)
				{
					int gbone = m_Mesh[x].SubMesh[j].weights_out[gtr[k]];


					if (m_nAnim > 0)
					{
						
						VectorTransform(m_Mesh[x].SubMesh[j].drvertices[gtr[k]], BoneTrans[gbone], newvert2);
						VectorRotate(m_Mesh[x].SubMesh[j].drnormal[gtr[k]]	, BoneTrans[gbone], normal);
 
					}
					else
					{
						memcpy(normal, m_Mesh[x].SubMesh[j].normal[gtr[k]], sizeof(vec3_t));
						memcpy(newvert2, m_Mesh[x].SubMesh[j].vertices[gtr[k]], sizeof(vec3_t));
					}
 
					glTexCoord2f(m_Mesh[x].SubMesh[j].uv[gtr[k]][0], m_Mesh[x].SubMesh[j].uv[gtr[k]][1]);
					glNormal3f(normal[0], normal[1], normal[2]);
					glVertex3f(newvert2[0], newvert2[1], newvert2[2]);

				}
				glEnd();

			}
			glPopMatrix();
		}
}


void Model::drawGetAnim()
{

	if (m_nAnim <= 0) return;
	if (m_Anim[m_iAnim].nkeyframe < 0) return;


	if (m_iFrame >= m_Anim[m_iAnim].nkeyframe - 1)
	{
		m_iFrame = 0;
		m_iTime = 0.0;
	}
	m_iTime += this->m_iFPS;
	float total_time = (float)(m_Anim[m_iAnim].listkeyframe[m_iFrame + 1] - m_Anim[m_iAnim].listkeyframe[m_iFrame] - 1);
	float is_f = (float)m_iTime / total_time;

	for (int i = 0; i < m_nBone; i++)
	{

		PostionSlerp(m_Anim[m_iAnim].frame[i].pos[m_iFrame], m_Anim[m_iAnim].frame[i].pos[m_iFrame + 1], is_f, tmpFrame[i].pos[0]);
		QuaternionSlerp(m_Anim[m_iAnim].frame[i].quats[m_iFrame], m_Anim[m_iAnim].frame[i].quats[m_iFrame + 1], is_f, tmpFrame[i].quats[0]);
	}
	if (is_f > 1)
	{
		m_iTime = 0;
		m_iFrame += 1;
	}
	boneTransform(tmpFrame);

}


void Model::boneTransform(ltb_frame* frame)
{
 
	for (int i = 0; i < m_nBone; i++)
	{
	 
		matt4x4_t mat;
		QuaternionMatrix(frame[i].quats[0], mat);
		mat[0][3] = frame[i].pos[0][0];
		mat[1][3] = frame[i].pos[0][1];
		mat[2][3] = frame[i].pos[0][2];

		if (m_Bone[i].par < 0) {
			
			memcpy(BoneTrans[i], mat, sizeof(matt4x4_t));
		}
		else
		{
			vec3_t vec;
			matt4x4_t mat2;
			ConcatTransforms(BoneTrans[m_Bone[i].par], mat, mat2);
			memcpy(BoneTrans[i], mat2, sizeof(matt4x4_t));
		}
	}
}
