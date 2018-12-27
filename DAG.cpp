#include"DAG.h"
//查找是否在CONCL中，返回位置
int Search_CONCL(vector<int> CONCL, int num) {
	for (int i = 0; i < CONCL.size(); i++) {
		if (CONCL[i] == num) {
			return i + 1;
		}
	}
	return 0;
}
//查找临时变量最大的数值+1
int SYNBL_Find_Var_Max(vector<SYNBL> syn_set) {
	vector<SYNBL>::iterator it = syn_set.begin();
	int max = 0;
	for (; it != syn_set.end(); it++) {
		if ((*it).name[0] == 't') {
			max++;
		}
	}
	return max + 1;
}
//查找操作数是否在DAG中,返回操作数的序号
int Search_operand(GraphAdjList DAG, POINT operand) {
	VertexNode ver;
	for (int i = DAG.adjlist.size() - 1; i >= 0; i--) {
		ver = DAG.adjlist[i];
		if (ver.M == operand) {
			return i + 1; //查找需要-1
		}
		for (int j = 0; j < ver.Ai.size(); j++) {
			if (ver.Ai[j] == operand) {
				return i + 1;
			}
		}
	}
	return 0;
}
//除了n之外的都查找
int Search_operand_to_delete(GraphAdjList DAG, POINT operand, int n) {
	VertexNode ver;
	for (int i = DAG.adjlist.size() - 1; i >= 0; i--) {
		if (i == n) { continue; }
		ver = DAG.adjlist[i];
		if (ver.M == operand) {
			return i + 1; //查找需要-1
		}
		for (int j = 0; j < ver.Ai.size(); j++) {
			if (ver.Ai[j] == operand) {
				return i + 1;
			}
		}
	}
	return 0;
}
bool Is_in_Arc_two(vector<int> &Arc, int m, int n) {
	int flag = 0;
	for (int i = 0; i < Arc.size(); i++) {
		if (Arc[i] == m || Arc[i] == n) {
			flag++;
		}
	}
	if (flag == 2)
		return true;
	else
		return false;
}
bool Is_in_Arc_one(vector<int> &Arc, int m) {
	int flag = 0;
	for (int i = 0; i < Arc.size(); i++) {
		if (Arc[i] == m) {
			flag++;
		}
	}
	if (flag == 1)
		return true;
	else
		return false;
}
int Search_two_operand(GraphAdjList DAG, string operatorr, POINT operand1, POINT operand2) {//返回存操作数的结点位置
	//int DAG_loc_A = 0;//DAG中A的位置i
	int DAG_loc_B = 0;//DAG中B的位置i 从1开始
	int DAG_loc_C = 0;//DAG中C的位置i
	int i = 0;//指示迭代器当前位置
	vector<POINT>::iterator it_B;
	vector<POINT>::iterator it_C;
	vector<VertexNode>::iterator it = DAG.adjlist.begin();
	vector<VertexNode>::reverse_iterator it_r = DAG.adjlist.rbegin();
	for (; it != DAG.adjlist.end(); it++) {
		it_B = find((*it).Ai.begin(), (*it).Ai.end(), operand1);
		it_C = find((*it).Ai.begin(), (*it).Ai.end(), operand2);
		if (it_B != (*it).Ai.end()) {
			DAG_loc_B = i + 1;
		}
		if (it_C != (*it).Ai.end()) {
			DAG_loc_C = i + 1;
		}
		if (operand1 == (*it).M) {
			DAG_loc_B = i + 1;
		}
		if (operand2 == (*it).M) {
			DAG_loc_C = i + 1;
		}
		i++;
	}
	i = 0;
	if (DAG_loc_B != 0 && DAG_loc_C != 0) {
		for (it_r = DAG.adjlist.rbegin(); it_r != DAG.adjlist.rend(); it_r++) { //逆序查找
			//if(!(*it_r).Arc.empty())
			//   cout<<DAG.adjlist[(*it_r).Arc[0] - 1].M.location<<" "<<DAG.adjlist[(*it_r).Arc[1] - 1].M.location<<endl;
			if (Is_in_Arc_two((*it_r).Arc, DAG_loc_B, DAG_loc_C) && (*it_r).w.compare(operatorr) == 0) {
				return DAG.adjlist.size() - i;
			}
			i++;
		}
	}
	return 0;
}
int Search_one_operand(GraphAdjList DAG, string operatorr, POINT operand1) {
	//int DAG_loc_A = 0;//DAG中A的位置i
	if (operatorr.compare("+") == 0 || operatorr.compare("-") == 0 ||
		operatorr.compare("*") == 0 || operatorr.compare("/") == 0) {
		return 0;
	}
	int DAG_loc_B = 0;//DAG中B的位置i 从1开始
	int i = 0;//指示迭代器当前位置
	vector<POINT>::iterator it_B;
	vector<VertexNode>::reverse_iterator it = DAG.adjlist.rbegin();
	for (; it != DAG.adjlist.rend(); it++) {
		it_B = find((*it).Ai.begin(), (*it).Ai.end(), operand1);
		if (it_B != (*it).Ai.end()) {
			DAG_loc_B = i + 1;
			break;
		}
		if (operand1 == (*it).M) {
			DAG_loc_B = i + 1;
		}
		i++;
	}
	i = 0;
	if (DAG_loc_B != 0) {
		for (it = DAG.adjlist.rbegin(); it != DAG.adjlist.rend(); it++) {
			if (Is_in_Arc_one((*it).Arc, DAG_loc_B) && (*it).w.compare(operatorr) == 0) {
				return DAG.adjlist.size() - i;
			}
			i++;
		}
	}
	return 0;
}
//删除Ai中重新被赋值的变量
void Delete_Ai(GraphAdjList &DAG, int num, POINT var) {
	vector<POINT>::iterator it = DAG.adjlist[num].Ai.begin();
	for (; it != DAG.adjlist[num].Ai.end(); it++) {
		//cout<<DAG.adjlist[num].Ai[0].location;
		if ((*it).location == var.location) {
			DAG.adjlist[num].Ai.erase(it);
			break;
		}
	}
}

