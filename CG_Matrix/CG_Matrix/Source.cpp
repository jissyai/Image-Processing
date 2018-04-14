/*==================================================*/
// CG法による穴埋め
/*==================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#define N    256
#define TMAX 100000
#define EPS  (1.0e-10)

typedef unsigned char BYTE;

//int count = 0;
//typedef double Vector[N*N-count];       // ベクトル
typedef double Matrix[N][N];    // 行列
//typedef double Matrix2[N*N-count][N*N-count];    // 行列

double M;

using namespace std;
vector<double> b(255,0.0);
vector<double> x(255,0.0);
vector<double> y(255,0.0);
vector<vector<double>> a(255, vector<double>(255,0.0));

// ベクトル初期化
void init_vector(vector<double> x)
{
	int i;
	for (i = 0; i < M; i++) {
		x[i] = 1.0;
	}
}

// ベクトルに行列を作用 y = Ax
void vector_x_matrix(vector<double> y, vector<vector<double>> a, vector<double> x)
{
	int    i, j;
	double vxm;
	for (i = 0; i < M; i++) {
		vxm = 0;
		for (j = 0; j < M; j++) {
			vxm += a[i][j] * x[j];
		}
		y[i] = vxm;
	}
}

// 内積を計算
double dot_product(vector<double> x, vector<double> y)
{
	int    i;
	double dot_p = 0;
	for (i = 0; i < M; i++) {
		dot_p += x[i] * y[i];
	}
	return dot_p;
}

// ベクトルノルムを計算
// ベクトルノルム := sgm(0〜N-1)|x[i]|
double vector_norm(vector<double> x)
{
	int    i;
	double norm = 0;
	for (i = 0; i < M; i++) {
		norm += fabs(x[i]);
	}
	return norm;
}

// CG法
void cg_method(vector<vector<double>> a, vector<double> x, vector<double> b, Matrix A)
{
	vector<double> p(M,0.0);   // 探索方向ベクトル
	vector<double> r(M,0.0);   // 残差ベクトル
	vector<double> ax(M,0.0);
	vector<double> ap(M,0.0);
	int           i, iter;

	FILE *fp;
	char str[256];

	// Axを計算
	//vector_x_matrix(ax, a, x);
	int    j,n,m;
	double vxm;
	for (i = 0; i < M; i++) {
		vxm = 0;
		for (j = 0; j < M; j++) {
			vxm += a[i][j] * x[j];
		}
		ax[i] = vxm;
	}

	// pとrを計算 p = r := b - Ax
	for (i = 0; i < M; i++) {
		p[i] = b[i] - ax[i];
		r[i] = p[i];
	}

	// 反復計算
	for (iter = 1; iter < TMAX; iter++) {
		double alpha, beta, err = 0;

		// alphaを計算
		//vector_x_matrix(ap, a, p);
		for (i = 0; i < M; i++) {
			vxm = 0;
			for (j = 0; j < M; j++) {
				vxm += a[i][j] * p[j];
			}
			ap[i] = vxm;
		}
		double dot_p1 = 0,dot_p2 = 0;
		for (i = 0; i < M; i++) {
			dot_p1 += r[i] * r[i];
			dot_p2 += p[i] * ap[i];
		}
		//alpha = +dot_product(p, r) / dot_product(p, ap);
		alpha = dot_p1 / dot_p2;

		for (i = 0; i < M; i++) {
			x[i] += alpha*p[i];
			r[i] -= alpha*ap[i];
		}

		double norm = 0;
		for (i = 0; i < M; i++) {
			norm += r[i]*r[i];
		}
		err = sqrt(norm);   // 誤差を計算
		printf("LOOP : %d\t Error : %g\n", iter, err);
		if (EPS > err) break;

		// EPS < err ならbetaとpを計算してループを継続
		dot_p1 = 0;
		dot_p2 = 0;
		for (i = 0; i < M; i++) {
			dot_p1 += r[i] * ap[i];
			dot_p2 += p[i] * ap[i];
		}
		//beta = -dot_product(r, ap) / dot_product(p, ap);
		beta = -1.0*(dot_p1 / dot_p2);
		for (i = 0; i < M; i++) {
			p[i] = r[i] + beta*p[i];
		}
	}

	/*printf("###Answer###\n");
	for (i = 0; i < M; i++) {
		printf("x[%d] = %2g\n", i, x[i]);
	}*/
	n = 0;
	m = 0;
	/*printf("###Original###\n");
	int cnt = 0;
	j = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("% 3.2f, ", A[i][j]);
			m++;
			if (m % N == 0) {
				printf("\n");
			}
		}
	}*/
	ofstream ofs("C:/Users/student/Documents/Visual Studio 2015/Projects/CG_Matrix/CG_Matrix/matrix_closing.csv");
	m = 0;
	printf("###Closing###\n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (A[i][j] == 1.0) {
				/*if (190.1<= x[n] && x[n] < 205.0) {
					x[n] = 205;
				}
				if (x[n] < 190.1) {
					x[n] = 0;
				}*/
				/*if (x[n] < 205) {
					x[n] = 205;
				}*/
				A[i][j] = x[n];
				n++;
			}
			if (A[i][j] != 0) {
				A[i][j] = A[i][j] + 203;
			}
			if (A[i][j] < 205) {
				A[i][j] = 0;
			}
			/*printf("% 3.2f, ", A[i][j]);
			m++;
			if (m % N == 0) {
				printf("\n");
			}*/
			ofs << (int)A[i][j] << ",";
		}
		ofs << endl;
	}
}

