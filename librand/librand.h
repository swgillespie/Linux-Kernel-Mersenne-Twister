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
 * @param data - The buffer to write random data to.
 * @param num_bytes - The number of bytes to write.
 * @returns 0 on success, -1 otherwise. Sets errno on -1.
 */
int mrand_random(void *data, size_t num_bytes);

/*
 * Destroy a Mersenne Twister random number generator.
 * @param device - The random device to destroy. Subsequent
 *                 calls using this device will fail.
 * @returns 0 on success, -1 otherwise. Sets errno on -1.
 */
int mrand_destroy(mrand_t device);
