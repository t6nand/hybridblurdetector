#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "config.h"

/**** GLOBAL MERITS ****/

class RuleOfThird{

public:

    int w;
    int h;
    double *weight;

    RuleOfThird(int w, int h);

    ~RuleOfThird();

    void rebuild_thirds_matrix();
};

class FiguresOfMerit
{
public:
    double composition;  /* sharpness weighted by rule of thirds */
    double brightness_idx;
    double saturation_idx;
    double density;
    double median_sharpness;
};

/**** LOCAL MERITS *****/

class BlockMetrics
{
public:
    /* Mean of sharpness along row and column */
    double sharpness;

    /* Variance of sharpness samples */
    double reflectivity;

    /* Mean Hue of the block */
    double mean_hue;

    /* Mean Saturation of the block */
    double mean_saturation;

    /* Mean Intensity of the block */
    double brightness;

    /* Ratio of sky pixels in a block to total pixels in the block */
    double sky_ratio;

    bool uniform;  // Same intensity throughout the block

    BlockMetrics();
};

/**** Image Data *****/

class ImageData
{
    // The original input image
    cv::Mat inputImage;

    // This function calculate HSI and sky ratio of each image block.
    void calculateBlockMeans();

public:

    // The ROI to be processed
    cv::Mat image;

    // Width and Height of image in Blocks
    int imageWidthInBlocks, imageHeightInBlocks;

    // Total number of blocks
    int numOfBlocks;

    // The block metrics for individual metrics
    BlockMetrics *blocks;

    // The HSV transformation of image
    std::vector<cv::Mat> hsiChannels;

    // RuleOfThird Object
    RuleOfThird* thirds;

    // Figures of merit of the image
    FiguresOfMerit merit;

    // The final classification output value
    int decision;

    // ToDo: Replace filename type to string and validate it
    ImageData(char* fileName);

    // ToDo: Will it deallocate the memory if allocation throws exception
    ~ImageData();
};

#endif // !IMAGE_DATA_H
