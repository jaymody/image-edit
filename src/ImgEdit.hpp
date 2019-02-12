// Includes
#include <opencv2/opencv.hpp>

#ifndef ImgEdit_hpp
#define ImgEdit_hpp

// Function Declarations
cv::Mat grayscale(cv::Mat matrix);
cv::Mat brightness(cv::Mat matrix, int beta);
cv::Mat contrast(cv::Mat matrix, double beta);
cv::Mat hue(cv::Mat matrix, int h_shift);
cv::Mat saturation(cv::Mat matrix, int s_shift);
cv::Mat gamma(cv::Mat, double gamma);
cv::Mat sharpness(cv::Mat matrix, double beta);

#endif /* ImgProc_hpp */
