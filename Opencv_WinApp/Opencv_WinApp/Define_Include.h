#pragma once

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
//#include <tchar.h>

#include <windows.h>     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <tchar.h>
#include <ctype.h>
#include <iostream>
//---------------------------------------------------------------------------
// OpenCV
//---------------------------------------------------------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/opencv.hpp>

// バージョン取得
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)

// ビルドモード
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif

// ライブラリのリンク（不要な物はコメントアウト）
#pragma comment(lib, "opencv_calib3d"     CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_contrib"     CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_core"        CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_features2d"  CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_flann"       CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_gpu"         CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_highgui"     CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_imgproc"     CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_legacy"      CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_ml"          CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_nonfree"     CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_objdetect"   CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_ocl"         CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_photo"       CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_stitching"   CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_superres"    CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_ts"          CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_video"       CV_VERSION_STR  CV_EXT_STR)
#pragma comment(lib, "opencv_videostab"   CV_VERSION_STR  CV_EXT_STR)
//
using namespace std;
using namespace cv;

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define  WIDTH			640
#define  HEIGHT			480
#define  R(img, x, y)  (img)[(x+(y)*WIDTH)*3+2]
#define  G(img, x, y)  (img)[(x+(y)*WIDTH)*3+1]
#define  B(img, x, y)  (img)[(x+(y)*WIDTH)*3]
#define  cvR(px, x)  (px)[x*3+2]
#define  cvG(px, x)  (px)[x*3+1]
#define  cvB(px, x)  (px)[x*3]

#define  V(img, x, y)  (img)[(x+(y)*WIDTH)*3+2]
#define  S(img, x, y)  (img)[(x+(y)*WIDTH)*3+1]
#define  H(img, x, y)  (img)[(x+(y)*WIDTH)*3]

#define  Y(img, x, y)  (img)[x+(y)*WIDTH]


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int SBGetSource(HWND hWnd);