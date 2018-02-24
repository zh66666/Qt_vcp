#include "CodedVarBuild_Util.h"
#include "BuildOutputFunc_Utils.h"
#include "gen_utils.h"
#include <iostream>
#include "statement.h"
#include "expression.h"
#include "var.h"
#include "compound_stmt.h"
#include "iteration_stmt.h"
#include "selection_stmt.h"
#include "gen_global_var.h"

#define TESTING_S1S2 0

//���ڼ�¼ѭ�����ڵ���Ҫ�����ı�����Ϣ���������ƣ��Ƿ�Ϊ��ֵ������ֵ
struct adj_var
{
	string name;//��¼��������
	string stamp;//��¼Ϊ��ֵ������ֵ
};

//�洢ѭ��������Ҫ�����ı�����Ϣ
list<adj_var>* adj_name_list=new list<adj_var>;


// ���ܣ��������õ��ı�����Ϣ����ȫ�ֵĴ���������������
// ���������l-����������������ָ��
// 			     a-�������ı�����Ϣ�ṹ��
// ���������l-����������������ָ��
// ����ֵ���ɹ��򷵻�void
// ���ߣ�����
void  add_adj_name(list<adj_var>* l, struct adj_var a)
{
	try
	{
		//��������������Ч��
		check_pt_arg(l,"adjust variable name list's pointer");
		
		list<adj_var>::iterator iter;//���������
		//�����洢�д�����������Ϣ������
		for(iter=l->begin();iter!=l->end();iter++)
		{
			//�жϵ�ǰ��������ָ�ı����������������ı��������Ƿ�һ��
			if(a.name==iter->name)
			{
				//���һ�£���˵���Ѿ����ڸñ��������޸�������ֵ��Ϣ
				iter->stamp=(a.stamp=="left")?"left":iter->stamp;
				return;
			}
		}
		//�жϵ������Ƿ�������ĩ��
		if(iter==l->end())
		{
			//����ǣ��򽫴˱�����Ϣ��������
			l->push_back(a);
		}
	}
	catch (string str)
	{
		string error_info="add adjust variable : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ�������ֵ����ұߵĸ�ֵ���ʽ�������ʽ�еı���������Ϊ��ֵ�������洢����
// ���������s-���ʽ����ָ��
// ���������l-���������Ҫ��ȡ�ı������Ƶ�����ָ��
// ����ֵ���ɹ��򷵻�void
// ���ߣ����ѣ�������
void get_var_names(Expression* s,list<adj_var>*  l)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"expression object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		//�жϱ��ʽ������
		switch(s->get_stamp())
		{
			case 0:
			{
				//����Ǵ��������������ı��ʽ����ֱ������ֵ���ʽ����ֵ���ʽ
				get_var_names(s->get_cel(),l);
				get_var_names(s->get_cer(),l);
			}
			break;
					
			case 1:
			{
				//�����ֻ���Ҳ������ı��ʽ���������ֵ���ʽ
				get_var_names(s->get_cer(),l);	
			}
			break;
			
			case 2:
			{
				//��������ű��ʽ������������еı��ʽ
				get_var_names(s->get_exp(),l);	
			}
			break;
			
			case 4:
			{
				//����Ǳ������ʽ
				//�жϱ����Ƿ�Ϊһ������������Ƿ�Ϊ��ʱ����
				if(0==s->get_cvar()->get_stamp())
				{
					if(!ExtraVarJudge(s->get_cvar()->get_name()))
					{
						//�����һ��ı��������Ҳ�����ʱ��������洢
						struct adj_var a;
						a.name=s->get_cvar()->get_name();
						a.stamp="right";
						add_adj_name(l,a);
					}
				}
				else
				{
					//������������
					get_var_names(s->get_cvar()->get_cexp(),l);
					struct adj_var array_ext={ARRAY_EXTR_NAME,"right"};
					add_adj_name(l,array_ext);
				}
			}
			break;
			default:
			{
				//������������ʽ�������
			}
		}	
	}
	catch (string str)
	{
		string error_info="get variable names in expression : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ�����ѭ���ṹ�и�ֵ�����ߵı����������Ϊ��ʱ����������Ϊ��ֵ�洢����
// ���������s-��������ָ��
// ���������l-����������������ָ��
// ����ֵ���ɹ��򷵻�void
// ���ߣ����ѣ�������
static void get_var_names(Var* s,list<adj_var>* l)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"variable object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		
		string name="";//�洢Ҫ��ŵı�������
		//�жϸñ����Ƿ�Ϊ�������
		if(1==s->get_stamp())
		{
			//��ȡ�����±���ʽ
			get_var_names(s->get_cexp(),l);
		}
		else
		{
			//������ǣ����������ΪҪ�洢������
			name=s->get_name();
			struct adj_var a={name,"left"};
			add_adj_name(l,a);
		}
	}
	catch (string str)
	{
		string error_info="get variable names in variable object : "+str;
		throw error_info;
	}
}

