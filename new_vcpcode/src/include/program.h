#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "common.h"

/* a class used to store the whole pragram structure*/
class Program
{
	private:
		list<Declaration>* declist_;//store the declartations
		list<Toke_num>* inclist_;//store the includings

	public:
		Program();
		explicit Program(list<Toke_num>*,list<Declaration>*);

		list<Declaration>* get_declist();//get the declarations
		list<Toke_num>* get_inclist();//get the includings
};

#endif

