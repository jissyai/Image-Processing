#include "ImageProcessing.h"

int SBGetSource(HWND hwnd)
{
	//�J�����̏�����
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
		// �i4�j�J��������1�t���[�����̉摜�f�[�^���擾���āA�ϐ�frame�Ɋi�[����
		cap >> frame;
		if (frame.empty()) break;//���s�Ń��[�v�I��
		flip(frame,frame, 1);//���E���]
		//�摜����
		SBImageProcessing(&frame);

		//q�L�[�����ŏI��
		auto key = cv::waitKey(1);
		if (key == 'q') break;
	}
	cv::destroyAllWindows();

	return 0;
}