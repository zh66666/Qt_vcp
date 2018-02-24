#include "CodedVarBuild_Util.h"
#include "gen_utils.h"
#include "Fun_info.h"
#include "declaration.h"
#include "var_declaration.h"
#include "initializer.h"
#include "init_declarator.h"
#include "declaration_specifiers.h"
#include "declarator.h"
#include "expression.h"
#include "statement.h"
#include "compound_stmt.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "cmnt.h"
#include "TmpVarIndexRange.h"
#include "gen_secondary.h"

// 定义在文件: src/main_utils.cpp
extern string enter_fun_name;
extern list<Cmnt>* cmnts;
extern int fake_line_num;
extern void out_cmnt(list<Cmnt>* cmnt_list, int line, ofstream& out);
extern map<string, TFuncTmpRange> g_MapFuncTmpVar;

// 定义在文件 : ../gen_pro.cpp
extern void gen_declist(list<Declaration>*);

// 定义在文件 : ./gen_stmt.cpp
extern void gen_stmtlist(list<Statement>*, list<Var_info>*);
extern void print_stmtlist(list<Statement>*, list<Var_info>*);
//定义在文件./gen_global_var.cpp
vector<string> get_global_var_infunc(string func_name);
string get_bind_local_var(string global_var, string func_name);
//定义在文件 ./gen_utils.h
extern const int P1;
extern const int P2;

static int if_level = 0; //记录当前分支语句所在的层数
static int while_level = 0; //记录当前循环语句所在的层数
static int if_level_max = -1; //记录函数体内所有分支语句嵌套的最大层数
static int while_level_max = -1; //记录函数体内所有循环语句嵌套的最大层数
//记录当前语句所在函数名
string function_name;
void BuildTmpVarDeclarationList(Compound_stmt *s, string __FuncName);

// 功能：此模块作用是计算分支和循环嵌套层数。遍历语句，计算语句中的
// 			分支和循环嵌套的层数。
// 输入参数：s-语句对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
static void count_if_while(Statement* s) {
	try {
		//检查参数：语句对象指针的有效性
		check_pt_arg(s, "statement object 's pointer");

		//如果不为空，则匹配语句的类型
		switch (s->get_stamp()) {
		case 0: {
			//如果语句为复合语句，则遍历该复合语句中的语句链表
			for (list<Statement>::iterator iter =
					s->get_ccstmt()->get_csl()->begin();
					iter != s->get_ccstmt()->get_csl()->end(); iter++) {
				count_if_while(&(*iter)); //遍历每条语句
			}
		}
			break;
		case 1: {
			//如果为分支语句
			if_level++; //分支层数加1

			//遍历if分支中的语句
			count_if_while(s->get_csstmt()->get_cs_if());
			//判断分支结构是否含有else分支
			if (s->get_csstmt()->get_stamp() == 1) {
				//如果含有else分支，则遍历else分支中的语句
				count_if_while(s->get_csstmt()->get_cs_else());
			} else {
				//如果没有else分支，则忽略
			}
			if_level--; //分支遍历结束，分支层数减1

			//将当前分支层数与全局分支嵌套最大层数进行比较，修改最大层数
			if_level_max = if_level >= if_level_max ? if_level : if_level_max;
		}
			break;
		case 2: {
			//如果为循环语句
			while_level++; //循环层数加1
			count_if_while(s->get_cistmt()->get_cs()); //遍历循环内的语句
			while_level--; //循环遍历结束，分支层数减1

			//将当前循环层数与全局循环嵌套最大层数进行比较，修改最大层数
			while_level_max =
					while_level >= while_level_max ?
							while_level : while_level_max;
		}
			break;
		default: {
			//如果为其他类型的语句，则不遍历
		}
		}
	} catch (string str) {
		string error_info = "count if and while  in statement : " + str;
		throw error_info;
	}
}


