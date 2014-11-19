#include <algorithm>
#include <vector>
#include <assert.h>
#include <iostream>

#include "types.hpp"
#include "utils.hpp"

using namespace std;

double integrating_mean(double time, double radius, list const &t, list const &x) {
    list::const_iterator low_it, up_it;
    low_it = lower_bound(t.begin(), t.end(), time - radius);
    up_it = upper_bound(t.begin(), t.end(), time + radius);

    int low = (int) (low_it - t.begin());
    int up = (int) (up_it - t.begin());

    if (low == up) {
        return 0;
    }

    if (up - low == 1) {
        return x[low];
    }

    double current = 0;
    for (int i = low; i < up; ++i) {
        if (i != low) {
            current += .5 * (t[i] - t[i - 1]) * (x[i] + x[i - 1]);
        }
    }
    return current / (t[up - 1] - t[low]);
}

double quantile_mean(double time, double radius, list const &t, list const &x, double percent) {
    list::const_iterator low_it, up_it;
    low_it = lower_bound(t.begin(), t.end(), time - radius);
    up_it = upper_bound(t.begin(), t.end(), time + radius);
    list x2;

    for (list::const_iterator it = low_it; it != up_it; ++it) {
        x2.push_back(x[it - t.begin()]);
    }

    //todo: optimize calculation to O(radius), without sorting
    sort(x2.begin(), x2.end());

    int n = (int) x2.size();
    int low = (int) floor(n * (0.5 * (1 - percent)));
    int up = (int) ceil(n * (0.5 * (1 + percent)));

    if (low == up) {
        return 0;
    }

    if (up - low == 1) {
        return x2[low];
    }

    double current = 0;
    for (int i = low; i < up; ++i) {
        current += x2[i];
    }
    return current / (up - low);
}

double simple_median(list &x) {
    // todo: optimize calculation to O(x.size()) without sorting, if method is needed
    sort(x.begin(), x.end());
    int n = (int) x.size();
    int low = 0;
    int up = n;
    if (low == up) {
        return 0;
    }
    return x[(low + up) / 2];
}

double median(double time, double radius, list const &t, list const &x) {
    list::const_iterator low_it, up_it;
    low_it = lower_bound(t.begin(), t.end(), time - radius);
    up_it = upper_bound(t.begin(), t.end(), time + radius);

    list x2;
    for (list::const_iterator it = low_it; it != up_it; ++it) {
        x2.push_back(x[it - t.begin()]);
    }

    return simple_median(x2);
}

void to_mean(list const &t, list const &x, list &res, double radius, double percent) {
    for (int i = 0; i < t.size(); ++i) {
        res.push_back(quantile_mean(t[i], radius, t, x, percent));
    }
}

double difference(int i, int j, list const &x, list const &y, list const &z) {
    return sqr(x[j] - x[i]) + sqr(y[j] - y[i]) + sqr(z[j] - z[i]);
}

vector<int> get_block_indices(list const &x, list const &y, list const &z, double threshold, bool adjacent) {
    vector<int> result;
    result.push_back(0);
    int last = 0;
    for (int i = 1; i < x.size(); ++i) {
        double diff = difference(last, i, x, y, z);
        if (diff >= threshold) {
            result.push_back(i);
            last = i;
        } else if (adjacent) {
            last = i;
        }
    }
    return result;

}

vector<vector<double>> get_rotation_matrix(double nx, double ny, double nz, double cos_phi, double sin_phi) {
    vector<vector<double>> m(3, vector<double>(3));
    m[0][0] = cos_phi + (1 - cos_phi) * nx * nx;
    m[0][1] = (1 - cos_phi) * nx * ny - sin_phi * nz;
    m[0][2] = (1 - cos_phi) * nx * nz + sin_phi * ny;
    m[1][0] = (1 - cos_phi) * ny * nx + sin_phi * nz;
    m[1][1] = cos_phi + (1 - cos_phi) * ny * ny;
    m[1][2] = (1 - cos_phi) * ny * nz - sin_phi * nx;
    m[2][0] = (1 - cos_phi) * nz * nx - sin_phi * ny;
    m[2][1] = (1 - cos_phi) * nz * ny + sin_phi * nx;
    m[2][2] = cos_phi + (1 - cos_phi) * nz * nz;
    return m;
}

struct Vector3d {
    double x, y, z, len_squared;
    Vector3d(double x, double y, double z) : x(x), y(y), z(z), len_squared(x * x + y * y + z * z) {
    }
};

bool operator <(Vector3d const & a, Vector3d const & b) {
    return a.len_squared < b.len_squared;
}

vector<vector<double>> get_z_rotation_matrix(int start, int end, list const &x, list const &y, list const &z, double part) {
    assert(start < end);
    // for now, the mean vector is assumed to be pointing down (long and short vectors are not counted)
    // todo: maybe a better algorithm, for example quantize vectors and take the most appearing one
    vector<Vector3d> acc;
    for (int i = start; i < end; ++i) {
        acc.push_back(Vector3d(x[i], y[i], z[i]));
    }
    sort(acc.begin(), acc.end());
    double xx = 0, yy = 0, zz = 0;
    int start2 = (int) (acc.size() * (1 - part) * 0.5);
    int end2 = (int) (acc.size() * (1 + part) * 0.5);
    assert(start2 < end2);
    for (int i = start2; i < end2; ++i) {
        xx += acc[i].x;
        yy += acc[i].y;
        zz += acc[i].z;
    }
    xx /= end2 - start2;
    yy /= end2 - start2;
    zz /= end2 - start2;
    double len = sqrt(sqr(xx) + sqr(yy) + sqr(zz));
    double len2 = sqrt(sqr(xx) + sqr(yy));

    if (len < EPSILON || len2 < EPSILON) {
        cerr << "Small len in get_z_rotation_matrix" << endl;
        return get_rotation_matrix(0, 0, 1, 1, 0);
    }

    double nx = yy / len2;
    double ny = -xx / len2;
    double nz = 0;
    double cos_phi = zz / len;
    double sin_phi = sqrt(1 - sqr(cos_phi));

    return get_rotation_matrix(nx, ny, nz, cos_phi, sin_phi);
}

vector<vector<double>> get_plane_rotation_matrix(int start, int end, list const &t, list const &x, list const &y,
        list const &tg, list const &zg) {
    assert(start < end);
    //todo: fix problem: it is unknown whether the X axis will be pointing forward or backward
    double xx = 0, yy = 0;
    double c = 0;
    for (int i = start; i < end; ++i) {
        double coeff = 1.0 / (1.0 + abs(zg[lower_bound(zg.begin(), zg.end(), t[i]) - zg.begin()]));
        xx += x[i] * coeff;
        yy += y[i] * coeff;
        c += coeff;
    }
    xx /= c;
    yy /= c;
    double len = sqrt(sqr(xx) + sqr(yy));
    if (len < EPSILON) {
        return get_rotation_matrix(0, 0, 1, 1, 0);
    }
    xx /= len;
    yy /= len;
    return get_rotation_matrix(0, 0, 1, -xx, -yy);
}

void rotate_block(int start, int end, list &x, list &y, list &z, vector<vector<double>> const &m) {
    for (int i = start; i < end; ++i) {
        double x2 = m[0][0] * x[i] + m[0][1] * y[i] + m[0][2] * z[i];
        double y2 = m[1][0] * x[i] + m[1][1] * y[i] + m[1][2] * z[i];
        double z2 = m[2][0] * x[i] + m[2][1] * y[i] + m[2][2] * z[i];
        x[i] = x2;
        y[i] = y2;
        z[i] = z2;
    }
}