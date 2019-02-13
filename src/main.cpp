// Imports
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "main.hpp"
#include "ImgEdit.hpp"

////////// Main Function //////////
int main()
{
    // initializes paths to images and mat objects for images
    std::vector<std::string> paths = {"test_image.png"};
    std::vector<cv::Mat> images = {};
    
    // Reads images and converts them into CV Mat objects
    for (int i = 0; i < paths.size(); i++)
    {
        // Reads image
        images.push_back(cv::imread(paths[i], cv::IMREAD_COLOR));
    }
    
    // Display images with adjustments
    for (int i = 0; i < images.size(); i++)
    {
        // Displays image
        cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
        cv::moveWindow("original", 500, 0);
        cv::imshow("original", images[i]);

        // Wait for keystroke
        cv::waitKey(0); // 0 = wait indefinetly

        
        /// /// /// Operations /// /// ///
        // BRIGHTNESS
        for (int j = -255; j <= 255; j+=2)
        {
            cv::Mat bright_image = brightness(images[i], j);
            cv::namedWindow("brightness" + std::to_string(j), cv::WINDOW_AUTOSIZE);
            cv::imshow("brightness" + std::to_string(j), bright_image);
            cv::waitKey(1);
            cv::destroyWindow("brightness" + std::to_string(j));

        }
        cv::waitKey(0);

        // CONTRAST
        for (double j = -255; j <= 255; j += 2)
        {
            cv::Mat contrast_image = contrast(images[i], j);
            cv::namedWindow("contrast" + std::to_string(j), cv::WINDOW_AUTOSIZE);
            cv::imshow("contrast" + std::to_string(j), contrast_image);
            cv::waitKey(1);
            cv::destroyWindow("contrast" + std::to_string(j));
        }
        cv::waitKey(0);

        // Saturation
        for (int j = -255; j <= 255; j+=2)
        {
            cv::Mat saturation_image = saturation(images[i], j);
            cv::namedWindow("saturation" + std::to_string(j), cv::WINDOW_AUTOSIZE);
            cv::imshow("saturation" + std::to_string(j), saturation_image);
            cv::waitKey(1);
            cv::destroyWindow("saturation" + std::to_string(j));
        }
        cv::waitKey(0);

        // HUE
        for (int j = 0; j <= 180; j++)
        {
            cv::Mat hue_image = hue(images[i], j);
            namedWindow("hue" + std::to_string(j), cv::WINDOW_AUTOSIZE);
            cv::imshow("hue" + std::to_string(j), hue_image);
            cv::waitKey(1);
            cv::destroyWindow("hue" + std::to_string(j));
        }
        cv::waitKey(0);

        // Gamma
        for (double j = -100.0; j < 100.0; j += 1)
        {
            cv::Mat gamma_image = gamma(images[i], j);
            cv::namedWindow("gamma" + std::to_string(j), cv::WINDOW_AUTOSIZE);
            cv::imshow("gamma" + std::to_string(j), gamma_image);
            cv::waitKey(1);
            cv::destroyWindow("gamma" + std::to_string(j));
        }
        cv::waitKey(0);

        // Sharpen
        for (double j = 0.0; j < 100; j += 0.5)
        {
            cv::Mat sharp_image = sharpness(images[i], j);
            cv::namedWindow("sharp" + std::to_string(j), cv::WINDOW_AUTOSIZE);
            cv::imshow("sharp" + std::to_string(j), sharp_image);
            cv::waitKey(1);
            cv::destroyWindow("sharp" + std::to_string(j));
        }
        cv::waitKey(0);
    }
    
    // Indicates end of program
    std::cout << "END OF PROGRAM" << std::endl;
}
