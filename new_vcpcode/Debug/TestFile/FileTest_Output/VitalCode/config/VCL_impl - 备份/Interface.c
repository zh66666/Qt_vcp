#include "Interface.h"
#include <stdio.h>

/*****************************************************************************
 *    Global Var Define
 *****************************************************************************/

/******************************************************************************
 *    Coded Operation Interface Implement
 ******************************************************************************/

/**
 * Encoding algorithm library's implement
 */
static T_CODE Array_EXTA;

static int RK[2];
static int Prime[2];

static long long i_temp_P1 = 8224836ll;
static long long i_temp_P2 = 11711720ll;

int g_GS_TL1;
int g_GS_TL2;
int dT;
long long add_enc(long long x, long long y)
 {
	return x + y;
}


