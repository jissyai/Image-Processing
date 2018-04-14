#include "ImageProcessing.h"

int SBGetSource(HWND hwnd)
{
	//カメラの初期化
	int camera_id = 0;
	VideoCapture cap(CV_CAP_DSHOW + camera_id);
	if (!cap.isOpened())
		return -1;
	cap.set(CV_CAP_PROP_FPS, 30.0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
	Mat frame;

	for (;;)
	{
		// （4）カメラから1フレーム分の画像データを取得して、変数frameに格納する
		cap >> frame;
		if (frame.empty()) break;//失敗でループ終了
		flip(frame,frame, 1);//左右反転
		//画像処理
		SBImageProcessing(&frame);

		//qキー押下で終了
		auto key = cv::waitKey(1);
		if (key == 'q') break;
	}
	cv::destroyAllWindows();

	return 0;
}