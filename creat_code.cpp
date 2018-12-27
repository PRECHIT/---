#include"creat_code_define.h"
// typedef struct if_active{
//     POINT name;//标识符名
//     bool active;//是否活跃
//     bool temp;  //是否为临时变量
// }if_active;
// typedef struct QT_with_active{
//     string operatorr;//操作符
//     if_active lop;   //左操作数
//     if_active rop;   //右操作数
//     if_active result;//结果变量
// }QT_with_active;
//QT转换为活跃状态和临时变量描述的QT'
void QT_Trans(vector<struct Quaternary> &QT, vector<QT_with_active> &QT_new, vector<SYNBL> &syn_set) {
	QT_with_active qt_new_temp;
	vector<struct Quaternary>::iterator it = QT.begin();
	for (; it != QT.end(); it++) {
		qt_new_temp.operatorr = (*it).operatorr;
		qt_new_temp.lop.name = (*it).operand1;
		qt_new_temp.lop.active = true;
		POINT A = { ((*it).var == 0) ? "NULL" : "SYNBL",(*it).var };
		if ((*it).operand1.form_name.compare("SYNBL") == 0) {
			qt_new_temp.lop.temp = (syn_set[(*it).operand1.location - 1].name[0] == 't') ? true : false;
		}
		else {
			qt_new_temp.lop.temp = false;
		}
		qt_new_temp.rop.name = (*it).operand2;
		qt_new_temp.rop.active = true;
		if ((*it).operand2.form_name.compare("SYNBL") == 0) {
			qt_new_temp.rop.temp = (syn_set[(*it).operand2.location - 1].name[0] == 't') ? true : false;
		}
		else {
			qt_new_temp.rop.temp = false;
		}
		qt_new_temp.result.name = A;
		qt_new_temp.result.active = true;
		if ((*it).var != 0) {
			qt_new_temp.result.temp = (syn_set[(*it).var - 1].name[0] == 't') ? true : false;
		}
		else {
			qt_new_temp.result.temp = false;
		}
		QT_new.push_back(qt_new_temp);
	}
}
//输出验证QT_new
void QT_new_output(vector<QT_with_active> &QT_new, vector<SYNBL> &syn_set, vector<int> &cons) {
	vector<QT_with_active>::iterator it = QT_new.begin();
	for (; it != QT_new.end(); it++) {
		cout << "(" << (*it).operatorr << ",";
		if ((*it).lop.name.form_name.compare("SYNBL") == 0) {
			cout << syn_set[(*it).lop.name.location - 1].name;
			cout << "(" << (*it).lop.active << ")" << ",";
		}
		else if ((*it).lop.name.form_name.compare("CONCL") == 0) {
			cout << cons[(*it).lop.name.location - 1] << ",";
		}
		else {
			cout << "_" << ",";
		}
		if ((*it).rop.name.form_name.compare("SYNBL") == 0) {
			cout << syn_set[(*it).rop.name.location - 1].name;
			cout << "(" << (*it).rop.active << ")" << ",";
		}
		else if ((*it).rop.name.form_name.compare("CONCL") == 0) {
			cout << cons[(*it).rop.name.location - 1] << ",";
		}
		else {
			cout << "_" << ",";
		}
		if ((*it).result.name.form_name == "NULL") {
			cout << "_)" << endl;
		}
		else {
			cout << syn_set[(*it).result.name.location - 1].name;
			cout << "(" << (*it).result.active << ")" << ")" << endl;
		}

	}
	cout << endl;
}
//活跃状态转换
void Activate_Info(vector<QT_with_active> &QT, vector<struct SYNBL> &Synbl) {
	map<string, bool> act_temp;  //暂存活跃信息
	QT_with_active qt_temp;  //暂存遍历的QT
	for (int i = 0; i < (int)QT.size(); i++) {//初始化暂存活跃信息表
		qt_temp = QT[i];
		if (qt_temp.lop.name.form_name.compare("SYNBL") == 0) {
			struct SYNBL sy_temp_operand1 = Synbl[qt_temp.lop.name.location - 1];//暂存遍历的SYNBL
			if (qt_temp.lop.temp) {//为临时变量
				act_temp.insert(pair<string, bool>(sy_temp_operand1.name, false));
			}
			else {
				act_temp.insert(pair<string, bool>(sy_temp_operand1.name, true));
			}
		}
		if (qt_temp.rop.name.form_name.compare("SYNBL") == 0) {
			struct SYNBL sy_temp_operand2 = Synbl[qt_temp.rop.name.location - 1];
			if (qt_temp.rop.temp) {//为临时变量
				act_temp.insert(pair<string, bool>(sy_temp_operand2.name, false));
			}
			else {
				act_temp.insert(pair<string, bool>(sy_temp_operand2.name, true));
			}
		}
		if (qt_temp.result.name.location != 0) {
			struct SYNBL sy_temp_var = Synbl[qt_temp.result.name.location - 1];
			if (qt_temp.result.temp) {//为临时变量
				act_temp.insert(pair<string, bool>(sy_temp_var.name, false));
			}
			else {
				act_temp.insert(pair<string, bool>(sy_temp_var.name, true));
			}
		}
	}
	vector<QT_with_active>::reverse_iterator it_qt = QT.rbegin();
	for (; it_qt != QT.rend(); ++it_qt) {
		if ((*it_qt).lop.name.form_name.compare("SYNBL") == 0) {  //为标识符
			struct SYNBL sy_temp = Synbl[(*it_qt).lop.name.location - 1];
			//Synbl[(*it_qt).lop.name.location-1].act = act_temp[sy_temp.name];
			(*it_qt).lop.active = act_temp[sy_temp.name];
			act_temp[sy_temp.name] = true;
		}
		if ((*it_qt).rop.name.form_name.compare("SYNBL") == 0) {  //为标识符
			struct SYNBL sy_temp = Synbl[(*it_qt).rop.name.location - 1];
			//Synbl[(*it_qt).rop.name.location-1].act = act_temp[sy_temp.name];
			(*it_qt).rop.active = act_temp[sy_temp.name];
			act_temp[sy_temp.name] = true;
		}
		if ((*it_qt).result.name.location != 0) {  //非空
			struct SYNBL sy_temp = Synbl[(*it_qt).result.name.location - 1];
			//Synbl[(*it_qt).result.name.location-1].act = act_temp[sy_temp.name];
			(*it_qt).result.active = act_temp[sy_temp.name];
			act_temp[sy_temp.name] = false;
		}
	}
	//for(map<string,bool>::iterator it = act_temp.begin();it!=act_temp.end();it++){
	//   cout<<it->second;
	//}
}

