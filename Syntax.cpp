//#include<cstdlib>
//#include <sstream>
#include "Syntax.h"

Syntax::Syntax()
{
    int finals[]= {30,31,32,33,34};
    sc=new Scanner(finals,5,1,29);
}
Syntax::~Syntax()
{
    delete sc;
}

//算符优先
int Syntax::switchCode(Token temp)
{
    if(temp.type=='I')
    {
        return 14;
    }
    else if(temp.type=='C'||temp.type=='B')
    {
        return 15;
    }
    else
    {
        if(temp.str=="=")
        {
            return 0;
        }
        else if(temp.str=="||")
        {
            return 1;
        }
        else if(temp.str=="&&")
        {
            return 2;
        }
        else if(temp.str=="==")
        {
            return 3;
        }
        else if(temp.str=="!=")
        {
            return 4;
        }
        else if(temp.str=="<")
        {
            return 5;
        }
        else if(temp.str==">")
        {
            return 6;
        }
        else if(temp.str=="<=")
        {
            return 7;
        }
        else if(temp.str==">=")
        {
            return 8;
        }
        else if(temp.str=="-")
        {
            return 9;
        }
        else if(temp.str=="+")
        {
            return 10;
        }
        else if(temp.str=="*")
        {
            return 11;
        }
        else if(temp.str=="/")
        {
            return 12;
        }
        else if(temp.str=="%")
        {
            return 13;
        }
        else if(temp.str=="(")
        {
            return 16;
        }
        else
        {
            return 17;
        }
    }
}
void Syntax::Expression()
{
    Token temp = { "<",'O',0 };
	if (sc->getToken().str == "{")
		ArryList();
	else
	{
		while (true)
		{
			if (sc->getToken().type == 'I' || sc->getToken().type == 'B' || sc->getToken().type == 'C' ||
				(sc->getToken().type == 'P' && (sc->getToken().str == "=" || sc->getToken().str == "||" ||
					sc->getToken().str == "&&" || sc->getToken().str == "==" || sc->getToken().str == "!=" ||
					sc->getToken().str == "<" || sc->getToken().str == ">" || sc->getToken().str == "<=" ||
					sc->getToken().str == ">=" || sc->getToken().str == "-" || sc->getToken().str == "+" ||
					sc->getToken().str == "*" || sc->getToken().str == "/" || sc->getToken().str == "%" ||
					sc->getToken().str == "(" || sc->getToken().str == ")")))
			{
				if (sc->OperatorStack.empty())
				{
					if (sc->getToken().str == ")")
					{
						return;
					}
					else
					{
						sc->OperatorStack.push_back(temp);
						sc->OperatorStack.push_back(sc->getToken());
						sc->nextWord();
					}
				}
				else
				{
					//sc->nextWord();
					if (OperatorRela[switchCode(sc->OperatorStack[sc->OperatorStack.size() - 1])][switchCode(sc->getToken())] == '>')
					{
						sc->semanticStack();//语义栈操作
						sc->OperatorStack.pop_back();
						sc->OperatorStack.pop_back();
					}
					else if (OperatorRela[switchCode(sc->OperatorStack[sc->OperatorStack.size() - 1])][switchCode(sc->getToken())] == '<')
					{
						sc->OperatorStack.push_back(temp);
						sc->OperatorStack.push_back(sc->getToken());
						sc->nextWord();
					}
					else if (OperatorRela[switchCode(sc->OperatorStack[sc->OperatorStack.size() - 1])][switchCode(sc->getToken())] == '=')
					{
						sc->OperatorStack.push_back(temp);
						sc->OperatorStack.push_back(sc->getToken());
						sc->nextWord();
					}
					else
					{
						cout << "erro:OperatorPre" << OperatorRela[switchCode(sc->OperatorStack[sc->OperatorStack.size() - 1])][switchCode(sc->getToken())] << endl;
						system("pause"); exit(0);
						//system("pause");
					}
				}
			}
			else
			{
				if (sc->OperatorStack.empty())
				{
					return;
				}
				else
				{
					for (int i = sc->OperatorStack.size(); i > 0;)
					{
						sc->semanticStack();//语义栈操作
						sc->OperatorStack.pop_back();
						sc->OperatorStack.pop_back();
						i -= 2;
					}
				}
			}
		}
	}
}


