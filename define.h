#ifndef DEFINE_H
#define DEFINE_H
#include<iostream>
#include<fstream>
#include<cstdlib>
#include <sstream>
#include<string>
#include<vector>
#include<queue>
#include<stack>
#include<map>
#include"AutoMa.h"
//using std::string;
//using std::vector;
using namespace std;
//���ű�
struct SYNBL {
	string name;//��ʶ��
	int typ_num; //���ͱ� TYPELλ��
	string cat; //����
	int addr;   //��ַ
	bool act;   //��Ծ״̬
};
struct POINT {
	string form_name;//ָ��������
	int location;    //ָ����λ��
	bool operator==(const POINT &p) {
		if (form_name == p.form_name && location == p.location) {
			return true;
		}
		else {
			return false;
		}
	}
};
//��Ԫʽ
struct Quaternary {
	string operatorr; //������
	POINT operand1;   //������   λ��
	POINT operand2;
	int var;
};
//token
struct Token {
	string str;
	char type;
	int addr;
};
//���ͱ�
struct TYPEL {
	char tval;  //����
	int tpoint; //λ��
};
//����
struct AINFL {
	int low;
	int up;
	int ctp_num; //�ɷ����� TYPELλ��
	int clen;   //�ɷ����ͳ���
};
//�ṹ��
struct RINFL {
	string id;  //����
	int off;    //����
	int tp;     //��ɷ����� TYPELλ��
};
//������
struct PFINFL {
	int level;  //��κ�
	int off;    //����
	int fn;     //��������
	int param;  //������    Parameterλ��
	int entry;  //��ڵ�ַ
};
void Block_Divide(vector<struct Quaternary> &QT, vector<vector<struct Quaternary>> &QT_set);
void QT_OUTPUT(vector<struct Quaternary> &QT, vector<struct SYNBL> &syn_set, vector<int> &CONCL);
void Change_to_Cons(vector<string> str, vector<int> &con);
#endif
