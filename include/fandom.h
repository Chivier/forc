/*
 * @Author       : Chivier Humber
 * @Date         : 2021-03-04 22:25:55
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-03-06 21:18:38
 * @Description  : file content
 * @FilePath     : /forc/fandom/fandom.h
 */

#include <time.h>
#include <math.h>

#define RAND_WITH_SEED

#ifdef RAND_WITH_SEED
// Random-number generator
const int kMultInitialExample = 32781;
const int kModInitialExample = 2796203;
const int kMulInitialExample = 125;
const int kLen1InitialExample = 9689;
const int kIfd1InitialExample = 471;
const int kLen2InitialExample = 127;
const int kIfd2InitialExample = 30;
const int kMaxRandomLengthInitialExample = 10000;

const int kRandomGeneratorMagicNumber1 = 387420489; // 3 ** 18
const int kRandomGeneratorMagicNumber2 = 1313131;
const int kRandomGeneratorMagicNumber3 = 8193;
const int kRandomGeneratorMagicNumber4 = 4099;

class random_generator_t {
  private:
    int mult;
    int mod;
    int mul;
    int len1;
    int ifd1;
    int len2;
    int ifd2;
    int maxlength;

    int seed;

    int ipnt1;
    int ipnf1;
    int ipnt2;
    int ipnf2;

    int *ir1;
    int *ir2;
    int *irn;

    int rand_number_index;

  public:
    random_generator_t(int mult, int mod, int mul, int len1, int ifd1, int len2, int ifd2, int maxlength, int seed):
        mult(mult), mod(mod), mul(mul),
        len1(len1), ifd1(ifd1), len2(len2), ifd2(ifd2),
        maxlength(maxlength), seed(seed),
        ir1(new int[len1]),
        ir2(new int[len2]),
        irn(new int[maxlength]) {
    }

    random_generator_t(int seed):
        mult(kMaxRandomLengthInitialExample),
        mod(kModInitialExample),
        mul(kMultInitialExample),
        len1(kLen1InitialExample),
        ifd1(kIfd1InitialExample),
        len2(kLen2InitialExample),
        ifd2(kIfd2InitialExample),
        maxlength(kMaxRandomLengthInitialExample),
        seed(seed),
        ir1(new int[kLen1InitialExample]),
        ir2(new int[kLen2InitialExample]),
        irn(new int[kMaxRandomLengthInitialExample]) {
    }

    ~random_generator_t() {
        delete [] ir1;
        delete [] ir2;
        delete [] irn;
    }

    void initialize() {
        rand_number_index = maxlength;
        int iseed = abs(seed) + 1;
        int k_r = kRandomGeneratorMagicNumber1 + 2 * iseed;
        int k1_r = kRandomGeneratorMagicNumber2 * iseed;

        k1_r = k1_r % mod;
        for (int index = 0; index < len1; ++index) {
            k_r = k_r * mult;
            k1_r = k1_r * mul % mod;
            ir1[index] = k_r + k1_r * kRandomGeneratorMagicNumber3;
        }

        for (int index = 0; index < len2; ++index) {
            k_r = k_r * mult;
            k1_r = k1_r * mul % mod;
            ir2[index] = k_r + k1_r * kRandomGeneratorMagicNumber4;
        }

        ipnt1 = 0;
        ipnt2 = 0;
        ipnf1 = ifd1;
        ipnf2 = ifd2;
    }

    double get_rand() {
        rand_number_index++;
        if (rand_number_index >= maxlength) {
            for (int index = 0; index < maxlength; ++index) {
                unsigned int l_r = ir1[ipnt1] ^ ir1[ipnf1];
                unsigned int k_r = ir2[ipnt2] ^ ir2[ipnf2];
                if (index == 0) {
                    std::cout << ir1[ipnf1] << std::endl;
                    std::cout << l_r << std::endl;
                    std::cout << k_r << std::endl;
                }

                irn[index] = l_r ^ k_r;
                ir1[ipnt1] = l_r;
                ir1[ipnf2] = k_r;
                ipnt1++;
                ipnf1++;
                if (ipnt1 == len1) ipnt1 = 0;
                if (ipnf1 == len1) ipnf1 = 0;
                ipnt2++;
                ipnf2++;
                if (ipnt2 == len2) ipnt2 = 0;
                if (ipnf2 == len2) ipnf2 = 0;
            }
            rand_number_index = 0;
        }
        return (1.0 * irn[rand_number_index]) / (1ll << 32) + 0.5;
    }
};
#else
#endif