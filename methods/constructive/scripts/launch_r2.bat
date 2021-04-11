@echo off
set inp=%1
set outp=%2
PATH C:\Program Files\R\R-3.6.1\bin;%path%
Rscript aggregate2.R %inp% > %outp%