void make_matrix(Matrix A) {
	int    i, n1, m1, n2, m2;
	double k;
	bool flag;
	n2 = 0;
	m2 = 0;

	for (n1 = 0; n1 < N; n1++) {
		for (m1 = 0; m1 < N; m1++) {

			flag = true;
			if (A[n1][m1] != 1.0) {
				flag = false;
			}

			bool flag1 = false;	//n-1フラグ
			bool flag2 = false;	//n+1フラグ
			bool flag3 = false;	//m-1フラグ
			bool flag4 = false; //m+1フラグ
			k = 4.0;
			if (flag == true) {
				if (n1 - 1 < 0) {
					k = k - 1.0;
					flag1 = true;
				}
				if (n1 + 1 == N) {
					k = k - 1.0;
					flag2 = true;
				}
				if (m1 - 1 < 0) {
					k = k - 1.0;
					flag3 = true;
				}
				if (m1 + 1 == N) {
					k = k - 1.0;
					flag4 = true;
				}
				if (flag1 == true && flag2 == false && flag3 == true && flag4 == false) {	//左上
					a[n2][m2] = k*A[n1][m1];
					if (m2 + 1 < M) 
						a[n2][m2 + 1] = -A[n1][m1 + 1];
					if (m2 + 4 < M)
						a[n2][m2 + 4] = -A[n1 + 1][m1];

				}
				if (flag1 == true && flag2 == false && flag3 == false && flag4 == false) {	//1行目2,3列
					a[n2][m2] = k*A[n1][m1];
					if (m2 - 1 >= 0)
						a[n2][m2 - 1] = -A[n1][m1 - 1];
					if (m2 + 1 < M)
						a[n2][m2 + 1] = -A[n1][m1 + 1];
					if (m2 + 4 < M)
						a[n2][m2 + 4] = -A[n1 + 1][m1];

				}
				if (flag1 == true && flag2 == false && flag3 == false && flag4 == true) {	//右上
					a[n2][m2] = k*A[n1][m1];
					if (m2 - 1 >= 0)
						a[n2][m2 - 1] = -A[n1][m1 - 1];
					if (m2 + 4 < M)
						a[n2][m2 + 4] = -A[n1 + 1][m1];
				}
				if (flag1 == false && flag2 == false && flag3 == true && flag4 == false) {	//2,3行目1列
					a[n2][m2] = k*A[n1][m1];
					if (m2 + 1 < M)
						a[n2][m2 + 1] = -A[n1][m1 + 1];
					if (m2 - 4 >= 0)
						a[n2][m2 - 4] = -A[n1 - 1][m1];
					if (m2 + 4 < M)
						a[n2][m2 + 4] = -A[n1 + 1][m1];
				}
				if (flag1 == false && flag2 == true && flag3 == true && flag4 == false) {	//左下
					a[n2][m2] = k*A[n1][m1];
					if (m2 + 1 < M)
						a[n2][m2 + 1] = -A[n1][m1 + 1];
					if (m2 - 4 >= 0)
						a[n2][m2 - 4] = -A[n1 - 1][m1];
				}
				if (flag1 == false && flag2 == true && flag3 == false && flag4 == false) {	//4行目2,3列
					a[n2][m2] = k*A[n1][m1];
					if (m2 - 1 >= 0)
						a[n2][m2 - 1] = -A[n1][m1 - 1];
					if (m2 + 1 < M)
						a[n2][m2 + 1] = -A[n1][m1 + 1];
					if (m2 - 4 >= 0)
						a[n2][m2 - 4] = -A[n1 - 1][m1];
				}
				if (flag1 == false && flag2 == true && flag3 == false && flag4 == true) {	//右下
					a[n2][m2] = k*A[n1][m1];
					if (m2 - 1 >= 0)
						a[n2][m2 - 1] = -A[n1][m1 - 1];
					if (m2 - 4 >= 0)
						a[n2][m2 - 4] = -A[n1 - 1][m1];
				}
				if (flag1 == false && flag2 == false && flag3 == false && flag4 == true) {	//2,3行目4列
					a[n2][m2] = k*A[n1][m1];
					if (m2 - 1 >= 0)
						a[n2][m2 - 1] = -A[n1][m1 - 1];
					if (m2 - 4 >= 0)
						a[n2][m2 - 4] = -A[n1 - 1][m1];
					if (m2 + 4 < M)
						a[n2][m2 + 4] = -A[n1 + 1][m1];

				}
				if (flag1 == false && flag2 == false && flag3 == false && flag4 == false) {	//真ん中
					a[n2][m2] = k*A[n1][m1];
					if (m2 - 1 >= 0)
						a[n2][m2 - 1] = -A[n1][m1 - 1];
					if (m2 + 1 < M)
						a[n2][m2 + 1] = -A[n1][m1 + 1];
					if (m2 - 4 >= 0)
						a[n2][m2 - 4] = -A[n1 - 1][m1];
					if (m2 + 4 < M)
						a[n2][m2 + 4] = -A[n1 + 1][m1];
				}

				if (A[n1][m1] != 1.0) {
					b[n2] += A[n1][m1];
					a[n2][m2] = 0.0;
				}
				if (m1 - 1 >= 0) {
					if (A[n1][m1 - 1] != 1.0) {
						b[n2] += A[n1][m1 - 1];
						if(m2 - 1 >= 0)
							a[n2][m2 - 1] = 0.0;
					}
				}
				if (m1 + 1 < N) {
					if (A[n1][m1 + 1] != 1.0) {
						b[n2] += A[n1][m1 + 1];
						if (m2 + 1 < M)
							a[n2][m2 + 1] = 0.0;
					}
				}
				if (n1 - 1 >= 0) {
					if (A[n1 - 1][m1] != 1.0) {
						b[n2] += A[n1 - 1][m1];
						if (m2 - 4 >= 0)
							a[n2][m2 - 4] = 0.0;
					}
				}
				if (n1 + 1 < N) {
					if (A[n1 + 1][m1] != 1.0) {
						b[n2] += A[n1 + 1][m1];
						if (m2 + 4 < M)
							a[n2][m2 + 4] = 0.0;
					}
				}

				n2++;
				m2++;
			}
		}
	}
}