//递归调用
void Syntax::mainFun()
{
    Program();
    if(sc->getToken().str=="#")
        cout<<"success"<<endl;
    else
        cout<<"erro:program"<<endl;
}
void Syntax::Program()
{
    sc->nextWord();
    CompoundStatement();
}
void Syntax::CompoundStatement()
{
    if(sc->getToken().str == "{")
    {
        sc->nextWord();
        CompoundStatement_next();
    }
    else
    {
        cout<<"erro:CompoundStatement()"<<endl;
        system("pause");exit(1);
    }
}

void Syntax::CompoundStatement_next()
{
	if ((sc->getToken().str == "int") || (sc->getToken().str == "char") || (sc->getToken().str == "struct"))
	{
		DeclarationList();
		CompoundStatement_next_next();
	}
	else if ((sc->getToken().str == "if") || (sc->getToken().str == "while")
		|| (sc->getToken().str == "continue") || (sc->getToken().str == "break")
		|| (sc->getToken().str == ";") || (sc->getToken().type == 'I')
		|| (sc->getToken().type == 'C') || (sc->getToken().str == "(")
		|| (sc->getToken().str == "{"))
	{
		StatementList();
		if (sc->getToken().str == "}")
			sc->nextWord();
		else
		{
			cout << "erro:CompoundStatement_next()1" << endl;
			system("pause"); exit(1);
		}
	}
	else if (sc->getToken().str == "}")
		sc->nextWord();
	else{
        cout<<"erro:CompoundStatement_next()2"<<endl;
        system("pause");exit(1);
    }
}
void Syntax::DeclarationList()
{
    Declaration();
    while(sc->getToken().str == "int" || sc->getToken().str == "char"|| sc->getToken().str == "struct")
    {
        Declaration();
    }
}
void Syntax::CompoundStatement_next_next()
{
    if(sc->getToken().str == "}")
        sc->nextWord();
    else
    {
        StatementList();
        if(sc->getToken().str=="}")
            sc->nextWord();
        else
        {
            cout<<sc->getToken().str<<endl;
            cout<<"erro:CompoundStatement_next_next()"<<endl;
            system("pause");exit(1);
        }
    }
}

void Syntax::StatementList()
{
    Statement();
    while((sc->getToken().str == "if" )||(sc->getToken().str == "while" )||(sc->getToken().str == "continue" )||(sc->getToken().str == "break" )||(sc->getToken().str == ";" )||(sc->getToken().type == 'I' )||(sc->getToken().type == 'C' )||(sc->getToken().str == "(" )||(sc->getToken().str == "{" ))
        Statement();
}
void Syntax::Statement()
{
    if(sc->getToken().str=="if")
        IfStatement();
    else if(sc->getToken().str=="while")
        WhileStatement();
    else if((sc->getToken().str=="continue")||(sc->getToken().str=="break"))
        JumpStatement();
    else  if((sc->getToken().str==";")||(sc->getToken().type=='I')||(sc->getToken().type=='C')||(sc->getToken().str=="("))
        ExpressionStatement();
    else if(sc->getToken().str=="{")
        CompoundStatement();
    else
    {
        cout<<"erro:Statement()"<<endl;
        system("pause");exit(1);
    }
}
void Syntax::IfStatement()
{
    if(sc->getToken().str == "if")
    {
        sc->nextWord();

        if(sc->getToken().str=="(")
        {
            sc->nextWord();
            Expression();
            sc->ActinoInRecursive("if");
            if(sc->getToken().str==")")
            {
                sc->nextWord();
                Statement();
                if (sc->getToken().str=="else")
                {
                    sc->ActinoInRecursive("el");
                    sc->nextWord();
                    Statement();
                }
                sc->ActinoInRecursive("ie");
            }
            else
            {
                cout<<sc->getToken().str<<endl;
                cout<<"erro:IfStatement()1"<<endl;
				//system("pause");
                system("pause");exit(1);
            }
        }
        else
        {
            cout<<"erro:IfStatement()2"<<endl;
            system("pause");exit(1);
        }
    }
    else
    {
        cout<<"erro:IfStatement()3"<<endl;
        system("pause");exit(1);
    }
}

