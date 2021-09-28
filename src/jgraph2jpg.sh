#!/bin/bash

../include/jgraph/jgraph -P $1 | ps2pdf - | convert -density 300 - -quality 100 $1.jpg