//vector中全部全局变量对应的绑定局部变量的声明语句，并为这些绑定局部变量分配签名
void gen_globalToProtect_dec(string strs, string fun_name, string var_scope) {
	string protecVarName = get_bind_local_var(strs, fun_name);
	//int var_sig = assign_var_sig(protecVarName, "common");
	//分配签名
	int sigIndex = -1;
	int sig = -1;
	sig = get_sig_index(&sigIndex);
	add_new_vars(var_infos, protecVarName, sig, var_scope, "common", sigIndex);
	//输出冗余信息
	out_indent(indent_num, gen_out);
	gen_out << "T_CODE " << protecVarName << " = {{0, {" << sig << ", "
			<< sig << "}}, "<<sig<<"};" << endl;

}
//全局变量赋值给绑定局部变量的语句
void genProtectToGlobal(string strs, string fun_name, string var_scope) {
	string protecVarName = get_bind_local_var(strs, fun_name);
	int globalSig = get_current_sig(var_infos, strs, "global");
	int protectSig = get_current_sig(var_infos, protecVarName, var_scope);

	int con1 = (protectSig - globalSig) % P1;
	int con2 = (protectSig - globalSig) % P2;
	out_indent(indent_num, gen_out);
	gen_out << protecVarName << ".Data.F = " << "TC_" << strs << ".Data.F;"
			<< endl;
	out_indent(indent_num, gen_out);
	gen_out << protecVarName << ".Data.C.C1 = " << "(TC_" << strs << ".Data.C.C1 + "
			<< con1 << ") % P1 + g_GS_TL1;" << endl;
	out_indent(indent_num, gen_out);
	gen_out << protecVarName << ".Data.C.C2 = " << "(TC_" << strs << ".Data.C.C2 + "
			<< con2 << ") % P2 + g_GS_TL2;" << endl << endl;
}

