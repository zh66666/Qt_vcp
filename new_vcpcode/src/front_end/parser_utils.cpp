#include "cmnt.h"
#include "expression.h"
#include "struct_specifier.h"
#include "statement.h"
#include "var.h"
#include "var_declaration.h"
#include "declaration_specifiers.h"
#include "type_specifier.h"
#include "init_declarator.h"
#include "declarator.h"
#include "program.h"

#include "vcp_yacc.h"
#include<map>
#include<list>
// ��¼��������ע�͹ؼ��ֵĳ��ִ���
static int bool_cmnt_key_num = 0;

//��¼NOADJע�͹ؼ��ֳ��ֵĴ���
static int noadj_cmnt_key_num = 0;

//��¼NODTע�͹ؼ��ֳ��ֵĴ���
static int nodt_cmnt_key_num = 0;

//��¼unsignedע�͹ؼ��ֳ��ֵĴ���  ����
static int unsign_cmnt_key_num = 0;

// ���浱ǰ�ļ����û��Զ������������
list<string>* type_names = new list<string>;

// һ����������,����һ���������������ע�͹ؼ���ʱ����true
bool enter_noparse = false;

//�洢�﷨�����Ľ������Ҫ��ʼ��ΪNULL
Program *front_end_pro = NULL;

//��¼λ�ڽṹ��Ĳ���������һ���ṹ�����ͣ��ͼ�1���˳�ʱ��1
//2010��3��4��׷��
int struct_body = 0;

// �������ļ� : src/main_utils.c
extern string int2str(int);
extern list<Cmnt>* cmnts;
extern int line_num;
extern void check_str_arg(string arg_name, string info);
extern void check_pt_arg(void *pt, string info);

// �������ļ� : ./vcp_lex.cpp
extern FILE *yyin;

// �������ļ� : ./vcp_yacc.cpp
extern int yyparse(void);

extern string compute_exp(Expression* s, int* result);

Var_declaration* get_var_declaration() {
	//todo
	/*���� ����õ�ǰ���������б�
	 ���������
	 ����ֵ��v_d ���������б�
	 ���ߣ�����*/

}

// ���ܣ�����ɨ�赽��ע�ͣ��������ȫ�ֱ�����ͬʱ�洢ע�͡�
// ���������cmnt-ע�������ָ��
// ����ֵ���ɹ�����void,ʧ���׳�������Ϣ
// ���ߣ���ա��߼γ�
void store_cmnts(Cmnt* cmnt) {
	try {
		//��������ע�Ͷ���ָ�����Ч��
		check_pt_arg(cmnt, "comment object's pointer");

		string cmnt_content = cmnt->get_content();	//��ȡע������

		//ƥ��ע�͹ؼ���
		if ("/*KWNOADJ*/" == cmnt_content) {
			//�����NOADJע�͹ؼ��֣�����������1
			noadj_cmnt_key_num++;

			//2010��3��4��׷��
			//�ж��Ƿ�λ�ڽṹ����
			if (struct_body != 0) {
				//���λ�ڽṹ���ڣ��򱨴�
				string error_info =
						"//*KWNOADJ*// should not be in struct body!";
				throw error_info;
			}
		} else if ("/*KWNODT*/" == cmnt_content) {
			//�����NODTע�͹ؼ��֣�����������1
			nodt_cmnt_key_num++;

			//2010��3��4��׷��
			//�ж��Ƿ�λ�ڽṹ����
			if (struct_body != 0) {
				//���λ�ڽṹ���ڣ��򱨴�
				string error_info = "//*KWNODT*// should not be in struct body!";
				throw error_info;
			}

		} else if ("/*KWCODEDBOOL*/" == cmnt_content) {
			//����ǲ���ע�͹ؼ��֣�����������1
			bool_cmnt_key_num++;
		} else if ("/*KWNOPARSE*/" == cmnt_content) {
			//����Ƿǹؼ�����ע�͹ؼ��֣�����ȫ�ֱ���enter_noparse
			enter_noparse = true;
		} else {
			//����ע�ͺ���
		}

		cmnts->push_back(*cmnt);	//�洢��ע��
	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "analyzing comment : " + str;
		throw error_info;
	}
}

