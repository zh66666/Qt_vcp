#include "gen_utils.h"
#include "var.h"
#include "expression.h"

// �������ļ�: src/main_utils.cpp
extern bool debug_flag;
extern string get_var_stamp(list<Var_info>* s, string var_name,
		string var_scope);
extern const int P1;
extern const int P2;
extern const string ARRAY_EXTR_NAME;
extern struct TAG_DATA T_DATA;
//�������ļ�gen_global_var.cpp
extern string get_bind_local_var(string global_var, string func_name);
extern bool is_global_var(string var_name);
//�������ļ�main_utils.cpp
extern string int2HexStr_data(const int num);
extern int str2int(string str);
// ���ܣ�������鸳ֵ������������е�ǰ�������룬��������ĸ�λ��ֵ������ĵ�λ��ֵ��
// 			���鸽�ӱ����ĸ�λ��ֵ��
// ����  �����±�ĸ�λ��ֵ
// ���������array_name-������
// 			      h_sub-�����±�ĸ�λ����
// 			      h_exp-���鸳ֵ���ʽ�ĸ�λ����
// 			      l_exp-���鸳ֵ���ʽ�ĵ�λ����
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
static void out_array_assign_three(string array_name, string h_sub,
		string h_exp, string l_exp) {
	try {
		//��������������Ч��
		check_str_arg(array_name, "array's name");
		check_str_arg(h_sub, "array subscript's high name");
		check_str_arg(h_exp, "assign expression 's high name");
		check_str_arg(l_exp, "assign expression 's low name");

		//������鸳ֵ�ĸ�λ����
		out_indent(indent_num, gen_out);
		gen_out << "H_" << array_name << "[" << h_sub << "] = " << h_exp << ";"
				<< endl;
		//������鸳ֵ�ĵ�λ����
		out_indent(indent_num, gen_out);
		gen_out << "L_" << array_name << "[" << h_sub << "] = " << l_exp << ";"
				<< endl;
		//������鸽�ӱ����ĸ�λ����
		//����     ��������±��λ
		out_indent(indent_num, gen_out);
		gen_out << "H_Array_EXTR_" << array_name << " = H_Array_EXTR_"
				<< array_name << " + " << h_sub << ";" << endl;
	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "output array assign's part codes : " + str;
		throw error_info;
	}
}

bool StrCompare(const char* str1, const char* str2, int size) {
	for (int i = 0; i < size; i++) {
		if (*str1 != *str2)
			return false;
		str1++;
		str2++;
	}
	return true;
}

