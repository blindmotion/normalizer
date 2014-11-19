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
#include "config.hpp"

using namespace std;

vector<vector<string>> read_table(string const &filename) {
    vector<vector<string>> table;
    ifstream input(filename);
    string line;
    int i = -1;
    while (getline(input, line)) {
        table.push_back(vector<string>());
        ++i;
        istringstream ss(line);
        string token;
        while (getline(ss, token, ';')) {
            table[i].push_back(token);
        }
    }
    return table;
}

void parse_data(vector<vector<string>> const &table, list &ta, list &xa, list &ya, list &za,
        list &tg, list &xg, list &yg, list &zg) {
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].size() == 0) {
            continue;
        }
        string type = table[i][0];
        if (type == "1") {
            double t = atof(table[i][2].c_str());
            ta.push_back(t);
            xa.push_back(atof(table[i][3].c_str()));
            ya.push_back(atof(table[i][4].c_str()));
            za.push_back(atof(table[i][5].c_str()));
        } else if (type == "4") {
            double t = atof(table[i][2].c_str());
            tg.push_back(t);
            xg.push_back(atof(table[i][3].c_str()));
            yg.push_back(atof(table[i][4].c_str()));
            zg.push_back(atof(table[i][5].c_str()));
        }
    }
}

void replace_data(vector<vector<string>> &table,
        list const &ta, list const &xa,
        list const &ya, list const &za,
        list const &tg, list const &xg,
        list const &yg, list const &zg) {
    int ia = 0;
    int ig = 0;
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].size() == 0) {
            continue;
        }
        string type = table[i][0];
        if (type == "1") {
            table[i][2] = to_string(ta[ia]);
            table[i][3] = to_string(xa[ia]);
            table[i][4] = to_string(ya[ia]);
            table[i][5] = to_string(za[ia]);
            ++ia;
        } else if (type == "4") {
            table[i][2] = to_string(tg[ig]);
            table[i][3] = to_string(xg[ig]);
            table[i][4] = to_string(yg[ig]);
            table[i][5] = to_string(zg[ig]);
            ++ig;
        }
    }
}

void write_data(string const &filename, vector<vector<string>> const &table) {
    ofstream output(filename);
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            output << table[i][j] << ((j + 1 < table[i].size()) ? ';' : '\n');
        }
    }
    output.close();

}

void dumb_track_calculation(list const &ta, list const &xa, list const &ya,
        list const &tg, list const &zg, list &x, list &y, int start = 0) {
    double x_cur = 0;
    double y_cur = 0;
    double alpha_cur = 0;
    double vx_cur = 0;
    double vy_cur = 0;
    for (int i = 0; i < start; ++i) {
        x.push_back(0);
        y.push_back(0);
    }
    for (int i = start, j = 0; i < ta.size() - 1; ++i) {
        while (j < zg.size() - 1 && tg[j + 1] < ta[i]) {
            alpha_cur += zg[j] * (tg[j + 1] - tg[j]);
            cout << alpha_cur << ' ' << j << ' ' << zg[j] << ' ' << tg[j + 1] << ' ' << tg[j] << endl;
            ++j;
        }
        double dt = ta[i + 1] - ta[i];
        double ax_cur = xa[i] * cos(alpha_cur) + ya[i] * sin(alpha_cur);
        double ay_cur = -xa[i] * sin(alpha_cur) + ya[i] * cos(alpha_cur);
        vx_cur += ax_cur * dt;
        vy_cur += ay_cur * dt;
        x_cur += vx_cur * dt;
        y_cur += vy_cur * dt;
        x.push_back(x_cur);
        y.push_back(y_cur);
    }
}

vector<vector<string>> table;

list ta, xa, ya, za;
list tg, xg, yg, zg;
list xa_mean, ya_mean, za_mean;
string output_filename;

#ifdef PYPLOT
PyPlot plt;
#endif

bool try_get_value(char const *arg, string const &key, double & result) {
    if (!strncmp((key + "=").c_str(), arg, key.length() + 1)) {
        result = atof(arg + (key.length() + 1));
        return true;
    } else {
        return false;
    }
}

bool try_get_value(char const *arg, string const &key, string & result) {
    if (!strncmp((key + "=").c_str(), arg, key.length() + 1)) {
        result = arg + (key.length() + 1);
        return true;
    } else {
        return false;
    }
}

bool parse_arg(char const *arg) {
    if (try_get_value("sm_radius", arg, config::sm_radius)) {
        return true;
    }
    if (try_get_value("block_thresh", arg, config::block_diff_thres)) {
        return true;
    }
    if (try_get_value("sm_part", arg, config::sm_range_part)) {
        return true;
    }
    if (try_get_value("z_part", arg, config::z_range_part)) {
        return true;
    }
    if (try_get_value("output", arg, output_filename)) {
        return true;
    }
    return false;
}

int main(int argc, char const *const *argv) {
    if (argc <= 1) {
        cerr << "No input file given" << endl;
        return -1;
    }

    table = read_table(argv[1]);
    parse_data(table, ta, xa, ya, za, tg, xg, yg, zg);
    output_filename = "../norm_" + string(argv[1]);
    for (int i = 2; i < argc; ++i) {
        if (!parse_arg(argv[i])) {
            cerr << "Unknown argument: " << argv[i] << endl;
        }
    }

    to_mean(ta, xa, xa_mean);
    to_mean(ta, ya, ya_mean);
    to_mean(ta, za, za_mean);

    vector<int> block_starts = get_block_indices(xa_mean, ya_mean, za_mean);
    cout << block_starts.size() << " block(s) found" << endl;
    for (int i = 0; i < block_starts.size(); ++i) {
        int start = block_starts[i];
        int finish = i < block_starts.size() - 1 ? block_starts[i + 1] : (int) ta.size();
        vector<vector<double>> rot_matrix = get_z_rotation_matrix(start, finish, xa_mean, ya_mean, za_mean);
        rotate_block(start, finish, xa_mean, ya_mean, za_mean, rot_matrix);
        int start2 = (int) (lower_bound(tg.begin(), tg.end(), ta[i]) - tg.begin());
        int finish2 = i < block_starts.size() - 1 ? (int) (lower_bound(tg.begin(), tg.end(), ta[i + 1]) - tg.begin()) :
                ((int) tg.size());
        rotate_block(start2, finish2, xg, yg, zg, rot_matrix);

        vector<vector<double>> rot_matrix2 = get_plane_rotation_matrix(start, finish, ta, xa_mean, ya_mean, tg, zg);
        rotate_block(start, finish, xa_mean, ya_mean, za_mean, rot_matrix2);
        rotate_block(start2, finish2, xg, yg, zg, rot_matrix2);
    }

    replace_data(table, ta, xa_mean, ya_mean, za_mean, tg, xg, yg, zg);
    write_data(output_filename, table);

#ifdef PYPLOT
//    list x, y;
//    dumb_track_calculation(ta, xa_mean, ya_mean, tg, zg, x, y, 1598); // 1598 to skip big pause in 2014-09-28_SensorDatafile
//    plt.plot(x, y);

    plt.plot(ta, xa_mean);
    plt.plot(ta, ya_mean);
    plt.plot(ta, za_mean);
    plt.plot(ta, zg);
    list vert;
    vert.push_back(-10);
    vert.push_back(10);
    for (int i = 1; i < block_starts.size(); ++i) {
        list hor;
        hor.push_back(ta[block_starts[i]]);
        hor.push_back(ta[block_starts[i]]);
        plt.plot(hor, vert, ", c='black', lw=3");
    }

    plt.show();
#endif

    return 0;
}