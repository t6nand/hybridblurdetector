#include "ImageUtils.h"
#include "config.h"
#include <opencv2/opencv.hpp>
using namespace cv;

cv::Mat ImageUtils::convertRGB2HSI(cv::Mat src)
{
    cv::Mat hsi(src.rows, src.cols, CV_32FC3);

    MatIterator_<Vec3b> srcIt, srcEnd;
    MatIterator_<Vec3f> hsiIt;
    int R, G , B;
    float r, g, b, sum;

    // Iterate each pixel of src image.
    for (srcIt = src.begin<Vec3b>(), srcEnd = src.end<Vec3b>(), hsiIt = hsi.begin<Vec3f>(); srcIt != srcEnd; ++srcIt, ++hsiIt)
    {
        B = (*srcIt)[0];
        G = (*srcIt)[1];
        R = (*srcIt)[2];

        sum = (float)(R + G + B);

        r = (float)R / (sum + EPSILON_F);
        g = (float)G / (sum + EPSILON_F);
        b = (float)B / (sum + EPSILON_F);

        // Compute Hue
        (*hsiIt)[0] = acosf((0.5f * ((r - g) + (r - b))) /
            (EPSILON_F + sqrtf(((r - g)*(r - g)) + ((r - b) * (g - b)))));
        if (b > g)
            (*hsiIt)[0] = (2.0f * M_PI_F) - (*hsiIt)[0];

        // Compute Saturation
        (*hsiIt)[1] = 1.0f - 3.0f * std::min(r, std::min(b, g));

        // Compute Intensity
        (*hsiIt)[2] = (float)(sum) / 765.0f;
    }

    return hsi;
}
