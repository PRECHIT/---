#include<fstream>
#include<cassert>
#include<sstream>
#include"AutoMa.h"
AutoMa::AutoMa(int m_finalState[], int m_finalSize, int m_initState, int m_stateNum)
	:initState(m_initState), stateNum(m_stateNum)
{    //construction
	preState = m_initState;

	for (int i = 0; i < m_finalSize; i++)
		finalState.insert(m_finalState[i]);
	for (int i = 0; i < m_stateNum; i++) {
		stateTranFun.push_back(new map<char, int>);
	}
}

AutoMa::~AutoMa() {
	for (auto i : stateTranFun)
		delete i;
}

int AutoMa::getPreState() {
	return preState;
}

bool AutoMa::init(const char *filename) {
	//read a file to initalize stateTranFun
	std::ifstream file;
	file.open(filename, std::ios::in);
	assert(file.is_open());
	std::string line;
	std::istringstream sin;
	int count = 0, ps = 0, bs = 0;
	char mc;
	while (!file.eof()) {
		getline(file, line);
		sin.str(line);
		sin >> ps >> mc >> bs;
		stateTranFun[ps]->operator[](mc) = bs;//->insert(std::pair<char, int>(mc, bs));
		ps = 0; bs = 0; count = 0;
		sin.clear();
		//system("pause");
	}
	file.close();
	return true;
}

int AutoMa::insert(char c) {
	//read a char
	if (isFinal()) {
		std::cout << "Auto mechine already over." << std::endl;
		return error;
	}
	char vc = c;
	//std::cout << c << std::endl;
	map<char, int>::iterator iter;
	if (isalpha(c)) vc = 'a';
	if (isdigit(c))
	{
		if (c != '0')
			vc = '1';
		else
			vc = '1';
	}
	iter = stateTranFun[preState]->find(vc);
	if (iter != stateTranFun[preState]->end()) {        //running
		preState = iter->second;
		str += c;
		return success;
	}
	else {
		vc = '$';
		iter = stateTranFun[preState]->find(vc);
		if (iter != stateTranFun[preState]->end()) {
			preState = iter->second;
			if (isFinal()) return backk;
			else {
				str += c;
				return success;
			}
		}
		if (preState != 0 && vc != '$')
		{
			std::cout << "auto mechine : Don't have this transition!" << std::endl;
			return error;
		}
		return error;
	}
}

bool AutoMa::isFinal() const {
	return finalState.count(preState) == 1 ? true : false;
}

bool AutoMa::isInit() const {
	return preState == initState ? true : false;
}

void AutoMa::restart() {
	preState = initState;
	str.clear();
}

std::string AutoMa::getString() const {
	return str;
}

