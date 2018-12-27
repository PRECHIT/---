#ifndef AUTOMA_H
#define AUTOMA_H
#include<iostream>
#include<string>
#include<set>
#include<vector>
#include<map>
#define backk -1
#define error 0
#define success 1
using namespace std;
class AutoMa {
private:
	set<int> finalState;
	vector<map<char, int> *> stateTranFun;
	int initState;
	int preState;
	int stateNum;
	std::string str;

public:
	int getPreState();
	AutoMa() { };
	AutoMa(int m_finalState[], int m_finalSize, int m_initState, int m_stateNum);
	~AutoMa();
	bool init(const char *filename);
	int insert(char c);
	bool isFinal() const;
	bool isInit() const;
	void restart();
	std::string getString() const;
};
#endif
