
#include "gen_utils.h"//��ģ�����õ��Ĳ���ȫ�����������ڴ�ͷ�ļ���
#include "expression.h"
#include "var.h"
#include "bool_var.h"
#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_global_var.h"
// �������ļ� : src/main_utils.c
extern bool debug_flag;
extern list<Bool_var>* bool_vars;
extern list<Bool_var>* bool_sigs;

// �������ļ� : ../gen_utils.c
extern int get_bool_value(string name,list<Bool_var>* bool_vars,int stamp);
extern int get_bool_sig(string name,list<Bool_var>* bool_sigs,int stamp);

//�������ļ� : ./inverse.c
extern long long inverse(long long ,const long long);

// ���ܣ����߼���������б��룬�������ĸ�λ����͵�λ���룬ѭ������ѭ�������ͬ���ı��뷽��
// ���������var_name-��ֵ������
//			      exp-��ֵ���ʽ����ָ��
// 			      v-��̬ǩ����ָ��
// ���������v-��̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����
void red_or(string var_name,Expression* exp,list<Var_info>* v)
{
	try
	{
		//��������������Ч��
		check_str_arg(var_name,"left variable's name");//����������ֵ���������ַ�������Ч��
		check_pt_arg(exp,"expression object's pointer");//����������ֵ���ʽ����ָ�����Ч��
		check_pt_arg(v,"var_info list's pointer");//����������̬ǩ����ָ�����Ч��
		
		int left=exp->get_cel()->get_stamp();//ȡ������������ʽ�ı�ǩ
		int right=exp->get_cer()->get_stamp();//ȡ���Ҳ��������ʽ�ı�ǩ
		//�ж����Ҳ������Ƿ��Ϊ����
		if(4==left && 4==right)
		{
			//����ǣ�����б���
			//int new_sig=get_sig();//Ϊ��ֵ������ǩ��
			int SigID=0;
			int new_sig=get_sig_index(&SigID);//�洢��ֵ��������ǩ��
			
			//ȡ����ֵ���������ֵ�����ֵ��ǩ�������������ֵ�ĵ�λ
//			int var_true_h=get_bool_value(var_name,bool_vars,1);//ȡ����ֵ��������ֵ
//			int var_true_sig=get_bool_sig(var_name,bool_sigs,1);//ȡ����ֵ������ֵ��ǩ��
			int var_true_h=TRUE_VALUE;
			int var_true_sig=Bool_True_Sig;
//			long long var_true_l_1=((-((long long)var_true_h*K_NUM_P1)%(long long)P1//������ֵ������ֵ�ĵ�λ
//												  +(long long)var_true_sig)%(long long)P1+(long long)P1)%(long long)P1;
//			long long var_true_l_2=((-((long long)var_true_h*K_NUM_P2)%(long long)P2//������ֵ������ֵ�ĵ�λ
//												+(long long)var_true_sig)%(long long)P2+(long long)P2)%(long long)P2;
//			int var_false_h=get_bool_value(var_name,bool_vars,0);//ȡ����ֵ�����ļ�ֵ
//			int var_false_sig=get_bool_sig(var_name,bool_sigs,0);//ȡ����ֵ������ֵ��ǩ��
//			long long var_false_l=((-((long long)var_false_h*K_NUM)%(long long)A//������ֵ������ֵ�ĵ�λ
//													  +(long long)var_false_sig)%(long long)A+(long long)A)%(long long)A;
													  
			//ȡ��������������ֵ���Լ����ֵ��ǩ�������Դ˼��������ֵ�ĵ�λ
			string expl_name=exp->get_cel()->get_cvar()->get_name();//ȡ���������������
//			int expl_true_h=get_bool_value(expl_name,bool_vars,1);//ȡ�������������ֵ
//			int expl_true_sig=get_bool_sig(expl_name,bool_sigs,1);//ȡ�����������ֵ��ǩ��
//			long long expl_true_l=((-((long long)expl_true_h*K_NUM)%(long long)A//�������������ֵ�ĵ�λ
//												  +(long long)expl_true_sig)%(long long)A+(long long)A)%(long long)A;
//			int expl_false_h=get_bool_value(expl_name,bool_vars,0);//ȡ����������ļ�ֵ
//			int expl_false_sig=get_bool_sig(expl_name,bool_sigs,0);//ȡ�����������ֵ��ǩ��
//			long long expl_false_l=((-((long long)expl_false_h*K_NUM)%(long long)A//�������������ֵ�ĵ�λ
//													 +(long long)expl_false_sig)%(long long)A+(long long)A)%(long long)A;
			
			//ȡ���Ҳ����������ֵ���Լ����ֵ��ǩ�������Դ˼��������ֵ�ĵ�λ
			string expr_name=exp->get_cer()->get_cvar()->get_name();//ȡ���Ҳ�����������
//			int expr_true_h=get_bool_value(expr_name,bool_vars,1);//ȡ���Ҳ���������ֵ
//			int expr_true_sig=get_bool_sig(expr_name,bool_sigs,1);//ȡ���Ҳ�������ֵ��ǩ��
//			long long expr_true_l=((-((long long)expr_true_h*K_NUM)%(long long)A//�����Ҳ�������ֵ�ĵ�λ
//													 +(long long)expr_true_sig)%(long long)A+(long long)A)%(long long)A;
//			int expr_false_h=get_bool_value(expr_name,bool_vars,0);//ȡ���Ҳ������ļ�ֵ
//			int expr_false_sig=get_bool_sig(expr_name,bool_sigs,0);//ȡ���Ҳ�������ֵ��ǩ��
//			long long expr_false_l=((-((long long)expr_false_h*K_NUM)%(long long)A//�����Ҳ�������ֵ�ĵ�λ
//													  +(long long)expr_false_sig)%(long long)A+(long long)A)%(long long)A;
			
			//ȡ�����Ҳ�����������ǩ��
			int right_sig=get_current_sig(v,get_global_string_sig(expr_name,var_scope),var_scope);//ȡ���Ҳ�����������ǩ��
			int left_sig=get_current_sig(v,get_global_string_sig(expl_name,var_scope),var_scope);//ȡ���������������ǩ��
//
			//���±�����Ϊ�����λ�����е���ʱ�õ�����ʱ����
//			long long temp1=((long long)expl_false_h-(long long)expl_true_h)*((long long)expr_false_h-(long long)expr_true_h);
//			long long temp2=(long long)var_false_h-(long long)var_true_h;
			//�����λ�����е���ֵ
//			int M_1=(inverse(temp1,(long long)A)%A)*temp2%A;
//			string str_M1=(M_1>=0)?int2str(M_1):("("+int2str(M_1)+")");//��M1ת��Ϊ�ַ���
			/*
			//���±�����Ϊ�����λ�е���ʱ�õ�����ʱ����
			long long templ=expl_false_l - expl_true_l + expl_true_sig - expl_false_sig;
			long long tempr=expr_false_l - expr_true_l + expr_true_sig - expr_false_sig;
			long long tempv=var_false_l - var_true_l + var_true_sig - var_false_sig;
			//�����λ����ʱ�õ�����ֵ
			int M_5=((templ % A)*(tempr % A)%A+A)%A;
			M_5=(inverse(M_5,(long long)A)*(tempv % A) % A )%A;
			string str_M5=(M_5>=0)?int2str(M_5):("("+int2str(M_5)+")");//��M5ת��Ϊ�ַ���*/
			
			//�����λ�������õ��Ĳ�������
			long long constant1=(((long long)new_sig-(long long)var_true_sig)
			                                  %(long long)P1+(long long)P1)%(long long)P1;
			//�����λ�������õ��Ĳ�������
			long long constant2=(((long long)new_sig-(long long)var_true_sig)
						                      %(long long)P2+(long long)P2)%(long long)P2;
			add_new_sig(v,get_global_string_sig(var_name,var_scope),new_sig,var_scope);//����ֵ��������ǩ�����붯̬ǩ����
			//�����Ҳ���������ֵ����ֵ��������ֵ���ֱ�ת��Ϊ�ַ���
//			string str_expl_true_h=(expl_true_h<0)?"("+int2str(expl_true_h)+")":int2str(expl_true_h);
//			string str_expr_true_h=(expr_true_h<0)?"("+int2str(expr_true_h)+")":int2str(expr_true_h);
//			string str_var_true_h=(var_true_h<0)?"("+int2str(var_true_h)+")":int2str(var_true_h);
			

			
		/*	//����߼�������ĸ�λ����
			out_indent(indent_num,gen_out);
			gen_out<<"H_"<<var_name<<" = ((("<<str_int64<<")H_"<<expl_name<<" - "
			<<str_expl_true_h<<") % "<<str_A<<"* (("<<str_int64<<")H_"<<expr_name<<" - "<<str_expr_true_h
			<<") % "<<str_A<<" + "<<str_A<<") * "<<str_M1<<" % "<<str_A<<" + "
			<<str_var_true_h<<";"<<endl;
			
			//�������ȫ�ֱ���tmpLogicCalVar1�ļ������
			out_indent(indent_num,gen_out);
			gen_out<<tmpLogicCalVar1<<" = "<<expl_true_sig-left_sig<<";"<<endl;
			
			//�������ȫ�ֱ���tmpLogicCalVar2�ļ������
			out_indent(indent_num,gen_out);
			gen_out<<tmpLogicCalVar2<<" = "<<expr_true_sig-right_sig<<";"<<endl;
				
			//����߼�������ĵ�λ����
			out_indent(indent_num,gen_out);
			gen_out<<red_Tmp_INT64<<" = ((("<<str_int64<<")L_"<<expl_name<<" - DT - "
			<<expl_true_l<<" + "<<tmpLogicCalVar1<<") % "<<str_A<<" * ((("<<str_int64<<")L_"
			<<expr_name<<" - DT - "<<expr_true_l<<" + "<<tmpLogicCalVar2<<") % "<<str_A
			<<") % "<<str_A<<" + "<<str_A<<") * "<<str_M5<<" % "<<str_A<<" + "<<constant<<" + DT;"<<endl;
			
			//����߼�������ĵ�λģA����
			out_indent(indent_num,gen_out);
			red_INT64MOD("L_"+var_name,red_Tmp_INT64);*/

			out_indent(indent_num,gen_out);
			string var_head=TmpVarJudge(var_name)?var_name:get_global_string(var_name,var_scope);
			string l_name_head=TmpVarJudge(expl_name)?expl_name:get_global_string(expl_name,var_scope);
			string r_name_head=TmpVarJudge(expr_name)?expr_name:get_global_string(expr_name,var_scope);
						gen_out << var_head  << ".Data = " << "F_VCL_LogicOr("<<l_name_head
								 << ".Data, "<<r_name_head  << ".Data, " << int2HexStr(left_sig)
								<< ", " << int2HexStr(right_sig) << ", " << int2HexStr(constant1) << ", "
								<< int2HexStr(constant2) << ");" << endl;
			//���ǩ��Id�������
			string LeftVar=var_head;
			string CodedFuncCallStr=BuildOutputFunc_str(LeftVar+VAR_CHK_ID_NAME,SIG_REG_ITF,
					int2HexStr(SigID).c_str(),NULL);
			out_indent(indent_num,gen_out);
			gen_out<<CodedFuncCallStr<<endl;
			//����ǵ���ģʽ�������������Ϣ
			if(true==debug_flag)
			{
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<var_name<<"'s new sig : "<<new_sig<<", true value :"<<var_true_h
				<<", true sig : "<<var_true_sig<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<expl_name<<"'s sig : "<<left_sig<<endl;
				out_indent(indent_num,gen_out);
				gen_out<<"//"<<expr_name<<"'s  sig : "<<right_sig<<endl;
			}
		}
		else
		{
			//������Ϊ�������ʱ���׳�������Ϣ
			string error_info="operand is invalid!";
			throw error_info;
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding logical or operation : "+str;
		throw error_info;
	}
}