// ���ܣ���ģ�����ѭ���ڲ�����䣬��������ֵ��������ֵ������������洢����
// ���������s-������ָ��
// ���������l-����������������ָ��
// ����ֵ���ɹ��򷵻�void
// ���ߣ����ѣ�������
void get_var_names(Statement* s,list<adj_var>* l)
{
	try
	{
		//��������������Ч��
		check_pt_arg(s,"statement object's pointer");
		check_pt_arg(l,"adjust variable name list's pointer");
		
		//�ж���������
		switch(s->get_stamp())
		{
			case 0:
			{
				//���Ϊ������䣬�������������е��������
				for(list<Statement>::iterator iter=s->get_ccstmt()->get_csl()->begin();
					   iter!=s->get_ccstmt()->get_csl()->end();iter++)
				{
					get_var_names(&(*iter),l);//����ÿ�����
				}
			}
			break;	
			case 1:
			{
				//����if��֧�е����
				get_var_names(s->get_csstmt()->get_cs_if(),l);

				//�ж��Ƿ���else��֧
				if(1==s->get_csstmt()->get_stamp())
				{
					//�������else��֧�������else��֧�е����
					get_var_names(s->get_csstmt()->get_cs_else(),l);
				}
				else
				{
					//�����else��֧�������
				}
			}
			break;	
			case 2:
			{
				//����ѭ�����е����
				get_var_names(s->get_cistmt()->get_cs(),l);
			}
			break;	
			case 4:
			{
				//Ϊ��ֵ���
				//������ֵ���ʽ����ֵ����
				string left_name;
				//��ֵ�Ƿ�Ϊ����
				//��������
				if(s->get_cvar()->get_stamp()==0)
				{
					//���жϸ�ֵ���ʽ�Ƿ�Ϊ��������
					if(s->get_ce()->get_stamp()==5)
					{
						//���������������ʽ����������Ϊ��ֵ�洢����
						for(list<Expression>::iterator  iter=s->get_ce()->get_args()->begin();
							   iter!=s->get_ce()->get_args()->end();iter++)
						{
							get_var_names(&(*iter),l);
						}
						//���������е�ȫ�ֱ�����Ϊ��ֵ����
						vector<string> global_var_list=get_global_var_infunc(var_scope);
						for(vector<string>::iterator it=global_var_list.begin();it!=global_var_list.end();it++)
						{
							struct adj_var adj_global_var={*it,"left"};
							add_adj_name(l,adj_global_var);
						}
					}
					else
					{
						//�����Ϊ�������ã�������ø�ֵ���ʽ
						get_var_names(s->get_ce(),l);
					}

					left_name=s->get_cvar()->get_name();//�洢��ֵ������
					if(!ExtraVarJudge(left_name))
					{
						 get_var_names(s->get_cvar(),l);
					}
				}
				//���鸳ֵ
				else
				{
					//���±�
					get_var_names(s->get_cvar(),l);
					//��ֵ
					get_var_names(s->get_ce(),l);
					//���ӱ���
					struct adj_var array_ext={ARRAY_EXTR_NAME,"left"};
					add_adj_name(l,array_ext);
				}
			}
			break;
			
			default:
			{
				//������䲻����������
			}
		}
	}
	catch (string str)
	{
		string error_info="get variable names in statement : "+str;
		throw error_info;
	}
}



