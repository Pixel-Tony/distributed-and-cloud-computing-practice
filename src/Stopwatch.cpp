#include "Stopwatch.hpp"
#include <omp.h>

Stopwatch::Stopwatch() : ts(omp_get_wtime()) {}

double Stopwatch::reset() {
	double old = ts;
	ts = omp_get_wtime();
	return ts - old;
}
