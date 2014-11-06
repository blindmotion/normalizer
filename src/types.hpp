#pragma once

#include <vector>

using namespace std;

typedef vector<double> list;

double const EXCEL_SECOND = 1.0 / (24 * 60 * 60);
double const EXCEL_MILLIS = EXCEL_SECOND * 0.001;

double const EPSILON = 1e-17;