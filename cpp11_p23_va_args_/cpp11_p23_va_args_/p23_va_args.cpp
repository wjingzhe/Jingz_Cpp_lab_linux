#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>


#define LOG(...) {\
		fprintf(stderr,"%s :\n",__FILE__);\
		fprintf(stderr,"Line %d:\n",__LINE__);\
		fprintf(stderr,__VA_ARGS__);\
		fprintf(stderr,"\n");\
	}


void foo(int i,...)
{
	va_list args;
	const char * temp;
	va_start(args, i);
	temp = va_arg(args, const char *);
	LOG("i= %d,string = %s", i, temp);
	va_end(args);
	
}


class A
{
public:
	A()
	{
		i = 10;
	};
	~A(){};
	virtual void display()
	{

	}
	int i;
};

class B
{
public:
	virtual void display()
	{

	}
	int j;
};

class C :public A,public B
{
public:
	int i;
	int k;
private:
	
	
};

class D:virtual public A
{
public:
	int di;
	virtual void display() override
	{

	}
private:

};

class  E :virtual public A
{
public:
	int ei;
	virtual void display() override
	{

	}

private:

};

class F : public D,public E
{
public:
	int fi;

	virtual void display() override
	{

	}

private:

};








int main(int argv, char **argc)
{
	foo(12345, "you are a stupid guy!!");
	int x = 3;
	LOG("X= %d", x);
	int b = 0;
	int i;
	try
	{
		if (b == 0) throw std::string("devied by 0 that caused overflow");
		i = x / b;
	}
	catch (std::string e)//__VA_ARGS__只能出现在宏中
	{
		LOG(e.c_str());
	}


	C * pC = new C();
	B * pB = dynamic_cast<B*>(pC);
	A * pA = dynamic_cast<A*>(pC);
	B * pB_s = static_cast<B*>(pC);
	A * pA_s = static_cast<A*>(pC);
	C * pCB_D = dynamic_cast<C*>(pB);
	C * pCA_D = dynamic_cast<C*>(pA);
	C * pCB_S = static_cast<C*>(pB);
	C * pCA_S = static_cast<C*>(pA);

	pC->i = 100;
	printf("%d\n",pA->i);
	

	printf("pC = %d,pB_d = %d,pA_d = %d,pB_s = %d,pA_s = %d\n", pC, pB, pA, pB_s, pA_s);

	printf("pCB_D = %d,pCA_D = %d,pCB_S = %d,pCA_S = %d\n", pCB_D, pCA_D, pCB_S, pCA_S);

	F * pF = new F();

	D * pD = dynamic_cast<D*>(pF);
	E * pE = dynamic_cast<E*>(pF);
	D * pD_s = static_cast<D*>(pF);
	E * pE_s = static_cast<E*>(pF);

	F * pFD_D = dynamic_cast<F*>(pD);
	F * pFE_D = dynamic_cast<F*>(pE);
	F * pFD_S = static_cast<F*>(pD);
	F * pFE_S = static_cast<F*>(pE);

	printf("pF = %d,pD_d = %d,pE_d = %d,pD_s = %d,pE_s = %d\n", pF, pD, pE, pD_s, pE_s);
	
	printf("pFD_D = %d, pFE_D = %d,pFD_S = %d, pFE_S = %d\n", pFD_D, pFE_D, pFD_S, pFE_S);
	
	return 0;
}