// ���ܣ���ȡ�����е��Զ����������ơ���������������storage specifier��
//			�������storage specifier ��typedef����˵����������Ϊ�Զ������ͣ�
//			ȡ���������ƣ�����list��
// ������� : v_d-Var_declaration��Ķ���ָ��
// ���������l-�Զ�������������ָ��
// ����ֵ���ɹ�����void��ʧ�����׳�����
// ���ߣ���ա��߼γ�
void get_type_name(list<string>* l, Var_declaration* v_d) {
	try {
		//���������Զ�������������ָ�����Ч��
		check_pt_arg(l, "name list's pointer");
		//��������������������ָ�����Ч��
		check_pt_arg(v_d, "varibale declaration object's pointer");

		//�жϸ���������ı�ǩֵ�����������storage specifier����
		if (1 == v_d->get_stamp()
				&& "typedef" == v_d->get_decl_spec()->get_stor_spec()) {
			//�����ǩΪ1����storage specifierΪtypedef��˵��Ϊ�Զ�����������
			for (list<Init_declarator>::iterator iter =
					v_d->get_inits()->begin(); iter != v_d->get_inits()->end();
					iter++) {
				//���Զ����������ƴ���������
				l->push_back(iter->get_dec()->get_name());
			}
		} else {
			//�����Զ������������������
		}

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "getting user-defined name : " + str;
		throw error_info;
	}
}

// ���ܣ��ж�ɨ�赽���ַ����Ǳ�ʶ���������Զ�����������������list�в����ַ�����
//			����У��򷵻غ궨��TYPE_NAME�����򷵻غ궨��IDENTIFIER��
// 			�궨��TYPE_NAME ��IDENTIFIER��bison���ɵ�ͷ�ļ����ж���
// ���������yytext-ɨ�赽���ַ���
// 		      l-���ڴ洢�Զ��������������Ƶ�listָ��
// ����ֵ���ɹ��򷵻�����ֵ��ʧ�����׳�����
// ���ߣ���ա��߼γ�
int check_type(string yytext, list<string>* l) {
	try {
		//��������ɨ�赽���ַ�������Ч��
		check_str_arg(yytext, "string whic scanned");

		//���������Զ���������������ָ�����Ч��
		check_pt_arg(l, "user-defined name list's pointer");

		list<string>::iterator result = find(l->begin(), l->end(), yytext);
		//�ж�list�������Ƿ񵽴���listĩβ
		if (result == l->end()) {
			//�ﵽlistĩ�ˣ�˵�����ַ���Ϊ��ʶ��
			return IDENTIFIER;		// ���ر�ʾ���ĺ궨��
		} else {
			//δ��listĩ�ˣ�˵�����ַ���Ϊ�Զ���������
			return TYPE_NAME;		// �����Զ������͵ĺ궨��
		}

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "checking type : " + str;
		throw error_info;
	}
}

