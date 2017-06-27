#include <cstdio>
using namespace std;

#include "ImageBlurDetector.h"
#include "config.h"

int main(int argc, char **argv) {
  ImageBlurDetector imageBlurDetector;
  int status = 0;

  for (int i = 1; i < argc; i++) {
    pair<int, double> blurDecisionFromDecisionTree =
        imageBlurDetector.computeImageBlur(argv[i]);
    pair<int, float> blurDecisionFromWaveletTransform =
        imageBlurDetector.detectBlurUsingWaveletTransform(argv[i]);
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
    printf("From Decision Tree: %i, Confidence from Decision Tree: %f, From "
           "Wavelet Transform: %d, Confidence from wavelet transform: %f, "
           "Is_Blurred: %d",
           blurDecisionFromDecisionTree.first,
           blurDecisionFromDecisionTree.second,
           blurDecisionFromWaveletTransform.first,
           blurDecisionFromWaveletTransform.second, finalDecision);
  }

  return status;
}
