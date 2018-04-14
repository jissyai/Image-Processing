#include "ImageProcessing.h"
#include "resource1.h"

// Global variables
//save
SYSTEMTIME STime;

Mat img_ori, img_cpy,img_save,img_gray;

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("ImageProcess");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int saveFileNameBox(HWND hWnd);      //GetSaveFileName�֐��p
TCHAR szSaveFileName[MAX_PATH];


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCSTR)IDR_MENU1;///////////////////////
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800,
		60,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int ret;
	const std::string base = "lenna";
	hdc = GetDC(hWnd);
	char	filename[256];
	GetLocalTime(&STime);//�ۑ��t�@�C�����p
	int i = 0;
	double gamma = 2.0;
	Mat lut_mat = Mat::zeros(1, 256, CV_8UC1);


	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case LOAD:
			//�I�[�v���t�@�C���̌���
			strcpy_s(filename, SelectFile(hWnd));
			if (strcmp(filename, "notSelect") == 0) return 0;
			img_ori = imread(filename, 1);
			imshow("Load Img", img_ori);
			img_cpy = img_ori.clone();
			img_save = img_ori.clone();
			return 0;

		case SAVE:
			ret = saveFileNameBox(hWnd);
			imwrite(szSaveFileName, img_save);
			return 0;

		case CORRECTION:
			img_cpy = img_ori.clone();
			uchar Lut[256];
			//�K���}�␳�e�[�u���̍쐬  
			for (i = 0; i < 256; i++){
				Lut[i] = (int)(pow((double)i / 255.0, 1.0 / gamma) * 255.0);
			}
			//Mat�֕ϊ�  
			lut_mat = Mat(1, 256, CV_8UC1, Lut);
			//���b�N�A�b�v�e�[�u���ϊ�  
			LUT(img_cpy, lut_mat, img_save);
			imshow("Gamma correction", img_save);
			return 0;

		case Binarization:
			img_cpy = img_ori.clone();
			cvtColor(img_cpy, img_gray, CV_RGB2GRAY);
			threshold(img_gray, img_save, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
			imshow("Binarization", img_save);
			return 0;

		case MEDIAN:
			//���f�B�A��
			img_cpy = img_ori.clone();

			medianBlur(img_cpy, img_save, 11);
			imshow("Median filta", img_save);
			return 0;

		case SOBEL:
			//soble
			img_cpy = img_ori.clone();
			cvtColor(img_cpy, img_gray, CV_RGB2GRAY);
			Sobel(img_gray, img_save, CV_32F,1,1);
			convertScaleAbs(img_save, img_save, 1, 0);
			imshow("Sobel filta", img_save);
			return 0;

		case LAPLACIAN:
			//���v���V�A��
			img_cpy = img_ori.clone();
			cvtColor(img_cpy, img_gray, CV_RGB2GRAY);
			Laplacian(img_gray, img_save, CV_32F, 3);
			convertScaleAbs(img_save, img_save, 1, 0);
			imshow("Laplacian filta", img_save);
			return 0;


		case FREAQFILTA:
			img_cpy = img_ori.clone();
			cvtColor(img_cpy, img_gray, CV_RGB2GRAY);
			FreaqFilta();
			return 0;

		case FACEDETECT:
			img_cpy = img_ori.clone();
			FaceDetect();
			return 0;

		case CAMERA:
			ret = SBGetSource(hWnd);
			return 0;

		default:
			return 0;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

/*****************************************************************************
*  �I�[�v������t�@�C����I��
******************************************************************************/
char *SelectFile(HWND hWnd)
{
	char	filename[128];
	OPENFILENAME fname;
	static char fn[256];
	char	ffilter[] = { 'a', 'l', 'l', '\0', '*', '.', '*', '\0' };

	memset(&fname, 0, sizeof(OPENFILENAME));
	fname.lStructSize = sizeof(OPENFILENAME);
	fname.hwndOwner = hWnd;
	fname.lpstrFilter = ffilter;
	fname.nFilterIndex = 1;
	fname.lpstrFile = fn;
	fname.nMaxFile = sizeof(fn);
	fname.lpstrFileTitle = filename;
	fname.nMaxFileTitle = sizeof(filename) - 1;
	fname.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if (!GetOpenFileName(&fname)){
		strcpy_s(filename, "notSelect");//�t�@�C�����I�΂�Ȃ�������
	}
	return filename;
}//SelectFile

/*-----------------------------------
�ۑ��t�@�C�������擾����
-------------------------------------*/
int saveFileNameBox(HWND hWnd)
{
	int i;
	OPENFILENAME ofn;

	memset(&ofn, 0, sizeof(OPENFILENAME));  // �\���̂̏�����

	ofn.lStructSize = sizeof(OPENFILENAME); //�\���̂̃T�C�Y
	ofn.hwndOwner = hWnd;   //�e�E�B���h�E�̃n���h��
	ofn.lpstrFilter = TEXT("All(*.*)\0*.*\0JPEG�t�@�C��(*.jpg)\0*.jpg\0PNG�t�@�C��(*.png)\0*.png\0\0"); //�t�B���^�[
	ofn.lpstrFile = szSaveFileName;     // �t���p�X�t�@�C����
	ofn.nMaxFile = sizeof(szSaveFileName);      //�t���p�X�t�@�C���̃T�C�Y
	ofn.lpstrFileTitle = szSaveFileName;       //�t�@�C����
	ofn.nMaxFileTitle = sizeof(szSaveFileName);        //�t�@�C�����̃T�C�Y
	ofn.Flags = OFN_OVERWRITEPROMPT;        // �㏑���m�F�_�C�A���O�̕\��
	ofn.lpstrDefExt = TEXT("*");    //�f�t�H���g�̊g���q
	ofn.lpstrTitle = TEXT("�摜�t�@�C���̕ۑ�");        //�^�C�g���o�[�ɕ\������镶����

	i = GetSaveFileName(&ofn);        //�t�@�C������t���ĕۑ��_�C�A���O�{�b�N�X
	if (i == 0) {      //OK�{�^����������Ă��Ȃ���
		return 1;
	}

//	MessageBox(hWnd, szSaveFileName, TEXT("debug"), MB_OK);

	return 0;
}