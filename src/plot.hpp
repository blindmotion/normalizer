/*
 * Copyright (c) 2014, Blind Motion Project 
 * All rights reserved.
 */

#pragma once

#include "types.hpp"

using namespace std;

/*
 * Uses python2.7 to show plots. 'pylab' and 'numpy' libraries are used.
 */
struct PyPlot {
    PyPlot();

    ~PyPlot();

    void plot(list const &x, list const &y, string const &add = "");

    void show() const;

    void custom_method(string const &f) const;

private:
    FILE *p = NULL;
    int counter = 0;

    void save(string const &filename, list const &x, list const &y) const;

    string get_temp_name(int i) const;
};