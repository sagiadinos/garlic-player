#!/bin/bash
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib ; export LD_LIBRARY_PATH
bin/garlic-player $1 $2

exit 0;