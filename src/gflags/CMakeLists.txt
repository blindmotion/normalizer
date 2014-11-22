# Cloned from https://github.com/libmv/libmv.git and edited

#IF(WIN32)
#  include_directories(
#    ../msinttypes
#    ../pthreads-w32/include
#  )
#ENDIF(WIN32)

add_library(gflags gflags.cpp gflags_reporting.cpp gflags_completions.cpp)
