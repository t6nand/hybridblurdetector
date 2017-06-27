#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <opencv2/core.hpp>

class ImageUtils {
public:
  /**
   * This method accepts an RGB image and returns its HSI image.
   * RGB image must be of CV_8UC3 type.
   */
  static cv::Mat convertRGB2HSI(cv::Mat rgbImage);
};

#endif
