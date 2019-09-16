#include "LtbLoader.h"
#include "GL/glut.h"
 
int main(int argc, char** argv) {
	  
	LtbLoader loader("Model_5.LTB");
	printf("Ltb version: %d\n", loader.m_version);
 
	printf("========= [Bone %d] =========\n", loader.m_nBone);
	for (int i = 0; i < loader.m_nBone; i++) {
		printf("m_Bone: %s %d\n", loader.m_Bone[i].name,loader.m_Bone[i].par);
	}
	
	printf("========= [Anim %d] =========\n", loader.m_nAnim);
	for (int i = 0; i < loader.m_nAnim; i++) {
		printf("m_Anim: %s\n", loader.m_Anim[i].name);
	}

	printf("========= [Mesh %d] =========\n", loader.m_nMesh);
	for (int i = 0; i < loader.m_nMesh; i++) {
		printf("m_Mesh: %s\n", loader.m_Mesh[i].name);
	}

 
}


