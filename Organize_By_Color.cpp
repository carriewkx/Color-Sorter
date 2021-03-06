#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;



void swap(double *xp, double *yp)
{
	double temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void swap_scalar(Scalar * xp, Scalar * yp)
{
	Scalar temp = *xp;
	*xp = *yp;
	*yp = temp;
}

Scalar * bubbleSort(double bright[], Scalar color[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {
	// Last i elements are already in place    
	for (j = 0; j < n - i - 1; j++) {
		if (bright[j] < bright[j + 1]) {
			swap(&bright[j], &bright[j + 1]);
			//swap_scalar(&(color[j]), &(color[j + 1]));
			Scalar temp;
			//(temp)[0] = (color[j + 1])[0];
		//	(temp)[1] = (color[j + 1])[1];
			//(temp)[2] = (color[j + 1])[2];
			//(color[j+1])[0] = (color[j])[0];
		//	(color[j + 1])[1] = (color[j])[1];
			//(color[j + 1])[2] = (color[j])[2];
		//	(color[j])[0] = (temp)[0];
		//	(color[j])[1] = (temp)[1];
		//	(color[j])[2] = (temp)[2];
			temp = color[j];
			color[j] = color[j + 1];
			color[j + 1] = temp;
		}
	}
}
//	for (int w = 0; w < n - 1; w++) {
//		cout << color[w] << endl;
	//}
	return color;
}

void bubbleSort_BGR(Scalar color[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {

		// Last i elements are already in place    
		for (j = 0; j < n - i - 1; j++) {
			if (color[j][2] > color[j + 1][2]) {
				swap_scalar(&color[j], &color[j + 1]);
			}
		}
	}
}


int main() {

	Mat old_image = imread("C:/Users/carri/Documents/Organize_By_Color/Test_Image/04.jpg");
	Vec3b buf;
	Mat image;
	int height = 40 * old_image.cols / old_image.rows;
	resize(old_image, image, Size(height, 40));
	Mat samples(image.rows * image.cols, 3, CV_32F);

	Mat color_stripe = Mat::zeros(Size(image.cols *  image.rows, 50), image.type());
	Mat color_stripe_sorted = Mat::zeros(Size(image.cols *  image.rows, 50), image.type());
	int stripe_width_1 = 0;
	int stripe_height_1 = 0;
	int stripe_width_2 = 1;
	int stripe_height_2 = 50;
	Scalar * color_sort_array;
	color_sort_array = new Scalar[image.cols *  image.rows];
	int sort_count = 0;

	double * brightness;
	brightness = new double[image.cols *  image.rows];

	for (int i = 0; i < image.cols; i++) {
		for (int j = 0; j < image.rows; j++) {
			Scalar colour = image.at<Vec3b>(j, i);



			color_sort_array[sort_count] = colour;
			
			//https://stackoverflow.com/questions/3014402/sorting-a-list-of-colors-in-one-dimension
			double bright = sqrt(0.299 *(colour[2]) *(colour[2])
				+ 0.587*(colour[1])*(colour[1]) + 0.114*(colour[0])*(colour[0]));

			brightness[sort_count] = bright;


			cv::rectangle(color_stripe, Point(stripe_width_1, stripe_height_1), Point(stripe_width_2, stripe_height_2), color_sort_array[sort_count], CV_FILLED);
			Vec3b intensity = image.at<Vec3b>(j, i);
			stripe_width_1++;
			stripe_width_2++;
			sort_count++;
			for (int k = 0; k < 3; k++) {
				uchar col = intensity.val[k];

				samples.at<float>(j + i * image.rows, k) = image.at<Vec3b>(j, i)[k];

				//std::cout << colour.val[0] <<std::endl;
			}
		}
	}

	int stripe_width_a = 0;
	int stripe_width_b = 1;
	int stripe_height_a = 0;
	int stripe_height_b = 50;
	Scalar * color_sort_array_2;
	color_sort_array_2 = new Scalar[image.cols *  image.rows];
	color_sort_array_2 = bubbleSort(brightness, color_sort_array, image.cols *  image.rows);
	
	for (int w = 0;w < (image.cols * image.rows); w++) {
	cv::rectangle(color_stripe_sorted, Point(stripe_width_a, stripe_height_a), Point(stripe_width_b, stripe_height_b), color_sort_array_2[w], CV_FILLED);
	//cout << color_sort_array_2[w]  << endl;
	stripe_width_a++;
	stripe_width_b++;

}





	int clusterCount = 6;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels,
		TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 10, 1),
		attempts, KMEANS_PP_CENTERS, centers);

	Mat new_image(image.size(), image.type());

	Scalar cluster_color[] = {(0,0,0), (0, 0, 0), (0, 0, 0), (0, 0, 0) ,(0, 0, 0), (0,0,0) };
	int colour_index = 0;
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * image.rows, 0);
			new_image.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
			new_image.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
			new_image.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);


			for (int count = 0; count < 6; count++) {
				if ((Scalar(new_image.at<Vec3b>(y, x)) != cluster_color[0]) &&
					(Scalar(new_image.at<Vec3b>(y, x)) != cluster_color[1]) && 
					(Scalar(new_image.at<Vec3b>(y, x)) != cluster_color[2]) && 
					(Scalar(new_image.at<Vec3b>(y, x)) != cluster_color[3]) && 
					(Scalar(new_image.at<Vec3b>(y, x)) != cluster_color[4])&&
					(Scalar(new_image.at<Vec3b>(y, x)) != cluster_color[5])) {
					cluster_color[colour_index] = new_image.at<Vec3b>(y, x);
					colour_index++;
				}

			}
		}
}
	Mat color_bar = Mat::zeros(Size(100,600), new_image.type());


	cv::rectangle(color_bar, Point(0,0) , Point(100,100), cluster_color[0], CV_FILLED);
	cv::rectangle(color_bar, Point(0, 100), Point(100, 200), cluster_color[1], CV_FILLED);
	cv::rectangle(color_bar, Point(0, 200), Point(100, 300), cluster_color[2], CV_FILLED);
	cv::rectangle(color_bar, Point(0, 300), Point(100, 400), cluster_color[3], CV_FILLED);
	cv::rectangle(color_bar, Point(0, 400), Point(100, 500), cluster_color[4], CV_FILLED);
	cv::rectangle(color_bar, Point(0, 500), Point(100, 600), cluster_color[5], CV_FILLED);

	cv::imshow("color bar", color_bar);
	cv::imshow("clustered image", new_image);
	//cv::imshow("stripe image", color_stripe);
	cv::imshow("stripe sorted", color_stripe_sorted);
	Mat img;
	cv::imwrite("C:/Users/carri/Documents/Organize_By_Color/Test_Image/color_bar.jpg", color_bar);
	//imshow("Image", image);
	delete[] color_sort_array;
	delete[] brightness;

	cv::waitKey(0);
}