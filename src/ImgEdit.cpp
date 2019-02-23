// Includes
#include "ImgEdit.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

// Global Constants
const int bgr_max = 255; // max value of a bgr pixel
const int bgr_half = 128; // middle value of a bgr pixel

///////// Image Proccessing Fucntions //////////
// Grayscale
cv::Mat grayscale(cv::Mat A)
{
    // Converts image to grayscale image
    cvtColor(A, A, cv::COLOR_BGR2GRAY);
    return A;
}

// Brightness
/**
 * Returns an input BGR Mat adjusted for brigthness
 *
 * @param matrix The input Mat
 * @param beta Brightness shift amount, ranges from -255 to 255 (increasing in brightness, 0 will have no effect)
 * @return Mat with the brightness adjustments
 */
cv::Mat brightness(cv::Mat matrix, int beta)
{
    cv::Mat processed_mat; // initializes output matrix
    
    short rtype = -1; // use same type as input matrix
    short alpha = 1; // pixel_value = pixel_value * alpha
    matrix.convertTo(processed_mat, rtype, alpha, beta); // pixel_val += beta (clips pixel_val to stay between 0 to 255)
    
    return processed_mat;
}

// Contrast
/**
 * Returns an input BGR Mat adjusted for contrast
 *
 * @param matrix The input Mat
 * @param beta Contrast factor, ranges from -255 (low contrast) to 255 (high contrast). beta=0 has no effect.
 * @return Mat with the contrast adjustments
 */
cv::Mat contrast(cv::Mat matrix, double beta)
{
    cv::Mat processed_mat; // output matrix
    matrix.convertTo(processed_mat, CV_32S); // converts the matrix to type CV_32S to allow aritmetic
    
    // Handles beta to be within the desired range (255 is equal to bgr_max, but this is only a coincidence)
    if (beta < -255)
        beta = -255;
    else if (beta > 255)
        beta = 255;
    
    int kappa = 259; // contrast parameter, the higher it is, the lower the effect of contrast
    double contrast_factor = (kappa * (beta + bgr_max)) / (bgr_max * (kappa - beta)); // calculates contrast factor
    
    // contrast is calculated using the equation:
    // new_pixel = contrast_factor * (old_pixel - 128) + 128
    cv::Scalar scale(bgr_half, bgr_half, bgr_half); // contains cv scalar depth 3 with values of 128
    
    processed_mat -= scale; // old_pixel - 128
    processed_mat *= contrast_factor; // contrast * p
    processed_mat += scale; // p + 128
    
    processed_mat.convertTo(processed_mat, CV_8U); // converts matrix back to original format

    return processed_mat;
}

// Saturation
/**
 * Returns an input BGR Mat adjusted for saturation (colour intensity)
 *
 * @param matrix The input Mat
 * @param s_shift Saturation shift amount, ranges from -255 to 255 (increasing in saturation, 0 will have no effect)
 * @return Mat with the saturation adjustments
 */
cv::Mat saturation(cv::Mat matrix, int s_shift)
{
    cv::Mat processed_mat; // initializes output matrix
    cv::cvtColor(matrix, processed_mat, cv::COLOR_BGR2HSV); // converts input matrix to HSV type
    
    std::vector<cv::Mat> channels; // vector that will hold each HSV channel
    cv::split(processed_mat, channels); // splits HSV mat into an 3 mats with 1 channel each for H, S and V
    
    short idx = 1; // index of saturation in HSV format
    short rtype = -1; // use same type as input matrix
    short alpha = 1; // sat_value *= alpha
    channels[idx].convertTo(channels[idx], rtype, alpha, s_shift); // sat_value += s_shift (clips sat_val to stay between 0 to 255)
    
    cv::merge(channels, processed_mat); // merges channels HSV back together into output matrix
    cv::cvtColor(processed_mat, processed_mat, cv::COLOR_HSV2BGR); // converts HSV back to BGR
    
    return processed_mat;
}

// Hue
/**
 * Returns an input BGR Mat adjusted by hue (colour)
 *
 * @param matrix The input Mat
 * @param h_shift Hue shift amount, ranges from 0 to 180 (shifts the hue value, 0 will have no effect)
 * @return Mat with the hue adjustments
 */
cv::Mat hue(cv::Mat matrix, int h_shift)
{
    cv::Mat processed_mat; // initializes output matrix
    cv::cvtColor(matrix, processed_mat, cv::COLOR_BGR2HSV); // converts input matrix to HSV type
    
    short idx = 0; // index of hue in HSV format
    // iterates through each pixel in mat to ahjust hue values
    for (int y = 0; y < processed_mat.rows; y++) // iterate columns
    {
        for (int x = 0; x < processed_mat.cols; x++) // iterate rows
        {
            short h = processed_mat.at<cv::Vec3b>(y,x)[idx]; // get current hue value at pixel (y, x)
            processed_mat.at<cv::Vec3b>(y,x)[idx] = (h + h_shift) % 180; // adjust hue
        }
    }
    
    cv::cvtColor(processed_mat, processed_mat, cv::COLOR_HSV2BGR); // converts HSV back to BGR
    return processed_mat;
}

// Gamma
/**
 * Returns an input BGR Mat with gamma adjustments
 *
 * @param matrix The input Mat
 * @param gamma Gamma factor, -100 for low gamma (bright), +100 for high gamma (dark)
 * @return Mat with the gamma adjustments
 */
cv::Mat gamma(cv::Mat matrix, double gamma)
{
    // Handles input to be within desired range
    gamma *= 0.05;
    if (gamma < 0)
        gamma = -1 / (gamma - 1);
    else
        gamma += 1;
    
    cv::Mat processed_mat = matrix.clone(); // initializes output matrix
    
    short max_n = bgr_max + 1; // number of possible pixel values
    cv::Mat lookUpTable(1, max_n, CV_8U); // lookup table mat
    uchar* p = lookUpTable.ptr(); // pointers for each entry in lookuptable
    for( int i = 0; i < max_n; ++i) // goes through each num in possible range to create lookup value of gamma
    {
        p[i] = cv::saturate_cast<uchar>(std::pow(i / (double)bgr_max, gamma) * (double)bgr_max); // gamma calculation
    }
    
    cv::LUT(processed_mat, lookUpTable, processed_mat); // uses lookup table to change
    
    return processed_mat;
}

// Sharpness
/**
 * Returns a sharpened input BGR Mat
 *
 * @param matrix The input Mat
 * @param beta Sharpness factor. Ranges from 0 to 100 (increasing in sharpness)
 * @return Mat with the sharpness adjustments
 */
cv::Mat sharpness(cv::Mat matrix, double beta)
{
    // Truncates the input to be within range
    if (beta < 0)
        beta = 0;
    else if (beta > 100)
        beta = -10;
    else
        beta /= -10;
    
    cv::Mat processed_mat = matrix.clone(); // initializes output matrix
    
    double sigma = 3; // standard deviation for the gaussian blur
    int size = 3; // kernel size
    
    double alpha = 1 + -1 * beta; // weight of the original matrix (beta is weight of gaussian blur)
    double gamma = 0; // constant added to the resulting matrix
    
    cv::GaussianBlur(processed_mat, processed_mat, cv::Size(size, size), sigma, sigma); // creates a matrix adjusted with gaussian blur
    cv::addWeighted(matrix, alpha, processed_mat, beta, gamma, processed_mat); // adds the orignal and blurred matrix with the weights alpha and beta respectively
    
    return processed_mat;
}
