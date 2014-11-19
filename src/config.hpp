#pragma once

namespace config {

    /*
     * Threshold for detecting when the phone was moved inside the vehicle.
     * Shows maximal difference between vectors of acceleration when they are considered from the same interval.
     */
    extern double block_diff_thres;

    /*
     * Type of the algorithm for detecting when the phone was moved inside the vehicle.
     */
    extern bool adjacent;

    /*
     * Radius in excel seconds, used in acceleration data smoothing.
     */
    extern double sm_radius;

    /*
     * Shows how many values will be taken for smoothing, very small and large ones will be thrown aside.
     */
    extern double sm_range_part;

    /*
     * Shows how many acceleration vectors will be taken for counting mean acceleration vector,
     * very small and large ones will be thrown aside.
     */
    extern double z_range_part;
}