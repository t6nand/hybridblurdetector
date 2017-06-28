#include <cstdio>
using namespace std;

#include "ImageBlurDetector.h"
#include "config.h"

int main(int argc, char **argv) {
  ImageBlurDetector imageBlurDetector;
  int status = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 's')
        sscanf(argv[i], "-s=%f", &SHARP_THRESHOLD);
      else if (argv[i][1] == 'k')
        sscanf(argv[i], "-k=%f", &SKY_THRESHOLD);
      else if (argv[i][1] == 'b')
        sscanf(argv[i], "-b=%f", &MIN_BANDPASS_LEVEL);
      else if (argv[i][1] == 'x')
        sscanf(argv[i], "-x=%f", &SHARP_SCALE);
      else if (!strcmp(argv[i], "-a=filter"))
        imageBlurDetector.setLocalBlurDetectionStrategy(IIR_FILTER);
      else if (!strcmp(argv[i], "-a=tlzz"))
        imageBlurDetector.setLocalBlurDetectionStrategy(TONG_LI_ZHANG_ZHANG);
      else if (argv[i][1] == 't')
        sscanf(argv[i], "-t=%f", &tlzz_threshold);
      else if (argv[i][1] == 'z')
        sscanf(argv[i], "-z=%f", &min_zero);
      else if (!strcmp(argv[i], "-a=mmz"))
        imageBlurDetector.setLocalBlurDetectionStrategy(MARICHAL_MA_ZHANG);
      else if (argv[i][1] == 'd')
        sscanf(argv[i], "-d=%d", &min_dct_value);
      else if (argv[i][1] == 'h')
        sscanf(argv[i], "-h=%f", &max_histogram_value);
      continue;
    }
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
      if (blurDecisionFromWaveletTransform.second > 0.55) {
        finalDecision = 1;
      } else {
        finalDecision = 0;
      }
    }
    if (blurDecisionFromWaveletTransform.first == 0 &&
        blurDecisionFromDecisionTree.first == 1) {
      if (blurDecisionFromDecisionTree.second > 0.55) {
        finalDecision = 1;
      } else {
        finalDecision = 0;
      }
      // This is the case with 100% sharp blocks detected, so lower metrices
      // were given importance.
      if (blurDecisionFromDecisionTree.second == 0.00) {
        finalDecision = 1;
      }
    }
    if (blurDecisionFromDecisionTree.first == 1 &&
        blurDecisionFromWaveletTransform.first == 1) {
      finalDecision = 1;
    }
    printf("From Decision Tree: %d, Confidence from Decision Tree: %f, From "
           "Wavelet Transform: %d, Confidence from wavelet transform: %f, "
           "Is_Blurred: %d",
           blurDecisionFromDecisionTree.first,
           blurDecisionFromDecisionTree.second,
           blurDecisionFromWaveletTransform.first,
           blurDecisionFromWaveletTransform.second, finalDecision);
  }

  return status;
}