int main(void)
{
	/*Matrix A = { {10.0, 8.0, 6.0, 4.0 },
				 { 8.0, 1.0, 1.0, 2.0 },
				 { 6.0, 1.0, 1.0, 0.0 },
				 { 4.0, 2.0, 0.0, 0.0 } };*/
	Matrix A = {};
	Matrix B = {};
	Matrix C = {};

	ifstream ifs("C:/Users/student/Documents/Visual Studio 2015/Projects/CG_Matrix/CG_Matrix/matrix.csv");
	string str;
	int j = 0;
	while (getline(ifs, str))
	{
		string token;
		istringstream stream(str);
		//1行のうち、文字列とコンマを分解する
		while (getline(stream, token, ','))
		{
			//すべて文字列として読み込まれるため
			//数値は変換が必要

			A[j / N][j % N] = stof(token.c_str());
			if (A[j / N][j % N] > 1) {
				A[j / N][j % N] = A[j / N][j % N] - 203;
			}
			if (A[j / N][j % N] != 0) {
				B[j / N][j % N] = 1;
			}
			else {
				B[j / N][j % N] = 0;
			}
			j++;
		}
	}

	int    i,l,n1,m1,n2,m2;
	double k;

	//Closing(B);
	for (l = 0; l < 20; l++) {
		for (i = 1; i<N - 1; i++) {
			for (j = 1; j<N - 1; j++) {
				if (B[i][j] == 1)
					C[i][j] = 1;
				else if (B[i - 1][j] == 1 || B[i - 1][j + 1] == 1 || B[i][j + 1] == 1 || B[i + 1][j + 1] == 1 ||
					B[i + 1][j] == 1 || B[i + 1][j - 1] == 1 || B[i][j - 1] == 1 || B[i - 1][j - 1] == 1)
					C[i][j] = 1;
				else 	C[i][j] = 0;
			}
		}
		memcpy(B, C, sizeof(C));
	}
	//収縮処理
	for (l = 0; l < 20; l++) {
		for (i = 1; i<N - 1; i++) {
			for (j = 1; j<N - 1; j++) {
				if (B[i][j] == 0)
					C[i][j] = 0;
				else if (B[i - 1][j] == 0 || B[i - 1][j + 1] == 0 || B[i][j + 1] == 0 || B[i + 1][j + 1] == 0 ||
					B[i + 1][j] == 0 || B[i + 1][j - 1] == 0 || B[i][j - 1] == 0 || B[i - 1][j - 1] == 0)
					C[i][j] = 0;
				else 	C[i][j] = 1;
			}
		}
		memcpy(B, C, sizeof(C));
	}

	int cnt = 0;
	for (n1 = 0; n1 < N; n1++) {
		for (m1 = 0; m1 < N; m1++) {
			if (B[n1][m1] == 1 && A[n1][m1] == 0.0) {
				A[n1][m1] = 1.0;
			}
			if (A[n1][m1] != 1.0) {
				cnt++;
			}
		}
	}

	M = N*N - cnt;
	b.resize(M);
	x.resize(M);
	y.resize(M);
	a.resize(M);
	for (i = 0; i < M; i++) {
		a[i].resize(M,0.0);
	}

	//vector<double> b(M, 0.0);
	//それぞれの要素に節点方程式を適用
	//行列aは正定値対象行列
	//vector<vector<double>> a{ {M,M}, {0.0,0.0} };

	make_matrix(A);

	// 初期化
	//init_vector(x);
	for (i = 0; i < M; i++) {
		x[i] = 1.0;
		y[i] = 0.0;
	}

	i = 0;
	/*printf("###Matrix a###\n");
	for (n2 = 0; n2 < M; n2++) {
		for (m2 = 0; m2 < M; m2++) {
			printf("% 3.1f, ", a[n2][m2]);
			i++;
			if (i % (int)M == 0) {
				printf("\n");
			}
		}
	}

	printf("###Vector b###\n");
	for (i = 0; i < M; i++) {
		printf("b[%d] = %2g\n", i, b[i]);
	}*/

	// CG法でAx=bを解く
	cg_method(a, x, b, A);

	/*printf("###Calc End.###\n");
	for (i = 0; i < M; i++) {
		printf("x[%d] = %2g\n", i, x[i]);
	}*/

	return 0;
}
