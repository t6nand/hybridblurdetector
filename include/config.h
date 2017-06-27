#ifndef CONFIG_H
#define CONFIG_H

const float EPSILON_F = 5.960465e-8f;

const float M_PI_F = 3.1415926535897932384626433832795f;

/**
* Note that detect_blur() must be changed if the number of levels has changed.
*/
enum { LEVELS = 3 };

/**
 * Tunable throughout, but I don't see why we wouldn't go with
 * something that's an even number of DCT blocks.
 */
enum { BLOCK_WIDTH = 16, BLOCK_HEIGHT = 16}; /* in pixels */

/**
 * The strategy to use for computation of Sharpness metrics.
 */
enum StrategyType { IIR_FILTER, TONG_LI_ZHANG_ZHANG, MARICHAL_MA_ZHANG };

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
 * How much bandpass energy to consider part of feature range when calculating sharpness
 */
static float MIN_BANDPASS_LEVEL = 0.12f;

/**
* Scaling factor on sharpness values
*/
static float SHARP_SCALE = 15.0f;

/*********** TONG-LI-ZHANG-ZHANG strategy tuning values ***********/

/**
 * THRESHOLD could be defined based on mean intensity of
 * Laplacian-xform of data set, as in [2], but currently we use a
 * value close to that given in [1], which is based on the expected
 * limitations of human vision.
 *
 * Note that changing this value radically changes the blur extent
 * result.
 */
static float tlzz_threshold = 32.0f;

static float min_zero = 0.10f;

/*********** MARICHAL-MA-ZHANG ***********/

static int min_dct_value = 1;	/* -d= */
static float max_histogram_value = 0.005f; /* -h= */

static float weights[] = {	/* diagonal weighting */
	8, 7, 6, 5, 4, 3, 2, 1,
	1, 8, 7, 6, 5, 4, 3, 2,
	2, 1, 8, 7, 6, 5, 4, 3,
	3, 2, 1, 8, 7, 6, 5, 4,
	4, 3, 2, 1, 8, 7, 6, 5,
	5, 4, 3, 2, 1, 8, 7, 6,
	6, 5, 4, 3, 2, 1, 8, 7,
	7, 6, 5, 4, 3, 2, 1, 8
};

static float total_weight = 344;

#endif // !CONFIG_H