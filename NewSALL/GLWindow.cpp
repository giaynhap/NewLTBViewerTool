#pragma once
#include <shlobj.h>

#define GL_VERTEX_ARRAY  
#include <gl\GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <iostream>;
#include <windows.h>
#include <Commdlg.h>
#include <CommCtrl.h>
#include <string>
#include "Model.h"
#include "LtbLoader.h"
#include "Textures.h"
#include "GLfunction.h"
#pragma comment(lib,"comctl32.lib")

// Style window
#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

HMENU hMenu, subFile, subTool, helpMenu;
HWND hWndEdit, hWndEdit2, track_bar, hWndCheckBox, hwndCombox2, hwndListMesh, hwndListSubMesh;
HWND hwndResize, hwndconvert;
wchar_t openFileName[1024];
HWND hWndListAnim, hWndbLoadModel, hWndbLoadText, hwndWepnView;
HWND hWndMain, hWndOpenGL, subWindow;
HDC g_HDC;

bool done;
static int window;
static int value = 0;

int mFpscontrolValue = 20;


int mfirstPoint[2];
bool IsCLick = false;

int m_meshSelect = 0;
int m_subMeshSelect = 0;

HMENU  ADDPOPUPMENU(HMENU hmenu, const char* string) {
	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, (LPCSTR)string);
	return hSubMenu;
}

enum
{
	ID_FILE_LOAD_LTB,
	ID_FILE_LOAD_TEXTURES,
	ID_FILE_EXIT,
	ID_EDIT,
	ID_TRACKBAR,
	ID_COMBOBOX,
	ID_CHECKBOX,
	ID_LOADTEXT,
	ID_TOOL_SMD,
	ID_TOOL_TEX,
	ID_EX_MESH,
	ID_EX_ANIM,
	ID_EX_PROCESS,
	ID_CV_RESIZE,
	ID_TOOL_UNZIP,
	ID_TOOL_STUDIO_MDL,
	ID_TOOL_ABOUT,
	ID_WPN_VIEW
};

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}



void SetupPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0 };
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void updateConboxListAnim()
{

	SendMessage(hWndListAnim, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (LtbFile == NULL) return;
	for (int k = 0; k < LtbFile->m_nAnim; k++)
	{
		SendMessageA(hWndListAnim, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)LtbFile->m_Anim[k].name);
	}
	SendMessageA(hWndListAnim, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

}


void updateComboxLisSubMesh(int index)
{
	char name[255];
	SendMessage(hwndListSubMesh, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	memset(&name, 0, 255);
	if (LtbFile == NULL) return;
	for (int k = 0; k < LtbFile->m_Mesh[index].m_nSubMesh; k++)
	{
		sprintf(name, "SubMesh %d", k);
		SendMessageA(hwndListSubMesh, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)name);

	}
	SendMessageA(hwndListSubMesh, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	m_meshSelect = 0;
}
void updateComboxLisMesh()
{

	SendMessage(hwndListMesh, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

	if (LtbFile == NULL) return;
	for (int k = 0; k < LtbFile->m_nMesh; k++)
	{
		SendMessageA(hwndListMesh, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)LtbFile->m_Mesh[k].name);
	}
	SendMessageA(hwndListMesh, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	updateComboxLisSubMesh(0);
	m_subMeshSelect = 0;
}

void ExePath(char* path) {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	for (int k = strlen(buffer) - 1; k > 0; k--)
		if (buffer[k] == '\\')
		{

			strncpy(path, buffer, k + 1);
			break;
		}

}



void convertDtxToBmp(char* szFileName, char* dstFile, bool isRescan)
{
	/*static unsigned char pBuffer[1024 * 1024 * 4];
	int pWidth, pHeight;
	LoadDTXImage(szFileName, NULL, &pWidth, &pHeight, pBuffer);
	FIBITMAP* dst = FreeImage_ConvertFromRawBits(pBuffer, pWidth, pHeight, 4 * pWidth, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	FreeImage_FlipVertical(dst);

	FIBITMAP* temp = FreeImage_ColorQuantizeEx(dst, FIQ_WUQUANT, 256, 0, (RGBQUAD*)1);
	int w = FreeImage_GetWidth(temp);
	int h = FreeImage_GetHeight(temp);
	if (isRescan)
		if (w > 512 || h > 512)
			temp = FreeImage_Rescale(temp, 512, 512);

	FreeImage_Save(FIF_BMP, temp, dstFile, BMP_DEFAULT);

	FreeImage_Unload(temp);*/
}


void doExeFile(char* exeFile, char* command, char* workingdir)
{
	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = exeFile;
	ShExecInfo.lpParameters = command;

	ShExecInfo.lpDirectory = workingdir;

	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteExA(&ShExecInfo);
}




void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:

		LtbFile->m_iFrame = 0;
		LtbFile->m_iTime = 0;
		LtbFile->m_iAnim -= 1;
		break;
	case GLUT_KEY_RIGHT:
		LtbFile->m_iTime = 0;
		LtbFile->m_iFrame = 0;
		LtbFile->m_iAnim += 1;

		break;
	case GLUT_KEY_UP:
		eyse_postion[0] -= eyse_postion[0] / 100.0;
		break;
	case GLUT_KEY_DOWN:
		eyse_postion[0] += eyse_postion[0] / 100.0;
		break;
	}

	if (LtbFile->m_iAnim >= LtbFile->m_nAnim)
	{
		LtbFile->m_iTime = 0;
		LtbFile->m_iFrame = 0;

		LtbFile->m_iAnim = 0;

	}
	else if (LtbFile->m_iAnim < 0)
	{
		LtbFile->m_iTime = 0;
		LtbFile->m_iFrame = 0;

		LtbFile->m_iAnim = LtbFile->m_nAnim - 1;
	}
	glutPostRedisplay();
}

void mouse(int but, int st, int x, int y)
{
	if (but == 0 && st == 0 && IsCLick == false)
	{
		mfirstPoint[0] = x;
		mfirstPoint[1] = y;
		IsCLick = true;
	}
	else if ((st == 1 && IsCLick == true))
	{
		IsCLick = 0;
		OldgAngle[0] += gRotatef[0];
		if (OldgAngle[0] > 360.0) OldgAngle[0] -= (int)(OldgAngle[0] / 360) * 360.0;
		OldgAngle[1] += gRotatef[1];
		gRotatef[1] = 0.0f;
		gRotatef[0] = 0.0f;

	}
}
void mousemove(int x, int y)
{
	if (IsCLick == true)
	{
		float angle = (mfirstPoint[0] - x) / ((float)500 / 2.0) * 360.0;
		float angle2 = (mfirstPoint[1] - y) / ((float)500 / 2.0) * 360.0;
		gRotatef[0] = angle;
		gRotatef[1] = angle2;

	}

}


VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (LtbFile != NULL)
	{
		char cnFrame[5];
		sprintf(cnFrame, "%d", LtbFile->m_iFrame);
		SetWindowTextA(hWndEdit, (LPCSTR)cnFrame);


		LtbFile->m_iFPS = (int)SendMessage(track_bar, TBM_GETPOS, NULL, NULL);


	}
	display();
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	static int width, height;
	static WORD x, y;
	x = LOWORD(lParam);
	y = HIWORD(lParam);
	PAINTSTRUCT ps;
	char filename2[1024];
	OPENFILENAMEA ofn2;
	ZeroMemory(&filename2, sizeof(filename2));
	ZeroMemory(&ofn2, sizeof(ofn2));
	ofn2.lStructSize = sizeof(ofn2);
	ofn2.hwndOwner = hwnd;
	ofn2.lpstrFilter = "All Files\0*.*\0";
	ofn2.lpstrFile = filename2;
	ofn2.nMaxFile = 1024;
	ofn2.lpstrTitle = "Select a File";
	ofn2.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;


	switch (message)
	{
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)

		{
			if ((HWND)lParam == hWndListAnim)
			{
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);
				TCHAR  ListItem[256];
				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
					(WPARAM)ItemIndex, (LPARAM)ListItem);
				LtbFile->m_iAnim = ItemIndex;
				LtbFile->m_iFrame = 0;
				//MessageBox(hwnd, (LPCWSTR) ListItem, TEXT("Item Selected"), MB_OK); 
				SetFocus(hWndOpenGL);
			}
			else
				if ((HWND)lParam == hwndCombox2)
				{
					int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
						(WPARAM)0, (LPARAM)0);
					modeShowModel = ItemIndex;
				}
				else
					if ((HWND)lParam == hwndListMesh)
					{
						int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
							(WPARAM)0, (LPARAM)0);
						updateComboxLisSubMesh(ItemIndex);
						m_meshSelect = ItemIndex;

					}
					else
						if ((HWND)lParam == hwndListSubMesh)
						{
							int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
								(WPARAM)0, (LPARAM)0);
							m_subMeshSelect = ItemIndex;

						}



			break;

		}
		else
		{


			if (
				(HWND)lParam == hWndCheckBox)
			{

				isShowBone = !isShowBone;
				CheckDlgButton(subWindow, ID_CHECKBOX, isShowBone);
				break;
			}

			if (
				(HWND)lParam == hwndWepnView)
			{

				isViewMode = !isViewMode;
				CheckDlgButton(subWindow, ID_WPN_VIEW, isViewMode);
				break;
			}


			if ((HWND)lParam == hWndbLoadText)
			{
				if (LtbFile == NULL) break;
				char filename[MAX_PATH];
				OPENFILENAMEA ofn;
				ZeroMemory(&filename, sizeof(filename));
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFilter = "DTX Files\0*.dtx\0TGA Files\0 * .tga\0";
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrTitle = "Select a File";
				ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

				if (GetOpenFileNameA(&ofn))
				{

					int pWidth, pHeight;
					int tex = GenerateTexturesID();
					LtbFile->m_Mesh[m_meshSelect].SubMesh[m_subMeshSelect].textindex = tex;
					char msg[512];
					sprintf(msg, "%d", ofn.nFilterIndex);


					if (ofn.nFilterIndex == 1)
					{
						LoadDTXImage(filename, tex, &pWidth, &pHeight);
					}
					else
					{

						LoadTgaImage(filename, tex, &pWidth, &pHeight);
					}
				}
				break;
			}

			switch (LOWORD(wParam))

			{
			case ID_TOOL_UNZIP:

				if (GetOpenFileNameA(&ofn2))
				{
					char path[512];
					char path1[512];
					char path2[1024];
					memset(path1, 0, sizeof(path));
					memset(path, 0, sizeof(path));
					ExePath(path);
					// MessageBox(0, path, "", 0);

					sprintf(path1, "%stools\\unzip.exe", path);
					sprintf(path2, "\"%s\"", filename2);
					doExeFile(path1, path2, path);
				}
				break;
			case ID_TOOL_STUDIO_MDL:

				if (GetOpenFileNameA(&ofn2))
				{
					char path[512];
					char path1[512];
					char path2[1024];
					memset(path1, 0, sizeof(path));
					memset(path, 0, sizeof(path));
					ExePath(path);
					// MessageBox(0, path, "", 0);

					sprintf(path1, "%stools\\studiomdl.exe", path);
					sprintf(path2, "\"%s\"", filename2);
					doExeFile(path1, path2, path);
				}
				break;
			case ID_TOOL_ABOUT:
				MessageBoxW(hwnd, L"---------------------------------\nProgram: NEW SALL TOOLS \nAuthor: Giấy Nháp\nVersion:1.0.1\n---------------------------------\n", L"QUick About", 0);

				break;
			case ID_TOOL_SMD:
				msgAnimShow(hwnd);
				break;
			case ID_TOOL_TEX:
				msgConvertImage(hwnd);
				break;
			case ID_FILE_EXIT:
				wglMakeCurrent(hDC, NULL);
				wglDeleteContext(hRC);
				done = true;
				PostQuitMessage(0);
				PostQuitMessage(0);
				done = true;
				break;
			case ID_FILE_LOAD_LTB:
				wchar_t filename[MAX_PATH];
				OPENFILENAMEW ofn;
				ZeroMemory(&filename, sizeof(filename));
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFilter = L"LTB File\0*.LTB\0Wmdl File\0*.WMDL\0";
				ofn.lpstrFile = filename;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrTitle = L"Select a File";
				ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

				if (GetOpenFileNameW(&ofn))
				{
					if (ofn.nFilterIndex == 1)
					{
						LtbFile = new LtbLoader(filename);
						LtbFile->prepareMesh();
					}
					else
					{
					//	LtbFile = new gnLoadWMDL(filename);
					}

					LtbFile->m_iAnim = 0;
					LtbFile->m_iFrame = 0;

					updateConboxListAnim();
					updateComboxLisMesh();

					memset(openFileName, 0, sizeof(openFileName));
					for (int k = wcslen(filename) - 1; k > 0; k--)
						if (filename[k] == '\\')
						{
							wcscpy(openFileName, filename + k + 1);
							break;
						}

				

				}
				break;


			}
		}

	} break;
	case WM_MOUSEWHEEL:

		if ((short)HIWORD(wParam) < 0)
			eyse_postion[0] += eyse_postion[0] / 100.0;
		else
			eyse_postion[0] -= eyse_postion[0] / 100.0;
		break;
	case WM_MOUSEMOVE:
		mousemove(x, y);
		break;
	case WM_LBUTTONDOWN:
		mouse(0, 0, x, y);
		break;
	case WM_LBUTTONUP:
		mouse(0, 1, x, y);
		break;
	case WM_CREATE:

		hDC = GetDC(hWndOpenGL);
		g_HDC = hDC;
		SetupPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		initialise();
		SetTimer(hwnd, 2, 10, TimerProc); \
		//	meshText = TEX_GenerateID();
		//	LoadTgaImage("c://AI_BIGGORILLA.tga",meshText,&SizeW,&SizeH);


		break;
	case WM_DESTROY:
	case WM_CLOSE:
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		done = true;
		PostQuitMessage(0);
		PostQuitMessage(0);
		done = true;
		break;

	case WM_PAINT:
		hDC = BeginPaint(hwnd, &ps);

		//	 SetTextColor(hDC, RGB(but_loadColor[0], but_loadColor[1], but_loadColor[2]));
		//	 SetBkColor(hDC, RGB(0, 0, 0));
			 //	TextOutA(hDC, 0, 0, "[Load model]", 12);

		EndPaint(hwnd, &ps);

		return 0;
	case WM_SIZE:
		height = HIWORD(lParam);
		width = LOWORD(lParam);
		MoveWindow(hWndOpenGL, 0, 100, width, height, true);
		if (height == 0)
		{
			height = 1;
		}
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 1.0f, 40960.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		return 0;
		break;


	case WM_CTLCOLORSTATIC:
	{
		HBRUSH hBrush = CreateSolidBrush(RGB(230, 230, 230));

		DWORD CtrlID = GetDlgCtrlID((HWND)lParam); //Window Control ID
   // if (CtrlID == IDC_STATIC1) //If desired control

		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(0, 0, 0));
		SetBkColor(hdcStatic, RGB(230, 230, 230));
		return  (INT_PTR)hBrush;

	}
	break;

	case WM_HSCROLL:
	{

		mFpscontrolValue = SendMessage(track_bar, TBM_GETPOS, 0, 0);

		break;
	}

	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}

