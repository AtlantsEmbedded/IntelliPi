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


double randn();
void stat_mean(double *a, double *mean, int dim_i, int dim_j);
void remove_mean_col(double *b, double *a, double *mean, int dim_i, int dim_j);
void show_matrix(double *A, int dim_i, int dim_j);
void randn_mtx(double *mtx, int dim_i, int dim_j);
void modify_mean_stddev(double *mtx,double new_mean,double new_stddev,int dim_i, int dim_j);

#endif