// ���ܣ������������Ƿ���ϰ�ȫ���봦�����Ӽ��淶����������ṹ�岼�������Ͳ����������
// 			����������void���͵ı���
// ���������v_d-��������ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ����
void check_var_dec(Var_declaration* v_d) {
	//��������������������ָ�����Ч��
	check_pt_arg(v_d, "varibale declaration object's pointer");

	//�жϴ˱��������������Ƿ�Ϊvoid����
	if (v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp() == 0
			&& v_d->get_decl_spec()->get_type_spec_list()->front().get_type_name()
					== "void") {
		string error_info = "void variable is not support!";
		throw error_info;
	}

	//�ж��Ƿ��ǲ�����������
	if (v_d->get_stamp() == 1
			&& (bool_cmnt_key_num % 2 == 1
					|| (v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()
							== 0
							&& v_d->get_decl_spec()->get_type_spec_list()->front().get_type_name()
									== "bool"))) {
		//�����������ı�������
		for (list<Init_declarator>::iterator iter = v_d->get_inits()->begin();
				iter != v_d->get_inits()->end(); iter++) {
			//���������������������׳�����
			if (iter->get_dec()->get_stamp() == 1) {
				string error_info = "boolean array is not support!";
				throw error_info;
			}
		}
	}
	//�ж��Ƿ��ǲ����ṹ���������
	if (v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp() == 2
			&& bool_cmnt_key_num % 2 == 1) {
		string error_info = "boolean struct variable is not support!";
		throw error_info;
	}

	if (v_d->get_decl_spec()->get_stor_spec() != "typedef"
			&& v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()
					== 2) {
		string error_info ="struct type must modifier by typedef!";
		throw error_info;
	}

	//2010��3��5��׷��
	//�жϵ�ǰ���������Ƿ�ṹ���������
	if (1 == v_d->get_stamp()
			&& v_d->get_decl_spec()->get_stor_spec() != "typedef"
			&& v_d->get_decl_spec()->get_type_spec_list()->front().get_stamp()
					== 2) {
		//����ǽṹ�������������ȡ���ṹ�����������
		string type_name =
				v_d->get_decl_spec()->get_type_spec_list()->front().get_struct_spec()->get_name();
		//�����������Ľṹ�����������Ƿ��б�����ͬ��������ͬ
		for (list<Init_declarator>::iterator iter = v_d->get_inits()->begin();
				iter != v_d->get_inits()->end(); iter++) {
			//�жϵ�ǰ�������ı����������Ƿ���ṹ����������ͬ
			if (type_name == iter->get_dec()->get_name()) {
				//�����ͬ���򱨴�
				string error_info =
						"struct type name should not be same as the variable's name!";
				throw error_info;
			}
		}
	}
}

// 2010��3��5��׷��
// ���ܣ��������Լ�������±���ʽ�Ƿ����C�Ӽ��淶������������±���ʽ���Ǽ򵥱������߳�����
//           �ͱ���
// ���������s-�����±���ʽ����ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ����
void check_array_sub(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");

	//�жϱ��ʽ����
	switch (s->get_stamp()) {
	case 0:	//���������������ı��ʽ

	case 1:	//����һ���������ı��ʽ
	{
		int i = 0;
		string result = compute_exp(s, &i);
		if ("wrong" == result) {
			string error_info = "array's subscript expression is invalid!";
			throw error_info;
		}
	}
		break;
	case 5:	//�������ñ��ʽ
	{
		string error_info = "array's subscript expression is invalid!";
		throw error_info;
	}
		break;

	case 2: {
		//��������ű��ʽ�����������еı��ʽ
		check_array_sub(s->get_exp());
	}
		break;

	case 3:	//�������ʽ
	case 4:	//�������ʽ
	{
	}
		break;
	}
}

// 2010��3��9��׷��
// ���ܣ������㺯���Ƿ��ڱ��ʽ�б����ã�����ǣ��򱨴�
// ���������s-���ʽ����ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ����
void check_fun(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");

	//�жϱ��ʽ����
	switch (s->get_stamp()) {
	case 0: {
		//���������������ı��ʽ�������������������Ҳ�����
		check_fun(s->get_cel());
		check_fun(s->get_cer());
	}
		break;
	case 1: {
		//����һ���������ı��ʽ����������Ҳ�����
		check_fun(s->get_cer());
	}
		break;
	case 2: {
		//���ű��ʽ����������еı��ʽ
		check_fun(s->get_exp());
	}
		break;
	case 5: {
		//�������ñ��ʽ������
		string error_info = "compute function calling is invalid!";
		throw error_info;
	}
		break;
	}
}

