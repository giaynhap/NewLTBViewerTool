#pragma once
#include "GL/freeglut.h"
#include <GL/GL.h>
#include <stdio.h>
#include "LtbLoader.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <GL/glut.h>
#include "tcx_dtx.h"
#ifndef  TEXTURES_H
#define TEXTURES_H

#define MAXSBONES 128
#define TEXTUREID_BASE 6000




extern int g_iTextureNums;
extern int meshText;

int GenerateTexturesID(void);
int LoadDTXImage(const char* szFileName, int Index, int* pWidth, int* pHeight);
int LoadTgaImage(const char* szFileName, int Index, int* SizeW, int* SizeH);



typedef struct dtx_header
{
	unsigned int iResType;
	int iVersion;
	unsigned short usWidth;
	unsigned short usHeight;
	unsigned short usMipmaps;
	unsigned short usSections;
	int iFlags;
	int iUserFlags;
	unsigned char ubExtra[12];
	char szCommandString[128];
} dtx_header_t;

typedef struct loadtga_s
{
	byte	id_length;
	byte	colormap_type;
	byte	image_type;
	word	colormap_index;
	word	colormap_length;
	byte	colormap_size;
	word	x_origin;
	word	y_origin;
	word	width;
	word	height;
	byte	pixel_size;
	byte	attributes;
}loadtga_t;


int get_file_size(FILE* p_file, bool reset = true);
#endif // ! TEXTURES_H
