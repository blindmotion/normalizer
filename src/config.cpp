/*
 * Copyright (c) 2014, Blind Motion Project 
 * All rights reserved.
 */

#include "config.hpp"
#include "types.hpp"

DEFINE_double(block_diff_thres, 100.0, "Threshold for detecting when the phone was moved inside the vehicle. "
        "Shows maximal difference between vectors of acceleration when they are considered from the same interval.");

DEFINE_double(block_time_thres, 3.0 * EXCEL_SECOND, "Threshold for detecting when the phone was moved inside the vehicle. "
        "Shows maximal difference between time of neighboring vectors of acceleration "
        "when they are considered from the same interval.");

DEFINE_bool(adjacent, false, "Type of the algorithm for detecting when the phone was moved inside the vehicle.");

DEFINE_double(sm_radius, 0.5 * EXCEL_SECOND, "Radius in excel time, used in acceleration data smoothing.");

DEFINE_double(sm_range_part, 0.5, "Shows how many values will be taken for smoothing, "
        "very small and large ones will be thrown aside.");

DEFINE_double(z_range_part, 0.3, "Shows how many acceleration vectors will be taken for counting mean acceleration vector, "
        "very small and large ones will be thrown aside.");

DEFINE_double(speed_detection_thres, 3.0 * EXCEL_SECOND, "Sets maximal time length (in excel time) "
        "between two speed values for which speed derivative can be calculated.");

DEFINE_string(output, "", "Output file name, if nothing passed, will be \"norm_<input>\"");
