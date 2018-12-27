#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include"Syntax.h"
#include"creat_code_define.h"
#include"DAG.h"
int main() {
	Syntax *sd = new Syntax();
	sd->sc->MyTypel.push_back(TYPEL{ 'i',0 });
	sd->sc->MyTypel.push_back(TYPEL{ 'r',0 });
	sd->sc->MyTypel.push_back(TYPEL{ 'c',0 });
	sd->sc->MyTypel.push_back(TYPEL{ 'b',0 });
	sd->mainFun();
	//cout<<sd->sc->MySynbl.size();
	sd->sc->outToFile();	//将符号表信息存入Excel
	vector<vector<struct Quaternary>> QT_set;
	Block_Divide(sd->sc->Quat, QT_set);
	vector<struct Quaternary> qt_temp;
	vector<vector<struct Quaternary>>::iterator it_set;
	vector<string> OBJ;
	struct POINT null_point = { "NULL",0 };
	RDL reg = { "R0",null_point };
	stack<int> SEM;
	vector<int> CONCL;
	Change_to_Cons(sd->sc->coun, CONCL);
	for (it_set = QT_set.begin(); it_set != QT_set.end(); it_set++) {
		qt_temp = *it_set;
		vector<QT_with_active> qt_set_new;//用于生成代码的四元式
		cout << endl;
		QT_OUTPUT(qt_temp, sd->sc->MySynbl, CONCL);
		//Activate_Info(qt_temp,syn_set);
		cout << endl;
		//SYNBL_OUTPUT(syn_set,typ_set,CONCL);
		GraphAdjList dag;
		CreateDAG(qt_temp, dag, sd->sc->MySynbl, CONCL);
		RebuildQT(qt_temp, dag, sd->sc->MySynbl);
		QT_OUTPUT(qt_temp, sd->sc->MySynbl, CONCL);
		cout << endl;
		QT_Trans(qt_temp, qt_set_new, sd->sc->MySynbl);
		//QT_new_output(qt_set_new,syn_set,CONCL);
		Activate_Info(qt_set_new, sd->sc->MySynbl);
		QT_new_output(qt_set_new, sd->sc->MySynbl, CONCL);
		Get_Target_Code(qt_set_new, OBJ, reg, SEM, sd->sc->MySynbl, CONCL);
		Target_Code_Out(OBJ);
	}
	return 0;
}