void CreateDAG(vector<struct Quaternary> &QT, GraphAdjList &DAG, vector<SYNBL> &syn_set, vector<int> &cons) {
	struct Quaternary qt_temp;
	for (int i = 0; i < QT.size(); i++) {
		qt_temp = QT[i];
		if (qt_temp.operatorr.compare("if") == 0 || qt_temp.operatorr.compare("el") == 0 ||
			qt_temp.operatorr.compare("ie") == 0 || qt_temp.operatorr.compare("wh") == 0 ||
			qt_temp.operatorr.compare("do") == 0 || qt_temp.operatorr.compare("we") == 0) {
			VertexNode ver;
			ver.w = qt_temp.operatorr;
			if (qt_temp.operand1.form_name.compare("NULL") != 0) {//有操作数
				ver.M = qt_temp.operand1;
			}
			else {
				ver.M = { ver.M.form_name = "NULL",ver.M.location = 0 };
			}
			DAG.adjlist.push_back(ver);
		}
		else {
			if (qt_temp.operatorr.compare("=") == 0 && qt_temp.operand1.form_name.compare("SYNBL") == 0 &&
				qt_temp.var != 0) {//赋值四元式A=B
				int num_B = Search_operand(DAG, qt_temp.operand1) - 1;//B的位置
				POINT A = { "SYNBL",qt_temp.var };
				//DAG.adjlist[num_B].Ai.push_back(A); //把A附在B上
				if (num_B < 0) {//B不存在,新建结点
					VertexNode ver; 
					ver.w = "NULL";
					ver.M = qt_temp.operand1;
					DAG.adjlist.push_back(ver);
					num_B = DAG.adjlist.size() - 1;
				}
				if (DAG.adjlist[num_B].M.form_name.compare("SYNBL") == 0 &&
					syn_set[DAG.adjlist[num_B].M.location - 1].name[0] == 't' && syn_set[A.location - 1].name[0] != 't') {//替换临时变量
					DAG.adjlist[num_B].Ai.push_back(DAG.adjlist[num_B].M);
					DAG.adjlist[num_B].M = A;
				}
				else {
					DAG.adjlist[num_B].Ai.push_back(A);
				}
				if (Search_operand_to_delete(DAG, A, num_B)) {  //A已定义过且不是主标记
					int num_A = Search_operand_to_delete(DAG, A, num_B) - 1;
					if (num_A >= 0 && num_A != num_B) {
						if (!(DAG.adjlist[num_A].M == A))
							//cout<<A.location<<endl;
							Delete_Ai(DAG, num_A, A);
					}
				}
			}
			else if ((qt_temp.operand1.form_name.compare("CONCL") == 0 &&
				qt_temp.operand2.form_name.compare("CONCL") == 0) ||
				(qt_temp.operand1.form_name.compare("CONCL") == 0 &&
					qt_temp.operatorr.compare("=") == 0)) {//常值表达式 A
				int result;
				if (qt_temp.operatorr == "=") {
					result = cons[qt_temp.operand1.location - 1];
					//cout<<result<<endl;
				}
				else {
					int operand1 = cons[qt_temp.operand1.location - 1];
					int operand2 = cons[qt_temp.operand2.location - 1];
					if (qt_temp.operatorr == "+") {
						result = operand1 + operand2;
					}
					else if (qt_temp.operatorr == "-") {
						result = operand1 - operand2;
					}
					else if (qt_temp.operatorr == "*") {
						result = operand1 * operand2;
					}
					else if (qt_temp.operatorr == "/") {
						result = operand1 / operand2;
					}
					else if (qt_temp.operatorr == ">") {
						result = operand1 > operand2 ? 1 : 0;
					}
					else if(qt_temp.operatorr == "%"){
						result = operand1 % operand2;
					}
					// ...
				}
				int loc = Search_CONCL(cons, result);
				POINT C;
				if (loc) {//CONCL中有则直接查找，没有则添加
					C.form_name = "CONCL"; 
					C.location = loc;
				}
				else {
					cons.push_back(result);
					C.form_name = "CONCL";
					C.location = cons.size();
				}
				POINT A = {"SYNBL",qt_temp.var };
				int new_node = 0;
				if (Search_operand(DAG, C)) {//result已定义过，A放入已定义处的Ai
					int adj_loc = Search_operand(DAG, C) - 1;
					DAG.adjlist[adj_loc].Ai.push_back(A);
					new_node = adj_loc;
				}
				else {//未定义过，新建结点
					VertexNode ver;
					ver.w = "="; 
					ver.M = C;
					ver.Ai.push_back(A);
					DAG.adjlist.push_back(ver);
					new_node = DAG.adjlist.size() - 1;
					//cout<<ver.w<<","<<ver.M.location<<","<<ver.Ai[0].location<<endl;
				}
				if (Search_operand_to_delete(DAG, A, new_node)) {  //A已定义过且不是主标记
					int num_A = Search_operand_to_delete(DAG, A, new_node) - 1;
					if (num_A >= 0 && num_A != new_node) {
						if (!(DAG.adjlist[num_A].M == A))
							//cout<<A.location<<endl;
							Delete_Ai(DAG, num_A, A);
					}
				}
			}
			else {//其他表达式
				int new_node = 0;
				VertexNode ver;
				POINT A = { "SYNBL",qt_temp.var };
				if (!Search_operand(DAG, qt_temp.operand1)) {//无BC结点则新建
					ver.w = "NULL"; 
					ver.M = qt_temp.operand1;
					DAG.adjlist.push_back(ver);
				}
				if (!Search_operand(DAG, qt_temp.operand2)) {
					ver.w = "NULL"; 
					ver.M = qt_temp.operand2;
					DAG.adjlist.push_back(ver);
				}
				if (Search_two_operand(DAG, qt_temp.operatorr, qt_temp.operand1, qt_temp.operand2) || Search_one_operand(DAG, qt_temp.operatorr, qt_temp.operand1)) {//分开
					int adj_loc;
					if (Search_two_operand(DAG, qt_temp.operatorr, qt_temp.operand1, qt_temp.operand2))
					{
						adj_loc = Search_two_operand(DAG, qt_temp.operatorr, qt_temp.operand1, qt_temp.operand2) - 1;
					}
					else
					{
						adj_loc = Search_one_operand(DAG, qt_temp.operatorr, qt_temp.operand1) - 1;
					}
					//POINT A = {.form_name = "SYNBL",.location = qt_temp.var};
					//DAG.adjlist[adj_loc].Ai.push_back(A);
					if (DAG.adjlist[adj_loc].M.form_name.compare("SYNBL") == 0 &&
						syn_set[DAG.adjlist[adj_loc].M.location - 1].name[0] == 't' && syn_set[A.location - 1].name[0] != 't') {//替换临时变量
						DAG.adjlist[adj_loc].Ai.push_back(DAG.adjlist[adj_loc].M);
						DAG.adjlist[adj_loc].M = A;
					}
					else {
						DAG.adjlist[adj_loc].Ai.push_back(A);
					}
					new_node = adj_loc;
				}
				else {//申请新结点A,BC已有
					VertexNode ver_A;
					//VertexNode ver_B;
					//VertexNode ver_C;
					//int ni = DAG.adjlist.size();
					int num_B;
					int num_C;
					//POINT A = {.form_name = "SYNBL",.location = qt_temp.var};
					ver_A = { ver_A.w = qt_temp.operatorr,ver_A.M = A };
					if (qt_temp.operand2.form_name == "NULL") {//A=wB
						//ver_B = {.w = "NULL",.M = qt_temp.operand1};
						//DAG.adjlist.push_back(ver_B);
						//ver_A.Arc.push_back(ni);
						num_B = Search_operand(DAG, qt_temp.operand1);
						ver_A.Arc.push_back(num_B);
					}
					else {//A=BwC
						//ver_B = {.w = "NULL",.M = qt_temp.operand1};
						//ver_C = {.w = "NULL",.M = qt_temp.operand2};
						//DAG.adjlist.push_back(ver_B);
						//DAG.adjlist.push_back(ver_C);
						num_B = Search_operand(DAG, qt_temp.operand1);
						num_C = Search_operand(DAG, qt_temp.operand2);
						ver_A.Arc.push_back(num_B);
						ver_A.Arc.push_back(num_C);
					}
					//if(!ver_A.Arc.empty())
					//    {cout<<ver_A.Arc[0]<<ver_A.Arc[1]<<endl;}
					DAG.adjlist.push_back(ver_A);
					new_node = DAG.adjlist.size() - 1;
					//cout<<DAG.adjlist[new_node].w<<DAG.adjlist[new_node].Arc[0]<<DAG.adjlist[new_node].Arc[1]<<new_node<<endl;
				}
				//POINT A = {.form_name = "SYNBL",.location = qt_temp.var};
				if (Search_operand_to_delete(DAG, A, new_node)) {  //A已定义过且不是主标记
					int num_B = Search_operand_to_delete(DAG, A, new_node) - 1;
					if (num_B >= 0 && num_B != new_node) {
						if (!(DAG.adjlist[num_B].M == A))
							//cout<<A.location<<endl;
							Delete_Ai(DAG, num_B, A);
					}
				}
			}
			//调整主标记 附加标记顺序
			//每次A附加时判断
		}
	}
}
void RebuildQT(vector<struct Quaternary> &QT, GraphAdjList &DAG, vector<SYNBL> &syn_set) {
	struct Quaternary qt_temp;
	QT.clear();
	POINT null_point = { "NULL",0 };
	vector<VertexNode>::iterator it_ver = DAG.adjlist.begin();
	for (; it_ver != DAG.adjlist.end(); it_ver++) {
		if (!(*it_ver).Ai.empty()) {//有附加标记
			if ((*it_ver).Arc.empty()) {//叶结点
				vector<POINT>::iterator it_point = (*it_ver).Ai.begin();
				for (; it_point != (*it_ver).Ai.end(); it_point++) {//生成Ai = B
					//cout<<(*it_point).location<<endl;
					if (syn_set[(*it_point).location - 1].name[0] != 't')//非临时变量
					{
						qt_temp = { qt_temp.operatorr = "=",
								qt_temp.operand1 = (*it_ver).M,
								qt_temp.operand2 = null_point,
								qt_temp.var = (*it_point).location };
						QT.push_back(qt_temp);
					}
				}
			}
			else {//非叶结点
				vector<POINT>::iterator it_point = (*it_ver).Ai.begin();
				if ((*it_ver).Arc.size() == 2) {
					int operand1 = (*it_ver).Arc[0];
					int operand2 = (*it_ver).Arc[1];
					qt_temp = { qt_temp.operatorr = (*it_ver).w,
								qt_temp.operand1 = DAG.adjlist[operand1 - 1].M,
								qt_temp.operand2 = DAG.adjlist[operand2 - 1].M,
								qt_temp.var = (*it_ver).M.location };
					QT.push_back(qt_temp);
				}
				else {
					int operand = (*it_ver).Arc[0];
					qt_temp = { qt_temp.operatorr = (*it_ver).w,
								qt_temp.operand1 = DAG.adjlist[operand - 1].M,
								qt_temp.operand2 = null_point,
								qt_temp.var = (*it_ver).M.location };
					QT.push_back(qt_temp);
				}
				for (; it_point != (*it_ver).Ai.end(); it_point++) {//生成Ai = A
					if (syn_set[(*it_point).location - 1].name[0] != 't') {//非临时变量
						qt_temp = { qt_temp.operatorr = "=",
								qt_temp.operand1 = (*it_ver).M,
								qt_temp.operand2 = null_point,
								qt_temp.var = (*it_point).location };
						QT.push_back(qt_temp);
					}

				}
			}
		}
		else {//没有附加标志
			if ((*it_ver).w.compare("if") == 0 || (*it_ver).w.compare("el") == 0 ||
				(*it_ver).w.compare("ie") == 0 || (*it_ver).w.compare("wh") == 0 ||
				(*it_ver).w.compare("do") == 0 || (*it_ver).w.compare("we") == 0) {
				if ((*it_ver).M.form_name.compare("NULL") == 0) {
					qt_temp = { qt_temp.operatorr = (*it_ver).w,
								qt_temp.operand1 = null_point,
								qt_temp.operand2 = null_point,
								qt_temp.var = 0
					};
				}
				else {
					qt_temp = { qt_temp.operatorr = (*it_ver).w,
								qt_temp.operand1 = (*it_ver).M,
								qt_temp.operand2 = null_point,
								qt_temp.var = 0
					};
				}
				QT.push_back(qt_temp);
			}
			else if (!(*it_ver).Arc.empty()) {//不为叶结点
				if ((*it_ver).Arc.size() == 2) {
					qt_temp = { qt_temp.operatorr = (*it_ver).w,
							qt_temp.operand1 = DAG.adjlist[(*it_ver).Arc[0] - 1].M,
							qt_temp.operand2 = DAG.adjlist[(*it_ver).Arc[1] - 1].M,
							qt_temp.var = (*it_ver).M.location
					};
					QT.push_back(qt_temp);
				}
				else {
					//cout<<(*it_ver).Arc.size()<<endl;
					qt_temp = { qt_temp.operatorr = (*it_ver).w,
							qt_temp.operand1 = DAG.adjlist[(*it_ver).Arc[0] - 1].M,
							qt_temp.operand2 = null_point,
							qt_temp.var = (*it_ver).M.location
					};
					QT.push_back(qt_temp);
				}
			}

		}
	}
}
