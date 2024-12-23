#include "omp-utils.hpp"

void setThreadNum(int num) {
    omp_set_dynamic(num == omp_get_num_procs());
    omp_set_num_threads(
        num != 0
        ? num
        : omp_get_num_procs()
    );
}
