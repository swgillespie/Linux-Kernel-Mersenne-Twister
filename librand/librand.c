#include "librand.h"
#include <unistd.h> // read(), write(), close()
#include <sys/stat.h>
#include <fcntl.h>    // open()
#include <string.h>
#include <math.h>  // pow()

#define DEVICE_NAME "/dev/mersenne"

mrand_t mrand_init() {
    mrand_t fd = open(DEVICE_NAME, O_RDONLY);
    return fd;
}

int mrand_destroy(mrand_t device) {
    return close(device);
}

int mrand_random(mrand_t device, void *data, size_t num_bytes) {
    return read(device, data, num_bytes); 
}

float mrand_float(mrand_t device) {
    char float_buf[23]; // 23 bits in significand for IEEE single precision
    uint32_t float_bits = 0;
    read(device, float_buf, 23); // read 23 random bits
    memcpy(&float_bits + (32 - 23), float_buf, 23); 
    float_bits |= (0xCB << 23);
    return *(float*)&float_bits;
}

double mrand_double(mrand_t device) {
    char double_buf[52]; // 52 bits in significand for IEEE double precision
    uint64_t double_bits = 0L;
    read(device, double_buf, 52); // read 52 random bits
    memcpy(&double_bits + (64 - 52), double_buf, 52);
    double_bits |= (0x3FCL << 52);
    return *(double*)&double_bits;
}

int mrand_int(mrand_t device) {
    int ret_val = 0;
    read(device, &ret_val, sizeof(int));
    return ret_val;
}

uint8_t mrand_coin_flip(mrand_t device) {
    // generate a random integer and see if its 17th bit is 1 or 0.
    int rand_val = 0;
    read(device, &rand_val, sizeof(int));
    if (rand_val & (1 << 17)) {
	return 1;
    } else {
	return 0;
    }
}

double mrand_gaussian(mrand_t device, uint32_t precision) {
    uint32_t i, j, counter;
    char buf[precision];
    read(device, buf, precision);
    for (i = 0; i < precision; i++) { // for every byte
	for (j = 0; j < 8; j++) {     // for every bit in a byte
	    if (buf[i] & (1 << j)) { // if the jth bit is 1, its a heads
		counter++;
	    }
	}
    }
    // normalize the result
    return (counter / (double)(precision * 8)) - 0.5;
} 

double mrand_chisquare(mrand_t device, uint32_t degrees_of_freedom, uint32_t precision) {
    uint32_t i;
    double running_total = 0;
    for (i = 0; i < degrees_of_freedom; i++) {
	running_total += pow(mrand_gaussian(device, precision), 2);
    }
    return running_total;
}
