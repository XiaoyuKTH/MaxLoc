#!/bin/bash

export OMP_NUM_THREADS=1
./MaxLoc

export OMP_NUM_THREADS=2
./MaxLoc

export OMP_NUM_THREADS=4
./MaxLoc

export OMP_NUM_THREADS=8
./MaxLoc

export OMP_NUM_THREADS=12
./MaxLoc

export OMP_NUM_THREADS=16
./MaxLoc

export OMP_NUM_THREADS=20
./MaxLoc

export OMP_NUM_THREADS=24
./MaxLoc

export OMP_NUM_THREADS=28
./MaxLoc

export OMP_NUM_THREADS=32
./MaxLoc
