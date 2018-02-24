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

// �������ļ�: src/main_utils.cpp
extern string enter_fun_name;
extern list<Cmnt>* cmnts;
extern int fake_line_num;
extern void out_cmnt(list<Cmnt>* cmnt_list, int line, ofstream& out);
extern map<string, TFuncTmpRange> g_MapFuncTmpVar;

// �������ļ� : ../gen_pro.cpp
extern void gen_declist(list<Declaration>*);

// �������ļ� : ./gen_stmt.cpp
extern void gen_stmtlist(list<Statement>*, list<Var_info>*);
extern void print_stmtlist(list<Statement>*, list<Var_info>*);
//�������ļ�./gen_global_var.cpp
vector<string> get_global_var_infunc(string func_name);
string get_bind_local_var(string global_var, string func_name);
//�������ļ� ./gen_utils.h
extern const int P1;
extern const int P2;

static int if_level = 0; //��¼��ǰ��֧������ڵĲ���
static int while_level = 0; //��¼��ǰѭ��������ڵĲ���
static int if_level_max = -1; //��¼�����������з�֧���Ƕ�׵�������
static int while_level_max = -1; //��¼������������ѭ�����Ƕ�׵�������
//��¼��ǰ������ں�����
string function_name;
void BuildTmpVarDeclarationList(Compound_stmt *s, string __FuncName);

// ���ܣ���ģ�������Ǽ����֧��ѭ��Ƕ�ײ�����������䣬��������е�
// 			��֧��ѭ��Ƕ�׵Ĳ�����
// ���������s-������ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
static void count_if_while(Statement* s) {
	try {
		//��������������ָ�����Ч��
		check_pt_arg(s, "statement object 's pointer");

		//�����Ϊ�գ���ƥ����������
		switch (s->get_stamp()) {
		case 0: {
			//������Ϊ������䣬������ø�������е��������
			for (list<Statement>::iterator iter =
					s->get_ccstmt()->get_csl()->begin();
					iter != s->get_ccstmt()->get_csl()->end(); iter++) {
				count_if_while(&(*iter)); //����ÿ�����
			}
		}
			break;
		case 1: {
			//���Ϊ��֧���
			if_level++; //��֧������1

			//����if��֧�е����
			count_if_while(s->get_csstmt()->get_cs_if());
			//�жϷ�֧�ṹ�Ƿ���else��֧
			if (s->get_csstmt()->get_stamp() == 1) {
				//�������else��֧�������else��֧�е����
				count_if_while(s->get_csstmt()->get_cs_else());
			} else {
				//���û��else��֧�������
			}
			if_level--; //��֧������������֧������1

			//����ǰ��֧������ȫ�ַ�֧Ƕ�����������бȽϣ��޸�������
			if_level_max = if_level >= if_level_max ? if_level : if_level_max;
		}
			break;
		case 2: {
			//���Ϊѭ�����
			while_level++; //ѭ��������1
			count_if_while(s->get_cistmt()->get_cs()); //����ѭ���ڵ����
			while_level--; //ѭ��������������֧������1

			//����ǰѭ��������ȫ��ѭ��Ƕ�����������бȽϣ��޸�������
			while_level_max =
					while_level >= while_level_max ?
							while_level : while_level_max;
		}
			break;
		default: {
			//���Ϊ�������͵���䣬�򲻱���
		}
		}
	} catch (string str) {
		string error_info = "count if and while  in statement : " + str;
		throw error_info;
	}
}


//vector��ȫ��ȫ�ֱ�����Ӧ�İ󶨾ֲ�������������䣬��Ϊ��Щ�󶨾ֲ���������ǩ��
void gen_globalToProtect_dec(string strs, string fun_name, string var_scope) {
	string protecVarName = get_bind_local_var(strs, fun_name);
	//int var_sig = assign_var_sig(protecVarName, "common");
	//����ǩ��
	int sigIndex = -1;
	int sig = -1;
	sig = get_sig_index(&sigIndex);
	add_new_vars(var_infos, protecVarName, sig, var_scope, "common", sigIndex);
	//���������Ϣ
	out_indent(indent_num, gen_out);
	gen_out << "T_CODE " << protecVarName << " = {{0, {" << sig << ", "
			<< sig << "}}, "<<sig<<"};" << endl;

}
//ȫ�ֱ�����ֵ���󶨾ֲ����������
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