// ���ܣ���Դ��̬ǩ�����У������б������Լ����ǵ�ǰ����Ϣ����һ�ݵ�Ŀ�Ķ�̬ǩ����
// ���������src-Դ��̬ǩ����ָ��
// ���������des-Ŀ�Ķ�̬ǩ����ָ��
// ����ֵ���ɹ��򷵻�void��ʧ�����׳�������Ϣ
// ���ߣ����ѡ�������
void copy_Var_infos(list<Var_info>* src,list<Var_info>* des)
{
	try
	{
		//��������������Ч��
		check_pt_arg(src,"source var_info  list's pointer");
		check_pt_arg(des,"destination var_info  list's pointer");
		
		//����Դ��̬ǩ����
		for(list<Var_info>::iterator iter=src->begin();iter!=src->end();iter++)
		{		
			//���Ƶ�ǰ��������ָ�Ķ��󣬲�����Ŀ������
			Var_info temp=Var_info(iter->get_name(),iter->get_sigs()->back(),iter->get_scope());
			temp.set_stamp(iter->get_stamp());
			des->push_back(temp);
		}
	}
	catch (string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="copy var_infos : "+str;
		throw error_info;
	}
}

// ���ܣ���������������ʽ��S1,S2��ֵ�����ݱ��ʽ�������Ĳ�ͬ�����ò�ͬ�����㷨
//			S1��ֵͨ���������أ�S2��ֵͨ������ֵ����
// ���������sig-�������ʽ��ֵ��ǩ��
// 			      k-���ѡȡ�������
// 			      op-�������ʽ�Ĳ�����
//			      stamp-�����ѭ����ڻ��Ƿ�֧��ڣ�Ϊtrue��Ϊ��֧��ڣ�Ϊfalse��Ϊѭ�����
// ���������s1-����S1ֵ�ĵ�ַ
// ����ֵ���ɹ��򷵻�һ������ֵ��ʧ�����׳�������Ϣ
// ���ߣ����ѡ�������
int  compute_S1S2(int sig, int k, int* s1,string op,bool stamp)
{
	//�ж��ǲ��Ǵ��ڵ��ڲ�����
	if(">="==op)
	{
		//����ǣ��������Ӧ��S1,S2
		*s1=(stamp)?(((long long)k * sig ) & (long long)2147483647)%A : sig ;//����S1
		int s2=(((long long)k * (((long long)sig%A+(K_NUM%A)*(K_NUM%A)%A)%A)) 
					& (long long)2147483647)%A;//����S2
		return s2;
	}
	//�ж��ǲ���С�ڲ�����
	else if("<"==op)
	{
		//����ǣ��������Ӧ��S1,S2
		 int s2=(((long long)k * sig)  & (long long)2147483647)%A;//����S2
		*s1=(stamp)? (((long long)k * (((long long)sig%A+(K_NUM%A)*(K_NUM%A)%A)%A))
					& (long long)2147483647)%A : 
					(((long long)sig%A+(K_NUM%A)*(K_NUM%A)%A)%A);//����S1
		return s2;
	}
	//�ж��ǲ��ǵ��ڲ�����
   else  if("=="==op)
	{
		//����ǣ��������Ӧ��S1,S2
		 *s1=(stamp)?(((long long)k * sig)  & (long long)2147483647)%A: sig;//����S1
		int s2=(((long long)k * (((long long)sig+(K_NUM%A))%A))
					& (long long)2147483647)%A;//����S2
		return s2;
	}
	else if("!="==op)
	{
		//����ǲ����ڲ������������Ӧ��S1,S2
		int s2=(((long long)k * sig)  & (long long)2147483647)%A;//����S2
		*s1=(stamp)?(((long long)k * (((long long)sig+(K_NUM%A))%A))
		& (long long)2147483647)%A:((long long)sig+K_NUM%A)%A;//����S1
		return s2;
	}
	else
	{
		//������ǣ����׳�������Ϣ
		string error_info="compute S1 S2 :  operator is invalid!";
		throw error_info;
	}
}

