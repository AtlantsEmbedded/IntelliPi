/**
 * @file stats.h
 * @author Billy Labelle, Atlans embedded
 * @date July, 2015
 * @brief This library contains the headers of the various utility functions to analyse data through statistics
 * presently in stats.c
 *
 * It should be noted that this library is row major. Matrix must be organized
 * such that the row elements are contiguous in memory.
 * Ex: a 3x4 matrix should be:
 * 1234
 * 5678
 * 9012
 * */

#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**Calculates the mean of each column of a matrix*/
void stat_mean(double *a, double *mean, int dim_i, int dim_j);

/**Removes the mean from each value depending of their column in the matrix*/
void remove_mean_col(double *a, double *mean,double *b, int dim_i, int dim_j);

/**Modifies the mean and the standard deviation of each column of a matrix with values
 * decided by the user.*/
void modify_mean_stddev(double *mtx,double new_mean,double new_stddev,int dim_i, int dim_j);

/**Generates a normally distributed random number*/
double randn();

/**Generates a matrix of normally distributed random number*/
void randn_mtx(double *mtx, int dim_i, int dim_j);

/**Shows the matrix to the user on the screen*/
void show_matrix(double *A, int dim_i, int dim_j);

#endif
