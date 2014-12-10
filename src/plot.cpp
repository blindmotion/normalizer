/*
 * Copyright (c) 2014, Blind Motion Project 
 * All rights reserved.
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "types.hpp"
#include "plot.hpp"

using namespace std;


PyPlot::PyPlot() {
    p = popen("python", "w");
    fprintf(p, "from pylab import *\nimport numpy as np\n");
}

PyPlot::~PyPlot() {
    pclose(p);
    p = NULL;
    for (int i = 0; i < counter; ++i) {
        remove(get_temp_name(i).c_str());
    }
}

void PyPlot::plot(list const &x, list const &y, string const &add) {
    string filename = get_temp_name(counter++);
    save(filename, x, y);
    fprintf(p, "xx = np.loadtxt('%s', delimiter=',')\n"
            "plot(xx[:, 0], xx[:, 1] %s)\n", filename.c_str(), add.c_str());
}

void PyPlot::show() const {
    fprintf(p, "show(True)\n");
}

void PyPlot::custom_method(string const &f) const {
    fprintf(p, "%s\n", f.c_str());
}

void PyPlot::save(string const &filename, list const &x, list const &y) const {
    ofstream output(filename.c_str());
    output << setprecision(13);
    for (int i = 0; i < (int) x.size(); ++i) {
        output << x[i] << "," << y[i] << endl;
    }
    output.close();
}

string PyPlot::get_temp_name(int i) const {
    return "tmpplotdata" + to_string(i) + ".txt";
}