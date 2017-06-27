#ifndef CONFIG_H
#define CONFIG_H

const float EPSILON_F = 5.960465e-8f;

const float M_PI_F = 3.1415926535897932384626433832795f;

const float DECISION_TREE_BLURINESS_COEFFICIENT = 0.55;

const float HAAR_WAVELET_TRANSFORM_BLURINESS_COEFFICIENT = 0.55;

/**
* Note that detect_blur() must be changed if the number of levels has changed.
*/
enum { LEVELS = 3 };

/**
 * Tunable throughout.
 */
enum { BLOCK_WIDTH = 16, BLOCK_HEIGHT = 16 }; /* in pixels */

/**
* The threshold at which a block is considered sharp.
*/
static float SHARP_THRESHOLD = 0.32f;

/**
* The minimum and maximum hue, in radians, to detect(ignored) sky blocks.
*/
static float SKY_HUE_LOW = 3.1f;
static float SKY_HUE_HIGH = 3.8f;

/**
* The threshold of sky hue to ignore a block.
*/
static float SKY_THRESHOLD = 0.7f;

/***********IIR filter strategy tuning values***********/

enum { FILTER_PAD = 4 };

/**
 * How much bandpass energy to consider part of feature range when calculating
 * sharpness
 */
static float MIN_BANDPASS_LEVEL = 0.12f;

/**
* Scaling factor on sharpness values
*/
static float SHARP_SCALE = 15.0f;

/*********** HAAR Wavelet Transform Constants & Configuration Parameters
 * ***********/

/**
 * Note that changing this value radically changes the blur extent
 * result. Since human vision systems are not sensible to intensity below
 *intensity 30, we choose this value between 30 - 35.
 */
static float tlzz_threshold = 32.0f;
/**
* In ideal situation, MinZero should be set to zero. In practice, it can be
*chosen as a positive real number that is very close to zero.
* The accuracy of detection changes a little from 0.01 to 0.10.
*/
static float min_zero = 0.10f;

#endif // !CONFIG_H
