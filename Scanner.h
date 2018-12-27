#ifndef Scanner_h
#define Scanner_h
//#include<fstream>
//#include<string>
//#include"AutoMa.h"
#include"define.h"
using namespace std;


class Scanner{
private:
    AutoMa *autoMachine;
    string str;
    char type;//k关键字、i标识符，s字符串，b字符，c常数，p阶符
    ifstream file;
    int point;//每种token相对位置；
    char c;
protected:
    void getString();//读取一个词
    void getType();//获取类型
    void delspace();//去除空格
    void readp();//读取阶符
    void readk();//读取关键字
    public:
    vector<Quaternary> Quat;    //存储四元式的栈（数组）
    vector<POINT> SEM;          //暂存标识符和常数的栈
	vector<SYNBL> MySynbl;		//语义分析及以后会用到的表
	vector<TYPEL> MyTypel;		//类型表
	vector<AINFL> MyAinfl;		//数组表
	vector<RINFL> MyRinfl;		//结构表
	vector<int>	VALL;			//VALL表
    int temporary = 1;          //临时变量计数
	int dimensionalityOfArray = 0;	//数组维数
	int addr_num = 0;				//addr累加计数
	int arry_addr = 1;				//
    Token remember;
    void useForQuaternary();
    void semanticStack();
    void outputQuart(int n);
    void ActinoInRecursive(string n);
    void RState(char n,string s);
	void FindInState(string s);
	void actionForArry(int d,char i);
	void putStructIntoSynbl();
	void actionForStruct();
	void outToFile();

	vector<SYNBL>::iterator findB(vector<SYNBL>::iterator begin, vector<SYNBL>::iterator end, string strr);
    vector<Token> tokenlist;
    vector<string> iden;//标识符表,语法分析及以前会用到的表,可能包含未定义的标识符
    vector<string> key;//关键字
    vector<string> party;//阶符
    vector<string> coun;//常数
    vector<string> sstr;//字符串
    vector<char> charactor;//字符
    vector<Token> OperatorStack;//优先算符栈
    Scanner(int f[], int a, int b, int c);
    ~Scanner();
    bool run();
    Token getToken();//读取一个token
    Token nextWord();
};
#endif /* Scanner_h */
