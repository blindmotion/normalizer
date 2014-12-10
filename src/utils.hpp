/*
 * Copyright (c) 2014, Blind Motion Project 
 * All rights reserved.
 */

#pragma once

#include "types.hpp"
#include "config.hpp"

using namespace std;

inline double sqr(double x) {
    return x * x;
}

/*
 * As points are not equidistant in ta, this mean is calculated by dividing integral in some neighbourhood
 * by the length of ta interval.
 */
double integrating_mean(double time, double radius, list const &t, list const &x);

/*
 * Mean is calculated by throwing aside boundary values and taking regular mean value of the rest.
 */
double quantile_mean(double time, double radius, list const &t, list const &x, double percent);

/*
 * Median in some neighbourhood with given radius.
 */
double median(double time, double radius, list const &t, list const &x);

/*
 * Mean function is applied to each element in array.
 */
void to_mean(list const &t, list const &x, list &res, double radius = FLAGS_sm_radius, double percent = FLAGS_sm_range_part);

double difference(int i, int j, list const &x, list const &y, list const &z);

/*
 * Returns division of the given array on different blocks.
 * New block is started if the current difference is bigger than the given threshold.
 * 'adjacent' parameter determines used algorithm for which points to use to count the current difference.
 */
vector<int> get_block_indices(list const &t, list const &x, list const &y, list const &z,
        double threshold = FLAGS_block_diff_thres, double time_thres = FLAGS_block_time_thres,
        bool adjacent = FLAGS_adjacent);

/*
 * Returns a rotation matrix which changes coordinates so that the Z axis points to the ground.
 */
vector<vector<double>> get_z_rotation_matrix(int start, int end, list const &x, list const &y, list const &z,
        double part = FLAGS_z_range_part);

/*
 * Returns a rotation matrix which changes coordinates so that the X axis points forward (currently may point backward).
 */
vector<vector<double>> get_plane_rotation_matrix(int start, int end, list const &t, list const &x, list const &y,
        list const &tg, list const &zg, list const &t_geo, list const &speed_geo, double speed_thres = FLAGS_speed_detection_thres);

/*
 * Applies rotation to given vectors of coordinates.
 */
void rotate_block(int start, int end, list &x, list &y, list &z, vector<vector<double>> const &m);