// ���ܣ���������������ʽ��S1,S2��ֵ�����ݱ��ʽ�������Ĳ�ͬ�����ò�ͬ�����㷨
//			S1��ֵͨ���������أ�S2��ֵͨ������ֵ����
// ���������SubSig-ת�ɼ���������������ֵ������ǩ��
// 			 k-���ѡȡ�������
//			 stamp-�����ѭ����ڻ��Ƿ�֧��ڣ�Ϊtrue��Ϊ��֧��ڣ�Ϊfalse��Ϊѭ�����
// ���������S1-����S1ֵ�ĵ�ַ,True��֧ǩ��
//          S2-False��֧ǩ��
// ����ֵ���ɹ��򷵻�һ������ֵ��ʧ�����׳�������Ϣ
#define CF_SIG_TEST 0
#if CF_SIG_TEST
int* compute_CFSig_S1S2(int SubSig[],int k,int *S1,bool stamp)
{
	int Prime[2]={P1,P2};
	int TwoPower64ModP[2]={0,0};//2^{2K} Mod P
	TwoPower64ModP[0]=((K_NUM%P1)*(K_NUM%P1))%P1;
	TwoPower64ModP[1]=((K_NUM%P2)*(K_NUM%P2))%P2;

	int SubSig1[6]={P1-1,1,P1-1,1,1,P1-1};
	int SubSig2[6]={1,P1-1,1,P1-1,P1-1,1};
	int Candidate_K[6]={1,1,P1-1,P1-1,1,1};
	bool stamp_arr[6]={true,true,true,true,false,false};

	int *S2=new int[2];
	for(int j=0;j<6;j++)
	{
		SubSig[0]=SubSig1[j];
		SubSig[1]=SubSig2[j];
		k=Candidate_K[j];
		stamp=stamp_arr[j];
		if(!stamp)
			k=1;

		cout<<"Var1 Sig="<<SubSig[0]<<endl;
		cout<<"Var2 Sig="<<SubSig[1]<<endl;
		cout<<"K="<<k<<endl;

		for(int i=0;i<2;i++)
		{
			int R1=((long long)SubSig[0]+(long long)SubSig[1])%Prime[i];
			int R2=((long long)SubSig[0]+(long long)SubSig[1]+(long long)TwoPower64ModP[i])%Prime[i];

			cout<<"Prime is "<<(i+1)<<" R1="<<R1<<endl;
			cout<<"Prime is "<<(i+1)<<" R2="<<R2<<endl;

			S1[i]=((long long)k*(long long)R1)%Prime[i];
			S2[i]=((long long)k*(long long)R2)%Prime[i];
		}

		cout<<"S1[0]="<<S1[0]<<" "<<"S1[1]="<<S1[1]<<endl;
		cout<<"S2[0]="<<S2[0]<<" "<<"S2[1]="<<S2[1]<<endl;
	}


	return S2;
}
#else

