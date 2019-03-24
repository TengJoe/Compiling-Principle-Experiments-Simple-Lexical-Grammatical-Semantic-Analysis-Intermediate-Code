#include<iostream>
#include<fstream>
#include<cstring>
#include<string.h>
#include<string>
#include<vector>
#include<stack>
using namespace std;

//�ʷ��������裺
//	1. ��������ָ�
//	2. �������ڷָ�������ж�
//����Ҹĳɱ�ʶ�������ǡ���ĸ+���֡�����ʽ

/*
	�ȿ�������ķ���Ҫע��ĵط������ȸ�ֵ����:=���Ƿ������=
*/

struct var_and_type;//�ýṹ�����﷨����ʱ��ű��������Ժ�ĸ�������
struct back_patch;//�ýṹ���ڻ�����Ҽ�¼����if��while��repeat����������

//���������﷨�����ĺ�������һ���ֺ�����Ҫ��¼�������������ٳ��ڣ�����Ҫ�õ�back_patch����ṹ
void var_declare();
void var_declare_1();
void var_declare_2();
void var_declare_3();
void var_declare_4();
void program();
void judge_type();
int identifier();
string getnext();
void b_and_e();
void statement_list();
void statement();
void statement_list_1();
back_patch if_sentence();
back_patch if_sentence_1(back_patch &judge);
back_patch while_sentence();
back_patch repeat_sentence();
void assignment_statement();
//�����濪ʼ��"<������>��>"��һ����ʽ�����First���н���
//û�а취��ֻ�ܹ�ͨ�������������ĸ���������﷨����
//�����Ļ�����ͨ����������ɵ�
back_patch bool_sentence();
back_patch bool_sentence_1(back_patch &judge);
back_patch bool_xiang(back_patch &judge);
back_patch bool_xiang_1(back_patch &judge);
back_patch bool_factor(back_patch &judge);
back_patch boolean(back_patch &judge);
back_patch arithmetic_sentence();
int arithmetic_sentence_1();
int arithmetic_quantity();
int xiang();
int xiang_1();
int factor();
int integer();
int relation_character(back_patch &temp);

struct bsf_zs_zfcs//������¼ʶ������ı�ʶ�����������ַ�������string���飨����������ƴ������ĸ(�ި���))
{
	string data;
	int type;//37Ϊ������3?Ϊ��ʶ����38Ϊ�ַ�����
};

string symbol[61] = { "","and","array","begin","bool","call","case",//����������������ʱ���Ҫ�õ���
	"char","constant","dim","do","else","end","false",
	"for","if","input","integer","not","of","or","output",
	"procedure","program","read","real","repeat","set",
	"stop","then","to","true","until","var","while","write","��ʶ��","����","�ַ�����",
	"(",")","*","*/","+",",","-",".","..","/","/*",":",":=",";","<",
	"<=","<>","=",">",">=","[","]" };

string word[10000];//������¼ʶ������Ĵʵ�string����
int num = 0;//������¼�ʵĸ����������1��ʼ��¼
bsf_zs_zfcs bsf_zs_zf[1000];//������¼ʶ������ı�ʶ�����������ַ�������string����
int bsf_zs_zf_num = 0;//������¼��ʶ���������1��ʼ��¼

string bsf[1000];//��ʶ����
int bsf_num = 1;

vector<int> line_first_pos;//������¼ÿһ�еĵ�һ������word�е�λ�ã�����ͳ������

bool error_judge(char a)
{
	if (a == '\'' || a == '(' || a == ')' || a == '*' || a == '+' || a == ',' || a == '-' || a == '.'
		|| a == '/' || a == ':' || a == ';' || a == '<' || a == '=' || a == '>' || a == '[' || a == ']') return true;
	else if ((a >= 'a'&&a <= 'z') || (a >= 'A' && a <= 'Z') || (a >= '0'&&a <= '9')) return true;
	else if (a == ' ' || a == '	') return true;//������пո�����tab
	else return false;
}

bool is_symbol(string a)//�ж�һ�������ǲ��Ǳ�����
{
	for (int i = 0; i <= 35; i++)
	{
		if (a == symbol[i]) return true;//˵���Ǳ�����
	}
	return false;
}

