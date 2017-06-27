#ifndef BLOCK_METRICS_CALCULATOR_IIR_H
#define BLOCK_METRICS_CALCULATOR_IIR_H

#include "BlockMetricsCalculator.h"
#include "ImageData.h"
#include "config.h"

/**
 * Butterworth High Pass filter
 */
const float HP_GAIN = 3.155634919e-02f;
const float hp_alpha[] = { -1.4590290622f, -0.9103690003f, -0.1978251873f };
const float hp_beta[] = { HP_GAIN, -3 * HP_GAIN, 3 * HP_GAIN, -HP_GAIN }; // butter(3, 0.75, 'high')

/**
 * Butterworth Band Pass filter, matlab butter(3, 0.1)
 */
const float bp_alpha[] = { 2.3741f, -1.9294f, 0.5321f };
const float bp_beta[] = { 0.0029f, 0.0087f, 0.0087f, 0.0029f }; // butter(3, 0.1)

class BlockMetricsCalculatorIIR :
    public BlockMetricsCalculator
{
    float column_sharpness[BLOCK_WIDTH], row_sharpness[BLOCK_HEIGHT];
    float bp_ring[4];
    float hp_ring[4];

    float bandpass(float *m, int x);

    float highpass(float *m, int x);

    float sharpness(float *m, int n);

    void copy_row(float *d, float *s);

    void calculate_block_reflectivity(BlockMetrics *block);

    void calculate_block_sharpness_values(ImageData& image, cv::Rect& blockRoi, BlockMetrics *block);

public:
    BlockMetricsCalculatorIIR();
    ~BlockMetricsCalculatorIIR();

    void calculate_blockwise_merits(ImageData& image);
};

#endif // !BLOCK_METRICS_CALCULATOR_IIR_H

