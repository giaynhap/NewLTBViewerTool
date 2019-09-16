#ifndef GNMODELCLASS
#define GNMODELCLASS
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <cstdint>
#include <math.h>
#include <cmath>  
#include "GnMath.h"

#define TIMESTEP   10
#ifndef M_PIl 
# define M_PIl          3.141592653589793238462643383279502884L 
#endif
#define LTB_MESHTYPE_NOTSKINNED  1
#define LTB_MESHTYPE_EXTRAFLOAT  2
#define LTB_MESHTYPE_SKINNED  4
#define LTB_MESHTYPE_SKINNEDALT  3
#define LTB_MESHTYPE_TWOEXTRAFLOAT  5
typedef unsigned int        uint;
typedef char			int8;
typedef short int		int16;
typedef int				int32;
typedef __int64         int64;
typedef unsigned char		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned __int64    uint64;
typedef char	filename[128];
typedef short	short3_t[3];
typedef int	 iweights[7];
typedef float	 fweight[4];
#define UnpackFromInt16(a)((float)a/16.0)

typedef struct ltb_bone
{
	char name[128];
	int nSubbone;
	matt4x4_t matdata;
	int isbone;
	int num2;
	int par;
} ltb_bone_t;

typedef struct ltb_frame
{
	int indexframe;
	vec3_t * pos;
	vec4_t * quats;
}ltb_frame_t;
typedef struct ltb_anim
{
	char name[128];
	int nkeyframe;
	int* listkeyframe;
	filename* listsound;
	float Dim[3];
	int interp_time;
	ltb_frame* frame;

} ltb_anim_t;



typedef struct ltb_submesh
{
	int nvertices;
	int nIdx;
	vec3_t* vertices;
	vec3_t* normal;
	fweight* weights;
	vec2_t* uv;
	iweights* weightsets;
	int* weights_out;
	short3_t* triangles;
	int textindex;
	int type;
	int unk1;
	int nWeight;
	vec3_t* drvertices;
	vec3_t* drnormal;
} ltb_submesh_t;


typedef struct ltb_mesh
{
	struct ltb_submesh * SubMesh;
	char name[128];
	int m_nSubMesh;

}ltb_mesh_t;

class Model
{


public:
	void prepareMesh();
	void drawBone();
	void drawMesh(int type);
	void drawGetAnim();
	void boneTransform(ltb_frame* frame);
	static int m_iFPS;
	int m_iAnim;
	float m_iTime;
	int m_iFrame;
	int m_version;
	int m_nBone;
	int m_nMesh;
	int m_nAnim;
	int m_TotalMesh;
	ltb_bone* m_Bone;
	ltb_mesh* m_Mesh;
	ltb_anim* m_Anim;

	matt4x4_t* BoneTrans;
private:
	ltb_frame* tmpFrame;
};




#endif