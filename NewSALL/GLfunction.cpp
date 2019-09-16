#include "GLfunction.h"

float rotate = 0.0;
GLfloat ambient[4] = { 0.1f, 0.1f, 0.1f,1.0f };
GLfloat diffuse[4] = { 0.5f, 0.5f, 0.5f,1.0f };
GLfloat spec[4] = { 0.0f, 0.0f, 0.0f,1.0f };
GLfloat emiss[4] = { 0.0f, 0.0f, 0.0f,2.0f };
GLfloat mat_shininess[] = { 500.0 };
GLfloat shine = 0.0f;

GLfloat gRotatef[] = { 0.0,0.0,0.0 };
GLfloat eyse_postion[] = { 150.0,0.0,0.0 };
GLfloat eyse_lookat[] = { 0.0,0.0,0.0 };
GLfloat OldgAngle[] = { 0.0,0.4,0.0 };
Model* LtbFile;
int SkyBoxIndex[6];

int modeShowModel = 0;
bool isViewMode = false;
bool isShowBone = true;
void load_sky_box()
{
	int Sizex, Sizey;
	for (int i = 0; i < 6; i++)
		SkyBoxIndex[i] = GenerateTexturesID();

	LoadTgaImage("SkyBox/de_stormft.tga", SkyBoxIndex[0], &Sizex, &Sizey);
	LoadTgaImage("SkyBox/de_stormrt.tga", SkyBoxIndex[2], &Sizex, &Sizey);
	LoadTgaImage("SkyBox/de_stormlf.tga", SkyBoxIndex[3], &Sizex, &Sizey);
	LoadTgaImage("SkyBox/de_stormbk.tga", SkyBoxIndex[1], &Sizex, &Sizey);
	LoadTgaImage("SkyBox/de_stormup.tga", SkyBoxIndex[4], &Sizex, &Sizey);
	LoadTgaImage("SkyBox/de_stormdn.tga", SkyBoxIndex[5], &Sizex, &Sizey);
}


void square(void) {

	glPushMatrix();

	glTranslatef(0, -90, 0);
	glScalef(200, 200, 200);
	glRotatef(90, 1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(-1, -1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-1, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(1, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(1, -1, 0);
	glEnd();
	glPopMatrix();
}


void initialise()
{

	glEnable(GL_TEXTURE_2D);
	load_sky_box();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}


void DrawOriginLines()
{
	float size;
	size = 25.0f;
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-size, 0.0f, 0.0f);
	glVertex3f(size, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -size, 0.0f);
	glVertex3f(0.0f, size, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -size);
	glVertex3f(0.0f, 0.0f, size);

	glEnd();
	glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex3f(size, 0.0f, 0.0f);
	glVertex3f(0.0f, size, 0.0f);
	glVertex3f(0.0f, 0.0f, size);
	glEnd();
	glPointSize(1.0f);

}



void draw_sky()
{
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(500 * cos((OldgAngle[0] + gRotatef[0]) * M_PIl / 180) * cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180), 500 * sin((OldgAngle[1] + gRotatef[1]) * M_PIl / 180), 500 * sin((OldgAngle[0] + gRotatef[0]) * M_PIl / 180) * -cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180),
		0, 0, 0, 0.0f, cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180) / abs(cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180)), 0.0f);

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glRotated(180, 1.0, 0.0, 0.0);

	float x = 0;
	float y = 0;
	float z = 0;
	float width = 5000;
	float height = 5000;
	float length = 5000;

	glBindTexture(GL_TEXTURE_2D, SkyBoxIndex[0]);

	x = x - width / 2;
	y = y - height / 2;
	z = (z - length / 2);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, SkyBoxIndex[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, SkyBoxIndex[4]);
	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, SkyBoxIndex[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, SkyBoxIndex[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);

	glEnd();
	glBindTexture(GL_TEXTURE_2D, SkyBoxIndex[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}


void display()
{

	if (LtbFile != NULL)
		LtbFile->drawGetAnim();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	float lightPos[4] = { -40,100,40,0 };

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.5, 0.6, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!isViewMode)
	{
		gluLookAt(eyse_postion[0] * cos((OldgAngle[0] + gRotatef[0]) * M_PIl / 180) * cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180), eyse_postion[0] * sin((OldgAngle[1] + gRotatef[1]) * M_PIl / 180), eyse_postion[0] * sin((OldgAngle[0] + gRotatef[0]) * M_PIl / 180) * -cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180),
			eyse_lookat[0], eyse_lookat[1], eyse_lookat[2], 0.0f, cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180) / abs(cos((OldgAngle[1] + gRotatef[1]) * M_PIl / 180)), 0.0f);
	}
	else {
		gluLookAt(0, -1, -4, 0, -2, 40, 0.0f, 1, 0.0f);
	}

	glColor3f(1.0f, 1.0f, 1.0f);

	if (LtbFile != NULL)
		LtbFile->drawMesh(modeShowModel);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	draw_sky();


	glDisable(GL_TEXTURE_2D);
	glColor4f(0.2f, 0.2f, 0.4f, 0.5f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	square();

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (LtbFile != NULL && isShowBone) {
		LtbFile->drawBone();
	}
	if (!isViewMode)
		DrawOriginLines();
	glEnable(GL_LIGHTING);
	SwapBuffers(g_HDC);

}
