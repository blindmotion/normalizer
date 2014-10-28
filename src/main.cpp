#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <string.h>

#include "types.hpp"
#include "plot.hpp"
#include "utils.hpp"

using namespace std;

string next_word(istream &s, char delim) {
    string str;
    getline(s, str, delim);
    return str;
}

void read_data(string const &filename, list &ta, list &xa, list &ya, list &za,
        list &tg, list &xg, list &yg, list &zg) {
    ifstream input(filename.c_str());
    string line;
    bool first = true;
    double offset = 0;
    while (getline(input, line)) {
        istringstream ss(line);
        string type;
        if (!getline(ss, type, ';')) {
            continue;
        }
        if (type == "1") {
            next_word(ss, ';');
            double t = atof(next_word(ss, ';').c_str());
            if (first) {
                first = false;
                offset = t;
            }
            t = (t - offset) / EXCEL_SECOND;
            ta.push_back(t);
            xa.push_back(atof(next_word(ss, ';').c_str()));
            ya.push_back(atof(next_word(ss, ';').c_str()));
            za.push_back(atof(next_word(ss, ';').c_str()));
        } else if (type == "4") {
            next_word(ss, ';');
            double t = atof(next_word(ss, ';').c_str());
            if (first) {
                first = false;
                offset = t;
            }
            t = (t - offset) / EXCEL_SECOND;
            tg.push_back(t);
            xg.push_back(atof(next_word(ss, ';').c_str()));
            yg.push_back(atof(next_word(ss, ';').c_str()));
            zg.push_back(atof(next_word(ss, ';').c_str()));
        }
    }
}

void write_data(string const &filename,
        list const &ta, list const &xa, list const &ya, list const &za,
        list const &tg, list const &xg, list const &yg, list const &zg) {
    ofstream output(filename);
    output << setprecision(11);
    for (int i = 0, j = 0; i < ta.size() || j < tg.size(); ) {
        if (j == tg.size() || i < ta.size() && ta[i] <= tg[j]) {
            output << "1;" << ta[i] << ';' << xa[i] << ';' << ya[i] << ';' << za[i] << endl;
            ++i;
        } else {
            output << "4;" << tg[i] << ';' << xg[i] << ';' << yg[i] << ';' << zg[i] << endl;
            ++j;
        }
    }
    output.close();

}

list ta, xa, ya, za;
list tg, xg, yg, zg;
list xa_mean, ya_mean, za_mean;

double radius = 4.0;
double diff_threshold = 1.0;
double range_part = 0.5;
string output_filename;

#ifdef PYPLOT
PyPlot plt;
#endif

int main(int argc, char const *argv[]) {
    if (argc <= 1) {
        cerr << "No input file given" << endl;
        return -1;
    }
    read_data(argv[1], ta, xa, ya, za, tg, xg, yg, zg);
    output_filename = "norm_" + string(argv[1]);
    for (int i = 2; i < argc; ++i) {
        if (!strncmp("radius=", argv[i], 7)) {
            radius = atof(argv[i] + 7);
        } else if (!strncmp("threshold=", argv[i], 10)) {
            diff_threshold = atof(argv[i] + 10);
        } else if (!strncmp("range=", argv[i], 6)) {
            range_part = atof(argv[i] + 6);
        } else if (!strncmp("output=", argv[i], 7)) {
            output_filename = argv[i] + 7;
        } else {
            cerr << "Unknown argument: " << argv[i] << endl;
        }
    }

    to_mean(ta, xa, radius, range_part, xa_mean);
    to_mean(ta, ya, radius, range_part, ya_mean);
    to_mean(ta, za, radius, range_part, za_mean);

    vector<int> block_starts = get_block_indices(xa_mean, ya_mean, za_mean, diff_threshold, false);
    for (int i = 0; i < block_starts.size(); ++i) {
        int start = block_starts[i];
        int finish = i < block_starts.size() - 1 ? block_starts[i + 1] : (int)ta.size();
        vector<vector<double> > rot_matrix = get_z_rotation_matrix(start, finish, xa_mean, ya_mean, za_mean);
        rotate_block(start, finish, xa_mean, ya_mean, za_mean, rot_matrix);
        int start2 = (int) (lower_bound(tg.begin(), tg.end(), ta[i]) - tg.begin());
        int finish2 = i < block_starts.size() - 1 ? (int) (lower_bound(tg.begin(), tg.end(), ta[i + 1]) - tg.begin()) :
                ((int)tg.size());
        rotate_block(start2, finish2, xg, yg, zg, rot_matrix);

        vector<vector<double> > rot_matrix2 = get_plane_rotation_matrix(start, finish, ta, xa_mean, ya_mean, tg, zg);
        rotate_block(start, finish, xa_mean, ya_mean, za_mean, rot_matrix2);
        rotate_block(start2, finish2, xg, yg, zg, rot_matrix2);

    }

    write_data(output_filename, ta, xa, ya, za, tg, xg, yg, zg);

#ifdef PYPLOT
    plt.plot(ta, xa_mean);
    plt.plot(ta, ya_mean);
    plt.plot(ta, za_mean);
    plt.plot(ta, zg);

    plt.show();
#endif

    return 0;
}