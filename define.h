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
//符号表
struct SYNBL {
	string name;//标识符
	int typ_num; //类型表 TYPEL位置
	string cat; //种类
	int addr;   //地址
	bool act;   //活跃状态
};
struct POINT {
	string form_name;//指向表的名称
	int location;    //指向表的位置
	bool operator==(const POINT &p) {
		if (form_name == p.form_name && location == p.location) {
			return true;
		}
		else {
			return false;
		}
	}
};
//四元式
struct Quaternary {
	string operatorr; //操作符
	POINT operand1;   //操作数   位置
	POINT operand2;
	int var;
};
//token
struct Token {
	string str;
	char type;
	int addr;
};
//类型表
struct TYPEL {
	char tval;  //类码
	int tpoint; //位置
};
//数组
struct AINFL {
	int low;
	int up;
	int ctp_num; //成分类型 TYPEL位置
	int clen;   //成分类型长度
};
//结构表
struct RINFL {
	string id;  //域名
	int off;    //区距
	int tp;     //域成分类型 TYPEL位置
};
//函数表
struct PFINFL {
	int level;  //层次号
	int off;    //区距
	int fn;     //参数个数
	int param;  //参数表    Parameter位置
	int entry;  //入口地址
};
void Block_Divide(vector<struct Quaternary> &QT, vector<vector<struct Quaternary>> &QT_set);
void QT_OUTPUT(vector<struct Quaternary> &QT, vector<struct SYNBL> &syn_set, vector<int> &CONCL);
void Change_to_Cons(vector<string> str, vector<int> &con);
#endif
