#include<stdio.h>
#include<string.h>
#include<stdlib.h>

FILE *fp;
char Exp[30],gra[30],prod[30]="",chExp='#';
int expSize=0,graSize=0,step=0;
int flag=0;

/*
E -> E+T|E-T|T

T -> T*F|T/F|F

F -> (E)|num 
������ݹ飬����ݹ�����֮��õ�
E -> T E'
T -> F T'
F -> ( E ) | num
E' -> epsilon | + T E' | - T E' 
T' -> epsilon | * F T' | / F T'

*/

void printGrammar()
{
	fprintf(fp,"\t\t\t�ݹ��½���������\n\t\t\n");
	fprintf(fp,"-----------------------------------------------------------\n");
	fprintf(fp,"\t\t\t (1)E->E+T|E-T|T\n");
	fprintf(fp,"\t\t\t (2)T->T*F|T/F|F\n");
	fprintf(fp,"\t\t\t (3)F->(E)|num\n");
	fprintf(fp,"����������ʽ������ݹ�֮��Ĳ���ʽ���£�\n");
	fprintf(fp,"\t\t\t (1)E->TA\n");
	fprintf(fp,"\t\t\t (2)T->FB\n");
	fprintf(fp,"\t\t\t (3)F->(E)|num\n");
	fprintf(fp,"\t\t\t (4)A->epsilon|+TA|-TA\n");
	fprintf(fp,"\t\t\t (5)B->epsilon|*FB|/FB\n"); 
	fprintf(fp,"-----------------------------------------------------------\n");
}

void GetExp()
{
	fprintf(fp,"��������ʽ:����#������\t");
	gets(Exp);
	fprintf(fp,"%s\n",Exp);
	expSize=strlen(Exp);
	chExp=Exp[0];
	fprintf(fp,"-----------------------------------------------------------\n");
}

void printHead()
{
	fprintf(fp,"���裺\t �﷨ջ��\t\t���봮��\t����ʽ��\n");
}

void printStep()
{
	fprintf(fp,"%d\t%-20s %10s\t \t%-15s\n",step,gra,Exp,prod);
	strcpy(prod,"");
	step++;
	if(chExp=='#'&&gra[graSize-1]=='#')
	{
		fprintf(fp,"\n���ʽ�����ɹ���\n");
		flag=1;
	}
}

void pushGraStack(char * ch)
{
	int i;
	for(i=0;i<strlen(ch);i++)
	{
		gra[graSize]=ch[strlen(ch)-1-i];
		graSize++;
	}
}


char popGraStack()
{
	char ch;
	ch=gra[graSize-1];
	gra[graSize-1]='\0';
	graSize--;
	return ch;
}


void nextChar()
{
	int i=0;
	for(i=0;i<expSize-1;i++)
	{
		Exp[i]=Exp[i+1];
	}
	Exp[expSize-1]='\0';
	expSize--;
	chExp=Exp[0];
	fprintf(fp,"��ǰchExp=:%c\n",chExp);
}

void InitGra()
{
	gra[graSize]='#';
	graSize++;
	gra[graSize]='E';
	graSize++;
	printStep();
}

void printError()
{
	fprintf(fp,"\n���ʽ��ƥ�䣡\n");
}

/*
E -> E+T|E-T|T

T -> T*F|T/F|F

F -> (E)|num 
������ݹ飬����ݹ�����֮��õ�
E -> T A
T -> F B
F -> ( E ) | num
A -> epsilon | + T A | - T A 
B -> epsilon | * F B | / F B

*/

int E()
{
	popGraStack();
	char graE[]="TA";
	pushGraStack(graE);
	strcpy(prod,"E->TA");
	printStep();
	T();
	A();
	return 1;
}

int T()
{
	popGraStack();
	char graT[]="FB";
	pushGraStack(graT);
	strcpy(prod,"T->FB");
	printStep();
	F();
	B();
	return 1;
}

int F()
{
	if(chExp=='(')
	{
		popGraStack();
		char graF[]="(E)";
		pushGraStack(graF);
		strcpy(prod,"F->(E)");
		printStep();
		popGraStack();
		nextChar();
		strcpy(prod,"ƥ��");
		printStep();
		E();
		if(chExp==')')
		{
			popGraStack();
			nextChar();
			strcpy(prod,"ƥ��");
			printStep();
			return 1;
		}
		else
		{
			printError();
			return 0;
		}
	}
	else if(chExp<='9' && chExp>='0')
	{
		popGraStack();
		char graF[]={chExp,'\0'};
		pushGraStack(graF);
		strcpy(prod,"F->");
		strcat(prod,graF);
		printStep();
		popGraStack();
		nextChar();
		strcpy(prod,"ƥ��");
		printStep();
		return 1;
	}
	else
	{
		printError();
		return 0;
	}
}
/*
E -> E+T|E-T|T

T -> T*F|T/F|F

F -> (E)|num 
������ݹ飬����ݹ�����֮��õ�
E -> T A
T -> F B
F -> ( E ) | num
A -> epsilon | + T A | - T A 
B -> epsilon | * F B | / F B

*/
int A()
{
	if(chExp=='+'||chExp=='-')
	{
		popGraStack();
		char graA[]={chExp,'T','A','\0'};
		pushGraStack(graA);
		strcpy(prod,"A->");
		strcat(prod,graA);
		printStep();
		popGraStack();
		nextChar();
		strcpy(prod,"ƥ��");
		printStep();
		T();
		A();
		return 1;
	}
	else
	{
		strcpy(prod,"A->epsilon");
		printStep();
		popGraStack();
		strcpy(prod,"ƥ��");
		printStep();
		return 1;
	}
}

int B()
{
	if(chExp=='*'||chExp=='/')
	{
		popGraStack();
		char graB[]={chExp,'T','B','\0'};
		pushGraStack(graB);
		strcpy(prod,"B->");
		strcat(prod,graB);
		printStep();
		popGraStack();
		nextChar();
		strcpy(prod,"ƥ��");
		printStep();
		T();
		B();
		return 1;
	}
	else
	{
		strcpy(prod,"B->epsilon");
		printStep();
		popGraStack();
		strcpy(prod,"ƥ��");
		printStep();
		return 1;
	}
}

int main()
{
	fp=fopen("result.txt","w");
	printGrammar();
	GetExp();
	printHead();
	InitGra();
	E();
	if(flag==0)
	{
		fprintf(fp,"\n���ʽ�������ɹ���\n");
	}
	return 0;
}