// 2010��3��9��׷��
// ���ܣ���鸳ֵ������ֵ���������ͣ�����Ǽ򵥱������±�Ϊ����������������ṹ��������±�Ϊ����
//		   �Ľṹ������������򷵻�0�����򷵻�1
// ���������s-��������ָ��
//����ֵ���ɹ�����һ������ ��ʧ���׳�����
// ���ߣ����
int check_left_var(Var* s) {
	//����������������ָ�����Ч��
	check_pt_arg(s, "variable object's pointer");

	//�жϱ���������
	switch (s->get_stamp()) {
	case 0: {
		//�����һ��ļ򵥱���
		return 0;
	}
		break;

	case 2: {
		//�������������������������±��ǲ��ǳ���
		list<Expression>::iterator iter;
		for (iter = s->get_exp_list()->begin();
				iter != s->get_exp_list()->end(); iter++) {
			//�ж��±���ʽ�Ƿ��ǳ���
			if (3 != iter->get_stamp()) {
				//������ǳ������򷵻�1
				return 1;
			}
		}
		//�ж��Ƿ��Ѿ������������±�����
		if (iter == s->get_exp_list()->end()) {
			//����Ѿ������꣬����ÿ�����ʽ��Ϊ����
			return 0;
		}
	}
		break;

	case 3: {
		//����ǽṹ�������������ṹ�����
		int i = check_left_var(s->get_var());
		return i;

	}
		break;

	case 4: {
		//����ǽṹ��������������ȷ���������±�
		list<Expression>::iterator iter;
		for (iter = s->get_exp_list()->begin();
				iter != s->get_exp_list()->end(); iter++) {
			//�ж��±���ʽ�Ƿ��ǳ���
			if (3 != iter->get_stamp()) {
				//������ǳ������򷵻�1
				return 1;
			}
		}
		//�ж��Ƿ��Ѿ������������±�����
		if (iter == s->get_exp_list()->end()) {
			//����Ѿ������꣬����ÿ�����ʽ��Ϊ����������������ṹ�����
			int i = check_left_var(s->get_var());
			return i;
		}
	}
		break;
	}
}

// 2010��3��9��׷��
// ���ܣ���鸳ֵ������Ƿ�����˼��㺯���ķǷ����ã�������ڣ��򱨴�
// ���������s-������ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ����
void check_assign_stmt(Statement* s) {
	//��������������ָ�����Ч��
	check_pt_arg(s, "statement object's pointer");

	//������ֵ����
	int i = check_left_var(s->get_cvar());

	//�ж���ֵ�����Ƿ���ϵ��ü��㺯���Ĺ淶
	if (0 == i) {
		//�����ֵ�������Ϲ淶�����ж��ұ��ʽ�Ƿ�Ϊ���㺯������
		if (5 == s->get_ce()->get_stamp()) {
			//�����ֵ���ʽ�Ǻ������ã��������������
			for (list<Expression>::iterator iter =
					s->get_ce()->get_args()->begin();
					iter != s->get_ce()->get_args()->end(); iter++) {
				check_fun(&(*iter));
			}
		} else {
			//�����ֵ���ʽ���Ǻ������ã����������
			check_fun(s->get_ce());
		}
	} else {
		//�����ֵ���������Ϲ淶����ֱ�ӷ�����ֵ���ʽ
		check_fun(s->get_ce());
	}
}

