#!/bin/bash

export OMP_NUM_THREADS=$1
make viewrst NUM=4