// ���ܣ��������±�͸�ֵ���ʽ���ǳ��������鸳ֵ������б��룬����ǩ���ķ���Ͳ���
// ���������array_name-��������
// 			      sub-�����±�ֵ
// 			      exp-���鸳ֵ���ʽ��ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
static void gen_array_assign_I(string array_name, int sub, int exp,
		list<Var_info>* v) {
	try {
		//��������������Ч��
		check_str_arg(array_name, "array's name");
		check_pt_arg(v, "var_info list's pointer");

		int i_sig = getConstantSig(sub); //Ϊ�±����ǩ��
		long long low_i = (-((long long) sub * K_NUM_P1) % P1 + i_sig) % P1; //�����±�ĵ�λ
		long long low_i2 = (-((long long) sub * K_NUM_P2) % P2 + i_sig) % P2; //�����±�ĵ�λ

		int exp_sig = getConstantSig(exp); //Ϊ��ֵ��������ǩ��
		long long low_exp = ((-((long long) exp * K_NUM_P1) % P1 + exp_sig) % P1
				+ P1) % P1; //���㸳ֵ�����ĵ�λ
		long long low_exp2 = ((-((long long) exp * K_NUM_P2) % P2 + exp_sig)
				% P2 + P2) % P2; //���㸳ֵ�����ĵ�λ

		//string arrExtr_name = "arrExtr_" + array_name; //�������鸽�ӱ�������
		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope); //ȡ�����鸽�ӱ���ǩ��
		int new_arr_index = 0;
		int new_sig = get_sig_index(&new_arr_index); //Ϊ���鸽�ӱ���������ǩ��
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global"); //����ǩ�����붯̬ǩ����
		//���㸽�ӱ�����λ�������õ��Ĳ�������
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//ȡ�������ǩ��
//		���������λ�����������õ��Ĳ�������
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;

		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;
		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;

		//����ǵ���ģʽ�������������Ϣ����ֵ������ǩ�����±곣����ǩ�������ӱ�������ǩ���������ǩ��
		if (true == debug_flag) {
			//out_indent(indent_num, gen_out);
//			gen_out << "/*" << exp << "'s sig :" << exp_sig << " " << sub
//					<< "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << array_name
//					<< "'s sig:" << array_sig << "*/" << endl << endl;
		}

		//��������±ꡢ��ֵ���ʽΪ�����ĵ�����ʽ	//����
		string sub_h = int2HexStr_data(sub);
		string exp_h = int2HexStr_data(exp);
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.F = CONST_NUM_"
				<< sub_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;				//����У����1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;				//����У����2
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.F = CONST_NUM_"
				<< exp_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;				//����У����1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;				//����У����2
		out_indent(indent_num, gen_out);

		string CSCArrayName = GetArrayNameInCSC(array_name);
		gen_out << CSCArrayName << "[AC_BIND_CONST_NUM_" << sub_h
				<< ".Data.F].Data = F_VCL_ArrayAssign(AC_BIND_CONST_NUM_"
				<< exp_h << ".Data, AC_BIND_CONST_NUM_" << sub_h << ".Data, "
				<< array_const1 << ", " << array_const2 << ");" << endl;

		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt("<< CSCArrayName
				<< "[AC_BIND_CONST_NUM_" << sub_h
				<< ".Data.F].Data.C, AC_BIND_CONST_NUM_" << exp_h
				<< ".Data.C, AC_BIND_CONST_NUM_" << sub_h << ".Data, "
				<< arrExtr_const1 << ", " << arrExtr_const2 << ");" << endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);
		//���Ӹ�ֵ��������ǩ��
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding array assign (subscript is constant, "
				"assign expression is constant): " + str;
		throw error_info;
	}
}