void Syntax::WhileStatement()
{
    if(sc->getToken().str == "while")
    {
        sc->ActinoInRecursive("wh");
        sc->nextWord();
        if(sc->getToken().str=="(")
        {
            sc->nextWord();
            Expression();
            sc->ActinoInRecursive("do");
            if(sc->getToken().str==")")
            {
                sc->nextWord();
                Statement();
                sc->ActinoInRecursive("we");
            }
            else
            {
                cout<<"erro:WhileStatement()1"<<endl;
                system("pause");exit(1);
            }
        }
        else
        {
            cout<<"erro:WhileStatement()2"<<endl;
            system("pause");exit(1);

        }
    }
    else
    {
        cout<<"erro:WhileStatement()3"<<endl;
        system("pause");exit(1);

    }
}
void Syntax::JumpStatement()
{
    if(sc->getToken().str=="continue")
    {
        sc->ActinoInRecursive("co");
        sc->nextWord();
        if(sc->getToken().str==";")
            sc->nextWord();
        else
        {
            cout<<"errro:JumpStatement()1"<<endl;
            system("pause");exit(1);
        }
    }
    else if(sc->getToken().str=="break")
    {
        sc->ActinoInRecursive("br");
        sc->nextWord();
        if(sc->getToken().str==";")
            sc->nextWord();
        else
        {
            cout<<"errro:JumpStatement()2"<<endl;
            system("pause");exit(1);
        }
    }
    else
    {
        cout<<"erro:JumpStatement()3"<<endl;
        system("pause");exit(1);

    }
}
void Syntax::ExpressionStatement()
{
    if(sc->getToken().str==";")
        sc->nextWord();
    else
    {
        Expression();
        if(sc->getToken().str==";")
            sc->nextWord();
        else
        {
            cout<<"erro:ExpressionStatement()"<<endl;
            system("pause");exit(1);
        }
    }
}
void Syntax::Declaration()
{
    char ty;
    //cout<<sc->getToken().str<<endl;
    if(sc->getToken().str=="int")
    {
		ty = 'i';
        sc->nextWord();//a
        DeclaratorList(ty);
        if(sc->getToken().str==";")
            sc->nextWord();
        else
        {
			cout << "erro:Declaration()1 except';',but" << sc->getToken().str << endl;
            system("pause");exit(1);

        }
    }
    else if(sc->getToken().str=="char")
    {
        ty='c';
        sc->nextWord();
        DeclaratorList(ty);
        if(sc->getToken().str==";")
            sc->nextWord();
        else
        {
            cout<<"erro:Declaration()2"<<endl;
            system("pause");exit(1);

        }
    }
	else if (sc->getToken().str == "struct")
	{
		ty = 'd';
		sc->nextWord();
		
		
		if (sc->getToken().type == 'I')
		{
			auto iter = sc->findB(sc->MySynbl.begin(), sc->MySynbl.end(), sc->getToken().str);
			if (iter != sc->MySynbl.end())
			{
				cout << "重复定义 " << sc->getToken().str << " 这个标识符" << endl;
				system("pause"); exit(1);
			}
            sc->putStructIntoSynbl();
			sc->nextWord();
			if (sc->getToken().str == "{")
			{
				sc->nextWord();
				Struct();
				if (sc->getToken().str == "}")
				{
					sc->nextWord();
					if (sc->getToken().str == ";")
					{
						sc->actionForStruct();
						sc->nextWord();
					}
					else {
						cout << "erro:Declaration()3 expect ';' but " << sc->getToken().str<<endl;
						system("pause"); exit(1);
					}
				}
				else {
					cout << "erro:Declaration()4 expect '}' but " << sc->getToken().str << endl;
					system("pause"); exit(1);
				}
			}
			else {
				cout << "erro:Declaration()4 expect '{' but " << sc->getToken().str << endl;
				system("pause"); exit(1);
			}
		}
		else {
			cout << "erro:Declaration()5 expect Identify but " << sc->getToken().str << endl;
			system("pause"); exit(1);
		}
	}
    else
    {
        cout<<"erro:Declaration()3"<<endl;
        system("pause");exit(1);
    }
}