int* compute_CFSig_S1S2(int SubSig[],int k,int *S1,bool stamp)
{
	int Prime[2]={P1,P2};
	int TwoPower64ModP[2]={0,0};//2^{2K} Mod P
	TwoPower64ModP[0]=((K_NUM_P1)*(K_NUM%P1))%P1;
	TwoPower64ModP[1]=((K_NUM_P2)*(K_NUM%P2))%P2;

	int *S2=new int[2];
	if(!stamp)
		k=1;
#if TESTING_S1S2
	cout<<"Var1 Sig="<<SubSig[0]<<endl;
	cout<<"Var2 Sig="<<SubSig[1]<<endl;
#endif
	for(int i=0;i<2;i++)
	{
		int R1=((long long)SubSig[0]+(long long)SubSig[1])%Prime[i];
		int R2=((long long)SubSig[0]+(long long)SubSig[1]+(long long)TwoPower64ModP[i])%Prime[i];
#if TESTING_S1S2
		cout<<"Prime is "<<(i+1)<<" R1="<<R1<<endl;
		cout<<"Prime is "<<(i+1)<<" R2="<<R2<<endl;
#endif
		S1[i]=((long long)k*(long long)R1)%Prime[i];
		S2[i]=((long long)k*(long long)R2)%Prime[i];
	}
#if TESTING_S1S2
	cout<<"S1[0]="<<S1[0]<<" "<<"S1[1]="<<S1[1]<<endl;
	cout<<"S2[0]="<<S2[0]<<" "<<"S2[1]="<<S2[1]<<endl;
#endif

	return S2;
}
#endif



//�жϷ�֧��ѭ��������������ʽ�Ƿ�Ϸ�
int check_condition_entrance_exp(Expression *s)
{
	int flag=0;
	try
	{
//		if(0==s->get_stamp() && 4==s->get_cel()->get_stamp()
//			&& 3== s->get_cer()->get_stamp() && "0"==s->get_cer()->get_c()
//			&& (">="==s->get_op() || "<"==s->get_op() || "=="==s->get_op() || "!="==s->get_op()))
//		{
//			flag=0;
//		}
//		else
//		{
//			//�������������ʽ�Ƿ������׳�������Ϣ
//			string error_info="the entrance expression  is invalid! ";
//			throw error_info;
//		}

		if(s->get_stamp()==0 && s->get_cel()->get_stamp()==4
			&& s->get_cer()->get_stamp()==3 && s->get_cer()->get_c()=="e_TRUE")
		{
			flag=1;
		}
		else
		{
			//�������������ʽ�Ƿ������׳�������Ϣ
			string error_info="the entrance expression  is invalid! ";
			throw error_info;
		}
	}
	catch(string str)
	{
		//���񱻵������׳��Ĵ�����Ϣ�����ϱ�ģ�����Ϣ�����������׳�
		string error_info="coding entrance expression : "+str;
		throw error_info;
	}
}
//У�����������������������ͨ������ A=(A+C)%P����ʽ
//���������鸽�ӱ�����F_VCL_ArrayCheckUpdate(C1,C2)����ʽ
//��ͨ�������������������䣬��ӦУ����1��У����2
//���鸽�ӱ��������һ����䡣
vector<string> var_check_adjust_stmt(string Var,int *Constants)
{
	string CheckField[2]={VAR_TDATA+VAR_CHK_NAME_1,VAR_TDATA+VAR_CHK_NAME_2};
	string PrimeStr[2]={"P1","P2"};

	vector<string> stmt;
	if(Var!=ARRAY_EXTR_NAME)
	{
		for(int i=0;i<2;i++)
		{
			string AdjVarName=TmpVarJudge(Var)? Var+CheckField[i]:"TC_"+Var+CheckField[i];
			string adj_opexp=BuildOperationExp_str(AdjVarName,"+",PrimeStr[i],
					AdjVarName.c_str(),string(int2HexStr(Constants[i])).c_str());
			adj_opexp=OutIndentStr(indent_num)+adj_opexp;
			stmt.push_back(adj_opexp);
		}
	}
	else
	{
		string ArrayAdjustFuncCall=BuildOutputFunc_str("",FUNC_ARRAYEXT_CHK_UPDATE,
				string(int2HexStr(Constants[0])).c_str(),string(int2HexStr(Constants[1])).c_str(),NULL);
		ArrayAdjustFuncCall=OutIndentStr(indent_num)+ArrayAdjustFuncCall.substr(3);
		stmt.push_back(ArrayAdjustFuncCall);
	}

	return stmt;
}