// ���ܣ��������±�Ϊ��������ֵ���ʽΪ�򵥱����ĵ����鸳ֵ������б��룬����ǩ���ķ���Ͳ���
// ���������array_name-��������
// 			      sub-�����±�ֵ
// 			      exp_name-���鸳ֵ���ʽ��ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
static void gen_array_assign_II(string array_name, int sub, string exp_name,
		list<Var_info>* v) {
	try {
		//��������������Ч��
		check_str_arg(array_name, "array's name");
		check_str_arg(exp_name, "variable's name");
		check_pt_arg(v, "var_info list's pointer");
		const char* name = exp_name.data();
		//�ж��Ƿ����м����"AC_Tmp_"
		bool result = StrCompare("AC_Tmp_", name, 7);

		int i_sig = getConstantSig(sub);		//Ϊ�����±����ǩ��
		long long low_i = (-((long long) sub * K_NUM_P1) % P1 + i_sig) % P1;//���������±�ĵ�λ
		long long low_i2 = (-((long long) sub * K_NUM_P2) % P2 + i_sig) % P2;//���������±�ĵ�λ
		//string arrExtr_name = "arrExtr_" + array_name;		//�������鸽�ӱ�������

		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope);//ȡ�����鸽�ӱ���ǩ��
		int sig_index = 0;
		int new_sig = get_sig_index(&sig_index);		//Ϊ���鸽�ӱ���������ǩ��
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global");	//����ǩ�����붯̬ǩ����
		//���㸽�ӱ�����λ�������õ��Ĳ�������
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//ȡ�������ǩ��

		string expl_name;
		bool expGlobal = is_global_var(exp_name);
		if (expGlobal == true) {
			expl_name = get_bind_local_var(exp_name, var_scope);
		} else {
			expl_name = exp_name;
		}

		int exp_sig = get_current_sig(v, expl_name, var_scope);		//ȡ����ֵ������ǩ��
		//���������λ�����������õ��Ĳ�������
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;
		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;

		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;

		//����ǵ���ģʽ�������������Ϣ����ֵ������ǩ�����±곣����ǩ�������ӱ�������ǩ���������ǩ��
		if (true == debug_flag) {
//			out_indent(indent_num, gen_out);
//			gen_out << "//" << exp_name << "'s sig :" << exp_sig << " " << sub
//					<< "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << array_name
//					<< "'s sig:" << array_sig << endl << endl;
		}
		string sub_h = int2HexStr_data(sub);
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.F = CONST_NUM_"
				<< sub_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;		//����У����1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << sub_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << sub_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;		//����У����2

		string CSCArrayName = GetArrayNameInCSC(array_name);
		out_indent(indent_num, gen_out);
		gen_out << CSCArrayName << "[AC_BIND_CONST_NUM_" << sub_h
				<< ".Data.F].Data = F_VCL_ArrayAssign(";
		if (result == true) {
			//�м���������TC
		} else {
			if (expGlobal == true) {
				//ȫ�ֱ��������TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data, AC_BIND_CONST_NUM_" << sub_h
				<< ".Data, " << array_const1 << ", " << array_const2 << ");"
				<< endl;
//////////////////////////////////////////////////////////
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt(" << CSCArrayName
				<< "[AC_BIND_CONST_NUM_" << sub_h << ".Data.F].Data.C, ";

		if (result == true) {
			//�����TC
		} else {
			if (expGlobal == true) {
				//ȫ�ֱ��������TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data.C, AC_BIND_CONST_NUM_" << sub_h
				<< ".Data, " << arrExtr_const1 << ", " << arrExtr_const2 << ");"
				<< endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);

		//���Ӹ�ֵ��������ǩ��
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding array assign (subscript is constant, "
				"assign expression is variable): " + str;
		throw error_info;
	}
}

// ���ܣ��������±�Ϊ�򵥱�������ֵ���ʽ�ǳ��������鸳ֵ������б��룬����ǩ���ķ���Ͳ���
// ���������array_name-��������
// 			      i_name-�����±�����
// 			      exp-���鸳ֵ���ʽ��ֵ
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
static void gen_array_assign_III(string array_name, string i_name, int exp,
		list<Var_info>* v) {
	try {
		//��������������Ч��
		check_str_arg(array_name, "array's name");
		check_str_arg(i_name, "array subscript's name");
		check_pt_arg(v, "var_info list's pointer");

		int exp_sig = getConstantSig(exp);		//Ϊ��ֵ��������ǩ��
		long long low_exp = ((-((long long) exp * K_NUM_P1) % P1 + exp_sig) % P1
				+ P1) % P1;		//���㸳ֵ�����ĵ�λ
		long long low_exp2 = ((-((long long) exp * K_NUM_P2) % P2 + exp_sig)
				% P2 + P2) % P2;		//���㸳ֵ�����ĵ�λ

		//string arrExtr_name = "arrExtr_" + array_name;		//�������鸽�ӱ�������
		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope);//ȡ�����鸽�ӱ���ǩ��
		int sig_index = 0;
		int new_sig = get_sig_index(&sig_index);		//Ϊ���鸽�ӱ���������ǩ��
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global");	//����ǩ�����붯̬ǩ����

		string il_name;
		const char* iname = i_name.data();
		bool iresult = StrCompare("AC_Tmp_", iname, 7);		//i�Ƿ��Ǿֲ������ж�
		bool indexGlobal = is_global_var(i_name);
		if (indexGlobal == true) {
			il_name = get_bind_local_var(i_name, var_scope);
		} else {
			il_name = i_name;
		}

		int i_sig = get_current_sig(v, il_name, var_scope);		//ȡ���±������ǩ��
		//���㸽�ӱ�����λ�������õ��Ĳ�������
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//ȡ�������ǩ��
		//���������λ�����������õ��Ĳ�������
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;
		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;

		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;
		//����ǵ���ģʽ�������������Ϣ����ֵ������ǩ�����±������ǩ�������ӱ�������ǩ���������ǩ��
		if (true == debug_flag) {
//			out_indent(indent_num, gen_out);
//			gen_out << "//" << exp << "'s sig :" << exp_sig << " " << i_name
//					<< "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << array_name
//					<< "'s sig:" << array_sig << endl << endl;
		}
		string exp_h = int2HexStr_data(exp);
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.F = CONST_NUM_"
				<< exp_h << ".Data.F;" << endl;
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C1 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C1"
						" + g_GS_TL1;" << endl;		//����У����1
		out_indent(indent_num, gen_out);
		gen_out << "AC_BIND_CONST_NUM_" << exp_h << ".Data.C.C2 = "
				<< "CONST_NUM_" << exp_h << ".Data.C.C2"
						" + g_GS_TL2;" << endl;		//����У����2
		out_indent(indent_num, gen_out);
		string CSCArrayName = GetArrayNameInCSC(array_name);
		gen_out << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name
				<< ".Data.F].Data = F_VCL_ArrayAssign(AC_BIND_CONST_NUM_"
				<< exp_h << ".Data,";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data, " << array_const1 << ", " << array_const2
				<< ");" << endl;
/////////////////////////////////////////////////////////////////
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt(" << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data.F].Data.C, AC_BIND_CONST_NUM_" << exp_h
				<< ".Data.C, ";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data, " << arrExtr_const1 << ", "
				<< arrExtr_const2 << ");" << endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);

		//���Ӹ�ֵ��������ǩ��
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding array assign (subscript is variable, "
				"assign expression is constant): " + str;
		throw error_info;
	}
}

// ���ܣ��������±�͸�ֵ���ʽ��Ϊ�򵥱��������鸳ֵ������б��룬����ǩ���ķ���Ͳ���
// ���������array_name-��������
// 			      i_name-�����±������
// 			     exp_name-���鸳ֵ������
// 			     v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
static void gen_array_assign_IV(string array_name, string i_name,
		string exp_name, list<Var_info>* v) {
	try {
		//��������������Ч��
		check_str_arg(array_name, "array's name");
		check_str_arg(i_name, "array subscript's name");
		check_str_arg(exp_name, "expression's name");
		check_pt_arg(v, "var_info list's pointer");
		const char* name = exp_name.data();
		//�ж��Ƿ����м����"AC_Tmp_"
		bool result = StrCompare("AC_Tmp_", name, 7);

		//string arrExtr_name = "arrExtr_" + array_name;		//�������鸽�ӱ�������
		int arrExtr_sig = get_current_sig(v, ARRAY_EXTR_NAME, var_scope);//ȡ�����鸽�ӱ���ǩ��
		int sig_index = 0;
		int new_sig = get_sig_index(&sig_index);		//Ϊ���鸽�ӱ���������ǩ��
		add_new_sig(v, ARRAY_EXTR_NAME, new_sig, "global");	//����ǩ�����붯̬ǩ����
		//�±��Ƿ���ȫ�ֱ���
		string il_name;
		const char* iname = i_name.data();
		bool iresult = StrCompare("AC_Tmp_", iname, 7);		//i�Ƿ��Ǿֲ������ж�
		bool indexGlobal = is_global_var(i_name);
		if (indexGlobal == true) {
			il_name = get_bind_local_var(i_name, var_scope);
		} else {
			il_name = i_name;
		}

		int i_sig = get_current_sig(v, il_name, var_scope);		//ȡ���±������ǩ��
		//���㸽�ӱ�����λ�������õ��Ĳ�������
		int arrExtr_const1 = (((long long) new_sig - arrExtr_sig - i_sig) % P1
				+ P1) % P1;
		int arrExtr_const2 = (((long long) new_sig - arrExtr_sig - i_sig) % P2
				+ P2) % P2;

		int array_sig = get_current_sig(v, array_name, var_scope);	//ȡ�������ǩ��
		//�жϸ�ֵ�����Ƿ���ȫ�ֱ���
		string expl_name;
		bool expGlobal = is_global_var(exp_name);
		if (expGlobal == true) {
			expl_name = get_bind_local_var(exp_name, var_scope);
		} else {
			expl_name = exp_name;
		}

		int exp_sig = get_current_sig(v, expl_name, var_scope);	//ȡ����ֵ���ʽ������ǩ��
		//���������λ�����������õ��Ĳ�������
		int array_const1 = (((long long) array_sig - exp_sig + i_sig) % P1 + P1)
				% P1;
		int array_const2 = (((long long) array_sig - exp_sig + i_sig) % P2 + P2)
				% P2;

		arrExtr_const1 = ((arrExtr_const1 + exp_sig - array_sig) % P1 + P1)
				% P1;
		arrExtr_const2 = ((arrExtr_const2 + exp_sig - array_sig) % P2 + P2)
				% P2;

		//����ǵ���ģʽ�������������Ϣ����ֵ������ǩ�����±������ǩ�������ӱ�������ǩ���������ǩ��
		if (true == debug_flag) {
			out_indent(indent_num, gen_out);
//			gen_out << "/*" << exp_name << "'s sig :" << exp_sig << " "
//					<< i_name << "'s sig :" << i_sig << " " << ARRAY_EXTR_NAME
//					<< "'s new sig :" << new_sig << " " << ARRAY_EXTR_NAME
//					<< "'s sig:" << array_sig << "*/" << endl << endl;
		}

		gen_out << endl;
		string CSCArrayName = GetArrayNameInCSC(array_name);
		out_indent(indent_num, gen_out);
		gen_out << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data.F].Data = F_VCL_ArrayAssign(";
		if (result == true) {
			//�����TC
		} else {

			if (expGlobal == true) {
				//ȫ�ֱ��������TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data,";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data, " << array_const1 << ", " << array_const2
				<< ");" << endl;
////////////////////////////////////////////////////////////
		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayAssignExt(" << CSCArrayName << "[";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data.F].Data.C, ";
		if (result == true) {
			//�����TC
		} else {
			if (expGlobal == true) {
				//ȫ�ֱ��������TC
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << expl_name << ".Data.C, ";
		if (iresult == true) {
		} else {
			if (indexGlobal == true) {
			} else {
				gen_out << "TC_";
			}
		}
		gen_out << il_name << ".Data," << arrExtr_const1 << ", "
				<< arrExtr_const2 << ");" << endl;
		//F_VCL_ArrayAssignExt(TC_arr[TC_i.Data.F].Data.C, TC_y.Data.C, TC_i.Data, CON3, CON4);

		out_indent(indent_num, gen_out);
		gen_out << "F_VCL_ArrayExtUpdate(" << new_sig << ");" << endl << endl;

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding array assign (subscript is variable, "
				"assign expression is variable): " + str;
		throw error_info;
	}
}

// ���ܣ�������ĸ�ֵ���б��룬��Ҫ���������±�Ϊ�������߱��������鸳ֵ���ʽΪ�������߱�����
//			������Ҫ����������㷨���������淶������鸳ֵ�İ�ȫ����
// ���������var-�����������ָ��
// 			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
void gen_array_assign(Var* var, Expression* exp, list<Var_info>* v) {
	try {
		//��������������Ч��
		check_pt_arg(var, "variable object's pointer");
		check_pt_arg(exp, "expression object's pointer");
		check_pt_arg(v, "var_info list's pointer");

		string array_name = "";		//�洢������
		string i_name = "";		//�洢�±���
		string exp_name = "";		//�洢��ֵ���ʽ����

		//�ж�������±��Ƿ�Ϊ��������ֵ���ʽ�Ƿ�Ϊ����
		if (3 == var->get_cexp()->get_stamp() && 3 == exp->get_stamp()) {
			//��������±�Ϊ���������ǳ�����ֵ������a[2]=3;
			array_name = var->get_name();		//ȡ�����������
			i_name = var->get_cexp()->get_c();		//�����±�����
			exp_name = exp->get_c();		//ȡ����ֵ����������

			//�������±�͸�ֵ��Ϊ���������鸳ֵ������б���
			gen_array_assign_I(array_name, str2int(i_name), str2int(exp_name),
					v);

		}
		//�ж������±��Ƿ�Ϊ��������ֵ���ʽ�Ƿ�Ϊ�򵥱���
		else if (3 == var->get_cexp()->get_stamp() && 4 == exp->get_stamp()
				&& 0 == exp->get_cvar()->get_stamp()) {
			//��������±�Ϊ���������Ǽ򵥱�����ֵ������a[2]=x;
			array_name = var->get_name();		//ȡ�����������
			i_name = var->get_cexp()->get_c();		//ȡ�������±���
			exp_name = exp->get_cvar()->get_name();		//ȡ����ֵ����������

			//�������±�Ϊ��������ֵ���ʽΪ�򵥱��������鸳ֵ������б���
			gen_array_assign_II(array_name, str2int(i_name), exp_name, v);

		}
		//�ж������±��Ƿ�Ϊ�򵥱�����ͬʱ��ֵ���ʽ�Ƿ�Ϊ����
		else if (4 == var->get_cexp()->get_stamp()
				&& 0 == var->get_cexp()->get_cvar()->get_stamp()
				&& 3 == exp->get_stamp()) {
			//��������±�Ϊ�򵥱�����ͬʱΪ������ֵ������a[x]=3;
			array_name = var->get_name();		//ȡ�����������
			i_name = var->get_cexp()->get_cvar()->get_name();		//ȡ�������±������
			exp_name = exp->get_c();		//ȡ����ֵ����������

			//�������±�Ϊ�򵥱�������ֵ���ʽΪ���������鸳ֵ������б���
			gen_array_assign_III(array_name, i_name, str2int(exp_name), v);
		}
		//�ж������±�͸�ֵ���ʽ�Ƿ��Ϊ�򵥱���
		else if (4 == var->get_cexp()->get_stamp()
				&& 0 == var->get_cexp()->get_cvar()->get_stamp()
				&& 4 == exp->get_stamp() && 0 == exp->get_cvar()->get_stamp()) {
			//��������±�͸�ֵ���ʽ��ΪΪ�򵥱���������a[x]=y;
			array_name = var->get_name();		//ȡ�����������
			i_name = var->get_cexp()->get_cvar()->get_name();		//ȡ�������±������
			exp_name = exp->get_cvar()->get_name();		//ȡ����ֵ����������

			//�������±�͸�ֵ���ʽ��Ϊ�򵥱��������鸳ֵ������б���
			gen_array_assign_IV(array_name, i_name, exp_name, v);
		} else {
			//������������鸳ֵ����Ϊ�Ƿ����׳�������Ϣ
			string error_info = "array assign statement is invalid!";
			throw error_info;
		}
	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding array assign : " + str;
		throw error_info;
	}
}
// ���ܣ�����������ý��б��룬��Ҫ���������±�Ϊ�������߱���������λ��ѭ�����ڻ���ѭ�����⣬����
// 			�Ƿ�Ϊnodt�����⼸������������㷨���������淶����������õİ�ȫ����
// ���������var_name-��ֵ������
// 			      var-�����������ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ��߼γ�
void gen_array_refer(string var_name, Var* var, list<Var_info>* v) {
	try {
		//��������������Ч��
		check_str_arg(var_name, "variable's name");		//����������ֵ�������ַ�������Ч��
		check_pt_arg(var, "array object's pointer");		//���������������ָ�����Ч��
		check_pt_arg(v, "var_info list's pointer");		//����������̬ǩ����ָ�����Ч��

		const char* name = var_name.data();
		//�ж��Ƿ����м����"AC_Tmp_"
		bool result = StrCompare("AC_Tmp_", name, 7);
		//�ж���ֵ�����Ƿ�Ϊ�������
		if (1 == var->get_stamp()) {
			//�����������������ж�������±��Ƿ�Ϊ����
			if (3 == var->get_cexp()->get_stamp()) {
				//��������±�Ϊ����������x=a[3];
				string array_name = var->get_name();		//ȡ��������
				string i_name = var->get_cexp()->get_c();		//ȡ���±��ַ�����
				int high_i = str2int(i_name);		//���±�ת��������

				int i_sig = getConstantSig(high_i);		//Ϊ�±����ǩ��
				long long low_i =
						(-((long long) high_i * K_NUM_P1) % P1 + i_sig) % P1;//�����±�ĵ�λ
				long long low_i2 = (-((long long) high_i * K_NUM_P2) % P2
						+ i_sig) % P2;
				int array_sig = get_current_sig(v, array_name, var_scope);//ȡ�������ǩ��

				int sig_index = 0;
				int new_sig = get_sig_index(&sig_index);		//�洢��ֵ����ǩ��

				string varl_name;
				bool varGlobal = is_global_var(var_name);
				if (varGlobal == true) {
					varl_name = get_bind_local_var(var_name, var_scope);
				} else {
					varl_name = var_name;
				}
				add_new_sig(v, varl_name, new_sig, var_scope);//����ֵ��������ǩ�����붯̬ǩ����

				int inComp1 = (((long long) new_sig - array_sig - i_sig) % P1
						+ P1) % P1;	//���㲹������
				int inComp2 = (((long long) new_sig - array_sig - i_sig) % P2
						+ P2) % P2;	//���㲹������

//				string inC1=int2HexStr(inComp1);ת��Ϊ16����
//				string inC2=int2HexStr(inComp2);

				//����ǵ���ģʽ�������������Ϣ����ֵ��������ǩ�����±��ǩ���������ǩ��
				if (true == debug_flag) {
//					out_indent(indent_num, gen_out);
//					gen_out << "/*" << var_name << "'s new sig :" << new_sig
//							<< " " << i_name << "'s sig :" << i_sig << " "
//							<< array_name << "'s sig:" << array_sig << "*/"
//							<< endl << endl;
				}

//����
				string high_h = int2HexStr_data(high_i);
				out_indent(indent_num, gen_out);
				gen_out << "AC_BIND_CONST_NUM_" << high_h
						<< ".Data.F = CONST_NUM_" << high_h << ".Data.F;"
						<< endl;
				out_indent(indent_num, gen_out);
				gen_out << "AC_BIND_CONST_NUM_" << high_h << ".Data.C.C1 = "
						<< "CONST_NUM_" << high_h << ".Data.C.C1"
						<< " + g_GS_TL1;" << endl;	//����У����1
				out_indent(indent_num, gen_out);
				gen_out << "AC_BIND_CONST_NUM_" << high_h << ".Data.C.C2 = "
						<< "CONST_NUM_" << high_h << ".Data.C.C2"
						<< " + g_GS_TL2;" << endl;	//����У����2
				out_indent(indent_num, gen_out);
				if (result == true) {
					//�����TC
				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				string CSCArrayName = GetArrayNameInCSC(array_name);
				gen_out << varl_name << ".Data = F_VCL_ArrayRef(" << CSCArrayName
						<< "[AC_BIND_CONST_NUM_" << high_h
						<< ".Data.F].Data, AC_BIND_CONST_NUM_" << high_h
						<< ".Data, " << inComp1 << ", " << inComp2 << ");"
						<< endl;				//CON1��CON2��Ϊ16����������
				out_indent(indent_num, gen_out);
				if (result == true) {

				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << varl_name << ".Id = F_VCL_RegisterCodeId(" << new_sig
						<< ");" << endl << endl;

			}
			//�ж�������±��Ƿ�Ϊ�򵥱���
			else if ((4 == var->get_cexp()->get_stamp())
					&& (0 == var->get_cexp()->get_cvar()->get_stamp())) {
				//�����������±�Ϊ����
				string array_name = var->get_name();				//ȡ�������������
				string i_name = var->get_cexp()->get_cvar()->get_name();//ȡ���±�����
				const char* iname = i_name.data();
				bool indexGlobal = is_global_var(i_name);
				bool varGlobal = is_global_var(var_name);
				bool i_result = StrCompare("AC_Tmp_", iname, 7);
				//�����±�i_name ����ֵ����var_name
				string il_name;
				string varl_name;
				if (indexGlobal == true) {
					il_name = get_bind_local_var(i_name, var_scope);
				} else {
					il_name = i_name;
				}
				if (varGlobal == true) {
					varl_name = get_bind_local_var(var_name, var_scope);
				} else {
					varl_name = var_name;
				}

				int i_sig = get_current_sig(v, il_name, var_scope);	//ȡ���±������ǩ��
				int array_sig = get_current_sig(v, array_name, var_scope);//ȡ�������ǩ��

				int sig_index = 0;
				int new_sig = get_sig_index(&sig_index);	//������ǩ����Ϊ��ֵ��ǩ��
				add_new_sig(v, varl_name, new_sig, var_scope);//����ֵ��������ǩ�����붯̬ǩ����

				int inComp1 = (((long long) new_sig - array_sig - i_sig) % P1
						+ P1) % P1;	//���㲹������
				int inComp2 = (((long long) new_sig - array_sig - i_sig) % P2
						+ P2) % P2;	//���㲹������

				//				string inC1=int2HexStr(inComp1);ת��Ϊ16����
				//				string inC2=int2HexStr(inComp2);

				//����ǵ���ģʽ�������������Ϣ����ֵ��������ǩ�����±��ǩ���������ǩ��
				if (true == debug_flag) {
					//					out_indent(indent_num, gen_out);
					//					gen_out << "/*" << var_name << "'s new sig :" << new_sig
					//							<< " " << i_name << "'s sig :" << i_sig << " "
					//							<< array_name << "'s sig:" << array_sig << "*/"
					//							<< endl << endl;
				}
				//����

				out_indent(indent_num, gen_out);
				if (result == true) {

				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				string CSCArrayName = GetArrayNameInCSC(array_name);
				gen_out << varl_name << ".Data = F_VCL_ArrayRef(" << CSCArrayName
						<< "[";
				if (i_result == true) {

				} else {
					if (indexGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << il_name << ".Data.F].Data, ";
				if (i_result == true) {

				} else {
					if (indexGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << il_name << ".Data, " << inComp1 << ", " << inComp2
						<< ");" << endl;					//CON1��CON2��Ϊ16����������
				out_indent(indent_num, gen_out);
				if (result == true) {

				} else {
					if (varGlobal == true) {
					} else {
						gen_out << "TC_";
					}
				}
				gen_out << varl_name << ".Id = F_VCL_RegisterCodeId(" << new_sig
						<< ");" << endl << endl;
				//end of ����ȫ�ֱ����Ĳ���

			} else {
				string error_info = "array's subscript is invalid!";
				throw error_info;
			}
		} else {
			string error_info = "it is not an array reference!";
			throw error_info;
		}

	} catch (string str) {
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info = "coding array refer : " + str;
		throw error_info;
	}
}

