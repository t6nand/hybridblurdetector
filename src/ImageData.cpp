#include "ImageData.h"
#include "DistanceUtils.h"
#include "ImageUtils.h"

RuleOfThird::RuleOfThird(int w, int h) : weight(NULL) {
  this->w = w;
  this->h = h;
  rebuild_thirds_matrix();
}

RuleOfThird::~RuleOfThird() {
  if (weight != NULL) {
    delete[] weight;
    weight = NULL;
  }
}

void RuleOfThird::rebuild_thirds_matrix() {
  double power_points[4 * 2] = {1.0 / 3, 1.0 / 3, 2.0 / 3, 1.0 / 3,
                                1.0 / 3, 2.0 / 3, 2.0 / 3, 2.0 / 3};

  if (weight != NULL) {
    delete[] weight;
    weight = NULL;
  }

  this->weight = new double[w * h];

  for (int i = 0; i < 4; i++) {
    power_points[2 * i] *= w;
    power_points[2 * i + 1] *= h;
    power_points[2 * i] += 0.5;
    power_points[2 * i + 1] += 0.5;
  }

  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
      this->weight[j + i * w] =
          1.0 - min_manhattan_distance(j + 0.5, i + 0.5, power_points, 4, w, h);
}

BlockMetrics::BlockMetrics()
    : sharpness(0.0), reflectivity(0.0), brightness(0.0), mean_saturation(0.0),
      mean_hue(0.0), sky_ratio(0.0), uniform(false) {}

ImageData::ImageData(char *fileName) : blocks(NULL), hsiChannels(3) {
  decision = 0;

  // Always read in 3-channel mode.
  inputImage = cv::imread(fileName);
  if (inputImage.data == NULL) {
    // ToDo: throw exception
  }

  imageWidthInBlocks = inputImage.cols / BLOCK_WIDTH;
  imageHeightInBlocks = inputImage.rows / BLOCK_HEIGHT;
  if (imageWidthInBlocks == 0 || imageHeightInBlocks == 0) {
    // ToDo: throw exception
  }
  numOfBlocks = imageWidthInBlocks * imageHeightInBlocks;

  // Compute distance of center of each block with nearest Rule of thirds
  // Intersection Points
  thirds = new RuleOfThird(imageWidthInBlocks, imageHeightInBlocks);

  // Initialize image to process to be of size in multiple of BLOCK_WIDTH and
  // BLOCK_HEIGHT
  cv::Rect roi(0, 0, BLOCK_WIDTH * imageWidthInBlocks,
               BLOCK_HEIGHT * imageHeightInBlocks);
  image = inputImage(roi).clone();

  // Allocate blocks to store metrics for each local block of the image
  this->blocks = new BlockMetrics[numOfBlocks];

  // Compute metrics for each local block of the image
  calculateBlockMeans();
}

ImageData::~ImageData() {
  if (this->thirds != NULL) {
    delete thirds;
  }

  if (this->blocks != NULL) {
    delete[] this->blocks;
  }
}

void ImageData::calculateBlockMeans() {

  // Create a float 3-channel image for creating HSV channels
  cv::Mat hsiMat = ImageUtils::convertRGB2HSI(image);
  cv::split(hsiMat, hsiChannels);
  cv::Mat hueSkyLikeMat;

  // Filter pixels which have hue like sky
  cv::inRange(hsiChannels.at(0), cv::Scalar(SKY_HUE_LOW),
              cv::Scalar(SKY_HUE_HIGH), hueSkyLikeMat);

  // Process for each block
  int blockIndex = 0;
  for (int y = 0; y < image.rows; y += BLOCK_HEIGHT) {
    for (int x = 0; x < image.cols; x += BLOCK_WIDTH) {
      BlockMetrics *block = blocks + blockIndex;
      cv::Rect roi = cv::Rect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT);

      // Compute mean hue, mean saturation and mean intensity
      cv::Scalar blockMeanHSV = cv::mean(hsiMat(roi));
      block->mean_hue = blockMeanHSV[0];
      block->mean_saturation = blockMeanHSV[1];
      block->brightness = blockMeanHSV[2];

      // Compute count of pixels in range (SKY_HUE_LOW, SKY_HUE_HIGH) in the
      // block
      double skyLikePixels = cv::countNonZero(hueSkyLikeMat(roi));
      block->sky_ratio = skyLikePixels / roi.area();

      blockIndex++;
    }
  }
}