// 功能：对函数体进行编码，包括输出函数体内附加变量的声明代码，对函数体内声明和语句的编码，以及输出对
// 			有参函数参数的低位调整代码
// 输入参数：s-函数体复合语句对象指针
// 			      f_i-函数信息对象指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_fun_body(Compound_stmt* s, Fun_info* f_i) {
	try {
		//检查参数：复合语句对象指针的有效性
		check_pt_arg(s, "compound statement object's pointer");
		//检查参数：函数信息对象指针的有效性
		check_pt_arg(f_i, "function information object's pointer");

		function_name = "";

		//初始化临时变量计数器
		InitCodedTmpVarCnt();
		//初始化当前临时变量下标索引
		TFuncTmpRange *CurFuncCodedIndexRange = new TFuncTmpRange();
		InitCodedTmpIndex(CurFuncCodedIndexRange);
		//初始化临时变量定义位置的特殊字符的索引号
		SpecStrId *CurSpecId = new SpecStrId();

		//开始函数体编码
		out_indent(indent_num, gen_out);
		gen_out << "{" << endl;
		indent_num++; //缩进个数加1

//		初始化用于计算循环和分支嵌套层数的全局变量
//		if_level=0;
//		while_level=0;
//		if_level_max=-1;
//		while_level_max=-1;

		//计算函数体内分支和循环嵌套的层数，其结果反映在本模块的全局变量中
		for (list<Statement>::iterator iter = s->get_csl()->begin();
				iter != s->get_csl()->end(); iter++) {
			count_if_while(&(*iter)); //遍历每条语句
		}

		string fun_name = f_i->get_name(); //记录该函数的名称
		if ("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name) {
			//如果是校验接口就不对函数体内的声明进行编码
		} else {
			//输出编码中临时变量的定义位置，用特殊字符表示

			//OutCodedTmpSpecStr(CurSpecId);

		}

		if ("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name) {
			//如果是校验接口就不对函数体内的声明进行编码
		} else {
			//将该函数中的临时附加变量的声明加入到函数声明链表的前部

			BuildTmpVarDeclarationList(s, fun_name);
			gen_declist(s->get_cdl()); //对函数体内的声明进行编码
			//闪闪追加5/20
//			vector<string> globalvar = get_global_var_infunc(fun_name);
//			vector<string>::iterator itrvec;
//			for (itrvec = globalvar.begin(); itrvec != globalvar.end();
//					itrvec++) {
//				gen_globalToProtect_dec(*itrvec, fun_name, var_scope);
//			}
//			for (itrvec = globalvar.begin(); itrvec != globalvar.end();
//					itrvec++) {
//				genProtectToGlobal(*itrvec, fun_name, var_scope);
//			}
		}

		int para_stamp = f_i->get_para_stamp(); //记录函数的参数标签值
		//判断该函数是否有参数
		if (para_stamp == 1) {
			if ("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name) {
				//如果是校验接口就不做相应的调整低位的操作
			} else {
				gen_out << endl;
				//如果有参数，则输出参数低位调整代码
				//遍历存储有函数形参名的链表
				/*for (list<string>::iterator iter = f_i->get_paras()->begin();
						iter != f_i->get_paras()->end(); iter++) {
					//输出每个形参的低位调整代码
					out_indent(indent_num, gen_out);

					gen_out << "TC_" << (*iter) << ".Data.C.C1 = (TC_"
							<< (*iter) << ".Data.C.C1 - g_ExFun_" << fun_name
							<< ")%P1;" << endl;
					out_indent(indent_num, gen_out);
					gen_out << "TC_" << (*iter) << ".Data.C.C2 = (TC_"
							<< (*iter) << ".Data.C.C2 - g_ExFun_" << fun_name
							<< ")%P2;" << endl;
					gen_out << endl;
				}*/
			}

		} else {
			//如果没有参数，则不作调整
			;
		}
		if ("F_USR_DataFetch" == fun_name || "F_USR_DataRecv" == fun_name
				|| "F_USR_DataSend" == fun_name
				|| "F_USR_DataDeliver" == fun_name) {
			//print_stmtlist(s->get_csl(), var_infos);				//直接输出RSC中语句
		} else {
			function_name = fun_name;
			//调整本函数出口签名
			out_indent(indent_num,gen_out);
			gen_out<<"int S_Fun_out_this=S_Fun_out;"<<endl;
			//调整函数入口签名
			S_this=*++iter;
			S_last=S_this;
			idx++;
			out_indent(indent_num,gen_out);
			gen_out<<"P_pre_idx="<<idx<<";"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<"RS=RS+"<<S_this<<"-S_Fun_entry;"<<endl;
			gen_out<<endl;
			gen_stmtlist(s->get_csl(), var_infos);				//对函数体内的语句进行编码
		}

		//输出源码中复合语句结束前的注释，此处功能是为了防止函数体中有注释，但是一条语句也没有
		//的情况
		out_cmnt(cmnts, s->get_fake_line(), gen_out);

		//判断该函数名称是否与编码入口函数名相同
//		if (fun_name == enter_fun_name) {
//			//如果相同，则在此处输出编码工程签名校验函数的调用代码
//			out_indent(indent_num, gen_out);
//			gen_out << "red_VarSigCheck();" << endl << endl;
//		} else {
//			//如果不相同，则忽略
//			;
//		}
		indent_num--;
		out_indent(indent_num, gen_out);
		if(f_i->get_return_stamp()==0)
		{
		//调整函数出口签名
		out_indent(indent_num,gen_out);
		gen_out<<"S_Fun_out=S_Fun_out_this;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+S_Fun_out_this-"<<*iter<<";"<<endl;
		gen_out<<endl;}
		gen_out << "}" << endl;

		//结束函数体编码
		//确定当前函数编码用到的临时变量索引值范围
		const int WHILE_AB_CODE = 8;		//While_A/B的索引号范围
		GetCodedTmpIndex(CurFuncCodedIndexRange,
				g_MapFuncTmpVar.find(var_scope)->second.VarRange[WHILE_AB_CODE]);
		//输出需要二次输出的临时变量定义代码
		BuildCodedTmpVarDeclrStmtSet(CurSpecId, CurFuncCodedIndexRange);

		delete CurFuncCodedIndexRange;
		delete CurSpecId;
	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding function's body :  " + str;
		throw error_info;
	}
}

