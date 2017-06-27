#include "GlobalMetricsCalculator.h"

#include "ImageData.h"
#include "config.h"

/**
* Refer to research paper by Lim, Yen and Wu @
* http://hplabs.hp.com/techreports/2005/HPL-2005-14.pdf
*/

static int double_cmp(const void *a_, const void *b_) {
  double a = *(double *)a_, b = *(double *)b_;
  return (a < b) ? -1 : (a > b) ? 1 : 0;
}

void GlobalMetricsCalculator::compute_global_merits(ImageData &image) {
  double composition_sum, mean_bright_sharp, mean_bright_blur, mean_sat_sharp,
      mean_sat_blur;
  double *sorted_sharpness;
  int n, sharp_count;

  RuleOfThird *thirds = image.thirds;

  sorted_sharpness = (double *)malloc(
      sizeof(double) * image.imageWidthInBlocks * image.imageHeightInBlocks);
  composition_sum = mean_bright_sharp = mean_bright_blur = mean_sat_sharp =
      mean_sat_blur = 0.0;
  n = sharp_count = 0;

  for (int i = 0; i < image.imageWidthInBlocks * image.imageHeightInBlocks;
       i++) {

    // ToDo: It's buggy. Look for different approach here.
    // if (image.blocks[i].sky_ratio > SKY_THRESHOLD)
    //    continue;

    n++;
    sorted_sharpness[i] = image.blocks[i].sharpness;
    composition_sum += image.blocks[i].sharpness * thirds->weight[i];
    if (image.blocks[i].sharpness > SHARP_THRESHOLD) {
      sharp_count++;
      mean_bright_sharp += image.blocks[i].brightness;
      mean_sat_sharp += image.blocks[i].mean_saturation;
    } else {
      mean_bright_blur += image.blocks[i].brightness;
      mean_sat_blur += image.blocks[i].mean_saturation;
    }
  }

  qsort(sorted_sharpness, n, sizeof(double), double_cmp);
  if (n % 2 == 0)
    image.merit.median_sharpness =
        (sorted_sharpness[n / 2] + sorted_sharpness[n / 2 - 1]) / 2.0f;
  else
    image.merit.median_sharpness = sorted_sharpness[n / 2];
  free(sorted_sharpness);

  image.merit.composition = composition_sum / n;
  image.merit.brightness_idx =
      (double)(mean_bright_sharp / (sharp_count + EPSILON_F) -
               mean_bright_blur / ((n - sharp_count) + EPSILON_F));
  image.merit.saturation_idx =
      (double)(mean_sat_sharp / (sharp_count + EPSILON_F) -
               mean_sat_blur / ((n - sharp_count) + EPSILON_F));
  image.merit.density = (double)sharp_count / n;
}