void check_assign_mix() {

}
// 2010��3��9��׷��
// ���ܣ������߼���������ߵı��ʽ�Ƿ����߼����㣬����У��򱨴�
// ���������s-���ʽ����ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ����
void check_unlogic_exp(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");

	//�жϱ��ʽ����
	switch (s->get_stamp()) {
	case 0: {
		//����Ǵ��������������ı��ʽ�����жϲ������Ƿ�Ϊ�߼������
		if ("&&" == s->get_op() || "||" == s->get_op()) {
			//���Ϊ�߼���������򱨴�
			string error_info = "&& ,|| expression is invalid!";
			throw error_info;
		} else {
			//�����Ϊ�߼��������������������Ҳ�����
			check_unlogic_exp(s->get_cel());
			check_unlogic_exp(s->get_cer());
		}
	}
		break;

	case 1: {
		//������߼������㡢��λȡ�����㣬�򱨴�
		if ("!" == s->get_op()) {
//			string error_info = "! expression is invalid!";
//			throw error_info;
		}
		if ("~" == s->get_op()) {
//			string error_info = "~ expression is invalid!";
//			throw error_info;
		}	//����׷��
	}
		break;

	case 2: {
		//��������ű��ʽ����������������еı��ʽ
		check_unlogic_exp(s->get_exp());
	}
		break;
	}
}

/*���ܣ���������������н����޷��Ų���
 ���������sl-sr ���ʽ����ָ��
 ����ֵ:�ɹ�����void,ʧ���׳��쳣
 ���ߣ�����*/
void check_subunsig_exp(Expression* sl, Expression* sr, map<string, string> v) {
	if (sl->get_stamp() == 4 && sr->get_stamp() == 4) {
		//������ͨ����
		int tl = 0;
		int tr = 0;
		string l_name = sl->get_cvar()->get_name();
		string r_name = sr->get_cvar()->get_name();
		map<string, string>::iterator scope_nt_lerator;
		string first = "";
		string second = "";

		for (scope_nt_lerator = v.begin(); scope_nt_lerator != v.end();
				++scope_nt_lerator) {
			first = scope_nt_lerator->first;
			second = scope_nt_lerator->second;

			if (second == "unsigned" && first == l_name)
				tl = 1;
			if (second == "unsigned" && first == r_name)
				tr = 1;
		}
		if (tl == 1 || tr == 1) {
			//yyerror("unsigned signed mixed is not support!");
			string error_info = "\n unsigned is not support in sub!";
			throw error_info;
		}

	}
}

/*2014��12��14��׷��
 ���ܣ���������������߱��ʽ�Ƿ����޷����з��Ż�����㣬����У��򱨴�
 ���������s-���ʽ����ָ��
 ����ֵ:�ɹ�����void,ʧ���׳��쳣
 ���ߣ�����*/
void check_mixsign_exp(Expression* sl, Expression* sr, map<string, string> v) {
	if (sl->get_stamp() == 4 && sr->get_stamp() == 4) {
		//������ͨ����
		int tl = 0;
		int tr = 0;
		string l_name = sl->get_cvar()->get_name();
		string r_name = sr->get_cvar()->get_name();
		map<string, string>::iterator scope_nt_lerator;
		string first = "";
		string second = "";

		for (scope_nt_lerator = v.begin(); scope_nt_lerator != v.end();
				++scope_nt_lerator) {
			first = scope_nt_lerator->first;
			second = scope_nt_lerator->second;

			if (second == "unsigned" && first == l_name)
				tl = 1;
			if (second == "unsigned" && first == r_name)
				tr = 1;
		}
		if ((tl != tr) && (tl == 1 || tr == 1)) {
			//yyerror("unsigned signed mixed is not support!");
			string error_info = "\n unsigned signed mixed is not support!";
			throw error_info;
		}

	}
}

//���ܣ��﷨��������֪��������ñ�������
//������ 2015��1��
list<string> get_type_var(string name, map<string, string> v) {
	map<string, string>::iterator scope_nt_lerator;
	string first = "";
	string second = "";
	list<string> list_type;
	for (scope_nt_lerator = v.begin(); scope_nt_lerator != v.end();
			++scope_nt_lerator) {
		first = scope_nt_lerator->first;
		second = scope_nt_lerator->second;
		if (first == name)
			list_type.push_back(second);
	}
	if (v.empty()) {
		string errorstr = " typelist is empty";
		throw errorstr;
	}
	if (list_type.empty()) {
		string errorstr = name + " var type is empty";
		throw errorstr;
	}

	return list_type;
}

