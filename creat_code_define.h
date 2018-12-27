#ifndef CREAT_CODE_DEFINE_H
#define CREAT_CODE_DEFINE_H
#include"define.h"
//带有活跃信息的QT
typedef struct if_active {
	POINT name;//名字位置
	bool active;//活跃状态
	bool temp;  //是否为临时变量
}if_active;
typedef struct QT_with_active {
	string operatorr;//操作符
	if_active lop;   //左操作数
	if_active rop;   //右操作数
	if_active result;//结果变量
}QT_with_active;
typedef struct RDL {
	string reg_name;//寄存器名字
	POINT data;     //寄存器存的数据
}RDL;
void Get_Target_Code(vector<QT_with_active> &QT, vector<string> &OBJ, RDL &reg, stack<int> &SEM, vector<SYNBL> &syn_set, vector<int> &cons);//
void QT_Trans(vector<struct Quaternary> &QT, vector<QT_with_active> &QT_new, vector<SYNBL> &syn_set);
void QT_new_output(vector<QT_with_active> &QT_new, vector<SYNBL> &syn_set, vector<int> &cons);
void Activate_Info(vector<QT_with_active> &QT, vector<struct SYNBL> &Synbl);
void Target_Code_if(vector<QT_with_active> QT, QT_with_active qt_temp, vector<string> &OBJ, vector<SYNBL> &syn_set, vector<int> &cons, stack<int> &SEM, RDL &reg);
void Target_Code_do(vector<QT_with_active> QT, QT_with_active qt_temp, vector<string> &OBJ, vector<SYNBL> &syn_set, vector<int> &cons, stack<int> &SEM, RDL &reg);
void Target_Code_Out(vector<string> OBJ);
#endif
