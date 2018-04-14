#include "ImageProcessing.h"
#include "process.h"

HWND			hwnds;			//このファイル内で使うウインドウハンドル
LPBITMAPINFO	lpBInfo;		//RGB画像用のビットマップインフォ
LPBITMAPINFO	lpDepthBInfo;	//深度画像用のビットマップインフォ

long			nFrame;			//現在のフレーム番号
char			buf[256];		//文字列格納用配列
Mat				Cimg0(HEIGHT, WIDTH, CV_8UC1);//１つ前のフレーム画像
Mat				Gimg0(HEIGHT, WIDTH, CV_8UC1);
Mat				Rimg0(HEIGHT, WIDTH, CV_8UC1);


void SBImageProcessing(Mat *image)
{
	//初期化
	HDC hdc;
	hdc = GetDC(hwnds);
	Mat RGBimg0 = image->clone();
	Mat imgSub = image->clone();
	Mat RGBimg1 = image->clone();
	Rimg0 = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC1);//黒画像
	int gx, gy;
	double scale = 4.0;


	//画像処理
	Mat gray, smallImg(saturate_cast<int>(HEIGHT / scale), saturate_cast<int>(WIDTH / scale), CV_8UC1);
	int xm, ym, widthm, heightm;
	// グレースケール画像に変換
	cvtColor(RGBimg0, gray, CV_BGR2GRAY);
	// 処理時間短縮のために画像を縮小
	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	// 分類器の読み込み
	string cascadeName = "haarcascades/haarcascade_frontalface_default.xml"; // Haar-like
	string cascadeName2 = "haarcascades/haarcascade_smile.xml";
	CascadeClassifier cascade,cascade2;
	if (!cascade.load(cascadeName))
		Beep(1000, 1000);
	if (!cascade2.load(cascadeName2))
		Beep(1000, 1000);

	vector<Rect> faces,faces2;
	/// マルチスケール（顔）探索xo
	// 画像，出力矩形，縮小スケール，最低矩形数，（フラグ），最小矩形
	cascade.detectMultiScale(smallImg, faces,
		1.3, 1,
		CV_HAAR_SCALE_IMAGE,
		Size(30, 30));

	cascade2.detectMultiScale(smallImg, faces2,
		1.3, 1,
		CV_HAAR_SCALE_IMAGE,
		Size(30, 30));

	// 結果の描画
	vector<Rect>::const_iterator r = faces.begin();
	for (; r != faces.end(); ++r) {
		xm = int(r->x*scale);
		ym = int(r->y*scale);
		widthm = int((r->width)*scale);
		heightm = int((r->height)*scale);
		Rect roi_rect(xm, ym, widthm, heightm);
		Mat roi = imgSub(roi_rect);
		Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, roi_rect.x, 0.0, 1.0, roi_rect.y);
		rectangle(imgSub, Point(xm, ym), Point(xm+widthm, ym+heightm), Scalar(0, 0, 255), 3, 4);

		//笑顔検出
		const int smile_neighbors = (int)faces2.size();
		static int max_neighbors = -1;
		static int min_neighbors = -1;
		if (min_neighbors == -1) min_neighbors = smile_neighbors;
		max_neighbors = MAX(max_neighbors, smile_neighbors);
		double intensityZeroOne = ((double)smile_neighbors - min_neighbors) / (max_neighbors - min_neighbors + 1);
		if (intensityZeroOne>=0.06) {
			xm = int(r->x*scale);
			ym = int(r->y*scale);
			widthm = int((r->width)*scale);
			heightm = int((r->height)*scale);
			Rect roi_rect(xm, ym, widthm, heightm);
			Mat roi = imgSub(roi_rect);
			Mat mat = (Mat_<double>(2, 3) << 1.0, 0.0, roi_rect.x, 0.0, 1.0, roi_rect.y);
			rectangle(imgSub, Point(xm, ym), Point(xm + widthm, ym + heightm), Scalar(255, 0, 0), 3, 4);
		}


	}

	namedWindow("result", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	imshow("result", imgSub);
	//imshow("result", imgSub);



	Cimg0 = Gimg0.clone();
	ReleaseDC(hwnds, hdc);
	nFrame++;
}

void SBRelease(HWND hwnd){
}

