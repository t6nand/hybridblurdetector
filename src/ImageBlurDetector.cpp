#include "ImageBlurDetector.h"

#include "BlockMetricsCalculator.h"
#include "BlockMetricsCalculatorIIR.h"
#include "GlobalMetricsCalculator.h"
#include "ImageData.h"
#include "WaveletTransform.h"
#include "config.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/**
* This class has all the methods to detect blur in images using different
* methods.
*/

ImageBlurDetector::ImageBlurDetector()
    : blockMetricsCalculator(NULL), globalMetricsCalculator(NULL) {
  setLocalBlurDetectionStrategy(IIR_FILTER);
  globalMetricsCalculator = new GlobalMetricsCalculator();
}

ImageBlurDetector::~ImageBlurDetector() {
  if (blockMetricsCalculator != NULL)
    delete blockMetricsCalculator;

  if (globalMetricsCalculator != NULL)
    delete globalMetricsCalculator;
}

pair<int, float>
ImageBlurDetector::detectBlurUsingWaveletTransform(char *imgFilePath) {
  cv::Mat img = imread(imgFilePath, 0);
  WaveletTransform waveletTransform;
  return waveletTransform.calculateWaveletBlurExtent(img);
}

pair<int, double> ImageBlurDetector::computeImageBlur(char *imgFilePath) {
  ImageData imageData(imgFilePath);

  // Compute metrics for each local block
  blockMetricsCalculator->calculate_blockwise_merits(imageData);

  // Compute global metrics of Image
  globalMetricsCalculator->compute_global_merits(imageData);
  imageData.decision = 1;
  bool considerMinorMetrices = false;
  float combinedWeightScore = imageData.merit.density +
                              imageData.merit.composition +
                              imageData.merit.median_sharpness;
  if (imageData.merit.density > 0.55) {
    if (imageData.merit.density == 1.00) {
      if (imageData.merit.composition > 3.0 &&
          imageData.merit.median_sharpness > 3.0 &&
          imageData.merit.brightness_idx > -0.15 &&
          imageData.merit.saturation_idx > -0.15)
        imageData.decision = 0;
    } else
      imageData.decision = 0;
  } else {
    if (imageData.merit.density > 0.20) {
      if (imageData.merit.composition > 3.0 &&
          imageData.merit.median_sharpness > 3.0 &&
          imageData.merit.brightness_idx > -0.15 &&
          imageData.merit.saturation_idx > -0.15)
        imageData.decision = 0;
    }
  }

  double blurriness_confidence = 1 - imageData.merit.density;
  return make_pair(imageData.decision, blurriness_confidence);
  // Display output
  //    output_metadata(imageData);
}

int decideBlurInImage(char *imgFilePath) {
  pair<int, double> blurDecisionFromDecisionTree =
      computeImageBlur(imgFilePath);
  pair<int, float> blurDecisionFromWaveletTransform =
      detectBlurUsingWaveletTransform(argv[i]);
  int finalDecision = -1;
  if (blurDecisionFromDecisionTree.first == 0 &&
      blurDecisionFromWaveletTransform.first == 0) {
    finalDecision = 0;
  }
  if (blurDecisionFromWaveletTransform.first == 1 &&
      blurDecisionFromDecisionTree.first == 0) {
    if (blurDecisionFromWaveletTransform.second >
        HAAR_WAVELET_TRANSFORM_BLURINESS_COEFFICIENT) {
      finalDecision = 1;
    } else {
      finalDecision = 0;
    }
  }
  if (blurDecisionFromWaveletTransform.first == 0 &&
      blurDecisionFromDecisionTree.first == 1) {
    if (blurDecisionFromDecisionTree.second >
        DECISION_TREE_BLURINESS_COEFFICIENT) {
      finalDecision = 1;
    } else {
      finalDecision = 0;
    }
  }
  if (blurDecisionFromDecisionTree.first == 1 &&
      blurDecisionFromWaveletTransform.first == 1) {
    finalDecision = 1;
  }
  return finalDecision;
}
void ImageBlurDetector::output_metadata(ImageData &image) {
  printf("%f, %f, %f, %f, %f, %d\n", image.merit.composition,
         image.merit.density, image.merit.median_sharpness,
         image.merit.brightness_idx, image.merit.saturation_idx,
         image.decision);
}