void CreateMainMenu(HWND hWnd)
{
	hMenu = CreateMenu();
	subFile = ADDPOPUPMENU(hMenu, "&File");
	ADDMENUITEM(subFile, ID_FILE_LOAD_LTB, "&Load Model file");
	ADDMENUITEM(subFile, ID_FILE_EXIT, "&Exit");
	subTool = ADDPOPUPMENU(hMenu, "&Tools");
//	ADDMENUITEM(subTool, ID_TOOL_SMD, "&Extract to SMD");
	//ADDMENUITEM(subTool, ID_TOOL_TEX, "&Convert Textures");
//	ADDMENUITEM(subTool, ID_TOOL_UNZIP, "&Unzip TryKid");
	//ADDMENUITEM(subTool, ID_TOOL_STUDIO_MDL, "&STUDIO MDL");

	helpMenu = ADDPOPUPMENU(hMenu, "&Help");
	ADDMENUITEM(helpMenu, ID_TOOL_ABOUT, "&About me");

	SetMenu(hWnd, hMenu);
}
HWND animMsg, hwndExtractMesh, hwndExtractAnim, hwndEXOk;
bool isExtractAnim = false;
bool isExtractMesh = false;

bool isReSize = true;
wchar_t pathSave[1024];

int CALLBACK BrowseForFolderCallback(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	char szPath[MAX_PATH];

	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
		break;

	case BFFM_SELCHANGED:
		if (SHGetPathFromIDList((LPITEMIDLIST)lp, szPath))
		{
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szPath);

		}
		break;
	}

	return 0;
}
LRESULT CALLBACK ChildWndProc2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	static int width, height;
	static WORD x, y;
	x = LOWORD(lParam);
	y = HIWORD(lParam);
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_COMMAND:
		if ((HWND)lParam == hwndResize)
		{
			isReSize = !isReSize;
			CheckDlgButton(hwndconvert, ID_CV_RESIZE, isReSize);

		}
		else if ((HWND)lParam == hwndEXOk)
		{

			char filename[MAX_PATH];
			OPENFILENAMEA ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "DTX Files\0*.dtx\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetOpenFileNameA(&ofn))
			{
				char source[512];

				memset(source, 0, 512);

				strcpy(source, ofn.lpstrFile);
				ofn.lpstrFilter = "BMP Files\0*.bmp\0";
				if (GetSaveFileNameA(&ofn))
				{

					convertDtxToBmp(source, filename, isReSize);
				}
			}
			DestroyWindow(animMsg);

		}
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	static int width, height;
	static WORD x, y;
	x = LOWORD(lParam);
	y = HIWORD(lParam);
	PAINTSTRUCT ps;
	switch (message)
	{
	case WM_COMMAND:
		if ((HWND)lParam == hwndExtractMesh)
		{
			isExtractMesh = !isExtractMesh;
			CheckDlgButton(hwnd, ID_EX_MESH, isExtractMesh);
		}
		if ((HWND)lParam == hwndExtractAnim)
		{
			isExtractAnim = !isExtractAnim;
			CheckDlgButton(hwnd, ID_EX_ANIM, isExtractAnim);
		}
		else if ((HWND)lParam == hwndEXOk)
		{
			BROWSEINFOW bi;

			LPITEMIDLIST pidl;
			BOOL bResult = FALSE;

			LPMALLOC pMalloc;

			if (SUCCEEDED(SHGetMalloc(&pMalloc)))
			{
				bi.hwndOwner = hwnd;
				bi.pidlRoot = NULL;
				bi.pszDisplayName = NULL;
				bi.lpszTitle = L"Test";
				bi.ulFlags = BIF_STATUSTEXT; //BIF_EDITBOX 
				bi.lpfn = BrowseForFolderCallback;
				bi.lParam = (LPARAM)"";

				pidl = SHBrowseForFolderW(&bi);
				if (pidl)
				{
					if (SHGetPathFromIDListW(pidl, pathSave))
					{

						/*wchar_t pathSaveFileMesh[1024];
						wsprintfW(pathSaveFileMesh, L"%s\\%s.smd", pathSave, openFileName);
						if (isExtractMesh) writeSmdMesh(LtbFile, pathSaveFileMesh);
						if (isExtractAnim)
						{
							for (int k = 0; k < LtbFile->m_nAnim; k++)
							{
								wchar_t pathSaveFileAnim[1024];
								wsprintfW(pathSaveFileAnim, L"%s\\%s.smd", pathSave, (wchar_t*)GetWC(LtbFile->m_Anim[k].name));
								writeSmdAnim(LtbFile, k, pathSaveFileAnim);

							}
						}
						DestroyWindow(animMsg);
						*/
					}
					pMalloc->Free(pidl);
					pMalloc->Release();

				}
			}

		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		CloseWindow(hwnd);
		break;
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}
void msgAnimShow(HWND parents)
{
	if (LtbFile == NULL) return;
	isExtractAnim = true;
	isExtractMesh = true;
	animMsg = CreateWindowExA(NULL, "SubClass",
		"SmdExtract",
		WS_POPUPWINDOW | WS_SYSMENU | WS_POPUP | WS_CAPTION,
		0, 0,
		200,
		200,
		parents, NULL, NULL, NULL);


	hwndExtractMesh = CreateWindow(TEXT("button"), TEXT("Extract Mesh"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 10, 150, 35,
		animMsg, (HMENU)ID_EX_MESH, NULL, NULL);
	CheckDlgButton(animMsg, ID_EX_MESH, isExtractMesh);
	hwndExtractAnim = CreateWindow(TEXT("button"), TEXT("Extract Anim"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 40, 150, 35,
		animMsg, (HMENU)ID_EX_ANIM, NULL, NULL);

	CheckDlgButton(animMsg, ID_EX_ANIM, isExtractAnim);

	hwndEXOk = CreateWindow(
		TEXT("BUTTON"), TEXT("OK")
		, WS_CHILD | WS_VISIBLE
		, 60, 100, 70, 30
		, animMsg, HMENU(ID_EX_PROCESS), NULL, NULL
	);

	ShowWindow(animMsg, SW_SHOW);
	UpdateWindow(animMsg);

}
void msgConvertImage(HWND parents)
{

	isExtractAnim = true;
	isExtractMesh = true;
	hwndconvert = CreateWindowExA(NULL, "SubClass2",
		"Convert DTX Image -> Bmp 256 colors",
		WS_POPUPWINDOW | WS_SYSMENU | WS_POPUP | WS_CAPTION,
		0, 0,
		200,
		200,
		parents, NULL, NULL, NULL);


	hwndResize = CreateWindow(TEXT("button"), TEXT("Resize to 512x512"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		20, 40, 150, 35,
		hwndconvert, (HMENU)ID_CV_RESIZE, NULL, NULL);

	CheckDlgButton(hwndconvert, ID_CV_RESIZE, isReSize);

	hwndEXOk = CreateWindow(
		TEXT("BUTTON"), TEXT("OK")
		, WS_CHILD | WS_VISIBLE
		, 60, 100, 70, 30
		, hwndconvert, HMENU(ID_EX_PROCESS), NULL, NULL
	);

	ShowWindow(hwndconvert, SW_SHOW);
	UpdateWindow(hwndconvert);

}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{


	WNDCLASSEXA windowClass;

	HWND    hwnd;
	MSG             msg;

	DWORD   dwExStyle;
	DWORD   dwStyle;
	RECT    windowRect;


	int width = 800;
	int height = 600;
	int bits = 32;

	windowRect.left = (long)0;
	windowRect.right = (long)width;
	windowRect.top = (long)0;
	windowRect.bottom = (long)height;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "MyClass";
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	WNDCLASSEXA windowClass2;
	windowClass2.cbSize = sizeof(WNDCLASSEX);
	windowClass2.style = CS_HREDRAW | CS_VREDRAW;
	windowClass2.lpfnWndProc = ChildWndProc;
	windowClass2.cbClsExtra = 0;
	windowClass2.cbWndExtra = 0;
	windowClass2.hInstance = NULL;
	windowClass2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass2.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass2.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));
	windowClass2.lpszMenuName = NULL;
	windowClass2.lpszClassName = "SubClass";
	windowClass2.hIconSm = LoadIcon(NULL, IDI_WINLOGO);



	WNDCLASSEXA windowClass3;
	windowClass3.cbSize = sizeof(WNDCLASSEX);
	windowClass3.style = CS_HREDRAW | CS_VREDRAW;
	windowClass3.lpfnWndProc = ChildWndProc2;
	windowClass3.cbClsExtra = 0;
	windowClass3.cbWndExtra = 0;
	windowClass3.hInstance = NULL;
	windowClass3.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass3.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass3.hbrBackground = CreateSolidBrush(RGB(230, 230, 230));
	windowClass3.lpszMenuName = NULL;
	windowClass3.lpszClassName = "SubClass2";
	windowClass3.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&windowClass3))
	{
		return 0;
	}
	if (!RegisterClassEx(&windowClass2))
	{
		return 0;
	}
	if (!RegisterClassEx(&windowClass))
	{
		return 0;
	}




	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;


	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);
	//	https://onedrive.live.com/?id=E38F9FC6490B29D9%21977&cid=E38F9FC6490B29D9

	hwnd = CreateWindowEx(NULL, "MyClass",
		"SALL TOOLS",
		dwStyle |
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL);


	CreateMainMenu(hwnd);

	hWndOpenGL = CreateWindowExA(NULL, "MyClass", "[GiayNhap] GL-windows",
		WS_CHILD | WS_VISIBLE,
		0, 100,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top - 100,//width, height,
		hwnd, NULL, hInstance, NULL);


	subWindow = CreateWindowExA(NULL, "MyClass", "",
		WS_CHILD | WS_VISIBLE,
		0, 0,
		windowRect.right - windowRect.left,
		100,//width, height,
		hwnd, NULL, hInstance, NULL);
	hWndEdit = CreateWindowEx(0, TEXT("Edit"), TEXT("20"),
		WS_CHILD | WS_VISIBLE | ES_NUMBER, 60, 60, 60,
		18, subWindow, HMENU(ID_EDIT), NULL, NULL);


	ShowWindow(subWindow, SW_SHOW);
	hWndListAnim = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | WS_VSCROLL,
		60, 10, 200, 200, subWindow, HMENU(ID_COMBOBOX), hInstance, NULL);

	CreateWindowEx(0, "STATIC", "Anim", WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU, 5, 10, 40, 20, subWindow, (HMENU)100, hInstance, NULL);
	CreateWindowEx(0, "STATIC", "FPS", WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU, 5, 40, 40, 20, subWindow, (HMENU)100, hInstance, NULL);
	CreateWindowEx(0, "STATIC", "Frame", WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU, 5, 60, 40, 20, subWindow, (HMENU)100, hInstance, NULL);
	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	track_bar = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control",
		WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
		57, 40, 200, 20, subWindow, (HMENU)123, NULL, NULL);
	SendMessage(track_bar, TBM_SETRANGE, TRUE, MAKELONG(0, 200));
	SendMessage(track_bar, TBM_SETPAGESIZE, 0, 10);
	SendMessage(track_bar, TBM_SETTICFREQ, 10, 0);
	SendMessage(track_bar, TBM_SETPOS, FALSE, mFpscontrolValue);

	CreateWindowEx(0, "STATIC", "Model: ", WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU, 290, 15, 40, 20, subWindow, (HMENU)100, hInstance, NULL);

	hWndCheckBox = CreateWindow(TEXT("button"), TEXT("Show bone"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		450, 5, 100, 35,
		subWindow, (HMENU)ID_CHECKBOX, hInstance, NULL);
	CheckDlgButton(subWindow, ID_CHECKBOX, isShowBone);

	hwndCombox2 = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | WS_VSCROLL,
		340, 10, 100, 200, subWindow, HMENU(ID_COMBOBOX), hInstance, NULL);
	SendMessage(hwndCombox2, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	SendMessageA(hwndCombox2, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Textures Shaded");
	SendMessageA(hwndCombox2, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"Smooth Shaded");
	SendMessageA(hwndCombox2, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)"WriteFrame");
	SendMessageA(hwndCombox2, CB_SETCURSEL, (WPARAM)0, (LPARAM)modeShowModel);

	CreateWindowEx(0, "STATIC", "Mesh: ", WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU, 290, 45, 40, 20, subWindow, (HMENU)100, hInstance, NULL);

	hwndListMesh = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | WS_VSCROLL,
		340, 40, 200, 200, subWindow, HMENU(ID_COMBOBOX), hInstance, NULL);
	CreateWindowEx(0, "STATIC", "Child: ", WS_CHILD | WS_VISIBLE | SS_LEFT | WS_SYSMENU, 290, 70, 40, 20, subWindow, (HMENU)100, hInstance, NULL);

	hwndListSubMesh = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | WS_VSCROLL,
		340, 65, 130, 200, subWindow, HMENU(ID_COMBOBOX), hInstance, NULL);
	hWndbLoadText = CreateWindow(
		TEXT("BUTTON"), TEXT("Texture")
		, WS_CHILD | WS_VISIBLE
		, 470, 65, 70, 25
		, subWindow, HMENU(ID_LOADTEXT), hInstance, NULL
	);
	hwndWepnView = CreateWindow(TEXT("button"), TEXT("Weapon view mode"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		550, 5, 200, 35,
		subWindow, (HMENU)ID_WPN_VIEW, hInstance, NULL);
	

	if (!hwnd)
	{
		return 0;
	}

	ShowWindow(hWndOpenGL, SW_SHOW);
	//UpdateWindow(hWndOpenGL);  

	done = false;

	ShowWindow(hwnd, SW_SHOW);  
	UpdateWindow(hwnd);
	//	

	hWndMain = hwnd;


	while (!done)
	{
		GetMessage(&msg, NULL, 0, 0);
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;

}

