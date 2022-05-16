#include "forkLabs.h"

#include <cmath>

double forkLabs::f(double x, double y) { return cos(y) + sin(x) * cos(x); }

bool forkLabs::isMaximum(double x, double y) {
	return (f(x, y) >= f(x, y - yI)) && (f(x, y) >= f(x, y + yI)) && (f(x, y) >= f(x - xI, y)) &&
		   (f(x, y) >= f(x + xI, y));
}

int forkLabs::countMaximums(double xA, double yA, int itersX, int itersY) {
	double y   = yA;
	double x   = xA;
	int result = 0;

	for (int i_y = 0; i_y < itersY; ++i_y) {
		x = xA;
		for (int i_x = 0; i_x < itersX; ++i_x) {
			if (forkLabs::isMaximum(x, y)) {
				++result;
			}
			x += forkLabs::xI;
		}
		y += forkLabs::yI;
	}
	return result;
}