// 功能：对复合语句进行编码，包括复合语句中的声明链表和语句链表
// 输入参数：s-复合语句对象指针
//                  v-动态签名表指针
// 输出参数：v-动态签名表指针
// 返回值：成功则返回void，失败则抛出错误信息
// 作者：李刚
void gen_compd_stmt(Compound_stmt* s, list<Var_info>* v) {
	try {
		//检查参数：复合语句对象指针的有效性
		check_pt_arg(s, "compound statement object's pointer");
		//检查参数：动态签名链表指针的有效性
		check_pt_arg(v, "var_info list's pointer");

		//如果均不为空，则进行编码
		out_indent(indent_num, gen_out);
		gen_out << "{" << endl;
		indent_num++;		//缩进加1

		gen_declist(s->get_cdl());		//对声明链表进行编码

		gen_stmtlist(s->get_csl(), v);		//对语句链表进行编码

		//输出源码中复合语句结束前的注释
		out_cmnt(cmnts, s->get_fake_line(), gen_out);

		indent_num--;		//编码结束后，缩进减1
		out_indent(indent_num, gen_out);
		gen_out << "}" << endl;
	} catch (string str) {
		//捕获被调函数抛出的错误信息，加上本模块的信息，继续向上抛出
		string error_info = "coding compound statement :  " + str;
		throw error_info;
	}
}
//构建临时变量声明链表
void BuildTmpVarDeclarationList(Compound_stmt *s, string __FuncName) {
	//加入函数中增加的临时变量
	string TmpVarNamePrefix[TMP_VAR_STYLE] = { ARITHM_INTTMP_PRE,
			ARITHM_UINTTMP_PRE, IF_TMP_BOOL_PRE, IF_LOGICSUB_PRE,
			WHILE_TMP_BOOL_PRE, WHILE_LOGICSUB_PRE, RELATION_CMP_PRE,
			VOID_FUNC_RETVAL, WHILE_AC_A_PRE, WHILE_AC_B_PRE };

	list<Declaration> *NewTmpVarDeclList = new list<Declaration>;

	Declaration *TmpDecl = NULL;
	Var_declaration *TmpVarDecl = NULL;
	Init_declarator *TmpVarInitDecl = NULL;
	Declaration_specifiers *TmpVarDecl_spec = NULL;
	list<Init_declarator> *TmpVarInit_List = NULL;

	TFuncTmpRange TmpRange = g_MapFuncTmpVar.find(__FuncName)->second;
	for (int TmpStyle = 0; TmpStyle <= TMP_VAR_STYLE - 3; TmpStyle++) {
		TmpVarDecl_spec = new Declaration_specifiers();
		TmpVarDecl_spec->set_fake_line(fake_line_num + 1);
		TmpVarInit_List = new list<Init_declarator>;
		for (int TmpVar_i = TmpRange.VarRange[TmpStyle].V_Start;
				TmpVar_i <= TmpRange.VarRange[TmpStyle].V_End; TmpVar_i++) {
			string TmpVarName;
			if(TmpStyle < 7)//不是无返回值函数的临时变量
			{
				TmpVarName = BuildTmpVarName(TmpVarNamePrefix[TmpStyle],
						__FuncName, TmpVar_i);
			}
			else
			{
				//无返回值函数临时变量
				TmpVarName=TmpVarNamePrefix[TmpStyle];
			}
			Declarator *TmpVarDecltor = new Declarator(TmpVarName);	//构造初始化表达式中的变量名，等号左边

			Expression *TmpVarInitExp = new Expression("0");	//构造初始化表达式的等号右边
			Initializer *TmpVarInitlizer = new Initializer(TmpVarInitExp);

			TmpVarInitDecl = new Init_declarator(TmpVarDecltor,
					TmpVarInitlizer);
			TmpVarInit_List->push_back(*TmpVarInitDecl);
		}
		TmpVarDecl = new Var_declaration(TmpVarDecl_spec, TmpVarInit_List);
		NewTmpVarDeclList->push_back(Declaration(TmpVarDecl));
	}

	//将新链表加入复合语句声明链表的前端
	list<Declaration> *cur_decl_list = s->get_cdl();
	cur_decl_list->insert(cur_decl_list->begin(), NewTmpVarDeclList->begin(),
			NewTmpVarDeclList->end());

	NewTmpVarDeclList->clear();
	delete NewTmpVarDeclList;
}

