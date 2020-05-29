/* 
 * pivot_base.h
 * 
 * Created: 28.02.2019
 * Author : Jakob Moosbauer
 */

#ifndef PIVOT_BASE_H_
#define PIVOT_BASE_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>
#include <algorithm>
#include <limits.h>
#include <bitset> //for debugging only

#define MAXN 16
#include "nauty.h"

#define POPCNT __builtin_popcount
#define POPCNTLL __builtin_popcountll

#define max_f(a) *(short*)(a+0)
#define max_f_r(a) *(a+2)
#define max_f_c(a) *(a+3)
#define med_f(a) *(float*)(a+4)
#define min_f(a) *(short*)(a+8)
#define min_f_r(a) *(a+10)
#define min_f_c(a) *(a+11)
#define fillin_max_f(a) *(short*)(a+12)
#define fillin_max_f_r(a) *(a+14)
#define fillin_max_f_c(a) *(a+15)
#define fillin_med_f(a) *(float*)(a+16)
#define fillin_min_f(a) *(short*)(a+20)
#define fillin_min_f_r(a) *(a+22)
#define fillin_min_f_c(a) *(a+23)
#define max_r(a) *(short*)(a+24)
#define max_r_r(a) *(a+26)
#define max_r_c(a) *(a+27)
#define med_r(a) *(float*)(a+28)
#define min_r(a) *(short*)(a+32)
#define min_r_r(a) *(a+34)
#define min_r_c(a) *(a+35)
#define fillin_max_r(a) *(short*)(a+36)
#define fillin_max_r_r(a) *(a+38)
#define fillin_max_r_c(a) *(a+39)
#define fillin_med_r(a) *(float*)(a+40)
#define fillin_min_r(a) *(short*)(a+44)
#define fillin_min_r_r(a) *(a+46)
#define fillin_min_r_c(a) *(a+47)

using namespace std;
typedef unsigned long long matr;
typedef unordered_map<matr, short> map;
typedef unsigned char* mata;
typedef struct{
  short max_field;
  float med_field;
  short min_field;
  short fillin_max_field;
  float fillin_med_field;
  short fillin_min_field;
  short max_ring;
  float med_ring;
  short min_ring;
  short fillin_max_ring;
  float fillin_med_ring;
  short fillin_min_ring;
} values;
typedef unordered_map<matr, values> map2;

map load_db(string, int);
map2 load_db2(string, int);
void increase(mata, char);
int elim(mata, char);
void swap_pivot(mata, char, char);
void printmat(mata, char);
void find_best_pivot(const mata, char, map&, short*, char*, char*);
void select_pivot(mata, char, map&, short*, char*, char*);
void select_pivot2(mata, char, map&, short*, char*, char*);
void write_result(string, int, matr*, char, short*, char*, char*);
void completion(graph*, int);
void canonisation(mata, mata);
int test_pivots(const mata, char, map2&, char*);
int test_pivots8(const mata, map2&, unsigned short*);

#endif
