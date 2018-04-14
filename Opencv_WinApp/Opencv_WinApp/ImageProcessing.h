/******************************************************************
"ImageProcessing.h"
�@�@�@�摜�����֐��Ƃ��͂����ɏ���
   2015/01/19  ���}
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
extern HWND			hwnds;			//���̃t�@�C�����Ŏg���E�C���h�E�n���h��
extern LPBITMAPINFO	lpBInfo;		//RGB�摜�p�̃r�b�g�}�b�v�C���t�H
extern LPBITMAPINFO	lpDepthBInfo;	//�[�x�摜�p�̃r�b�g�}�b�v�C���t�H

extern long			nFrame;			//���݂̃t���[���ԍ�
extern char			buf[256];		//������i�[�p�z��

/*RGB�f�[�^�i�[�p�z��*/
extern BYTE			*RGBBuffer;		//�Î~��C���[�W(DIB�F�f�[�^)�p�̈�ւ̃|�C���^
extern BYTE			*RGBimg0;		//�ŏ��Ɋi�[����RGB�摜�f�[�^�i�摜�����ɉe������Ȃ��j


extern Mat img_ori, img_cpy, img_save, img_gray;

void	SBImageProcessing(Mat*);
void	SBRelease(HWND);
//winmain
char *SelectFile(HWND hwnd);

//�摜����
void	SBRGBtoGray(Mat *, Mat *);
void	SBHaikeiSabun(Mat *, Mat *, Mat *);
void	SBDetectCenterGra(Mat *, int *, int *);
void	SBPointMark(Mat *, int *, int *, int, int, int);
void	FreaqFilta();
void	FaceDetect();
