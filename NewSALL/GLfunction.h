#pragma once

#include "Textures.h"
extern int SkyBoxIndex[6];
extern Model* LtbFile;
extern GLfloat OldgAngle[];
extern GLfloat gRotatef[];
extern GLfloat eyse_postion[];
extern bool isViewMode;
extern int modeShowModel;
extern bool isShowBone ;
extern HDC g_HDC;
#define M_PIl          3.141592653589793238462643383279502884L 

void msgConvertImage(HWND parents = NULL);
void msgAnimShow(HWND parents = NULL);
void display();
void initialise();
#define ADDMENUITEM(hmenu, ID, string) \
	 AppendMenu(hmenu, MF_STRING, ID, string);