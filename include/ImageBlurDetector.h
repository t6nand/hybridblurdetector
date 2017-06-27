#ifndef IMAGE_BLUR_DETECTOR_H
#define IMAGE_BLUR_DETECTOR_H

#include "ImageData.h"

// Forward Declarations for faster compilation
class BlockMetricsCalculator;
class GlobalMetricsCalculator;

class ImageBlurDetector {
  /**
  * The delegate to individual block metrics calculator.
  */
  BlockMetricsCalculator *blockMetricsCalculator;

  /**
  * The delegate to global image metrics calculator.
  */
  GlobalMetricsCalculator *globalMetricsCalculator;

  /**
   * This function prints the global blur metrics calculated for an image.
   */
  void output_metadata(ImageData &imageData);

public:
  /**
   * Default Constructor.
   * Allocates necessary memory. If failure in memory exception, throws an
   * error.
   */
  ImageBlurDetector();

  /**
  * Deallocates allocated memory.
  */
  ~ImageBlurDetector();

  /**
  * The method computes image blur.
  * @param imgFilePath The path to image file.
  */
  std::pair<int, double> computeImageBlur(char *imgFilePath);

  /**
  * This function detect blur in image using Wavelet Transform
  * @param imgFilePath The path to image file.
  */
  std::pair<int, float> detectBlurUsingWaveletTransform(char *imgFilePath);

  /**
  * This method is used to make final judgement on blurriness in image using
  * tunable Decision tree which can also be improved for accuracy and
  * performance
  * using various learning schemes.
  */
  int decideBlurInImage(char *imgFilePath);
};

#endif // !IMAGE_BLUR_DETECTOR_H