void check_mixsign_exp(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");
	switch (s->get_stamp()) {
	case 0: {
		//����Ǵ������������ı��ʽ�������жϲ������Ƿ��ǻ�������
		if ("+" == s->get_op() || "-" == s->get_op() || "*" == s->get_op()
				|| "/" == s->get_op()) {
			//���Ϊ�����������������������Ҳ�����
			check_mixsign_exp(s->get_cel());
			check_mixsign_exp(s->get_cer());
		}
	}
		break;
	case 2: {
		//��������ű��ʽ����������������еı��ʽ
		check_unlogic_exp(s->get_exp());
	}
		break;
	case 3: {
		//����ǳ���
	}
		break;

	case 4: {
		//����Ǳ�����������������жϱ����Ƿ�Ϊһ�����ͨ����
		/*Var_declaration* leftv = s->get_cel();
		 Var_declaration* leftr = s->get_cer();
		 int lsign = leftv->get_signed_stamp();
		 int rvsign = leftr->get_signed_stamp();
		 if (lsign != rvsign) {
		 //unsigned
		 string error_info = "signed unsigned mixed do not support!";
		 throw error_info;
		 } else {

		 }*/
	}
		break;

	}
}

// 2010��3��9��׷��
// ���ܣ�����߼���������ߵı��ʽ�Ƿ�����C�Ӽ��淶����������㣬�򱨴�
// ���������s-���ʽ����ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ����
void check_logic_exp(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");

	//�жϱ��ʽ����
	switch (s->get_stamp()) {
	case 0: {
		//����Ǵ��������������ı��ʽ�����жϲ������Ƿ�Ϊ�߼������
		if ("&&" == s->get_op() || "||" == s->get_op()) {
			//���Ϊ�߼��������������������Ҳ�����
			check_logic_exp(s->get_cel());
			check_logic_exp(s->get_cer());
		} else if (">=" == s->get_op() || ">" == s->get_op()
				|| "<=" == s->get_op() || "<" == s->get_op()
				|| "==" == s->get_op() || "!=" == s->get_op()) {
			//���Ϊ��ϵ�������������������Ҳ�����
			check_unlogic_exp(s->get_cel());
			check_unlogic_exp(s->get_cer());
		} else {
			//�����Ϊ�߼���������򱨴�
			string error_info = "logical expression is invalid!";
			throw error_info;
		}
	}
		break;

	case 1: {
		//������߼������㡢��λȡ�����㣬����������Ҳ�����
		check_logic_exp(s->get_cer());
	}
		break;

	case 2: {
		//��������ű��ʽ����������������еı��ʽ
		check_logic_exp(s->get_exp());
	}
		break;

	case 3: {
		//����ǳ������򱨴�
		string error_info = "logical expression has constant!";
		throw error_info;
	}
		break;

	case 4: {
		//����Ǳ�����������������жϱ����Ƿ�Ϊһ�����ͨ����
		//if (0 != s->get_cvar()->get_stamp()) {
			//���������ͨ�������򱨴�
			string error_info = "logical operand expression has var or array!";
			throw error_info;
		//}
	}
		break;

	case 5: {
		//����Ǻ������ã��򱨴�
		string error_info = "logical expression is invalid!";
		throw error_info;
	}
		break;
	}
}

