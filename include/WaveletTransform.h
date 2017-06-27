#include <opencv2/core.hpp>

class WaveletTransform {
public:
WaveletTransform();
~WaveletTransform();
void getHaarWavelet(const cv::Mat& src, cv::Mat& dst);
void getEmax(const cv::Mat& src, cv::Mat& dst, int scale);
std::pair<int, float> calculateWaveletBlurExtent(cv::Mat imageMat);
};