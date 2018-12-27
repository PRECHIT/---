#include<string>
#include<fstream>
#include<cassert>
#include<algorithm>
#include <sstream>
#include "Scanner.h"

Scanner::Scanner(int f[], int a, int b, int c){
    autoMachine = new AutoMa(f,a,b,c);
    autoMachine->init("/Users/liutou/Desktop/auto_state.txt");//初始化
    file.open("/Users/liutou/Desktop/source.txt",  std::ios::in);
    assert(file.is_open());
    readp();//填写阶符表
    readk();//填写关键字表
}

Scanner::~Scanner(){
    delete autoMachine;
}

void Scanner::readp(){
    ifstream file;
    file.open("/Users/liutou/Desktop/p.txt");
    assert(file.is_open());
    string line;
    while(getline(file, line))
    {party.push_back(line);}
    file.close();
}
void Scanner::readk(){
    ifstream file;
    file.open("/Users/liutou/Desktop/k.txt");
    assert(file.is_open());
    string line;
    while(getline(file, line))
    {key.push_back(line);}
    file.close();
}

bool Scanner::run(){
    autoMachine->restart();
    delspace();
    char c;
    while(!file.eof()){
        file.get(c);
        //cout << c << ' ';
        switch(autoMachine->insert(c)){
            case success: break;
            case backk: file.seekg(-1, std::ios::cur);break;
            case error: return false;
            default:
                break;
        }
        if(autoMachine->isFinal())
            break;
    }
    getString();
    getType();
    if(file.eof()) return false;
    else return true;
}

void Scanner::getString(){
    this->str = autoMachine->getString();
}

void Scanner::getType(){
    switch(autoMachine->getPreState()){
        case 30:
        {
            auto iter = find(key.begin(), key.end(), str);
            type = 'K';
            if(iter != key.end())
                this->point = int(iter - key.begin());
            else{
                type='I';
				auto iter = find(iden.begin(), iden.end(), str);
				if (iter != iden.end())
					this->point = int(iter - iden.begin());
				else {
					iden.push_back(str);    //向符号表添加内容
					this->point = int(iden.size() - 1);
				}
                break;
            }
            break;
        }
        case 31:{
            type='S';
            auto iter = find(sstr.begin(), sstr.end(), str);
            if(iter != sstr.end())
                this->point = int(iter - sstr.begin());
            else{
                sstr.push_back(str);
                this->point = int(sstr.size() - 1);
            }
            break;
        }
        case 32:
        {
            type='B';
			str.erase(std::remove(str.begin(), str.end(), '\''), str.end());//删除'\''
			char p = str[0];
			charactor.push_back(p);
            auto iter = find(charactor.begin(), charactor.end(), p);
            if(iter != charactor.end())
                this->point = int(iter - charactor.begin());
            else{
				
                this->point = int(charactor.size() - 1);
            }
            break;
        }

        case 33:
        {
            type='C';
            auto iter = find(coun.begin(), coun.end(), str);
            if(iter != coun.end())
                this->point = int(iter - coun.begin());
            else{
                coun.push_back(str);
                this->point = int(coun.size() - 1);
            }
            break;
        }
        case 34:{
            type='P';
            auto iter = find(party.begin(), party.end(), str);
            if(iter != party.end())
                this->point = int(iter - party.begin());
            else
                exit(0);
            break;
        }
    }
}

void Scanner::delspace(){
    if(autoMachine->isInit()){
        char c;
        file.get(c);
        while(isspace(c) && !file.eof()){
            file.get(c);
        }
        file.seekg(-1, std::ios::cur);
    }
}


vector<SYNBL>::iterator Scanner::findB(vector<SYNBL>::iterator begin,vector<SYNBL>::iterator end, string strr){
    vector<SYNBL>::iterator i;
    for(i = begin; i != end; i++)
        if(i->name == strr)
            return i;
    return i;
}

Token Scanner::getToken(){
    return Token{ str, type, point};
}

Token Scanner::nextWord(){
    if(run()){
        tokenlist.push_back(getToken());
        //std::cout << getToken().str << endl;
        return getToken();
    }
    //else if(file.eof());
    else{
        cout << "词法错误";
        exit(0);
    }
}