// 2015��1��
// ���ܣ���鰴λ��������ߵı��ʽ�Ƿ�����C�Ӽ��淶����������㣬�򱨴�
// ���������s-���ʽ����ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ�����
void check_bit_exp(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");

	//�жϱ��ʽ����
	switch (s->get_stamp()) {
	case 1: {
		//������߼������㣬�򱨴�
		string error_info = " bit_inverse expression is invalid!";
		throw error_info;
	}
		break;

	case 2: {
		//��������ű��ʽ����������������еı��ʽ
		check_bit_exp(s->get_exp());
	}
		break;
	}
}
//��λȡ��Լ������
//������ ���
void check_inverse_exp(Expression* s) {
	//�����������ʽ����ָ�����Ч��
	check_pt_arg(s, "expression object's pointer");

	//�жϱ��ʽ����
	switch (s->get_stamp()) {
	case 0: {
		//����Ǵ��������������ı��ʽ�����жϲ������Ƿ�Ϊ�߼������
		if ("&&" == s->get_op() || "||" == s->get_op()) {
			//���Ϊ�߼��������������������Ҳ�����
			check_logic_exp(s->get_cel());
			check_logic_exp(s->get_cer());
		} else if (">=" == s->get_op() || ">" == s->get_op()
				|| "<=" == s->get_op() || "<" == s->get_op()
				|| "==" == s->get_op() || "!=" == s->get_op()) {
			//���Ϊ��ϵ�������������������Ҳ�����
			check_unlogic_exp(s->get_cel());
			check_unlogic_exp(s->get_cer());
		} else {
			//�����Ϊ�߼���������򱨴�
//			string error_info = "inver logical expression is invalid!";
//			throw error_info;
			check_inverse_exp(s->get_cel());
			check_inverse_exp(s->get_cer());
		}
	}
		break;

	case 1: {
		//��λȡ�����㣬����������Ҳ�����
		check_inverse_exp(s->get_cer());
	}
		break;

	case 2: {
		//��������ű��ʽ����������������еı��ʽ
		check_inverse_exp(s->get_exp());
	}
		break;

	case 3: {
		//����ǳ������򱨴�
//		string error_info = "logical expression has constant!";
//		throw error_info;
	}
		break;

	case 4: {
		//����Ǳ�����������������жϱ����Ƿ�Ϊһ�����ͨ����
//		if (0 != s->get_cvar()->get_stamp()) {
//			//���������ͨ�������򱨴�
//			string error_info = "logical operand is invalid!";
//			throw error_info;
//		}
	}
		break;

	case 5: {
		//����Ǻ������ã��򱨴�
		string error_info = "function logical expression is invalid!";
		throw error_info;
	}
		break;
	}
}

//enumԼ���淶
//������ ׷��
void check_enum_dec(string str) {
	string error_info = "enum declaration error : " + str;
	throw error_info;
}

void check_if_while(Statement* s){
	 //��������������ָ�����Ч��
	check_pt_arg(s, "statement object's pointer");
	if(s->get_stamp() != 0) {
		string error_info = "if/while do not with {}";
		throw error_info;
		}
}
//Լ��if��else��while������{}
//����
void check_else_if(Statement* s){
		check_pt_arg(s, "statement object's pointer");
		if(s->get_stamp() != 0) {
			if(s->get_stamp() == 1){

			}else{
				string error_info = "else/esle if  do not with {}";
				throw error_info;
			}
		}

}

// ���ܣ���������������������ݳ�Ա�����������ݳ�Ա��ʾ�������Ƿ����������ͱ�����
//           �Ƿ��ǲ����������Ƿ���NODT���� ,�Ƿ����з��ű���
// ���������v_d-��������ָ��
//����ֵ���ɹ�����void ��ʧ���׳�����
// ���ߣ���ա��߼γ�
//
void set_var_dec(Var_declaration* v_d) {
	try {
		//��������������������ָ�����Ч��
		check_pt_arg(v_d, "varibale declaration object's pointer");

		v_d->set_noadj_stamp(noadj_cmnt_key_num % 2);	//���øñ���������noadj��ǩ
		v_d->set_nodt_stamp(nodt_cmnt_key_num % 2);	 //���øñ���������nodt��ǩ
		v_d->set_bool_stamp(bool_cmnt_key_num % 2);	 //���øñ���������bool��ǩ
		//v_d->set_unsigned_stamp(unsign_cmnt_key_num % 2);//���øñ���������unsigned��ǩ  ����
		//���ڲ�ʹ�ò���ע�͹ؼ��֣���ʹ��bool�ؼ��������Ĳ���������Ӧ�������
		//���ݷ������������Ӧ�����ݱ�ǩ
		//ʹ�� unsigned �ؼ������������α���
		for (list<Type_specifier>::iterator iter =
				v_d->get_decl_spec()->get_type_spec_list()->begin();
				iter != v_d->get_decl_spec()->get_type_spec_list()->end();
				iter++) {
			//�ж����͵ı�ǩ�����͵�����
			if (iter->get_stamp() == 0 && iter->get_type_name() == "bool") {
				//�����������Ϊһ�����ͣ��ҹؼ���Ϊ"bool"�������ò�����ǩΪ1
				v_d->set_bool_stamp(1);
			} else if (iter->get_stamp() == 0
					&& iter->get_type_name() == "unsigned") {
				//�޷�������   ����
				//v_d->set_unsigned_stamp(1);
			}
		}
	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "setting variable declaration : " + str;
		throw error_info;
	}
}

