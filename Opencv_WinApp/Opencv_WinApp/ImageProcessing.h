/******************************************************************
"ImageProcessing.h"
　　　画像処理関数とかはここに書く
   2015/01/19  今枝
   **********************************************************************/
#pragma once
//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "Define_Include.h"


//---------------------------------------------------------------------------
// Globals
//---------------------------------------------------------------------------
extern HWND			hwnds;			//このファイル内で使うウインドウハンドル
extern LPBITMAPINFO	lpBInfo;		//RGB画像用のビットマップインフォ
extern LPBITMAPINFO	lpDepthBInfo;	//深度画像用のビットマップインフォ

extern long			nFrame;			//現在のフレーム番号
extern char			buf[256];		//文字列格納用配列

/*RGBデータ格納用配列*/
extern BYTE			*RGBBuffer;		//静止画イメージ(DIB色データ)用領域へのポインタ
extern BYTE			*RGBimg0;		//最初に格納するRGB画像データ（画像処理に影響されない）


extern Mat img_ori, img_cpy, img_save, img_gray;

void	SBImageProcessing(Mat*);
void	SBRelease(HWND);
//winmain
char *SelectFile(HWND hwnd);

//画像処理
void	SBRGBtoGray(Mat *, Mat *);
void	SBHaikeiSabun(Mat *, Mat *, Mat *);
void	SBDetectCenterGra(Mat *, int *, int *);
void	SBPointMark(Mat *, int *, int *, int, int, int);
void	FreaqFilta();
void	FaceDetect();