// ���ܣ��Ժ�������б��룬��������������ڸ��ӱ������������룬�Ժ����������������ı��룬�Լ������
// 			�вκ��������ĵ�λ��������
// ���������s-�����帴��������ָ��
// 			      f_i-������Ϣ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_fun_body(Compound_stmt* s, Fun_info* f_i) {
	try {
		//������������������ָ�����Ч��
		check_pt_arg(s, "compound statement object's pointer");
		//��������������Ϣ����ָ�����Ч��
		check_pt_arg(f_i, "function information object's pointer");

		function_name = "";

		//��ʼ����ʱ����������
		InitCodedTmpVarCnt();
		//��ʼ����ǰ��ʱ�����±�����
		TFuncTmpRange *CurFuncCodedIndexRange = new TFuncTmpRange();
		InitCodedTmpIndex(CurFuncCodedIndexRange);
		//��ʼ����ʱ��������λ�õ������ַ���������
		SpecStrId *CurSpecId = new SpecStrId();

		//��ʼ���������
		out_indent(indent_num, gen_out);
		gen_out << "{" << endl;
		indent_num++; //����������1

//		��ʼ�����ڼ���ѭ���ͷ�֧Ƕ�ײ�����ȫ�ֱ���
//		if_level=0;
//		while_level=0;
//		if_level_max=-1;
//		while_level_max=-1;

		//���㺯�����ڷ�֧��ѭ��Ƕ�׵Ĳ�����������ӳ�ڱ�ģ���ȫ�ֱ�����
		for (list<Statement>::iterator iter = s->get_csl()->begin();
				iter != s->get_csl()->end(); iter++) {
			count_if_while(&(*iter)); //����ÿ�����
		}

		string fun_name = f_i->get_name(); //��¼�ú���������
		if ("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name) {
			//�����У��ӿھͲ��Ժ������ڵ��������б���
		} else {
			//�����������ʱ�����Ķ���λ�ã��������ַ���ʾ

			//OutCodedTmpSpecStr(CurSpecId);

		}

		if ("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name) {
			//�����У��ӿھͲ��Ժ������ڵ��������б���
		} else {
			//���ú����е���ʱ���ӱ������������뵽�������������ǰ��

			BuildTmpVarDeclarationList(s, fun_name);
			gen_declist(s->get_cdl()); //�Ժ������ڵ��������б���
			//����׷��5/20
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

		int para_stamp = f_i->get_para_stamp(); //��¼�����Ĳ�����ǩֵ
		//�жϸú����Ƿ��в���
		if (para_stamp == 1) {
			if ("F_VCE_CheckVar" == fun_name || "F_VCE_CheckArr" == fun_name) {
				//�����У��ӿھͲ�����Ӧ�ĵ�����λ�Ĳ���
			} else {
				gen_out << endl;
				//����в����������������λ��������
				//�����洢�к����β���������
				/*for (list<string>::iterator iter = f_i->get_paras()->begin();
						iter != f_i->get_paras()->end(); iter++) {
					//���ÿ���βεĵ�λ��������
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
			//���û�в�������������
			;
		}
		if ("F_USR_DataFetch" == fun_name || "F_USR_DataRecv" == fun_name
				|| "F_USR_DataSend" == fun_name
				|| "F_USR_DataDeliver" == fun_name) {
			//print_stmtlist(s->get_csl(), var_infos);				//ֱ�����RSC�����
		} else {
			function_name = fun_name;
			//��������������ǩ��
			out_indent(indent_num,gen_out);
			gen_out<<"int S_Fun_out_this=S_Fun_out;"<<endl;
			//�����������ǩ��
			S_this=*++iter;
			S_last=S_this;
			idx++;
			out_indent(indent_num,gen_out);
			gen_out<<"P_pre_idx="<<idx<<";"<<endl;
			out_indent(indent_num,gen_out);
			gen_out<<"RS=RS+"<<S_this<<"-S_Fun_entry;"<<endl;
			gen_out<<endl;
			gen_stmtlist(s->get_csl(), var_infos);				//�Ժ������ڵ������б���
		}

		//���Դ���и���������ǰ��ע�ͣ��˴�������Ϊ�˷�ֹ����������ע�ͣ�����һ�����Ҳû��
		//�����
		out_cmnt(cmnts, s->get_fake_line(), gen_out);

		//�жϸú��������Ƿ��������ں�������ͬ
//		if (fun_name == enter_fun_name) {
//			//�����ͬ�����ڴ˴�������빤��ǩ��У�麯���ĵ��ô���
//			out_indent(indent_num, gen_out);
//			gen_out << "red_VarSigCheck();" << endl << endl;
//		} else {
//			//�������ͬ�������
//			;
//		}
		indent_num--;
		out_indent(indent_num, gen_out);
		if(f_i->get_return_stamp()==0)
		{
		//������������ǩ��
		out_indent(indent_num,gen_out);
		gen_out<<"S_Fun_out=S_Fun_out_this;"<<endl;
		out_indent(indent_num,gen_out);
		gen_out<<"RS=RS+S_Fun_out_this-"<<*iter<<";"<<endl;
		gen_out<<endl;}
		gen_out << "}" << endl;

		//�������������
		//ȷ����ǰ���������õ�����ʱ��������ֵ��Χ
		const int WHILE_AB_CODE = 8;		//While_A/B�������ŷ�Χ
		GetCodedTmpIndex(CurFuncCodedIndexRange,
				g_MapFuncTmpVar.find(var_scope)->second.VarRange[WHILE_AB_CODE]);
		//�����Ҫ�����������ʱ�����������
		BuildCodedTmpVarDeclrStmtSet(CurSpecId, CurFuncCodedIndexRange);

		delete CurFuncCodedIndexRange;
		delete CurSpecId;
	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding function's body :  " + str;
		throw error_info;
	}
}

// ���ܣ��Ը��������б��룬������������е�����������������
// ���������s-����������ָ��
//                  v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void gen_compd_stmt(Compound_stmt* s, list<Var_info>* v) {
	try {
		//������������������ָ�����Ч��
		check_pt_arg(s, "compound statement object's pointer");
		//����������̬ǩ������ָ�����Ч��
		check_pt_arg(v, "var_info list's pointer");

		//�������Ϊ�գ�����б���
		out_indent(indent_num, gen_out);
		gen_out << "{" << endl;
		indent_num++;		//������1

		gen_declist(s->get_cdl());		//������������б���

		gen_stmtlist(s->get_csl(), v);		//�����������б���

		//���Դ���и���������ǰ��ע��
		out_cmnt(cmnts, s->get_fake_line(), gen_out);

		indent_num--;		//���������������1
		out_indent(indent_num, gen_out);
		gen_out << "}" << endl;
	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding compound statement :  " + str;
		throw error_info;
	}
}
//������ʱ������������
void BuildTmpVarDeclarationList(Compound_stmt *s, string __FuncName) {
	//���뺯�������ӵ���ʱ����
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
			if(TmpStyle < 7)//�����޷���ֵ��������ʱ����
			{
				TmpVarName = BuildTmpVarName(TmpVarNamePrefix[TmpStyle],
						__FuncName, TmpVar_i);
			}
			else
			{
				//�޷���ֵ������ʱ����
				TmpVarName=TmpVarNamePrefix[TmpStyle];
			}
			Declarator *TmpVarDecltor = new Declarator(TmpVarName);	//�����ʼ�����ʽ�еı��������Ⱥ����

			Expression *TmpVarInitExp = new Expression("0");	//�����ʼ�����ʽ�ĵȺ��ұ�
			Initializer *TmpVarInitlizer = new Initializer(TmpVarInitExp);

			TmpVarInitDecl = new Init_declarator(TmpVarDecltor,
					TmpVarInitlizer);
			TmpVarInit_List->push_back(*TmpVarInitDecl);
		}
		TmpVarDecl = new Var_declaration(TmpVarDecl_spec, TmpVarInit_List);
		NewTmpVarDeclList->push_back(Declaration(TmpVarDecl));
	}

	//����������븴��������������ǰ��
	list<Declaration> *cur_decl_list = s->get_cdl();
	cur_decl_list->insert(cur_decl_list->begin(), NewTmpVarDeclList->begin(),
			NewTmpVarDeclList->end());

	NewTmpVarDeclList->clear();
	delete NewTmpVarDeclList;
}

