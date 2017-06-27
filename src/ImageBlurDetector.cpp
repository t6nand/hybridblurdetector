#include "ImageBlurDetector.h"

#include "config.h"
#include "GlobalMetricsCalculator.h"
#include "BlockMetricsCalculator.h"
#include "BlockMetricsCalculatorIIR.h"
#include "ImageData.h"
#include "WaveletTransform.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

ImageBlurDetector::ImageBlurDetector() : blockMetricsCalculator(NULL), globalMetricsCalculator(NULL)
{
    setLocalBlurDetectionStrategy(IIR_FILTER);
    globalMetricsCalculator = new GlobalMetricsCalculator();
}

ImageBlurDetector::~ImageBlurDetector()
{
    if (blockMetricsCalculator != NULL)
        delete blockMetricsCalculator;

    if (globalMetricsCalculator != NULL)
        delete globalMetricsCalculator;
}

void  ImageBlurDetector::setLocalBlurDetectionStrategy(StrategyType strategyType) {
    if (strategyType != this->strategyType || blockMetricsCalculator == NULL) {

        if (blockMetricsCalculator != NULL)
            delete blockMetricsCalculator;

        if (strategyType == IIR_FILTER) {
            blockMetricsCalculator = new BlockMetricsCalculatorIIR();
        }
        else if (strategyType == TONG_LI_ZHANG_ZHANG) {
            // Technique not implemented in OpenCV
        }
        else if (strategyType == MARICHAL_MA_ZHANG) {
            // Technique not implemented in OpenCV
        }
    }
}

std::pair<int, float> ImageBlurDetector::detectBlurUsingWaveletTransform(char *imgFilePath) {
    cv::Mat img = imread(imgFilePath, 0);
    WaveletTransform waveletTransform;
    return waveletTransform.calculateWaveletBlurExtent(img);
}
pair<int, double> ImageBlurDetector::computeImageBlur(char *imgFilePath)
{
    ImageData imageData(imgFilePath);

    // Compute metrics for each local block
    blockMetricsCalculator->calculate_blockwise_merits(imageData);

    // Compute global metrics of Image
    globalMetricsCalculator->compute_global_merits(imageData);
    
    // ToDo: Recompute thresholds for plagiarism detection. Slight improvement should be seen.
    // Decision should be calculated in java caller method.
//    if ((imageData.merit.density > 0.60 && imageData.merit.density != 1) ||
//             (imageData.merit.composition > 3.0 &&
//             imageData.merit.median_sharpness > 3.0 &&
//             imageData.merit.brightness_idx > -0.15 &&
//             imageData.merit.saturation_idx > -0.15 &&
//             imageData.merit.density > 0.20))
//             imageData.decision = 1;

    imageData.decision = 1;
    bool considerMinorMetrices = false;
    float combinedWeightScore = imageData.merit.density + imageData.merit.composition + imageData.merit.median_sharpness;
    if(imageData.merit.density > 0.55) {
        if(imageData.merit.density == 1.00) {
            if(imageData.merit.composition > 4.0 &&
            imageData.merit.median_sharpness > 4.0 &&
            imageData.merit.brightness_idx > 0.0 &&
            imageData.merit.saturation_idx > 0.0)
                imageData.decision = 0;
        }
        else
            imageData.decision = 0;
    } else {
            if(imageData.merit.density > 0.20) {
                if(imageData.merit.composition > 3.0 &&
                imageData.merit.median_sharpness > 3.0 &&
                imageData.merit.brightness_idx > -0.15 &&
                imageData.merit.saturation_idx > -0.15)
                    imageData.decision = 0;
            }
    }

    double blurriness_confidence = 1 - imageData.merit.density;
    // Display output
//    output_metadata(imageData);
    return make_pair(imageData.decision, blurriness_confidence);
}

void ImageBlurDetector::output_metadata(ImageData& image)
{
    printf("%f, %f, %f, %f, %f, %d\n",
        image.merit.composition, image.merit.density, image.merit.median_sharpness,
        image.merit.brightness_idx, image.merit.saturation_idx, image.decision);
}
