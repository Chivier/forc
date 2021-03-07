/*
 * @Author       : Chivier Humber
 * @Date         : 2021-03-05 08:04:03
 * @LastEditors  : Chivier Humber
<<<<<<< HEAD
 * @LastEditTime : 2021-03-06 21:08:00
=======
 * @LastEditTime : 2021-03-07 11:54:10
>>>>>>> 976b31ac2f318e466c6af771791caaf89da76ab4
 * @Description  : file content
 * @FilePath     : /forc/test/test.cpp
 */

#include "fime.h"
#include "farray.h"
#include "fandom.h"

#include<unistd.h>

using namespace std;

void Test1() {
    fime_t time1;
    fime_t time2;
    time1.measure();
    cout << time1 << endl;
    usleep(3000000);
    time2.measure();
    cout << time2 << endl;
    cout << time2 - time1 << endl;
}

void Test2() {
    int seed = 1234;
    random_generator_t rng(seed);
    rng.initialize();
    double test;
    for (int index = 0; index < 10; index++) {
        test = rng.get_rand();
        cout << test << endl;
    }
    cout << test << endl;
}

int main(int argc, char **argv) {
    Test2();
}