//���ʷָ��������ʶ��string����ȡ����Ĵ���line��ʶ�����ʱ�����õ�
//����ʶ�𵽵����һ�����ʵ�ʱ��Ҫ��������ʵ�num��¼һ��
void word_analysis(string a, int line)
{
	int i = 0;//a��ָ�룬��ʾ��������
	int first_flag = 0;//������ʶ�Ƿ��Ѿ���¼�ı��е�һ���ʵ�λ��

	//����ַ����������Ϊ�ո���ô��������κ�һ��if�����Լ���whileѭ��
	while (i < a.size())
	{
		if (error_judge(a[i]) == false) {
			cout << "��⵽�Ƿ��ַ�����������Ϊ��" << line
				<< "  ����Ĵ����ַ�Ϊ��" << a[i] << endl << endl; abort();
		}
		if (a[i] == ' ' || a[i] == '	') { i++; continue; }//ʶ�𵽿ո��tab������
		if (a[i] == '(' || a[i] == ')' || a[i] == '+' || a[i] == ',' || a[i] == '*'//�ж��ǲ��ǵ����������ǵ��������ôֱ�Ӽ�¼Ϊһ����
			|| a[i] == '-' || a[i] == ';' || a[i] == '[' || a[i] == ']' || a[i] == '=')
		{
			num++; word[num] = a[i]; i++;
			if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
		}

		else if (a[i] >= '0' && a[i] <= '9')//�ж��ǲ�������
		{
			string t;//û��������һ���ʾ͸�ֵ��t����t�����Ժ��ٰ�t�ŵ���¼�ʵ�string��������
			while (i < a.size() && a[i] >= '0' && a[i] <= '9')//�ж��ǲ������ִ�
			{
				t += a[i];
				i++;
				//�����������һ���ַ���Ϊ0~9��˵���������������
				if (i < a.size())//��ֹԽ��
				{
					if (a[i] < '0' || a[i] > '9')
					{
						//����Ҫ���һ�������ʶ����ǰ�Ƿ���ֹ���������ֹ�����ôҪ����
						int check = 0;
						for (int j = 1; j <= bsf_zs_zf_num; j++)
						{
							if (t == bsf_zs_zf[j].data)
							{
								check = 1;
								break;
							}
						}
						if (check == 1)
						{
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						}
						else
						{
							bsf_zs_zf_num++; bsf_zs_zf[bsf_zs_zf_num].data = t; bsf_zs_zf[bsf_zs_zf_num].type = 37;
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						}
					}
				}
				else if (i == a.size())
				{
					int check = 0;
					for (int j = 1; j <= bsf_zs_zf_num; j++)
					{
						if (t == bsf_zs_zf[j].data)
						{
							check = 1;
							break;
						}
					}
					if (check == 1)
					{
						num++; word[num] = t;
						if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
					}
					else
					{
						bsf_zs_zf_num++; bsf_zs_zf[bsf_zs_zf_num].data = t; bsf_zs_zf[bsf_zs_zf_num].type = 37;
						num++; word[num] = t;
						if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
					}
				}
			}
		}

		else if ((a[i] >= 'a'&&a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z'))//�ж��ǲ�����ĸ�������γɱ�ʶ��
		{
			string t;
			while (i < a.size() && ((a[i] >= 'a'&&a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0'&&a[i] <= '9')))
			{
				t += a[i];
				i++;
				//�����������һ���ַ��Ȳ�����ĸҲ�������֣�˵�������ʶ���Ѿ���ȡ����
				if (i < a.size())//��ֹԽ��
				{
					if ((a[i] < 'a' || a[i] > 'z') && (a[i] < 'A' || a[i] > 'Z') && (a[i] < '0' || a[i] > '9'))
					{
						//������Ҫ�����������ǲ��Ǳ����֣�����ǣ��򲻼��뵽bsf�У�ֱ�Ӽӵ�word�м���
						if (is_symbol(t))
						{
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						}
						else //Ȼ��Ҫ���һ�������ʶ����ǰ�Ƿ���ֹ���������ֹ�����ôҪ����
						{
							int check = 0;
							for (int j = 1; j <= bsf_zs_zf_num; j++)
							{
								if (t == bsf_zs_zf[j].data)
								{
									check = 1;
									break;
								}
							}
							if (check == 1)
							{
								num++; word[num] = t;
								if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
							}
							else
							{
								bsf_zs_zf_num++; bsf_zs_zf[bsf_zs_zf_num].data = t; bsf_zs_zf[bsf_zs_zf_num].type = 36;
								num++; word[num] = t;
								if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
							}
						}
					}
				}
				else if (i == a.size())//����պõ�string�����һ���ַ�
				{
					//������Ҫ�����������ǲ��Ǳ����֣�����ǣ��򲻼��뵽bsf�У�ֱ�Ӽӵ�word�м���
					if (is_symbol(t))
					{
						num++; word[num] = t;
						if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
					}
					else //Ȼ��Ҫ���һ�������ʶ����ǰ�Ƿ���ֹ���������ֹ�����ôҪ����
					{
						int check = 0;
						for (int j = 1; j <= bsf_zs_zf_num; j++)
						{
							if (t == bsf_zs_zf[j].data)
							{
								check = 1;
								break;
							}
						}
						if (check == 1)
						{
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						}
						else
						{
							bsf_zs_zf_num++; bsf_zs_zf[bsf_zs_zf_num].data = t; bsf_zs_zf[bsf_zs_zf_num].type = 36;
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						}
					}
				}
			}
		}
		else if (a[i] == '\'') //�ж��ǲ���',����ǣ���������Ķ����ַ�����
		{
			string t; t += a[i];
			i++;
			int check_end = 0;//������ʾ�ܷ���ͬһ�м����ڶ�����
			while (i < a.size() && error_judge(a[i]) == true && a[i] != '\'')//����ַ����ַ����У����Ҳ��ǵڶ���������ô������ȡ��һ���ַ�
			{
				t += a[i];
				i++;
				if (i < a.size())
				{
					if (a[i] == '\'')//�ж���һ���ַ��Ƿ�Ϊ'��������Ǿ�Ҫ�������´���
					{
						check_end = 1;
						t += a[i];//�ѡ��ӵ�t���棬Ȼ��t��������
						i++;
						//����Ҫ���һ������ַ�������ǰ�Ƿ���ֹ���������ֹ�����ôҪ����
						int check = 0;
						for (int j = 1; j <= bsf_zs_zf_num; j++)
						{
							if (t == bsf_zs_zf[j].data)
							{
								check = 1;
								break;
							}
						}
						if (check == 1)
						{
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
							break;
						}
						else
						{
							bsf_zs_zf_num++; bsf_zs_zf[bsf_zs_zf_num].data = t; bsf_zs_zf[bsf_zs_zf_num].type = 38;
							num++; word[num] = t;
							if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
							break;
						}
					}
				}
				else if (i == a.size())
				{
					int check = 0;
					for (int j = 1; j <= bsf_zs_zf_num; j++)
					{
						if (t == bsf_zs_zf[j].data)
						{
							check = 1;
							break;
						}
					}
					if (check == 1)
					{
						num++; word[num] = t;
						if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						break;
					}
					else
					{
						bsf_zs_zf_num++; bsf_zs_zf[bsf_zs_zf_num].data = t; bsf_zs_zf[bsf_zs_zf_num].type = 38;
						num++; word[num] = t;
						if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
						break;
					}
				}

			}
			if (i == a.size() && check_end == 0) { cout << "��⵽\'û�еõ�ƥ�䣬��������Ϊ��" << line << endl << endl; abort(); }
		}
		else if (a[i] == '/')//�����������ַ�����ô��Ҫ�ж�һ����һ���ַ��Ƿ�Ϊ*�������Ϊ*��/��ʾ����
		{
			string t; t += a[i];
			i++;
			int check_end = 0;
			if (a[i] == '*')//���a[i]Ϊ�˺ţ���ô�����/*��ע�������һ��ζ�Ҫʡ��
			{
				i++;
				while (i < a.size() && error_judge(a[i]) == true)
				{
					if (a[i] == '*')//�����⵽*�ţ����п��ܻ����*/����Ҫ����������ж�
					{
						i++;
						if (a[i] == '/')//���a[i]Ϊ/�������*/������ע�ͽ���
						{
							check_end = 1;
							i++;
							break;
						}
					}
					else i++;//�������*�ţ��Ǿͼ�����ע�ͳ���
				}
				if (i == a.size() && check_end == 0) { cout << "��⵽/*û�еõ�ƥ�䣬��������Ϊ��" << line << endl << endl; abort(); }
			}
			else//�������ͨ��/�ţ���ֱ����Ϊ�������¼��OK
			{
				num++; word[num] = t;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
		}
		else if (a[i] == '.')//�ж���.����..
		{
			string t; t += a[i];
			i++;
			if (i < a.size() && a[i] == '.')//���a[i]��.����ô�Ͱ����ӵ�t���棬t�����..��Ȼ���ٸ�ֵ��word
			{
				t += a[i]; num++; word[num] = t; i++;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
			else//���a[i]����.����ôֱ�ӵ����������¼
			{
				num++; word[num] = t;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
		}
		else if (a[i] == ':')//�ж���:����:=
		{
			string t; t += a[i];
			i++;
			if (i < a.size() && a[i] == '=')//���a[i]��=����ô�Ͱ����ӵ�t���棬t�����:=��Ȼ���ٸ�ֵ��word
			{
				t += a[i]; num++; word[num] = t; i++;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
			else//���a[i]����=����ôֱ�ӵ����������¼
			{
				num++; word[num] = t;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
		}
		else if (a[i] == '<')//�ж���< ���� <= ���� <>
		{
			string t; t += a[i];
			i++;
			if (i < a.size() && (a[i] == '=' || a[i] == '>'))//���a[i]��=����>����ô�Ͱ����ӵ�t���棬t�����<=��<>��Ȼ���ٸ�ֵ��word
			{
				t += a[i]; num++; word[num] = t; i++;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
			else//���a[i]����=����ôֱ�ӵ����������¼
			{
				num++; word[num] = t;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
		}
		else if (a[i] == '>')//�ж��� > ���� >=
		{
			string t; t += a[i];
			i++;
			if (i < a.size() && a[i] == '=')//���a[i]��=����ô�Ͱ����ӵ�t���棬t�����>=��Ȼ���ٸ�ֵ��word
			{
				t += a[i]; num++; word[num] = t; i++;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
			else//���a[i]����=����ôֱ�ӵ����������¼
			{
				num++; word[num] = t;
				if (first_flag == 0) { line_first_pos.push_back(num); first_flag = 1; }
			}
		}
	}
}

/*****************************************************�����ķָ���1*****************************************************/

//�﷨�����������м�������Ԫʽ��
//�﷨�����������Ҫ�ı������£�
int p = 0;//������¼�����ڼ���TOKEN��
string TOKEN;//���������һ��Ҫ�������

//��¼�����������ʶ���Ƿ��Ѿ�����ȡ����identifier������ʹ�ã��жϳ������Ƿ񱻶���Ϊ���������ı�ʶ��������Ǿͱ���
int if_program_name;

//������������Ҫ�õ�����������
int var_flag;//��־�Ƿ����ڽ��б���������������ڽ��б�������������Ҫ��<��ʶ��>identifier�����������var_list�ļ�¼����
struct var_and_type//����������������ı�������
{
	string data;//����Ѿ������ı�ʶ���Ĵ�����
	double value;//���������������Ĵ�С���������������ֻ��int���͵ĸ�ֵ�����㣩
	int type;//1Ϊinteger��2Ϊchar��3Ϊbool

	//֮����Ҫ��Ĭ��������Ϊ-1������Ϊ�����������ͻ����ʱ��Ҫ����-1��Ԫ�ؿ�ʼ���л���
	var_and_type()
	{
		type = -1;//Ĭ������Ϊ-1
	}

	var_and_type(string d, int t)
	{
		data = d;
		type = t;
	}
};
//������¼��������ʱ���������б���������������������Ϊ��ʱ��û��ʼ�������������ڱ�����������var_declare�����б�ʹ�ã�
vector<var_and_type> var_list;

int var_end = 0;//������¼�����������û�У�����identifier���棩

//��¼���ݵ�λ�ã��ڿ��ܳ���First�������ĵط�Ҫ��¼һ�µ�ǰ�����ֵ�λ�ã����һ�־���ƥ��ʧ�ܣ�����p���ݵ��ñ�����
int backtrack_pos = 0;

//������¼�﷨�������ڼ��У����㱨��
int error_line = 0;

//������һЩ������ԪʽҪ�õ��ı���
vector<vector<string>> quaternary_list;//������¼�������ɵ���Ԫʽ
stack<var_and_type> cal_stack;//����������������ı�����¼�����ջ����
int temp_var_num;//��¼��ʱ���������ж��ٸ�����¼��ǰ��ʱ����������

//�ýṹ���ڻ�����Ҽ�¼����if��while��repeat����������
struct back_patch
{
	vector<int> true_chain;//�����������
	vector<int> false_chain;//������ļ���
	vector<int> else_if_chain;//��else�ľ��ӣ�if then֮��Ҫֱ������������if else
	//����ж��and�������һ��or����ô���and������ļٳ��ڲ��ܼӵ��������棬���Ǽӵ����������
	//ÿ��ֻҪ�ٴ�����һ��or���Ǿͽ��л�����һֱ���������ʽ������û��or���Ǿ�ֱ�Ӱ�and������ļ������ӵ������ļ�������
	vector<int> and_or_false_chain;

	int codebegin;//��һ����Ԫʽ�����
	int accept;//������ʶʽ���ǲ����������ʽ
	string op;//��<�������ʽ><��ϵ��><�������ʽ>��������ԪʽҪ�õ�
	bool is_swap;//������¼������Ƿ���not���ֲ�Ҫ���������
	back_patch() { accept = 0; is_swap = 0; }
	//���bool���ʽ������or�Ѷ�����������������أ���ô�����������Ǹ�bool��ܵ�����ٳ��ڣ�������ǵ�����Ҫ�����������һ�����
	void addTrueChain(back_patch &st)
	{
		if (st.is_swap == 0)	true_chain.insert(true_chain.end(), st.true_chain.begin(), st.true_chain.end());
		else true_chain.insert(true_chain.end(), st.false_chain.begin(), st.false_chain.end());
	}
	void addFalseChain(back_patch &st)//�����λ�õĳ��ڼӵ�������
	{
		if (st.is_swap == 0)	false_chain.insert(false_chain.end(), st.false_chain.begin(), st.false_chain.end());
		else false_chain.insert(false_chain.end(), st.true_chain.begin(), st.true_chain.end());
	}
	void addAndOrFalseChain(back_patch &st)
	{
		and_or_false_chain.insert(and_or_false_chain.end(), st.and_or_false_chain.begin(), st.and_or_false_chain.end());
	}
};

//�﷨��������
void grammar_analysis()
{
	TOKEN = getnext();
	program();
	//�������������򶼽����ˣ�������Ԫʽ(sys,-,-,-)
	vector<string> temp;
	temp.push_back("sys");
	temp.push_back("-");
	temp.push_back("-");
	temp.push_back("-");
	quaternary_list.push_back(temp);
	return;
}

//���ڻ�ȡ��һ���ֵ�TOKEN������
//ÿ�ζ��ʵ�ʱ�򶼼��һ��p�Ƿ���Ҫ���е�ʱ��
string getnext()
{
	p++;
	//ÿ�ζ�һ���ʶ����һ���Ƿ�Ҫ���У�����ÿ�μ�鶼�����ڶ�ȡ��������ʼ
	int i = error_line;
	if (p == line_first_pos[i]) { /*cout << word[p]<<" " <<error_line<< endl;*/ error_line++; }//���һ��������ǲ�����һ�еĵ�һ����

	return word[p];
}

void print_error_line()
{
	cout << "�������������" << error_line << "	�������" << endl << endl;
}

//�﷨�����Ĵ��󱨸�
void error(int i)
{
	if (i == 1) { cout << "�﷨���󣺳����һ���ʲ���program" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 2) { cout << "�﷨���󣺱�����������var	����ȱ�ٱ�ʶ��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 3) { cout << "�﷨���󣺳�����������program <��ʶ��>	����ȱ�ٷֺš�;��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 4) { cout << "�﷨����ȱ�ٳ����������.��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 5) { cout << "�﷨���󣺳��������ܱ���������������" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 6) { cout << "�﷨���󣺱�����������ȱ��ð�š�:��������ȱ�ٹؼ���begin���޷�������������" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 7) { cout << "�﷨���󣺱�����������ȱ�ٷֺš�;��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 8) { cout << "�﷨���󣺱�����������û���ҵ���Ӧ�ı�ʶ��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 9) { cout << "�﷨���󣺱����������������ڵı������ͣ�����integer/char/bool��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 10) { cout << "�﷨����ȱ�ٱ�����begin" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 11) { cout << "�﷨����ȱ�ٱ�����end����������;ȱ�ٷֺš�;������������в����ڵı�ʶ��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 12) { cout << "�﷨����if���ȱ��if������" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 13) { cout << "�﷨����if����<�������ʽ>����" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 14) { cout << "�﷨����if���ȱ��then������,��������еĲ������ʽ����" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 15) { cout << "�﷨����while���ȱ�ٱ�����while" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 16) { cout << "�﷨����while���ȱ��do�ؼ���" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 17) { cout << "�﷨����repeat���ȱ�ٱ�����repeat" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 18) { cout << "�﷨����repeat���ȱ�ٱ�����until" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 19) { cout << "�﷨����repeat��䣬until���<�������ʽ>����" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 20) { cout << "�﷨���󣺸�ֵ��û���ҵ���ʶ��,��ʶ��δ���������߸������Ͳ��Ϸ�" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 21) { cout << "�﷨���󣺸�ֵ��ȱ��:= ���� û���ҵ���Ӧ�ı�ʶ��" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 22) { cout << "�﷨���󣺸�ֵ����������ʽ����" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 23) { cout << "�﷨������������\"(<�������ʽ>)\"ȱ��������" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 24) { cout << "�﷨���󣺲������еĲ������ʽȱ��������" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 25) { cout << "�﷨����\"<�������ʽ><��ϵ��><�������ʽ>\"��<��ϵ��>ûʶ�����" << endl; print_error_line(); system("pause"); exit(0); }
	if (i == 26) { cout << "�﷨���󣺲����������޷�ʶ��Ĳ���������������������ж�����Ƿ���ȷ" << endl; print_error_line(); system("pause"); exit(0); }
}

//<����> �� program <��ʶ��> ��<����˵��> <�������> .
void program()
{
	string tmp;

	tmp = "program";
	if (TOKEN.compare(tmp) != 0) error(1);//����1�������һ���ʲ���program
	TOKEN = getnext();

	if (identifier() == -1) error(2);//ת��<��ʶ��>������2��û���ҵ���ʶ��
	//����Ҫ����һ�µ�һ����Ԫʽ��program,������,-,-)
	vector<string> temp;
	temp.push_back("program"); temp.push_back(word[p - 1]); temp.push_back("-"); temp.push_back("-");
	quaternary_list.push_back(temp);

	tmp = ";";
	if (TOKEN.compare(tmp) != 0) error(3);//����3��ȱ�ٷֺ�
	TOKEN = getnext();


	var_declare();//ת��<����˵��>
	b_and_e();//ת��<�������>

	tmp = ".";
	if (TOKEN.compare(tmp) != 0) error(4);//����4��û�г��������
	return;
}

//��ʶ��ʶ�𣬼��TOKEN�Ƿ��ʶ�����������Ƿ��Ǳ������������г��ֵı�ʶ��
//����ǣ���ôҪ�ѱ�ʶ�����뵽var_list����
//��������������±꣬���û�ҵ���������������򷵻�-1
int identifier()
{
	//������¼�ҵ��ı�ʶ�������������б�ʶ���ĵڼ���
	int t = -1;
	//���ҵ����ڵ�TOKEN�ǵڼ�����ʶ��
	for (int i = 1; i <= bsf_num; i++)
	{
		if (TOKEN.compare(bsf[i]) == 0) { t = i; TOKEN = getnext(); break; }
	}

	//�ڱ����������Ժ���һ��TOKENӦ����begin������var_declare4��Ҫ���µ���var_declare1
	//����Ҫ���ǲ���begin,����ǣ���ô�˳�����������־λvar_end��1
	string tmp = "begin";
	if (t == -1 && TOKEN.compare(tmp) == 0) { var_end = 1; return 0; }


	//��Ϊ�жϾ��͵�ʱ���������if��while��repeat�����Ͼ䡢��ô�ͻ��Զ����ൽ��ֵ��
	//����ֵ�������һ��Ҫȡ�ľ���end����������ֱ������������һ�������ֵ����ֵ��
	tmp = "end";
	if (t == -1 && TOKEN.compare(tmp) == 0 && word[p + 1].compare(".") == 0) { return -2; }

	//���û���ڱ�ʶ���������ҵ��ñ�ʶ������ֱ�ӷ���
	if (t == -1) return -1;

	//���ĳ����ʶ���ǵ�һ����ʶ��(������)�����ǵ�һ�ζ�����if_program==0���﷨��ȷ
	//һ���ڶ��λ����������������Ϊ���������ı�ʶ������ô�����ǳ���
	if (t == 1)
	{
		if (if_program_name == 1) error(5);//����5������������������������
		if_program_name = 1;//��һ�ζ����������������Ҫ�ѱ�־λ��1
		return 0;
	}

	//��ʶ�����ʶ����ʱ�������������1. ���������еı���	2. �����и�������͸�ֵ���ȵı�������					

	if (var_flag == 1)//�����1. ���������г��ֵı�������ô��Ȼ��Ҫ�浽var_list�����
	{
		var_and_type tmp;
		tmp.data = word[p - 1];
		var_list.push_back(tmp);
		return 0;
	}
	else //�����2. ��ô���еı�ʶ��Ӧ�ö���var_list�����ˣ������и�������͸�ֵ���ȵı�����������ôҲҪ������Ӧ�ı�����¼����
	{
		//���ҵ�var_list�����Ӧ���string�ı�ʶ��������ҵ����ͷ��������ʶ����var_list��λ��
		for (int i = 0; i < var_list.size(); i++)
		{
			if (word[p - 1].compare(var_list[i].data) == 0) return i;
		}
	}
}

//<����˵��> �� var <��������>����
void var_declare()
{
	//�ȿ����ó���Ƭ����û�б�������
	string tmp;
	tmp = "var";
	//�����⵽var˵���б�������
	if (TOKEN.compare(tmp) == 0)
	{
		TOKEN = getnext();
		var_flag = 1;//׼���������ı������浽var_list����
		var_declare_1();//��������
		var_flag = 0;//������������������������ټ�⵽����Ҳ���ü��뵽var_list����
	}
	//���û���κ��ַ�������ô�ͽ�����ֱ�ӽ��븴�Ͼ䣬�˳��ú���
	return;
}

//<��������> �� <��ʶ����> ��<����> ��var_declare_4����������ʽ��
void var_declare_1()
{
	var_declare_2();//ת����ʶ����

	if (var_end == 1) return;//��ʶ������Ҫ�����ǲ���begin���������ô����var_declare���й���

	string tmp = ":";//ð��
	if (TOKEN.compare(tmp) != 0) error(6);//����6���������崦ȱ��ð��
	TOKEN = getnext();

	judge_type();

	tmp = ";";//�ֺ�
	if (TOKEN.compare(tmp) != 0) error(7);//����7���������崦ȱ�ٷֺ�
	TOKEN = getnext();

	var_declare_4();
	return;
}

// <��ʶ����> �� <��ʶ��> var_declare_3����������ʽ��
void var_declare_2()
{
	if (identifier() == -1) error(8);//�ж�TOKEN�Ƿ�Ϊ��ʶ�������Ϊ-1���� ����8��û���ҵ���Ӧ��ʶ��
	if (var_end == 1) return;//��ʶ������Ҫ�����ǲ���begin���������ô����var_declare���й���
	var_declare_3();//ת��<var_declare_3>
	return;
}

//<����> �� integer��bool��char		����һ�����ͻ����
//!!!!!�˴������������ͣ�Ӧ�öԱ��������ı������л������!!!!!
void judge_type()
{
	string tmp1 = "integer", tmp2 = "char", tmp3 = "bool";
	if (TOKEN.compare(tmp1) == 0)
	{
		TOKEN = getnext();
		//�ȱ�������var_list������⵽������typeΪ-1��ʱ��˵����Щ�����Ǳ������������ȥ�ģ���ô�ͽ������ͻ���
		for (vector<var_and_type>::iterator it = var_list.begin(); it != var_list.end(); it++)
		{
			if (it->type == -1) it->type = 1;
		}
		return;
	}
	else if (TOKEN.compare(tmp2) == 0)
	{
		TOKEN = getnext();
		//�ȱ�������var_list������⵽������typeΪ-1��ʱ��˵����Щ�����Ǳ������������ȥ�ģ���ô�ͽ������ͻ���
		for (vector<var_and_type>::iterator it = var_list.begin(); it != var_list.end(); it++)
		{
			if (it->type == -1) it->type = 2;
		}
		return;
	}
	else if (TOKEN.compare(tmp3) == 0)
	{
		TOKEN = getnext();
		//�ȱ�������var_list������⵽������typeΪ-1��ʱ��˵����Щ�����Ǳ������������ȥ�ģ���ô�ͽ������ͻ���
		for (vector<var_and_type>::iterator it = var_list.begin(); it != var_list.end(); it++)
		{
			if (it->type == -1) it->type = 3;
		}
		return;
	}
	else error(9);//����9�������ڵı������ͣ�����integer/char/bool��
}

//var_declare_4 �� <��������>����
void var_declare_4()
{
	var_declare_1();//���ñ�������
}


//var_declare_3 ����<��ʶ����>����
void var_declare_3()
{
	string tmp = ",";
	if (TOKEN.compare(tmp) == 0) { TOKEN = getnext(); var_declare_2(); }//����ܹ�ƥ���϶��ţ���ô����<��ʶ����>
	return;//���û�ж��ţ���ôֱ�ӷ���
}

//<���Ͼ�> �� begin <����> end
void b_and_e()
{
	string tmp = "begin";
	if (TOKEN.compare(tmp) != 0) error(10);//����10��ȱ�ٱ�����begin
	TOKEN = getnext();

	statement_list();

	tmp = "end";
	if (TOKEN.compare(tmp) != 0) error(11);//����11��ȱ�ٱ�����end������ȱ�ٷֺš�;�����³������
	TOKEN = getnext();

	return;
}

//<����> �� <���> statement_list_1����������ʽ��
void statement_list()
{
	statement();
	statement_list_1();
	return;
}

//statement_list_1 ->��<����>����
void statement_list_1()
{
	string tmp = ";";
	if (TOKEN.compare(tmp) == 0) { TOKEN = getnext(); statement_list(); }//�������ӷֺţ���ôת��<����>���������κ�����
	return;
}


/*****************************************************�����ķָ���2********************************************************/
//���뵽����ʶ����������ˣ��ܶ�first�����н������ҵĽ���취�ǻ��ݣ���ʵ������һ����ȥ���ԣ�������κ�һ�����Ͷ�ƥ�䲻�Ͼͱ���

//Ȼ���������ı�����������͵ĺ������ܣ�<���> ����ֻ������Ҫ�����־���

//ע�⣬���������Ժ����д���ֻ���ڸ������ĺ������淢������һЩ��;������ʶ���ս����Ҳ���ܷ�����

//<���> �� <��ֵ��>��<if��>��<while��>��<repeat��>��<���Ͼ�>
void statement()
{
	back_patch temp;
	//��������Ҫ�ж�����һ��������ʲô���͵�
	//���ȿ�����һ�����ǲ���if,while,repeat,begin�е�һ����������ǣ���ô���Ǹ�ֵ��
	string tmp1 = "if", tmp2 = "while", tmp3 = "repeat", tmp4 = "begin";
	if (TOKEN.compare(tmp1) == 0) //Ҫô��if���
	{
		temp = if_sentence();
		//����Ҫ����һ��judge��else_if��
		for (int i = 0; i < temp.else_if_chain.size(); i++)
		{
			quaternary_list[temp.else_if_chain[i]][3] = to_string(quaternary_list.size());
		}
	}
	else if (TOKEN.compare(tmp2) == 0) { temp = while_sentence(); } //Ҫô��while���
	else if (TOKEN.compare(tmp3) == 0) { temp = repeat_sentence(); }//Ҫô��repeat���
	else if (TOKEN.compare(tmp4) == 0) { b_and_e(); }//Ҫô�Ǹ��Ͼ�
	else { assignment_statement(); }//ȫ�����Ǿ�ֻ���Ǹ�ֵ�����
	return;
}

//<if��>��if <�������ʽ> then <���>  if_sentence_1����������ʽ��
back_patch if_sentence()
{
	back_patch judge;
	string tmp = "if";
	if (TOKEN.compare(tmp) != 0) error(12);//����12��if���ȱ��if�����֣������ܵĴ���
	TOKEN = getnext();

	int codebegin = quaternary_list.size();//�Ȱ�if����λ�ü�¼������˵

	judge = bool_sentence();//ת��<�������ʽ>
	if (judge.accept == -1) error(13);//����13��if����<�������ʽ>����

	tmp = "then";
	if (TOKEN.compare(tmp) != 0) error(14);//����1?��if���ȱ��then������
	TOKEN = getnext();
	//ִ����bool_sentence()��ÿ�������������ڡ��ٳ��ڼ���<�������ʽ>����Ԫʽ�Ѿ��������ˣ����ڽ�����Ҫ��������

	//���￪ʼҪ�Բ������ʽ����������ٳ��ڽ��л���
	//bool�������У����Զ������ĳ��ڽ��л�����
	for (int i = 0; i < judge.true_chain.size(); i++)
	{
		quaternary_list[judge.true_chain[i]][3] = to_string(quaternary_list.size());
	}

	statement();//ת��<���>

	//����Ҫ�ж�һ�º�����û��else����У���ôִ����then��������
	if (TOKEN.compare("else") == 0)
	{
		//���������ļٳ���Ҳһ�������ˣ�����Ҫ�Ѽٳ��������ȥ
		vector<string> tempp; tempp.push_back("j"); tempp.push_back("-"); tempp.push_back("-"); tempp.push_back("������ļٳ���");
		quaternary_list.push_back(tempp);
		judge.else_if_chain.push_back(quaternary_list.size() - 1);//ר����һ�������������else��then�����ִ�к�������λ��
	}

	judge = if_sentence_1(judge);//ת��if_sentence_1

	//�����ܵ�else��ִ���꣬�Ϳ��Ի�������ĳ�����


	//ûelseʱֱ����<���>ִ�к󷵻ص�codebegin����¼����������ʼ���ɵ���Ԫʽ��
	if (judge.accept == -2)
	{
		for (int i = 0; i < judge.false_chain.size(); i++)
		{
			quaternary_list[judge.false_chain[i]][3] = to_string(quaternary_list.size());
		}
		//����and�������һֱ��û������or��˵������and_or���ͣ���ôֱ�Ӷ�and������ļ������л���
		for (int i = 0; i < judge.and_or_false_chain.size(); i++)
		{
			quaternary_list[judge.and_or_false_chain[i]][3] = to_string(quaternary_list.size());
		}
	}
	//����һ���������else if ��䣬else if�����Ҫ�ж�֮����û��else���
	//����У���ô�ٳ��ھ�����һ��else����ŵ����
	//���û�У���ôֱ������
	else //��else�ǿ϶�����else��<���>���ص�codebegin��(��¼else��<���>�ʼ���ɵ���Ԫʽ��
	{
		for (int i = 0; i < judge.false_chain.size(); i++)
		{
			quaternary_list[judge.false_chain[i]][3] = to_string(judge.codebegin);
		}
		//����and�������һֱ��û������or��˵������and_or���ͣ���ôֱ�Ӷ�and������ļ������л���
		for (int i = 0; i < judge.and_or_false_chain.size(); i++)
		{
			quaternary_list[judge.and_or_false_chain[i]][3] = to_string(judge.codebegin);
		}
	}

	return judge;
}

//if_sentence_1 ->(else<���>| �� )
back_patch if_sentence_1(back_patch &judge)
{
	string tmp = "else";
	if (TOKEN.compare(tmp) == 0)//ת��<���>���������ִ���Ժ�Ҫ��ȡһ�����ڵ���Ԫʽ���ɵ�������
	{
		judge.codebegin = quaternary_list.size();
		TOKEN = getnext();
		statement();
	}
	else judge.accept = -2;//������ʾ��û��ִ�й�else
	return judge;
}

//<while��> �� while <�������ʽ> do <���>
back_patch while_sentence()
{
	back_patch judge;
	string tmp = "while";
	if (TOKEN.compare(tmp) != 0) error(15);//����15��while���ȱ�ٱ�����while�������ܵĴ���
	TOKEN = getnext();

	judge = bool_sentence();//ת��<�������ʽ>

	tmp = "do";
	if (TOKEN.compare(tmp) != 0) error(16);//����16��while���ȱ��do�ؼ���
	TOKEN = getnext();

	//��������Ҫ��������
	for (int i = 0; i < judge.true_chain.size(); i++)
	{
		quaternary_list[judge.true_chain[i]][3] = to_string(quaternary_list.size());
	}

	statement();//ת��<���>
	//���о���ִ����һ�飬Ҫ�ٴλص����ӿ�ͷȥ�ж�,����֮�⻹Ҫ�Ѽ���������
	vector<string> temp; temp.push_back("j"); temp.push_back("-"); temp.push_back("-"); temp.push_back(to_string(judge.codebegin));
	quaternary_list.push_back(temp);

	//�������
	for (int i = 0; i < judge.false_chain.size(); i++)
	{
		quaternary_list[judge.false_chain[i]][3] = to_string(quaternary_list.size());
	}

	//����and�������һֱ��û������or��˵������and_or���ͣ���ôֱ�Ӷ�and������ļ������л���
	for (int i = 0; i < judge.and_or_false_chain.size(); i++)
	{
		quaternary_list[judge.and_or_false_chain[i]][3] = to_string(quaternary_list.size());
	}

	return judge;
}

//<repeat��> �� repeat <���> until <�������ʽ>
back_patch repeat_sentence()
{
	string tmp = "repeat";
	if (TOKEN.compare(tmp) != 0) error(17);//����17��repeat���ȱ�ٱ�����repeat�������ܵĴ���
	TOKEN = getnext();

	int temp_codebegin = quaternary_list.size();//�Ȱ�repeat�����Ŀ�ͷ��¼����

	statement();//ת��<���>

	tmp = "until";
	if (TOKEN.compare(tmp) != 0) error(18);//����18��repeat���ȱ�ٱ�����until
	TOKEN = getnext();

	back_patch judge;

	judge = bool_sentence();//ת��<�������ʽ>

	if (judge.accept == -1) error(19);//����19��repeat��䣬until���<�������ʽ>����

	//����repeat until�ṹ��������ʼ���������ͼ���
	for (int i = 0; i < judge.true_chain.size(); i++)
	{
		quaternary_list[judge.true_chain[i]][3] = to_string(quaternary_list.size());
	}

	for (int i = 0; i < judge.false_chain.size(); i++)
	{
		quaternary_list[judge.false_chain[i]][3] = to_string(temp_codebegin);
	}

	//����and�������һֱ��û������or��˵������and_or���ͣ���ôֱ�Ӷ�and������ļ������л���
	for (int i = 0; i < judge.and_or_false_chain.size(); i++)
	{
		quaternary_list[judge.and_or_false_chain[i]][3] = to_string(temp_codebegin);
	}

	return judge;
}

//<��ֵ��> �� <��ʶ��> := <�������ʽ>
void assignment_statement()
{
	//��ֵ����һ���������Ԫʽ�ģ������п��ܲ���������Ԫʽ����˻���Ҫ�������������Ҫ��¼һ�¿�ʼλ��
	int codebegin = quaternary_list.size();

	int judge = identifier();
	if (judge == -1) error(20);//����20����ֵ��û���ҵ���ʶ��,��ʶ��û������������ʲô���Ͷ�ûʶ��
	else if (judge == -2) return;
	//�������һ����ʶ������ôҪ�����������ֵ�ı�ʶ�����±�
	int assigned_bsf = judge;

	string tmp = ":=";
	if (TOKEN.compare(tmp) != 0) error(21);//����21����ֵ��ȱ��:=
	TOKEN = getnext();

	int now_pos = p;//��¼��ǰ��λ�ã���һ��������ԪʽҪ�õ�

	judge = arithmetic_sentence().accept;//ת��<�������ʽ>
	if (judge == -1) error(22);//����22����ֵ����������ʽ����

	var_and_type x = cal_stack.top(); cal_stack.pop();//һ����ֵ��������ջ�Ķ��˴�ŵ�Ӧ���Ǹ�ֵ�������������ʽ��var_and_type
	vector<string> temp;
	temp.push_back(":=");
	temp.push_back(x.data);
	temp.push_back("-");//ռλ��
	temp.push_back(var_list[assigned_bsf].data);
	quaternary_list.push_back(temp);
	cal_stack.push(var_list[assigned_bsf]);

	return;
}

//<�������ʽ> �� <������> bool_sentence_1
back_patch bool_sentence()
{
	back_patch judge;
	judge.codebegin = quaternary_list.size();//��ȡ�������ɵ���Ԫʽ��λ�ã���֪������Ҫ��Ҫ����
	judge = bool_xiang(judge);//ת��<������>
	if (judge.accept != -1) bool_sentence_1(judge);//ת��bool_sentence_1

	return judge;//�������judge���������ػ�����Ԫʽ�����������
}

//bool_sentence_1 �� or <������> bool_sentence_1 | ��
back_patch bool_sentence_1(back_patch &judge)
{
	string tmp = "or";
	if (TOKEN.compare(tmp) == 0)
	{
		//����Ҫ��and_or_false_chain���л��Ȼ�����and_or_false_chain
		for (int i = 0; i < judge.and_or_false_chain.size(); i++)
		{
			quaternary_list[judge.and_or_false_chain[i]][3] = to_string(quaternary_list.size());
		}
		judge.and_or_false_chain.clear();

		//����Ҫע��һ�£�or�ļٳ���Ҫ���һ���������codebeginλ�ã�ֱ�����һ��or���֣�������ļٳ��ڲ��������ļٳ���
		back_patch judge2;//������ʱ�洢ÿ��������ش��������
		TOKEN = getnext();
		judge2 = bool_xiang(judge2);//ת��<������>

		judge.addTrueChain(judge2);//����or�Ժ�Ĳ�������������ȴ�����
		judge.addFalseChain(judge2);//����ǰ�Ĳ������ʽ�е�����Ǹ���Ҫ����ļ���������judge��
		judge.addAndOrFalseChain(judge2);//����and_or_false_chain

		if (judge.accept == -1) return judge;//���bool_xiang��ʶ��ʧ�ܣ���ֱ�ӷ�����
		judge = bool_sentence_1(judge);//ת��......
	}
	return judge;
}

//<������> �� <������> bool_xiang_1
back_patch bool_xiang(back_patch &judge)
{
	judge = bool_factor(judge);
	if (judge.accept == -1) return judge;//˵��<������>ʶ��ʧ�ܣ�ֱ�ӷ���

	judge = bool_xiang_1(judge);

	return judge;
}

//bool_xiang_1 �� and <������> bool_xiang_1 | ��
back_patch bool_xiang_1(back_patch &judge)
{
	string tmp = "and";
	if (TOKEN.compare(tmp) == 0)
	{
		back_patch judge3;
		TOKEN = getnext();
		judge3 = bool_factor(judge3);


		//�������ǿ�ʼ����

		judge.addTrueChain(judge3);//����or�Ժ�Ĳ�������������ȴ�����
		judge.addFalseChain(judge3);//����ǰ�Ĳ������ʽ�е�����Ǹ���Ҫ����ļ���������judge��
		judge.addAndOrFalseChain(judge3);

		if (judge.accept == -1) return judge;//���judgeΪ-1��˵��<������>ʶ��ʧ�ܣ�ֱ�ӷ���
		judge = bool_xiang_1(judge);
	}
	return judge;
}

//<������> �� <������>��not <������>
back_patch bool_factor(back_patch &judge)
{
	string tmp = "not";
	if (TOKEN.compare(tmp) == 0)
	{
		TOKEN = getnext();
		//������������Ҫ������
		judge.is_swap = 1;
		judge = bool_factor(judge);
		//�����ǿ���Ҫ��Ҫ����swapchain�������ֻ��һ��������Ĳ������ʽ���򲻻ᾭ�������ӵĲ��裬�����������ﵥ���жϣ�
		if (judge.true_chain.size() == 1 && judge.false_chain.size() == 1)
		{
			vector<int> temp = judge.true_chain;
			judge.true_chain = judge.false_chain;
			judge.false_chain = temp;
		}
	}
	else
	{
		judge = boolean(judge);
		//����Ҫ������Ԫʽ
	}
	return judge;
}

//<������> �� <��������>��( <�������ʽ> )��<�������ʽ> <��ϵ��> <�������ʽ> | ��ʶ��
back_patch boolean(back_patch &judge)
{
	string tmp1 = "true", tmp2 = "false", tmp3 = "(", tmp4 = "-", tmp5 = ")";
	if (TOKEN.compare(tmp1) == 0) //true
	{
		TOKEN = getnext();
		//���if��while��repeat�����жϾ��Ӵ�ŵ���true����ô�����������������һ��(jump,-,-,-)��Ȼ������ٸ��������������л���
		//�Ҳ��������û�дif true and false������䣬��Ϊû�����壬ֻ����if/while (true)������
		vector<string> tp;
		tp.push_back("j"); tp.push_back("-"); tp.push_back("-"); tp.push_back("����ڴ�����");
		quaternary_list.push_back(tp);

		//�����Ԫʽ��λ��Ҫ��¼һ�£���һ�»�Ҫ���
		judge.true_chain.push_back(quaternary_list.size() - 1);
		var_and_type temp; temp.data = "true";
		//������Ԫʽ֮����Ϊ�����������Ҫ����AND���㣬����Ҫ��ջ
		cal_stack.push(temp);

		//���������ļٳ���Ҳһ�������ˣ�����Ҫ�Ѽٳ��������ȥ
		vector<string> tempp; tempp.push_back("j"); tempp.push_back("-"); tempp.push_back("-"); tempp.push_back("������ļٳ���");
		quaternary_list.push_back(tempp);
		judge.false_chain.push_back(quaternary_list.size() - 1);

		judge.codebegin = quaternary_list.size() + 1;//��Ϊֻ����if/while (true)�����ģ���������ڱ�Ȼ������������Ԫʽ�����ĵط�
		return judge;
	}
	else if (TOKEN.compare(tmp2) == 0) //false
	{
		TOKEN = getnext();
		//���if��while��repeat�����жϾ��Ӵ�ŵ���true����ô�����������������һ��(jump,-,-,-)��Ȼ������ٸ��������������л���
		//�Ҳ��������û�дif true and false������䣬��Ϊû������
		vector<string> tp;
		tp.push_back("j"); tp.push_back("-"); tp.push_back("-"); tp.push_back("�ٳ��ڴ�����");
		quaternary_list.push_back(tp);
		//�����Ԫʽ��λ��Ҫ��¼һ�£���һ�»�Ҫ���
		judge.false_chain.push_back(quaternary_list.size());
		var_and_type temp; temp.data = "false";
		//������Ԫʽ֮����Ϊ�����������Ҫ����AND���㣬����Ҫ��ջ
		cal_stack.push(temp);
		return judge;
	}

	backtrack_pos = p;//��¼���ݵ�
	//����������н����ģ�ֻ��ͨ���������г��ԣ����оͻ���

	//֮������while����Ϊ��Ҫ����judge�Ľ������break����������������������������������������������������������������

	while (TOKEN.compare(tmp3) == 0)//(<�������ʽ>)
	{
		TOKEN = getnext();
		judge = bool_sentence();
		if (judge.accept == -1) break;//���<�������ʽ>ƥ�䲻�ϣ���ô����
		if (TOKEN.compare(tmp5) != 0) error(24);//����24���������еĲ������ʽȱ��������
		TOKEN = getnext();
		return judge;
	}//����Ӧ��Ҫ����һЩ����

	p = backtrack_pos;//��pλ�����õ����ݵ�
	while (1) //<�������ʽ> <��ϵ��> <�������ʽ>
	{
		back_patch temp_judge;
		temp_judge = arithmetic_sentence();
		if (temp_judge.accept == -1) break;//����������ʽƥ�䲻�ϣ���ô����
		judge.accept = relation_character(judge);
		if (judge.accept == -1) break;//�����ϵ��ƥ�䲻�ϣ���ô����
		temp_judge = arithmetic_sentence();
		if (temp_judge.accept == -1) break;//����������ʽƥ�䲻�ϣ���ô����
		//���ڵ�cal_stack���������������ֱ���<��ϵ��>��ߺ��ұߵ�<�������ʽ>
		//�������ǿ��Կ�ʼ������ת�жϵ���Ԫʽ�ˣ�(j<��ϵ��>,�������ʽ1���������ʽ2��������������)
		//��Ϊ���������Ǵ�����ģ���������Ҫ�������뵽judge����������
		var_and_type x = cal_stack.top(); cal_stack.pop();
		var_and_type y = cal_stack.top(); cal_stack.pop();
		vector<string> temp; string tmpp = "j"; tmpp += judge.op;
		temp.push_back(tmpp); temp.push_back(y.data); temp.push_back(x.data); temp.push_back("������������");
		quaternary_list.push_back(temp);

		//�����ǰѸò��������ɵ���Ԫʽ�ӵ��������棬���ǣ�����������������������and
		//��ô���ܼӵ��������棬��Ϊ���ǻ�Ҫ�ж�һ��and����Ĳ������ǲ���Ϊ�棬���Գ���Ӧ����and����Ĳ�����
		if (TOKEN.compare("and") != 0) //�������д������ڵ�����Ԫʽ���е�λ��
		{
			if (TOKEN.compare("or") == 0)//����ڱ���Ӧ�ü��뵽true_chain���棬���������or�Ļ�
										 //�Ͳ�����ô���ˣ�ֱ����ת����һ�����������ʼλ��
			{
				if (judge.is_swap == 0) judge.true_chain.push_back(quaternary_list.size() - 1);
				//not�ıȽ��ر���Ϊ��not�Ĳ������������൱�ڼٳ���
				//������ļٳ�������һ����Ϊor������¾�ֱ����ת����һ��������codebegin��λ��
				else quaternary_list[quaternary_list.size() - 1][3] = to_string(quaternary_list.size() + 1);
			}
			else judge.true_chain.push_back(quaternary_list.size() - 1);
		}
		else//�����and����Ȼ��ֱ����ת����һ��������Ŀ�ʼλ��
		{
			//�������ǻ�Ҫ����һ��not������
			if (judge.is_swap == 0) quaternary_list[quaternary_list.size() - 1][3] = to_string(quaternary_list.size() + 1);
			else judge.true_chain.push_back(quaternary_list.size() - 1);//not����£����������ٳ��ڵߵ���and���������Ȼ��ֱ�Ӽӵ�������
		}

		//���������ļٳ���Ҳһ�������ˣ�����Ҫ�Ѽٳ��������ȥ�����ǣ��������������������ŵ���or
		//��ô���ܼӵ��������棬��Ϊ���ǻ�Ҫ�ж�һ��or����Ĳ������ǲ���Ϊ�٣����Լٳ���Ӧ����or����Ĳ������codebegin
		vector<string> tempp; tempp.push_back("j"); tempp.push_back("-"); tempp.push_back("-"); tempp.push_back("������ļٳ���");
		quaternary_list.push_back(tempp);

		if (TOKEN.compare("or") != 0)//�������or
		{
			//�������ǻ�Ҫ�ж�һ���Ƿ���and
			if (TOKEN.compare("and") == 0)
			{
				//������һ���鷳��������and���滹����һ��or����ô��Ӧ����ת�����or��codebegin��λ��
				//�������Ǹɴ������and������ļٳ���ȫ���ӵ�һ���������棬���ݺ����﷨�����Ƿ���or
				//���ж��Ƿ���Ҫ����Щand������ļٳ�������Ϊ��һ��or��codebegin
				if (judge.is_swap == 0) judge.and_or_false_chain.push_back(quaternary_list.size() - 1);
				else quaternary_list[quaternary_list.size() - 1][3] = to_string(quaternary_list.size());
			}
			else judge.false_chain.push_back(quaternary_list.size() - 1);
		}
		else//����ͱȽ������ˣ��������������or��Ӧ��ȡnextstat����quaternary.size())��Ϊ�ٳ��ڵ�
			//��������и�����������и�not�ڣ��ò�������������ת���ͱ������������ڵ�λ�ã�����ֱ�Ӽ��뵽����������������������
		{
			if (judge.is_swap == 0) quaternary_list[quaternary_list.size() - 1][3] = to_string(quaternary_list.size());
			else judge.false_chain.push_back(quaternary_list.size() - 1);
		}

		judge.codebegin = quaternary_list.size() - 2;

		return judge;
	}

	p = backtrack_pos;//��pλ�����õ����ݵ�
	while (1)
	{
		judge.accept = identifier();
		//���û���ҵ���ʶ���ֻ��������ʶ������bool���͵ı���
		if (judge.accept == -1 || var_list[judge.accept].type != 3) error(26);//�����������޷�ʶ��Ĳ�������
		break;
	}//����Ӧ��Ҫ����һЩ����
	return judge;
}

//<�������ʽ> �� <��> arithmetic_sentence_1
back_patch arithmetic_sentence()
{

	back_patch judge;

	judge.accept = xiang();//ת��<��>
	if (judge.accept == -1) return judge;//���judgeΪ-1��˵��<��>ʶ��ʧ�ܣ�����-1

	judge.accept = arithmetic_sentence_1();//ת��arithmetic_sentence_1
	return judge;
}

//arithmetic_sentence_1 �� + <��>arithmetic_se^_1�� - <��>arithmetic_se^_1 | ��
int arithmetic_sentence_1()
{
	int judge = 0;
	string tmp1 = "+", tmp2 = "-";
	if (TOKEN.compare(tmp1) == 0 || TOKEN.compare(tmp2) == 0)
	{
		//����������ʽʶ�𵽵���+ -�ţ���ô�϶���Ҫ����������,��������϶���Ҫ�浽tempp�е�
		string op = TOKEN;//��������ż�����
		TOKEN = getnext();
		judge = xiang();
		if (judge == -1) return judge;//���judgeΪ-1��˵��<��>ʶ��ʧ�ܣ�����-1

		//��¼��������������ֵ��������������var_and_type�ṹ
		string temp_var_name = "T";//�ȹ������ʱ������
		temp_var_name += to_string(temp_var_num + 1); temp_var_num++;
		var_and_type tempp(temp_var_name, 1);

		//OK��ִ����+ -�Ÿ��ڵĵڶ������Ժ�ջ��һ����������Ԫ�أ����ڰ���ȡ������������Ԫʽ
		//ע�⣬�����Ҳ�����ֱ�Ӽ����ý��������������Ԫʽ����Ҫ�ɵ��£�����ֻ����������Ԫʽ
		var_and_type x = cal_stack.top(); cal_stack.pop();
		var_and_type y = cal_stack.top(); cal_stack.pop();
		vector<string> temp;
		temp.push_back(op);
		temp.push_back(y.data);
		temp.push_back(x.data);
		temp.push_back(tempp.data);
		quaternary_list.push_back(temp);
		cal_stack.push(tempp);//�п����ǣ��������ʽ+ - �������ʽ ����������������ʽҲҪ������

		judge = arithmetic_sentence_1();
	}
	//������ǼӼ�������˵����һ�����ӻ�"-����"��ɵ���Ԫʽ��ջ����Ķ����϶��ǿ���ֱ��������ֵ�ģ����Բ��ù�
	return judge;
}

//����ʽ��һ��
// <��> �� <����> xiang_1
int xiang()
{
	int judge = 0;
	judge = factor();
	if (judge == -1) return judge;//���judgeΪ-1��˵��<����>ʶ��ʧ�ܣ�����-1

	judge = xiang_1();
	return judge;
}

//xiang_1 �� * <����> xiang_1��/ <����> xiang_1| ��
int xiang_1()
{
	int judge = 0;
	string tmp1 = "*", tmp2 = "/";
	if (TOKEN.compare(tmp1) == 0 || TOKEN.compare(tmp2) == 0)
	{
		//���һ�������ɶ��������˳��õ��ģ���ô�϶���Ҫ����������,��������϶���Ҫ�浽tempp�е�
		//��¼�������������ֵ�����ﻹ��Ҫ��������var_and_type�ṹ����Ϊ�����ɶ�����ӳ˳��õ���
		string temp_var_name = "T";//�ȹ������ʱ������
		temp_var_name += to_string(temp_var_num + 1); temp_var_num++;
		var_and_type tempp(temp_var_name, 1);

		string op = TOKEN;//��������ż�����
		TOKEN = getnext();
		judge = factor();
		if (judge == -1) return judge;//���judgeΪ-1��˵��<����>ʶ��ʧ�ܣ�����-1

		//OK��ִ����* /�Ÿ��ڵĵڶ��������Ժ�ջ��һ����������Ԫ�أ����ڰ���ȡ������������Ԫʽ
		//ע�⣬�����Ҳ�����ֱ�Ӽ����ý��������������������ԪʽҪ�ɵ��£�������ֻ����������Ԫʽ
		var_and_type x = cal_stack.top(); cal_stack.pop();//�����ȡ����һ�������ջ�е�����������������������ô�������ݹ����������������������
		var_and_type y = cal_stack.top(); cal_stack.pop();
		vector<string> temp;
		temp.push_back(op);
		temp.push_back(y.data);
		temp.push_back(x.data);
		temp.push_back(tempp.data);
		quaternary_list.push_back(temp);
		cal_stack.push(tempp);//�п����ǣ��������ʽ+ - �������ʽ ����������������ʽҲҪ������

		judge = xiang_1();
	}
	return judge;
}

//<����> �� <������>��- <����>
int factor()
{
	//����Ҳ�п�����һ������������������������������ͻῪʼ�ݹ飬�����Ѿ��Ӵ����ս������
	//���������µ���ʱ���������������ʽ���ɳ����ľͿ����ˣ��������Ҫ��һ����ȡ��������ô�ͱ�������һ����ʱ������


	int judge = 0;
	string tmp = "-";//����Ǹ���
	if (TOKEN.compare(tmp) == 0)
	{
		//����и��ű�����ʱ����һ������
		string temp_var_name = "T";
		temp_var_name += to_string(temp_var_num + 1); temp_var_num++;
		var_and_type tempp(temp_var_name, 1);

		string op = "-";
		//���ȡ����һ����������ôҲҪ������һ����Ԫʽ
		judge = factor();

		//����Ǹ��ţ���ôֻ��Ҫ��������ջ���Ľ��(һ������)ȡ���Ϳ�����
		var_and_type x = cal_stack.top(); cal_stack.pop();
		vector<string> temp;
		temp.push_back("-");
		temp.push_back(x.data);
		temp.push_back("-");//ռλ��
		temp.push_back(tempp.data);
		quaternary_list.push_back(temp);
		cal_stack.push(tempp);//ѹջ
	}
	else//������Ǹ��ţ���ô�ͽ����������ķ�֧����������֧û�б�Ҫ������Ԫʽ������������ѹջ���������
	{
		judge = arithmetic_quantity();
		//����������Ҫ�ģ���Ϊջ��Դͷ���ս�����ս���Ƚ�ջ���к������������֮���
	}
	return judge;
}

//<������> �� <����>��<��ʶ��>���� <�������ʽ> ��
int arithmetic_quantity()
{
	int i, t = -1, judge = 0;

	t = integer();//�����ǲ�������
	if (t != -1) //�����Ҫ����һ��ѹջ����
	{
		//���ڵ����ս���ˣ������һ����ȷ�������������������ڿ�ʼ��ջ
		var_and_type tempp(bsf_zs_zf[t].data, 1);
		cal_stack.push(tempp);//ѹջ
		return judge;
	}

	judge = identifier();//���ǲ������ͱ�ʶ��
	if (judge != -1)
	{
		//ʶ�𵽱�ʶ����Ҫ��ջ
		var_and_type tempp(var_list[judge].data, 1);
		cal_stack.push(tempp);//ѹջ
		return judge;
	}

	string tmp = "(";//���ǲ���(<�������ʽ>)
	if (TOKEN.compare(tmp) == 0)
	{
		TOKEN = getnext();
		arithmetic_sentence();
		tmp = ")";
		if (TOKEN.compare(tmp) != 0) error(23);//����23����������"(<�������ʽ>)"ȱ����������
	}
	else judge = -1;

	return judge;
}

//<����>
int integer()
{
	int i;
	for (i = 1; i <= bsf_zs_zf_num; i++)//���ҿ��ǲ�������
	{
		if (bsf_zs_zf[i].type == 37)
		{
			if (TOKEN.compare(bsf_zs_zf[i].data) == 0) { TOKEN = getnext(); return i; }//����Ӧ��Ҫ��һЩ��¼���ݵĲ���
		}
	}
	return -1;
}

int relation_character(back_patch &temp)
{
	string tmp1 = "<", tmp2 = "<>", tmp3 = "<=", tmp4 = ">=", tmp5 = ">", tmp6 = "=";
	if (TOKEN.compare(tmp1) == 0) { temp.op = TOKEN; TOKEN = getnext(); return 0; }//����Ҫ��¼һ�¹�ϵ��
	if (TOKEN.compare(tmp2) == 0) { temp.op = TOKEN; TOKEN = getnext(); return 0; }//����Ҫ��¼һ�¹�ϵ��
	if (TOKEN.compare(tmp3) == 0) { temp.op = TOKEN; TOKEN = getnext(); return 0; }//����Ҫ��¼һ�¹�ϵ��
	if (TOKEN.compare(tmp4) == 0) { temp.op = TOKEN; TOKEN = getnext(); return 0; }//����Ҫ��¼һ�¹�ϵ��
	if (TOKEN.compare(tmp5) == 0) { temp.op = TOKEN; TOKEN = getnext(); return 0; }//����Ҫ��¼һ�¹�ϵ��
	if (TOKEN.compare(tmp6) == 0) { temp.op = TOKEN; TOKEN = getnext(); return 0; }//����Ҫ��¼һ�¹�ϵ��

	error(25);//����25��"<�������ʽ><��ϵ��><�������ʽ>"�Ĺ�ϵ��ûʶ�����
}

//����ר������ѹ�������������Ծ�ĺ���
int compress(int i)
{
	if (quaternary_list[i][0].compare("j") == 0 && quaternary_list[i][1].compare("-") == 0 &&
		quaternary_list[i][2].compare("-") == 0)
	{
		quaternary_list[i][3] = to_string(compress(stoi(quaternary_list[i][3])));
		return stoi(quaternary_list[i][3]);
	}
	else return i;
}

//��ӡ��Ԫʽ�ĺ���
void print_quaternary()
{
	for (int i = 0; i < quaternary_list.size(); i++)
	{
		compress(i);
		cout << i << " (";
		for (int j = 0; j < quaternary_list[i].size(); j++)
		{
			cout << quaternary_list[i][j];
			if (j < 3) cout << ',';
		}
		cout << ")" << endl;
	}
}

/*���Դʷ���������
int main()
{
	string a = "var  A,B,C:integer;";
	word_analysis(a, 1);
	for (int i = 1; i < num; i++)
	{
		cout << word[i] << " ";
	}
	cout << endl << endl;
	for (int i = 1; i <= bsf_zs_zf_num; i++)
	{
		cout << bsf_zs_zf[i].data << " ";
	}
	cout << endl << endl;

	system("pause");
}
*/
int main()
{
	//char path[100] = "E:\\Desktop\\test.txt";
	char path[100];
	int type = 0;//��¼��������һ���ַ���ʲô����
	int line = 1;//������¼����������
	char l[1000];//��������txt��һ��

	cout << "���������	�༶���ƿ�1��	ѧ�ţ�201635598966\n\n";
	cout << "��������Ҫ���д���������ļ���:";
	cin >> path; cout << endl;
	cout << endl;

	ifstream a(path, ios::in);//���ļ�
	if (!a)
	{
		cout << "���ļ�ʧ��" << endl << endl;
		system("pause");
	}

	cout << "������ĳ�������:" << endl << endl;

	//�ʷ�����
	while (a.getline(l, 1000))//��temp����һ��
	{
		string temp = l;//�Ѷ�����txt��һ��תΪstring
		cout << temp << endl;
		word_analysis(temp, line);
		line++;
	}
	a.close();

	//��ʶ��������
	for (int i = 0; i <= bsf_zs_zf_num; i++)
	{
		if (bsf_zs_zf[i].type == 36)
		{
			bsf[bsf_num] = bsf_zs_zf[i].data;
			bsf_num++;
		}
	}
	line_first_pos.push_back(-1);//Ϊ�˱������vectorԽ�磬����һ�������Ԫ��

	/*cout << "���Ǽ�⵽�����е��ʣ�" << endl;
	for (int i = 0; i < num; i++)
	{
		cout << word[i] << " ";
	}
	cout << "\n\n���Ǽ�⵽�ı�ʶ�����������ַ�������" << endl;
	for (int i = 0; i <= bsf_zs_zf_num; i++)
	{
		cout << bsf_zs_zf[i] << " ";
	}
	cout << endl << endl;*/

	//�ʷ������������
	cout << endl << "���濪ʼ���дʷ�������" << endl << endl;
	for (int i = 1; i <= num; i++)
	{
		//�ȼ��һ���ǲ��Ǳ�ʶ�����������ַ�����
		for (int j = 1; j <= bsf_zs_zf_num; j++)
		{
			if (word[i] == bsf_zs_zf[j].data)
			{
				cout << "(" << bsf_zs_zf[j].type << ',' << j << ")" << " ";
				break;
			}
		}

		for (int k = 1; k < 61; k++)
		{
			if (word[i] == symbol[k]) cout << "(" << k << ',' << "-)" << " ";
		}
		if (i % 5 == 0) cout << endl;
	}
	cout << endl << endl << "��������﷨�����������м���루��Ԫʽ����" << endl << endl;


	//�﷨��������
	grammar_analysis();
	//�����Ԫʽ
	print_quaternary();

	cout << endl;
	system("pause");

}

