#include <stdint.h>
#include <sys/types.h>
/*
 * mrand_t - Corresponds to an open file descriptor
 * to /dev/mersenne.
 */
typedef int mrand_t;

/*
 * Initialize a Mersenne Twister random number generator. Returns
 * a positive value upon successful completion. On an error, returns
 * -1 and sets errno.
 */
mrand_t mrand_init(); 

/*
 * Reads num_bytes number of random bytes and puts it in data.
 * @param device - A valid open mrand_device
 * @param data - The buffer to write random data to.
 * @param num_bytes - The number of bytes to write.
 * @returns 0 on success, -1 otherwise. Sets errno on -1.
 */
int mrand_random(mrand_t device, void *data, size_t num_bytes);

/*
 * Destroy a Mersenne Twister random number generator.
 * @param device - The random device to destroy. Subsequent
 *                 calls using this device will fail.
 * @returns 0 on success, -1 otherwise. Sets errno on -1.
 */
int mrand_destroy(mrand_t device);

/*
 * Generate a random IEEE floating point number between 0 and 1 inclusive.
 * @param device - A valid open mrand device
 * @returns A random float
 */
float mrand_float(mrand_t device);

/*
 * Generate a random IEEE double precision number between 0 and 1 inclusive.
 * @param device - A valid open mrand device
 * @returns A random double
 */
double mrand_double(mrand_t device);

/*
 * Generate a random integer.
 * @param device - A valid open mrand device
 * @returns A random integer
 */
int mrand_int(mrand_t device);
/*
 * Perform a coin flip. Returns 1 on heads, and 0 on tails. Each happens with roughly
 * 50% probability.
 * @param device - A valid open mrand device
 * @returns 1 for heads, 0 for tails
 */
uint8_t mrand_coin_flip(mrand_t device);

/*
 * Returns a sample (0 to 1 inclusive) from a standard normal distribution using the given precision. This
 * function relies on the fact that the sum of Bernoulii random variables is a Binomial
 * random variable, and that the limit of a Binomial distribution is a Normal (or Gaussian)
 * distribution. The param precision in this case is the number of Bernoulii random variables
 * used to generate and sample from the Binomial distribution; since the limit of the resulting
 * distribution is a Normal distribution, with large enough precision, the sampled distribution
 * becomes arbitrarily close to a Normal distribution.
 * @param device - A valid open mrand device
 * @param precision - The number of Bernoulii random variables used to generate the Binomial distribution
                      that is sampled from. As precision increases, the sampled distribution grows arbitrarily
                      close to the Normal distribution. For best results, use a value greater than 7.
 *
 * @returns A random gaussian between 0 and 1 inclusive.
 */ 
double mrand_gaussian(mrand_t device, uint32_t precision);

/*
 * Returns a sample (from 0 to infinity) from a chi square distribution with degrees_of_freedom degrees
 * of freedom. The usage of precision is the same as above.
 * @param device - A valid open mrand device
 * @param degrees_of_freedom - The number of degrees of freedom
 * @param precision - See mrand_gaussian for description.
 * @returns A random chi-square sample
 */
double mrand_chisquare(mrand_t device, uint32_t degrees_of_freedom, uint32_t precision);