// ���ܣ��������ϲ��׳��﷨�ʷ������еĴ�����Ϣ
// ���������str-������ʾ��Ϣ
// ����ֵ��void
// ���ߣ���ա��߼γ�
void yyerror(string str) {
	//�ڴ������Ϣǰ���ϱ�ģ�����Ϣ�����������׳�
	string error_info = "\n flex and bison : " + str;

	throw error_info;
}

// ���ܣ���ʼ��ǰ��ģ���е�ȫ�ֱ���
// ��������
// ����ֵ����
// ���ߣ���ա��߼γ�
static void initial_front_end(void) {
	yyin = NULL;							//�ļ�ָ���ʼ��ΪNULL
	front_end_pro = NULL;			//�ṹָ���ʼ��ΪNULL
	bool_cmnt_key_num = 0;		//����ע�͹ؼ��ָ�����ʼ��Ϊ0
	nodt_cmnt_key_num = 0;		//NODTע�͹ؼ��ָ�����ʼ��Ϊ0
	noadj_cmnt_key_num = 0;	//NOADJע�͹ؼ��ָ�����ʼ��Ϊ0
	unsign_cmnt_key_num = 0;	//signע�͹ؼ��ָ�����ʼ��Ϊ0  ����
	enter_noparse = false;			//�ǹؼ�����ʷ��������س�ʼ��Ϊfalse
	type_names->clear();			//�Զ��������������ʼ��ʱ���
}

// ���ܣ�ǰ�˴�������ǰ�����ݳ�ʼ���ʹʷ��﷨�������õ��м�ṹָ��
// ���������file_name-���������ļ�·������
// ���������pro-�м�ṹָ�������
// ����ֵ��
// ���ߣ���ա��߼γ�
void front_end(string file_name, Program*& pro) {
	try {
		//���������ļ����ַ�������Ч��
		check_str_arg(file_name, "file's name");

		initial_front_end();	//��ʼ��ǰ��ģ��

		yyin = fopen(file_name.c_str(), "r");	//��Ҫ������ļ�
		//�ж��ļ�ָ���Ƿ�ΪNULL
		if (yyin == NULL) {
			// ����ļ���ʧ�ܣ��׳�����
			string error_info = "file  open faild!";
			throw error_info;
		} else {
			//�ļ��򿪳ɹ��������﷨����
			do {
				yyparse(); //�﷨�����������м�ṹ
			} while (!feof(yyin));
		}

		//�ж��﷨������Լ���м�ṹָ���Ƿ�Ϊ��
		if (NULL == front_end_pro) {
			// �����Լ�����м�ṹָ��Ϊ�գ�˵���﷨����ʧ��
			string error_info = "file  parsing faild!";
			throw error_info;
		} else {
			//��Ϊ�գ����丳������pro
			pro = front_end_pro;
		}
		fclose(yyin); //�ر��ļ�
	} catch (string str) {
		//�����׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "front end : " + str;
		throw error_info;
	}
}

