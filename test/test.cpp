/*
 * @Author       : Chivier Humber
 * @Date         : 2021-03-05 08:04:03
 * @LastEditors  : Chivier Humber
 * @LastEditTime : 2021-03-06 01:12:33
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
    cout << rng.get_rand() << endl;
    cout << rng.get_rand() << endl;
    cout << rng.get_rand() << endl;
}

int main(int argc, char **argv) {
    Test2();
}