void Syntax::Struct()
{
	char ty;
	if (sc->getToken().str == "int")
	{
		ty = 'i';
		sc->nextWord();
		DeclaratorList(ty);
		if (sc->getToken().str == ";")
		{
			sc->nextWord();
			Struct();
		}
		else {
			cout << "erro:Struct()1 expect ';' but " << sc->getToken().str << endl;
			system("pause"); exit(1);
		}
	}
	else if (sc->getToken().str == "char")
	{
		ty = 'c';
		sc->nextWord();
		DeclaratorList(ty);
		if (sc->getToken().str == ";")
		{
			sc->nextWord();
			Struct();
		}
		else {
			cout << "erro:Struct()1 expect ';' but " << sc->getToken().str << endl;
			system("pause"); exit(1);
		}
	}
}
void Syntax::DeclaratorList(char i)
{
    Declarator(i);
    while(sc->getToken().str==",")
    {
        if(sc->getToken().str==",")
        {
            sc->nextWord();
            Declarator(i);
        }
    }
}
void Syntax::Declarator(char i)
{
    Token rem;
    Token temp;
    if(sc->getToken().type=='I')
    {
		auto iter = sc->findB(sc->MySynbl.begin(), sc->MySynbl.end(), sc->getToken().str);
		if (iter != sc->MySynbl.end())
		{
			cout << "重复定义 " << sc->getToken().str << " 这个标识符" << endl;
			system("pause"); exit(1);
		}
        rem = sc->getToken();
		//cout << "cou";
		sc->RState(i, sc->getToken().str);	//向符号表添加

        sc->nextWord();
        if(sc->getToken().str=="=") 
        {
			Declarator_next(rem,i);
        }
		else if (sc->getToken().str == "[")
		{
			Declarator_next(rem, i);
			int a = sc->MySynbl[(int)sc->MySynbl.size() - 1].addr;
			sc->addr_num += (sc->MyAinfl[a].up + 1) * (sc->MyAinfl[a].clen);
		}
    }
    else
    {
        cout<<"erro:Declarator()"<<endl;
        system("pause");exit(1);
    }
}

void Syntax::Declarator_next(Token rem,char i)
{
	int d;
	std::istringstream sin;
	Token temp = { "<",'O',0 };

	if (sc->getToken().str == "=")
	{
		sc->OperatorStack.push_back(temp);
		sc->OperatorStack.push_back(rem);
		sc->semanticStack();//语义栈操作
		sc->OperatorStack.push_back(temp);
		sc->OperatorStack.push_back(sc->getToken());//将'='入栈
		//cout << sc->getToken().str << endl;
		//sin.str(sc->nextWord().str);
		//sin >> d;
		//sc->VALL.push_back(d);
		//sc->MySynbl[(int)sc->MySynbl.size() - 1].addr = (int)sc->VALL.size() - 1;
		sc->nextWord();
		Expression();
		//sc->nextWord();
	}
	else
	{
		if (sc->nextWord().type == 'C')
		{
			sin.str(sc->getToken().str);
			sin >> d;
			
			if (sc->nextWord().str == "]")
			{
				sc->actionForArry(d, i);
				if (sc->nextWord().str == "[")
				{
					sc->dimensionalityOfArray++;
					Declarator_next(rem, i);
				}
				else sc->dimensionalityOfArray=0;
				
				//数组第一维长度*第一维成分类型长度
			}
			else
			{
				cout << "erro:Declarator except']'";
				system("pause"); exit(1);
			}

		}
	}
}

//
void Syntax::ArryList()
{
	ExpressionList();

	if (sc->nextWord().str != "}")
	{
		cout << "erro: arryList expect'}'" << endl;
		system("pause"); exit(1);
	}
}

//
void Syntax::ExpressionList()
{
	Expression();
	ExpressionList_next();
}

void Syntax::ExpressionList_next()
{
	if (sc->getToken().str == ",")
		ExpressionList();
}
