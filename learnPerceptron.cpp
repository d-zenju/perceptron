#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

vector<double> w;
double rho = 0.05;

int main()
{
	ifstream ifs("c:/temp/ground_truth.txt");
	string str;

	int data[512];
	vector<Mat> images;

	if (ifs.fail()){
		cerr << "failed to open" << endl;
		return -1;
	}

	int count = 0;

	for (int i = 0; i < 512; i++){
		stringstream ss;
		ss << "c:/temp/" << i << ".bmp";
		cout << ss.str() << endl;
		Mat img = imread(ss.str(), 0);
		images.push_back(img);
	}

	while (getline(ifs, str)){
		if (str == "1"){
			data[count] = 1;
		}
		else{
			data[count] = 0;
		}

		count++;
	}
	ifs.close();

	for (int i = 0; i < 10; i++){
		w.push_back(1);
	}

	//learning process
	for (int i = 0; i < 512; i++){
		int gt = data[i];

		vector<double> x;
		x.push_back(images[i].at<uchar>(0, 0));
		x.push_back(images[i].at<uchar>(0, 1));
		x.push_back(images[i].at<uchar>(0, 2));
		x.push_back(images[i].at<uchar>(1, 0));
		x.push_back(images[i].at<uchar>(1, 1));
		x.push_back(images[i].at<uchar>(1, 2));
		x.push_back(images[i].at<uchar>(2, 0));
		x.push_back(images[i].at<uchar>(2, 1));
		x.push_back(images[i].at<uchar>(2, 2));
		x.push_back(-1);

		double inner_product =
			w[0] * images[i].at<uchar>(0, 0) + w[1] * images[i].at<uchar>(0, 1) + w[2] * images[i].at<uchar>(0, 2)
			+ w[3] * images[i].at<uchar>(1, 0) + w[4] * images[i].at<uchar>(1, 1) + w[5] * images[i].at<uchar>(1, 2)
			+ w[6] * images[i].at<uchar>(2, 0) + w[7] * images[i].at<uchar>(2, 1) + w[8] * images[i].at<uchar>(2, 2)
			+ w[9] * -1;

		if (gt == 1 && inner_product < w[9]){
			// w - px
			for (int i = 0; i < w.size(); i++){
				w[i] = w[i] + rho * x[i];
			}
		}
		else if (gt == 0 && inner_product > w[9]){
			// w + px
			for (int i = 0; i < w.size(); i++){
				w[i] = w[i] - rho * x[i];
			}
		}

	}

	ofstream ofs("C:\\temp\\weight.txt");

	for (int i = 0; i < w.size(); i++){
		ofs << w[i] << endl;
	}
	ofs.close();
	cout << "END" << endl;
	return 0;
}
