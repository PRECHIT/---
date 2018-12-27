#ifndef DAG_H
#define DAG_H
#include"define.h"
typedef struct VertexNode {
	string w;            //操作符
	POINT M;             //主标记
	vector<POINT> Ai;    //附加标记
	vector<int> Arc;
	//ArcNode *firstarc; 
}VertexNode;

typedef struct GraphAdjList {
	vector<VertexNode> adjlist; //邻接表

}GraphAdjList;
int Search_CONCL(vector<int> CONCL, int num);
int SYNBL_Find_Var_Max(vector<SYNBL> syn_set);
int Search_operand(GraphAdjList DAG, POINT operand);
bool Is_in_Arc_one(vector<int> &Arc, int m);
bool Is_in_Arc_two(vector<int> &Arc, int m, int n);
int Search_two_operand(GraphAdjList DAG, string operatorr, POINT operand1, POINT operand2);
int Search_one_operand(GraphAdjList DAG, string operatorr, POINT operand1);
void Delete_Ai(GraphAdjList &DAG, int num, POINT var);
void CreateDAG(vector<struct Quaternary> &QT, GraphAdjList &DAG, vector<SYNBL> &syn_set, vector<int> &cons);
void RebuildQT(vector<struct Quaternary> &QT, GraphAdjList &DAG, vector<SYNBL> &syn_set);
#endif
