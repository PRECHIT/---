#include"define.h"
#include<sstream>
////生成活跃信息表
//void Activate_Info(vector<struct Quaternary> &QT, vector<struct SYNBL> &Synbl){
//    map<string,bool> act_temp;  //暂存活跃信息
//    struct Quaternary qt_temp;  //暂存遍历的QT
//    for(int i = 0; i < QT.size();i++){//初始化暂存活跃信息表
//        qt_temp = QT[i];
//        if(qt_temp.operand1.form_name.compare("SYNBL") == 0){
//            struct SYNBL sy_temp_operand1 = Synbl[qt_temp.operand1.location-1];//暂存遍历的SYNBL
//            if(sy_temp_operand1.name[0] == 't'){//为临时变量
//                act_temp.insert(pair<string,bool> (sy_temp_operand1.name,false));
//            }
//            else{
//                act_temp.insert(pair<string,bool> (sy_temp_operand1.name,true));
//            }
//        }
//        if(qt_temp.operand2.form_name.compare("SYNBL") == 0){
//            struct SYNBL sy_temp_operand2 = Synbl[qt_temp.operand2.location-1];
//            if(sy_temp_operand2.name[0] == 't'){//为临时变量
//                act_temp.insert(pair<string,bool> (sy_temp_operand2.name,false));
//            }
//            else{
//                act_temp.insert(pair<string,bool> (sy_temp_operand2.name,true));
//            }
//        }
//        if(qt_temp.var != 0){
//            struct SYNBL sy_temp_var = Synbl[qt_temp.var-1];
//            if(sy_temp_var.name[0] == 't'){//为临时变量
//                act_temp.insert(pair<string,bool> (sy_temp_var.name,false));
//            }
//            else{
//                act_temp.insert(pair<string,bool> (sy_temp_var.name,true));
//            }
//        }
//    }
//    vector<Quaternary>::reverse_iterator it_qt = QT.rbegin();
//    for(;it_qt != QT.rend();++it_qt){
//        if((*it_qt).operand1.form_name.compare("SYNBL") == 0){  //为标识符
//            struct SYNBL sy_temp = Synbl[(*it_qt).operand1.location-1];
//            Synbl[(*it_qt).operand1.location-1].act = act_temp[sy_temp.name];
//            act_temp[sy_temp.name] = true;
//        }
//        if((*it_qt).operand2.form_name.compare("SYNBL") == 0){  //为标识符
//            struct SYNBL sy_temp = Synbl[(*it_qt).operand2.location-1];
//            Synbl[(*it_qt).operand2.location-1].act = act_temp[sy_temp.name];
//            act_temp[sy_temp.name] = true;
//        }
//        if((*it_qt).var != 0){  //非空
//            struct SYNBL sy_temp = Synbl[(*it_qt).var-1];
//            Synbl[(*it_qt).var-1].act = act_temp[sy_temp.name];
//            act_temp[sy_temp.name] = false;
//        }
//    }
//    //for(map<string,bool>::iterator it = act_temp.begin();it!=act_temp.end();it++){
//    //    cout<<it->second;
//    //}
//}
////q
void Queue_Copy(vector<struct Quaternary> &target, vector<struct Quaternary> &QT, int a, int b) {
	for (int i = a; i <= b; i++) {
		target.push_back(QT[i]);
	}
}
//基本快划分
void Block_Divide(vector<struct Quaternary> &QT, vector<vector<struct Quaternary>> &QT_set) {
	vector<struct Quaternary> qt_set_temp;
	queue<int> q_in;
	queue<int> q_out;
	q_in.push(1);
	for (int i = 0; i < QT.size(); i++) {
		if (QT[i].operatorr.compare("if") == 0 || QT[i].operatorr.compare("el") == 0 || QT[i].operatorr.compare("ie") == 0) {
			q_in.push(i + 2);     //入口为转移到的语句
			q_out.push(i + 1);    //出口为转移语句
		}
		//if(QT[i].operatorr.compare("while") == 0 && q_in.front()!=i+1)
		//{
		//    q_in.push(i+1);
		//}
		if (QT[i].operatorr.compare("do") == 0 || QT[i].operatorr.compare("we") == 0) { //QT[i].operatorr.compare("wh") == 0 ||
			q_in.push(i + 2);
			q_out.push(i + 1);
		}
	}
	if (q_out.empty() || q_out.back() != QT.size()) {
		q_out.push(QT.size());
	}
	//else{
	//    q_in.back();
	//}
	while (!q_in.empty() && !q_out.empty()) {
		int a = q_in.front();
		q_in.pop();
		if (q_in.empty() && !q_out.empty()) {
			Queue_Copy(qt_set_temp, QT, a - 1, q_out.front() - 1);
		}
		else if (q_out.front() < q_in.front()) {
			//Copy_Quaternary(QT,qt_set_temp,a,q_out.front);
			Queue_Copy(qt_set_temp, QT, a - 1, q_out.front() - 1);
			//qt_set_temp.assign(QT.begin()+a-1,QT.begin()+q_out.front()-1);
			q_out.pop();
		}
		else {
			//Copy_Quaternary(QT,qt_set_temp,a,q_in.front);
			Queue_Copy(qt_set_temp, QT, a - 1, q_in.front() - 1);
			//qt_set_temp.assign(QT.begin()+a-1,QT.begin()+q_in.front()-1);
			q_in.pop();
		}
		QT_set.push_back(qt_set_temp);
		qt_set_temp.clear();
	}
}
void SYNBL_Get_Data(struct SYNBL &syn, string name, int typ_num, string cat, int addr, bool act) {
	syn.name = name;
	syn.typ_num = typ_num;
	syn.cat = cat;
	syn.addr = addr;
	syn.act = act;
}
void TYPEL_Get_Data(struct TYPEL &typ, char tval, int tpoint) {
	typ.tval = tval;
	typ.tpoint = tpoint;
}
void QT_Get_Data(struct Quaternary &QT, string operatorr, POINT operand1, POINT operand2, int var) {
	QT.operatorr = operatorr;
	QT.operand1 = operand1;
	QT.operand2 = operand2;
	QT.var = var;
}
void QT_OUTPUT(vector<struct Quaternary> &QT, vector<struct SYNBL> &syn_set, vector<int> &CONCL) {
	vector<struct Quaternary>::iterator it;
	for (it = QT.begin(); it != QT.end(); it++) {
		cout << "(" << (*it).operatorr << ",";
		if ((*it).operand1.form_name.compare("NULL") == 0) {
			cout << "_" << ",";
		}
		else {
			if ((((*it).operand1).form_name).compare("SYNBL") == 0) {
				SYNBL syn = syn_set[(*it).operand1.location - 1];
				cout << syn.name << ",";
			}
			else {
				cout << CONCL[(*it).operand1.location - 1] << ",";
			}
		}
		if ((*it).operand2.form_name.compare("NULL") == 0) {
			cout << "_" << ",";
		}
		else {
			if ((((*it).operand2).form_name).compare("SYNBL") == 0) {
				SYNBL syn = syn_set[(*it).operand2.location - 1];
				cout << syn.name << ",";
			}
			else {
				cout << CONCL[(*it).operand2.location - 1] << ",";
			}
		}
		if ((*it).var == 0) {
			cout << "_" << ")" << endl;
		}
		else {
			SYNBL syn = syn_set[(*it).var - 1];
			cout << syn.name << ")" << endl;
		}
	}
}
void SYNBL_OUTPUT(vector<struct SYNBL> syn_set, vector<struct TYPEL> typ_set, vector<int> CONCL) {
	vector<struct SYNBL>::iterator it_syn;
	vector<struct TYPEL>::iterator it_typ;
	vector<int>::iterator it_int;
	cout << "符号表" << endl;
	for (it_syn = syn_set.begin(); it_syn != syn_set.end(); it_syn++) {
		cout << "标识符名字" << "\t" << "类型表指针" << "\t" << "种类" << "\t" << "地址" << "\t" << "活跃状态" << endl;
		cout << (*it_syn).name << "\t";
		if (typ_set[(*it_syn).typ_num - 1].tval == 'i' || typ_set[(*it_syn).typ_num - 1].tval == 'r' ||
			typ_set[(*it_syn).typ_num - 1].tval == 'c' || typ_set[(*it_syn).typ_num - 1].tval == 'b') {
			cout << typ_set[(*it_syn).typ_num].tval << "pt" << "\t";
		}
		else {
			cout << "数组和结构体还没写" << "\t";
		}
		cout << (*it_syn).cat;// << "\t";
		if ((*it_syn).cat.compare("c") == 0) {
			cout << "CONCL," << CONCL[(*it_syn).addr - 1] << "\t";
		}
		else if ((*it_syn).cat.compare("v") == 0) {
			cout << "临时变量区" << "\t";
		}
		cout << (*it_syn).act << endl;
	}
	cout << "类型表" << endl;
	for (it_typ = typ_set.begin(); it_typ != typ_set.end(); it_typ++) {
		cout << "序号" << "\t" << "类码" << "\t" << "位置" << "\t" << endl;
		cout << (*it_typ).tval << "\t" << "NULL" << endl;
	}
	cout << "常数表" << endl;
	for (it_int = CONCL.begin(); it_int != CONCL.end(); it_int++) {
		cout << (*it_int) << endl;
	}
}
void Change_to_Cons(vector<string> str, vector<int> &con) {
	stringstream ss;
	int cons = 0;
	for (int i = 0; i < str.size(); i++) {
		ss.clear();
		ss << str[i];
		ss >> cons;
		con.push_back(cons);
	}
}
