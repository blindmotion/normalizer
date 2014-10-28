normalizer
==========

A library that normalizes sensors values as recording device is always in the same orientation

## Input format
Currently normalizer accepts only file with data, which format is described [here](https://github.com/blindmotion/docs/wiki/Csv-file-format). But its code can be easily changed to handle online incoming data.

## Output format
Output format differs from input format, normilizer outputs file with lines formatted like:
```
<type>;<seconds>;<x>;<y>;<z>
```
where `<type>` is either 1 or 4 (accelerometer/gyroscope), time is given in seconds from the start.

## Command line arguments
First argument is always an input filename. Other arguments are optional:
* `radius=<rad>`
* `threshold=<thres>`
* `range=<range>`
* `output=<name>` by default: `<name> = "norm_" + input_name`
