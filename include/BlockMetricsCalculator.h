#ifndef BLOCK_METRICS_CALCULATOR_H
#define BLOCK_METRICS_CALCULATOR_H

#include "ImageData.h"

class BlockMetricsCalculator
{
public:
    virtual void calculate_blockwise_merits(ImageData& imageData) = 0;
};

#endif // !BLOCK_METRICS_CALCULATOR_H
