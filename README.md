# HW2
for special project. 

## Q1
* Linear filter
* Any size with specified filter mask
<br/>
Flow : 
  1. Padding
  2. 2D convolution ij is the index within the filter window
    y = sum{ Pij*Wij }  

## Q2  
* Nonlinear filter
* Any size, max/min median
<br/>
Flow :
  1. Padding
  2. Sort Pij within the filter window
  3. Choose max/min/median as result

## Q3
* Sobel
* Gaussian
* Laplacian
* Unsharp (USM)
** Amount (A)
** Radious (R)
** Threshold (T)

Based on Q1, test and compare the effect of different filter