void SBRGBtoGray(Mat *inimg, Mat *outimg){
	for (int y = 0; y < HEIGHT; y++){
		const uchar *inpx = inimg->ptr<uchar>(y);
		uchar *outpx = outimg->ptr<uchar>(y);
		for (int x = 0; x < WIDTH; x++){
			outpx[x] = saturate_cast<uchar>(cvR(inpx,x));
		}
	}
}

void SBHaikeiSabun(Mat *inimg, Mat *Backimg, Mat *outimg){//
	for (int y = 0; y < HEIGHT; y++){
		const uchar *inpx = inimg->ptr<uchar>(y);
		const uchar *bcpx = Backimg->ptr<uchar>(y);
		uchar *outpx = outimg->ptr<uchar>(y);
		for (int x = 0; x < WIDTH; x++){
			if ((inpx[x] - bcpx[x]) > 35 || ((inpx[x] - bcpx[x])) < -35){
				outpx[x] = saturate_cast<uchar>(255);
			}
		}
	}
}

//背景差分画像の重心をマーク
void SBDetectCenterGra(Mat	*inimg, int *gx, int *gy){
	int count = 0;
	*gx = 0, *gy = 0;

	for (int y = 0; y < HEIGHT; y++){
		const uchar *inpx = inimg->ptr<uchar>(y);
		for (int x = 0; x < WIDTH; x++){
			if (inpx[x] == 255){
				*gx += x;
				*gy += y;
				count++;
			}
		}
	}
	if (count != 0 && 100< count && 20000>count){//ホワイトバランスの調整時ははじくのと、ノイズを除去
		*gx = int(*gx / count);
		*gy = int(*gy / count);
	}
}

void SBPointMark(Mat *inimg, int *x, int *y, int r, int g, int b){
	if (*x > 0 && *y > 0 && * x < WIDTH && *y < HEIGHT){
		for (int i = -1; i <= 1; i++){
			for (int j = -1; j <= 1; j++){
				R(inimg->data, (*x + i), (*y + j)) = r;
				G(inimg->data, (*x + i), (*y + j)) = g;
				B(inimg->data, (*x + i), (*y + j)) = b;
			}
		}
	}
}

void	FreaqFilta(){

	int M = getOptimalDFTSize(img_gray.rows);
	int N = getOptimalDFTSize(img_gray.cols);
	Mat padded;
	copyMakeBorder(img_gray, padded, 0, M - img_gray.rows, 0, N - img_gray.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg);

	dft(complexImg, complexImg);

	// compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
	split(complexImg, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat mag = planes[0];
	mag += Scalar::all(1);
	log(mag, mag);

	// crop the spectrum, if it has an odd number of rows or columns
	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));

	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	// rearrange the quadrants of Fourier image
	// so that the origin is at the image center
	Mat tmp;
	Mat q0(mag, Rect(0, 0, cx, cy));
	Mat q1(mag, Rect(cx, 0, cx, cy));
	Mat q2(mag, Rect(0, cy, cx, cy));
	Mat q3(mag, Rect(cx, cy, cx, cy));

	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(mag, mag, 0, 1, CV_MINMAX);

	imshow("spectrum magnitude", mag);
}
void	FaceDetect(){
	// グレースケール画像に変換
	cvtColor(img_cpy, img_gray, CV_BGR2GRAY);

	// 分類器の読み込み
	string cascadeName = "haarcascades/haarcascade_frontalface_default.xml"; // Haar-lik

	CascadeClassifier cascade;
	if (!cascade.load(cascadeName))
		Beep(1000, 1000);
	vector<Rect> faces;
	// 画像，出力矩形，縮小スケール，最低矩形数，（フラグ），最小矩形
	cascade.detectMultiScale(img_gray, faces,
		1.1, 2,
		CV_HAAR_SCALE_IMAGE);

	// 結果の描画
	vector<Rect>::const_iterator r = faces.begin();
	for (; r != faces.end(); ++r) {
		rectangle(img_save, Point(int(r->x), int(r->y)), Point(int((r->width + r->x)), int((r->y + r->height))), Scalar(0, 0, 255), 3, 4);
	}

	namedWindow("result", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	imshow("result", img_save);

}
