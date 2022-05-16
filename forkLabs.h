#ifndef FORK_FORKLABS_H
#define FORK_FORKLABS_H

#include <thread>

namespace forkLabs {
double f(double x, double y);
bool isMaximum(double x, double y);
int countMaximums(double xA, double yA, int itersX, int itersY);

const double xA = -10000;
const double xB = 10000;
const double xI = 1;

const int count_iterations_x = int((xB - xA + 1) / xI);

const double yA = -10000;
const double yB = 10000;
const double yI = 1;

const int count_iterations_y = int((yB - yA + 1) / yI);

const int COUNT_PROCESSES = int(std::thread::hardware_concurrency());
/* Maybe division by zero */
const int COUNT_ITERS_Y_PROCESS = count_iterations_y / COUNT_PROCESSES;
const int COUNT_ITERS_X_PROCESS = count_iterations_x / COUNT_PROCESSES;

}  // namespace forkLabs


#endif	// FORK_FORKLABS_H