//lzf
//生成四元式的函数
void Scanner::useForQuaternary()
{
    ostringstream os;
    string myStr;
	if (remember.str != "=")
	{
		os << temporary;
		myStr = 't';
		myStr += os.str();
		temporary++;
		MySynbl.push_back(SYNBL{ myStr,0,"",0,0 });
		os.str("");//清空数据
		//cout << remember.str << endl;
		Quat.push_back(Quaternary{ remember.str, SEM[SEM.size() - 2], SEM[SEM.size() - 1], (int)MySynbl.size() });
		SEM.pop_back();
		SEM.pop_back();
		SEM.push_back(POINT{ "SYNBL",(int)MySynbl.size() });
	}
	else {
		Quat.push_back(Quaternary{ "=", SEM[SEM.size() - 1], POINT{"NULL",0}, SEM[SEM.size()-2].location });
		//cout << SEM[SEM.size() - 2].location;
		SEM.pop_back();
		SEM.pop_back();
	}
    outputQuart((int)Quat.size()-1);
}
//语义栈操作
void Scanner::semanticStack()
{
    remember = OperatorStack[OperatorStack.size()-1];
	//cout << remember.str << endl;
    if(remember.type == 'P')
    {
		if (remember.str == "=" || remember.str == "||" ||
			remember.str == "&&" || remember.str == "==" || remember.str == "!=" ||
			remember.str == "<" || remember.str == ">" || remember.str == "<=" ||
			remember.str == ">=" || remember.str == "-" || remember.str == "+" ||
			remember.str == "*" || remember.str == "/" || remember.str == "%" )
		{
			//cout << remember.str << endl;
			useForQuaternary();
		}
    }
    else
    {
        if(remember.type == 'C')
        {
            SEM.push_back(POINT{"CONCL",(remember.addr +1)});//对应的表名为coun，在Scanner.h中可查
        }
        else if(remember.type == 'I')
        {
			//cout << " 2 ";
			FindInState(remember.str);//检查一下标识符是否在表定义过了
			//cout << remember.str<<" "<< remember.addr<<endl;
            SEM.push_back(POINT{"SYNBL",(remember.addr +1)});//对应的表名为MySynbl，在Scanner.h中可查
        }
        else if(remember.type == 'B')
        {
            SEM.push_back(POINT{"CHAR",(remember.addr +1)});//对应的表名为character，在Scanner.h中可查
        }
    }
}
//输出四元式
void Scanner::outputQuart(int n)
{
    cout<<'('<<Quat[n].operatorr<<" ";
    if(Quat[n].operand1.form_name == "CONCL")
        cout<<coun[Quat[n].operand1.location-1]<<" ";
    else if(Quat[n].operand1.form_name == "SYNBL")
        cout<< MySynbl[Quat[n].operand1.location-1].name<<" ";
    else if(Quat[n].operand1.form_name == "CHAR")
        cout<<'\''<<charactor[Quat[n].operand1.location-1]<< '\'' << " ";
    else if(Quat[n].operand1.form_name == "NULL")
        cout<<'_'<<" ";

    if(Quat[n].operand2.form_name == "CONCL")
        cout<<coun[Quat[n].operand2.location-1]<<" ";
    else if(Quat[n].operand2.form_name == "SYNBL")
        cout<< MySynbl[Quat[n].operand2.location-1].name<<" ";
    else if(Quat[n].operand2.form_name == "CHAR")
        cout<< '\'' << charactor[Quat[n].operand2.location-1]<< '\'' << " ";
    else if(Quat[n].operand2.form_name == "NULL")
        cout<<'_'<<" ";
	//cout << MySynbl[(int)MySynbl.size() - 1].name << MySynbl.size() << Quat[n].var <<endl;
    if(Quat[n].var!=0)
        cout<< MySynbl[Quat[n].var-1].name;
    else
        cout<<'_';

    cout<<')'<<endl;
}
//@IF
void Scanner::ActinoInRecursive(string n)
{
    if(n=="if"||n=="do")
    {
        Quat.push_back(Quaternary{n, SEM[SEM.size()-1], POINT{"NULL",0}, 0});
        SEM.pop_back();
        //temporary++;
    }
    else if(n=="el"||n=="ie"||n=="wh"||n=="we"||n=="co"||n=="br")
    {
        Quat.push_back(Quaternary{n, POINT{"NULL",0}, POINT{"NULL",0}, 0});
    }
	outputQuart((int)Quat.size() - 1);
}
//定义性标识符,更新符号表
void Scanner::RState(char n,string s)
{
	int num;
	string t;
	int ttyp;
	switch (n)
	{
	case 'i':
		num = 4;
		t = "v";
		ttyp = 0;
		break;
	case 'c':
		num = 1;
		t = "v";
		ttyp = 2;
		break;
	default:
		num = 0;
	}

	MySynbl.push_back(SYNBL{s,ttyp,t,addr_num,0});    //向符号表添加内容
		cout << MySynbl[MySynbl.size()-1].name << "-添加成功"<<endl;
	if(ttyp==0||ttyp==2)
		addr_num += num;
}

//应用型标识符,需要看看符号表中是否包含,不包含要返回错误
void Scanner::FindInState(string s)
{
	auto iter = findB(MySynbl.begin(), MySynbl.end(), s);
	if (iter == MySynbl.end())
	{
		cout << "使用时,你还没定义 " << s << " 这个标识符"<<endl;
		system("pause"); exit(1);
	}
}

