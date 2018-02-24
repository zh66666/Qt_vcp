/*
 * init_Save_SigInfo.cpp
 *
 *  Created on: 2017年1月18日
 *      Author: cuihanze
 */
#include <string>
#include "new_Siginfo.h"
#include <iostream>
#include<vector>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int MAX = 10000;
using namespace std;

extern string red_path;

int S_last,S_this;
vector<int> sig_vector;//顺序结构签名表
vector<int>::iterator iter;//顺序结构签名表迭代器
int idx=0;//顺序结构当前预期签名索引
vector<int> sig_vector_select;//分支结构签名表
vector<int>::iterator iter_select;//分支结构签名表迭代器
int idx_select=0;//分支结构当前预期签名索引

/*extern void Init_Save_SigInfo();
extern int* sig_p;
extern vector<int> sig_vector;
vector<int>::iterator iter;

extern const int MAX = 10000;

bool isNotExit(int temp_sig){
	for (iter = sig_vector.begin(); iter != sig_vector.end(); iter++) {
		if(temp_sig == *iter){
			return false;
		}
	}
	return true;
}

void init_sig(){
	 srand((unsigned)time(NULL));
	 int temp_sig;
	 for (int i = 0; i < MAX; i++) {
		 temp_sig = rand();
		if(isNotExit(temp_sig)){
			sig_vector.push_back(temp_sig);
		}else{
			i--;
		}
	}
}

void Init_Save_SigInfo(){
	char filename[] = "signature.txt";
	ofstream fout(filename);
	init_sig();
	for (iter = sig_vector.begin(); iter != sig_vector.end(); iter++) {
		fout << *iter << endl;
	}
}*/

bool isNotExit(int temp_sig,vector<int>::iterator iter){
	for (iter = sig_vector.begin(); iter != sig_vector.end(); iter++) {
		if(temp_sig == *iter){
			return false;
		}
	}
	return true;
}

void init_sig(){
	 srand((unsigned)time(NULL));
	 int temp_sig;
	 for (int i = 0; i < MAX; i++) {
		 temp_sig = rand();
		if(isNotExit(temp_sig,iter)){
			sig_vector.push_back(temp_sig);
		}else{
			i--;
		}
	}

}
void init_sig_select(){
	 srand((unsigned)time(NULL));
	 int temp_sig;
	 for (int i = 0; i < MAX; i++) {
		 temp_sig = rand();
		if(isNotExit(temp_sig,iter_select)){
			sig_vector_select.push_back(temp_sig);
		}else{
			i--;
		}
	}

}

void Init_Save_SigInfo(){
	char thepath[100];
	char filename[]="";
	//初始化顺序结构签名表
	strcpy(filename,red_path.c_str());
	strcat(filename,"/siginfo.txt");
	ofstream fout(filename);
	//vector<int> sig_vector;
	init_sig();
	for (iter = sig_vector.begin(); iter != sig_vector.end(); iter++) {
		fout << *iter << endl;
	}
	iter = sig_vector.begin();
	S_last=*iter;
	//初始化分支结构签名表
/*	strcpy(filename,red_path.c_str());
	strcat(filename,"/siginfo_select.txt");
	ofstream fout2(filename);
	//vector<int> sig_vector;
	init_sig_select();
	for (iter_select = sig_vector_select.begin(); iter_select != sig_vector_select.end(); iter_select++) {
		fout2 << *iter_select << endl;
	}

	iter_select = sig_vector_select.begin();*/
}

/*
int main(){
	//Init_Save_SigInfo();
	Init_Save_SigInfo();
	iter = sig_vector.begin();
	sig_p = &(*iter);
	cout<<*sig_p++<<endl;
	cout<<*sig_p++;
	return 0;
}

*/