//在QT_new 查找
bool Find_Active(vector<QT_with_active> QT, POINT A) {
	QT_with_active qt_temp;
	bool act = false;
	for (int i = 0; i < QT.size(); i++) {
		qt_temp = QT[i];
		if (qt_temp.lop.name == A) {
			act = qt_temp.lop.active;
			//break;
		}
		else if (qt_temp.rop.name == A) {
			act = qt_temp.rop.active;
			//break;
		}
		else if (qt_temp.result.name == A) {
			act = qt_temp.result.active;
			//break;
		}
	}
	return act;
}

//生成目标代码
//BACK()反填
void Get_Target_Code(vector<QT_with_active> &QT, vector<string> &OBJ, RDL &reg, stack<int> &SEM, vector<SYNBL> &syn_set, vector<int> &cons) {
	//stack<int> SEM;
	POINT null_point = { "NULL",0 };
	//RDL reg = {.reg_name = "R0",.data = null_point};
	QT_with_active qt_temp;
	string obj_temp;
	for (int i = 0; i < QT.size(); i++) {
		qt_temp = QT[i];
		if (qt_temp.operatorr.compare("if") == 0) {
			Target_Code_if(QT, qt_temp, OBJ, syn_set, cons, SEM, reg);
		}
		else if (qt_temp.operatorr.compare("el") == 0) {
			if (Find_Active(QT, reg.data)) {
				obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
				OBJ.push_back(obj_temp);
			}
			int top1 = SEM.top();
			SEM.pop();
			obj_temp = "JMP\t";
			OBJ.push_back(obj_temp);
			SEM.push(OBJ.size());
			int top2 = SEM.top();
			OBJ[top1 - 1].append(to_string(top2 + 1));
		}
		else if (qt_temp.operatorr.compare("ie") == 0) {
			if (Find_Active(QT, reg.data)) {
				obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
				OBJ.push_back(obj_temp);
			}
			int top = SEM.top();
			SEM.pop();
			OBJ[top - 1].append(to_string(OBJ.size() + 1));
		}
		else if (qt_temp.operatorr.compare("wh") == 0) {
			SEM.push(OBJ.size() + 1);                           ////为0？？？
		}
		else if (qt_temp.operatorr.compare("do") == 0) {
			Target_Code_do(QT, qt_temp, OBJ, syn_set, cons, SEM, reg);
		}
		else if (qt_temp.operatorr.compare("we") == 0) {
			if (Find_Active(QT, reg.data)) {
				obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
				OBJ.push_back(obj_temp);
			}
			reg.data = null_point;
			int top = SEM.top();
			SEM.pop();
			OBJ[top - 1].append(to_string(OBJ.size() + 2));
			top = SEM.top();
			SEM.pop();
			obj_temp = "JMP\t" + to_string(top);
			OBJ.push_back(obj_temp);
		}
		else if (qt_temp.operatorr.compare("=") == 0) {//赋值四元式
			if (reg.data == null_point) { //RDL=0
				if (qt_temp.lop.name.form_name == "SYNBL") {
					obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CONCL") {
					obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CHAR") {

				}
				else {
					cout << "error-赋值四元式1" << endl;
				}
			}
			else if (reg.data == qt_temp.lop.name) { //RDL=B
				if (Find_Active(QT, qt_temp.lop.name)) {
					obj_temp = "ST\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
			}
			else if (!(reg.data == qt_temp.result.name)) { //RDL=D
				if (Find_Active(QT, reg.data)) {
					obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
					OBJ.push_back(obj_temp);
					//
					if (qt_temp.lop.name.form_name == "SYNBL") {
						obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
						OBJ.push_back(obj_temp);
					}
					else if (qt_temp.lop.name.form_name == "CONCL") {
						obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
						OBJ.push_back(obj_temp);
					}
					else if (qt_temp.lop.name.form_name == "CHAR") {

					}
					else {
						cout << "error-赋值四元式4" << endl;
					}
				}
				else {
					if (qt_temp.lop.name.form_name == "SYNBL") {
						obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
						OBJ.push_back(obj_temp);
					}
					else if (qt_temp.lop.name.form_name == "CONCL") {
						obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
						OBJ.push_back(obj_temp);
					}
					else if (qt_temp.lop.name.form_name == "CHAR") {

					}
					else {
						cout << "error-赋值四元式5" << endl;
					}
				}
			}
			else {
				cout << "error-赋值四元式6" << endl;
			}
			reg.data = qt_temp.result.name;
		}
		else {//表达式四元式
			if (reg.data == null_point) { //RDL空 1
				if (qt_temp.lop.name.form_name == "SYNBL") {
					obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CONCL") {
					obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CHAR") {

				}
				else {
					cout << "error-表达式四元式1" << endl;
				}

				if (qt_temp.rop.name.form_name == "SYNBL") {
					obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + syn_set[qt_temp.rop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.rop.name.form_name == "CONCL") {
					obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + to_string(cons[qt_temp.rop.name.location - 1]);
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.rop.name.form_name == "CHAR") {

				}
				else {
					cout << "error-表达式四元式3" << endl;
				}
			}
			else if (reg.data == qt_temp.lop.name) { //RDL存B 2
				if (qt_temp.lop.active) {
					obj_temp = "ST\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				//
				obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + syn_set[qt_temp.rop.name.location - 1].name;
				OBJ.push_back(obj_temp);
				
			}
			else if (reg.data == qt_temp.rop.name &&
				(qt_temp.operatorr == "+" || qt_temp.operatorr == "*" || qt_temp.operatorr == "||" || qt_temp.operatorr == "&&")) { //RDL存C ,w可交换 3
				if (qt_temp.rop.active) {
					obj_temp = "ST\t" + reg.reg_name + "," + syn_set[qt_temp.rop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				//
				if (qt_temp.lop.name.form_name == "SYNBL") {
					obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CONCL") {
					obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CHAR") {

				}
				else {
					cout << "error-表达式四元式3" << endl;
				}
			}
			else { //4
				bool active_d = Find_Active(QT, reg.data);//RDL中存的是否活跃
				if (active_d) {
					obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				//
				if (qt_temp.lop.name.form_name == "SYNBL") {
					obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CONCL") {
					obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.lop.name.form_name == "CHAR") {

				}
				else {
					cout << "error-表达式四元式5" << endl;
				}
				if (qt_temp.rop.name.form_name == "SYNBL") {
					obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + syn_set[qt_temp.rop.name.location - 1].name;
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.rop.name.form_name == "CONCL") {
					obj_temp = qt_temp.operatorr + "\t" + reg.reg_name + "," + to_string(cons[qt_temp.rop.name.location - 1]);
					OBJ.push_back(obj_temp);
				}
				else if (qt_temp.rop.name.form_name == "CHAR") {

				}
				else {
					cout << "error-表达式四元式6" << endl;
				}
			}
			reg.data = qt_temp.result.name; //A 放入RDL
		}
	}
}

void Target_Code_if(vector<QT_with_active> QT, QT_with_active qt_temp, vector<string> &OBJ, vector<SYNBL> &syn_set, vector<int> &cons, stack<int> &SEM, RDL &reg) {
	POINT null_point = { "NULL",0 };
	string obj_temp;
	if (reg.data == null_point) {
		if (qt_temp.lop.name.form_name == "SYNBL") {
			obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
			OBJ.push_back(obj_temp);
		}
		else if (qt_temp.lop.name.form_name == "CONCL") {
			obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
			OBJ.push_back(obj_temp);
		}
		else if (qt_temp.lop.name.form_name == "CHAR") {

		}
		else {
			cout << "error-if" << endl;
		}
		//
		obj_temp = "FJ\t" + reg.reg_name + ",";
		OBJ.push_back(obj_temp);
		SEM.push(OBJ.size());
	}
	else if (reg.data == qt_temp.lop.name) {
		if (Find_Active(QT, qt_temp.lop.name)) {
			obj_temp = "ST\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
			OBJ.push_back(obj_temp);
		}
		obj_temp = "FJ\t" + reg.reg_name + ",";
		OBJ.push_back(obj_temp);
		SEM.push(OBJ.size());
		reg.data = null_point;
	}
	else {
		if (Find_Active(QT, reg.data)) {
			obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
			OBJ.push_back(obj_temp);
		}
		if (qt_temp.lop.name.form_name == "SYNBL") {
			obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
			OBJ.push_back(obj_temp);
		}
		else if (qt_temp.lop.name.form_name == "CONCL") {
			obj_temp = "LD\t" + reg.reg_name + "," + to_string(cons[qt_temp.lop.name.location - 1]);
			OBJ.push_back(obj_temp);
		}
		else if (qt_temp.lop.name.form_name == "CHAR") {

		}
		else {
			cout << "error-if2" << endl;
		}
		obj_temp = "FJ\t" + reg.reg_name + ",";
		OBJ.push_back(obj_temp);
		SEM.push(OBJ.size());
		reg.data = null_point;
	}
}

void Target_Code_do(vector<QT_with_active> QT, QT_with_active qt_temp, vector<string> &OBJ, vector<SYNBL> &syn_set, vector<int> &cons, stack<int> &SEM, RDL &reg) {
	POINT null_point = { "NULL",0 };
	string obj_temp;
	if (reg.data == null_point) {
		obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
		OBJ.push_back(obj_temp);
		obj_temp = "FJ\t" + reg.reg_name + ",";
		OBJ.push_back(obj_temp);
		SEM.push(OBJ.size());
	}
	else if (reg.data == qt_temp.lop.name) {
		if (Find_Active(QT, qt_temp.lop.name)) {
			obj_temp = "ST\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
			OBJ.push_back(obj_temp);
		}
		obj_temp = "FJ\t" + reg.reg_name + ",";
		OBJ.push_back(obj_temp);
		SEM.push(OBJ.size());
		reg.data = null_point;
	}
	else {
		if (Find_Active(QT, reg.data)) {
			obj_temp = "ST\t" + reg.reg_name + "," + syn_set[reg.data.location - 1].name;
			OBJ.push_back(obj_temp);
		}
		obj_temp = "LD\t" + reg.reg_name + "," + syn_set[qt_temp.lop.name.location - 1].name;
		obj_temp = "FJ\t" + reg.reg_name + ",";
		OBJ.push_back(obj_temp);
		SEM.push(OBJ.size());
		reg.data = null_point;
	}
}

void Target_Code_Out(vector<string> OBJ) {
	string s;
	for (int i = 0; i < OBJ.size(); i++) {
		s = OBJ[i];
		if (s[0] == '+') {
			s.replace(0, 1, "ADD");
		}
		else if (s[0] == '-') {
			s.replace(0, 1, "SUB");
		}
		else if (s[0] == '*') {
			s.replace(0, 1, "MUL");
		}
		else if (s[0] == '/') {
			s.replace(0, 1, "DIV");
		}
		else if (s[0] == '>') {
			if (s[1] == '=')
				s.replace(0, 2, "GTE");
			else
				s.replace(0, 1, "GT");
		}
		else if (s[0] == '<') {
			if (s[1] == '=')
				s.replace(0, 2, "LTE");
			else
				s.replace(0, 1, "LT");
		}
		else if (s.substr(0, 2) == "==") {
			s.replace(0, 2, "EQU");
		}
		else if (s.substr(0, 2) == "!=") {
			s.replace(0, 2, "UEQ");
		}
		else if (s.substr(0, 2) == "||") {
			s.replace(0, 2, "OR");
		}
		else if (s.substr(0, 2) == "&&") {
			s.replace(0, 2, "AND");
		}
		cout << s << endl;
	}
}