//@actionForArry
void Scanner::actionForArry(int d,char i)
{
	int num = 0;
	if (i == 'i')num = 4;
	if (i == 'c')num = 1;
	
	MyAinfl.push_back(AINFL{ 0,d-1,i,num });
	MyTypel.push_back(TYPEL{ 'a',(int)MyAinfl.size() - 1 });
	MySynbl[(int)MySynbl.size() - 1].typ_num = (int)MyTypel.size() - 1;

	if (dimensionalityOfArray != 0)
	{
		MyAinfl[(int)MyAinfl.size() - 2].ctp_num = (int)MyTypel.size() - 1;
		MyAinfl[(int)MyAinfl.size() - 2].clen = num*d;
	}
}

//@actionForStruct
void Scanner::actionForStruct()
{
	int num = 0;
	int a;
	int i = 0;
	for (i = (int)MySynbl.size() - 1; MySynbl[i].cat != "t"; i--)
	{
		MySynbl[i].cat = "d";	//种类变为"d"
		switch (MyTypel[MySynbl[i].typ_num].tval)
		{
		case 'i':
			addr_num -= 4;
			MySynbl[i].addr = 4;
			num += 4;
			break;
		case 'c':
			addr_num -= 1;
			MySynbl[i].addr = 1;
			num += 1;
			break;
		case 'a':
			a = MySynbl[(int)MySynbl.size() - 1].addr;
			MySynbl[i].addr = (MyAinfl[a].up + 1) * (MyAinfl[a].clen);
			addr_num -= MySynbl[i].addr;
			num += MySynbl[i].addr;
			break;
		default:
			break;
		}
	}
	MySynbl[i].addr = num; i++; 
	int s = 0;
	for (; i < (int)MySynbl.size(); i++)
	{
		MyRinfl.push_back(RINFL{ MySynbl[i].name,s,MySynbl[i].typ_num });
		switch (MyTypel[MySynbl[i].typ_num].tval)
		{
		case 'i':
			s += 4;
			break;
		case 'c':
			s += 1;
			break;
		case 'a':
			a = MySynbl[(int)MySynbl.size() - 1].addr;
			MySynbl[i].addr = (MyAinfl[a].up + 1) * (MyAinfl[a].clen);
			s += MySynbl[i].addr;
			break;
		default:
			break;
		}
	}
}
void Scanner::putStructIntoSynbl()
{
	MyTypel.push_back(TYPEL{ 'a',(int)MyRinfl.size()});
	MySynbl.push_back(SYNBL{ getToken().str,(int)MyTypel.size() - 1,"t",0,0 });
}
void Scanner::outToFile()
{
	// 写文件
	int i;
	ofstream outFile;
	outFile.open("/Users/liutou/Desktop/SYNBL.csv", ios::trunc); //主表
	outFile << "NAME" << ',' << "TYPE" << ',' << "CAT" << ',' << "ADDR" << endl;
	for (i = 0; i < (int)MySynbl.size(); i++)
		outFile << MySynbl[i].name << ',' << MyTypel[MySynbl[i].typ_num].tval << ',' << MySynbl[i].cat << ',' << MySynbl[i].addr << endl;
	outFile.close();

	outFile.open("/Users/liutou/Desktop/TYPEL.csv", ios::trunc); //类型表
	outFile << "TVAL" << ',' << "TPOINT" << endl;
	for (i = 0; i < (int)MyTypel.size(); i++)
		outFile << MyTypel[i].tval << ',' << MyTypel[i].tpoint << endl;
	outFile.close();

	outFile.open("/Users/liutou/Desktop/AINFL.csv", ios::trunc); // 数组表
	outFile << "LOW" << ',' << "UP" << ',' << "CTP_NUM" << ',' << "CLEN" << endl;
	for (i = 0; i < (int)MyAinfl.size(); i++)
		outFile << MyAinfl[i].low << ',' << MyAinfl[i].up << ',' << MyAinfl[i].ctp_num << ',' << MyAinfl[i].clen << endl;
	outFile.close();

	outFile.open("/Users/liutou/Desktop/RINFL.csv", ios::trunc); // 结构表
	outFile << "ID" << ',' << "OFF" << ',' << "TP" << endl;
	for (i = 0; i < (int)MyRinfl.size(); i++)
		outFile << MyRinfl[i].id << ',' << MyRinfl[i].off << ',' << MyRinfl[i].tp << endl;
	outFile.close();
    
    
    outFile.open("/Users/liutou/Desktop/TOKENLIST.csv", ios::trunc); //主表
    outFile << "NAME" << ',' << "TYPE" << ',' <<"ADDR" << endl;
    for (i = 0; i < (int)tokenlist.size(); i++)
        outFile << tokenlist[i].str<< ',' << tokenlist[i].type<< ',' << tokenlist[i].addr<< endl;
    outFile.close();
}
