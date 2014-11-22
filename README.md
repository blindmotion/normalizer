normalizer
==========

A library that normalizes sensors values as recording device is always in the same orientation

## Input format
Currently normalizer accepts only file with data, which format is described [here](https://github.com/blindmotion/docs/wiki/Csv-file-format). But its code can be easily changed to handle online incoming data.

## Output format
Same as input.

## Command line arguments
First argument is always an input filename. Other arguments are optional:
* `-block_diff_thres <bl_thres>`
* `-block_time_thres <tm_thres>`
* `-sm_radius <sm_rad>`
* `-sm_range_part <sm_part>`
* `-z_range_part <z_part>`
* `-speed_detection_thres <sp_thres>`
* `-adjacent` & `-noadjacent`
* `-output <name>` by default: `<name> = "norm_" + input_name` (default doesn't assume that `input_name` can be a path to file)<br>
Descriptions and default values of constants are available in config.cpp file or by `-help` argument.
