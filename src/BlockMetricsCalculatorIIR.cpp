#include "BlockMetricsCalculatorIIR.h"

/**
* Refer to research paper by Lim, Yen and Wu @
* http://hplabs.hp.com/techreports/2005/HPL-2005-14.pdf
*/

BlockMetricsCalculatorIIR::BlockMetricsCalculatorIIR() {}

BlockMetricsCalculatorIIR::~BlockMetricsCalculatorIIR() {}

void BlockMetricsCalculatorIIR::calculate_blockwise_merits(ImageData &image) {
  int blockIndex = 0;
  for (int blocky = 0; blocky < image.imageHeightInBlocks; blocky++) {
    for (int blockx = 0; blockx < image.imageWidthInBlocks; blockx++) {
      cv::Rect blockRoi(blockx * BLOCK_WIDTH, blocky * BLOCK_HEIGHT,
                        BLOCK_WIDTH, BLOCK_HEIGHT);
      calculate_block_sharpness_values(image, blockRoi,
                                       &image.blocks[blockIndex]);
      blockIndex++;
    }
  }
}

void BlockMetricsCalculatorIIR::calculate_block_sharpness_values(
    ImageData &image, cv::Rect &blockRoi, BlockMetrics *block) {
  float pixels[((BLOCK_WIDTH > BLOCK_HEIGHT) ? BLOCK_WIDTH : BLOCK_HEIGHT) +
               FILTER_PAD] = {0};

  // Extract the block to process and create its transpose for processing
  // columns
  cv::Mat blockIntensity = image.hsiChannels.at(2)(blockRoi);
  cv::Mat blockIntensityTranspose = blockIntensity.t();

  for (int i = 0; i < BLOCK_WIDTH; i++) {
    copy_row(pixels + FILTER_PAD, (float *)blockIntensityTranspose.row(i).data);
    column_sharpness[i] = sharpness(pixels + FILTER_PAD, BLOCK_HEIGHT);
    block->sharpness += column_sharpness[i];
  }

  for (int i = 0; i < BLOCK_HEIGHT; i++) {
    copy_row(pixels + FILTER_PAD, (float *)blockIntensity.row(i).data);
    row_sharpness[i] = sharpness(pixels + FILTER_PAD, BLOCK_WIDTH);
    block->sharpness += row_sharpness[i];
  }

  block->sharpness /= (BLOCK_WIDTH + BLOCK_HEIGHT);
  calculate_block_reflectivity(block);
}

void BlockMetricsCalculatorIIR::calculate_block_reflectivity(
    BlockMetrics *block) {
  double sum_of_squares = 0.0;

  for (int i = 0; i < BLOCK_WIDTH; i++) {
    sum_of_squares += pow(column_sharpness[i] - block->sharpness, 2.0);
  }

  for (int i = 0; i < BLOCK_HEIGHT; i++) {
    sum_of_squares += pow(row_sharpness[i] - block->sharpness, 2.0);
  }

  block->reflectivity = sum_of_squares / (BLOCK_WIDTH + BLOCK_HEIGHT - 1);
}

void BlockMetricsCalculatorIIR::copy_row(float *d, float *s) {
  memcpy(d, s, BLOCK_WIDTH * sizeof(float));
}

float BlockMetricsCalculatorIIR::sharpness(float *m, int n) {
  float sum, bp, hp, r;
  int lastx = -1;
  float lastr = 0.0;

  memset(bp_ring, 0, sizeof(bp_ring));
  memset(hp_ring, 0, sizeof(hp_ring));
  sum = 0.0;
  for (int x = 0; x < n; x++) {
    bp = bandpass(m, x);
    hp = highpass(m, x); /* to keep the ring up to date  */
    // If band pass energy is high enough to consider the pixel
    if (bp < MIN_BANDPASS_LEVEL)
      continue;

    // Find ratio of High pass energy to Band pass energy
    r = hp / bp;
    r *= r;

    // Integration of High frequency features energy till the next pixelin the
    // block
    // This integration is divided by 2 after the loop ends.
    if (lastx != -1)
      sum += (x - lastx) * (r + lastr);
    lastx = x;
    lastr = r;
  }
  sum *= 0.5; // Divide by 2
  return SHARP_SCALE * sum;
}

float BlockMetricsCalculatorIIR::highpass(float *m, int x) {
  float alpha, beta;
  beta = m[x] * hp_beta[0] + m[x - 1] * hp_beta[1] + m[x - 2] * hp_beta[2] +
         m[x - 3] * hp_beta[3];
  alpha = hp_ring[(x + 3) & 3] * hp_alpha[0] +
          hp_ring[(x + 2) & 3] * hp_alpha[1] +
          hp_ring[(x + 1) & 3] * hp_alpha[2];
  return hp_ring[x & 3] = alpha + beta;
}

float BlockMetricsCalculatorIIR::bandpass(float *m, int x) {
  float alpha, beta;
  beta = m[x] * bp_beta[0] + m[x - 1] * bp_beta[1] + m[x - 2] * bp_beta[2] +
         m[x - 3] * bp_beta[3];
  alpha = bp_ring[(x + 3) & 3] * bp_alpha[0] +
          bp_ring[(x + 2) & 3] * bp_alpha[1] +
          bp_ring[(x + 1) & 3] * bp_alpha[2];
  return bp_ring[x & 3] = alpha + beta;
}
