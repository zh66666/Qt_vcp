/*
 * Siginfo.h
 *
 *  Created on: 2017Äê1ÔÂ19ÈÕ
 *      Author: zhouhang
 */
#include <string>
#include <iostream>
#include "program.h"
#include "var_info.h"
#include <fstream>
#include <cstdlib>
#ifndef SRC_NEW_SIGINFO_H_
#define SRC_NEW_SIGINFO_H_
extern int* sig_p;
extern vector<int> sig_vector;
extern vector<int>::iterator iter;
void Init_Save_SigInfo();

#endif /* SRC_NEW_SIGINFO_H_ */
