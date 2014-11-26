// functor-adapter_p431.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <algorithm>//元素操作算法
#include <functional> //函数配接器，bind、compose、negate
#include <iostream>
#include <iterator>
#include <vector>
#include <memory>


using std::cout;
using std::cin;
using std::endl;

bool print(int i)
{
	std::cout<< i << " ";
	return true;
}

	

class Int
{
public:
	explicit Int(int i):m_i(i){};
	~Int(void){};

	void print1(void) const
	{
		cout<<"["<<m_i<<"]";
	}
	void print2(Int &i)const
	{
		cout<<"["<<i.m_i<<"]";
	}
	static void print3(Int &i)
	{
		cout<<"["<<i.m_i<<"]";
	}
	
	void addNum(int j)
	{
		m_i+=j;
	}


int m_i;

private:
	
};

Int operator + (const Int &lhi,const Int &rhi)
	{
		Int temp(lhi.m_i);
		temp.m_i+=rhi.m_i;
		return temp;
	}


typedef bool (*FunPtr)(int);
typedef std::function<void ()> FunObejct;  
  
class A  
{  
public:  
    virtual void f()  
    {  
        std::cout<<"A::f()"<<endl;  
    }  
  
    void init()  
    {  
        //std::bind可以表现出多态行为  
        FunObejct f=std::bind(&A::f,this);  
        f();  
    } 
	bool print(int i)
	{
		std::cout<< i << "：";
		return true;
	}

	inline bool operator() (int i)
	{
		std::cout<< i << "/";
		return true;
	}
	

};  


class B:public A  
{  
public:  
    virtual void f()  
    {  
        cout<<"B::f()"<<endl;  
    }  
};  


void inc(int &a)
{
	++a;
}


#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	std::ostream_iterator<int> out_it(cout,",");//输出迭代器绑定到命令窗输出行中,每次输出填充一个,和print区别开来

	int ia[6] = {2,21,12,7,19,23};

	std::shared_ptr<A>pa(new B()); //父类指针指向子类对象
    pa->init(); //多态、函数绑定实现的多台
	/*
	*   
	template<class _Ret,
	class _Rx,
	class _Farg0,
	class _Arg0> inline
	typename enable_if<!is_same<_Ret, _Rx>::value,
		_Bind<true, _Ret, _Pmd_wrap<_Rx _Farg0::*, _Rx, _Farg0>, _Arg0>
			>::type
		bind(_Rx _Farg0::* const _Pmd, _Arg0&& _A0)
	{	// bind a wrapped member object pointer
	return (_Bind<true, _Ret,
		_Pmd_wrap<_Rx _Farg0::*, _Rx, _Farg0>, _Arg0>(
		_Pmd_wrap<_Rx _Farg0::*, _Rx, _Farg0>(_Pmd),
			_STD forward<_Arg0>(_A0)));
	}
	*
	*
	*/
	std::function<bool(int)> printInt = std::bind(&B::print,pa,std::placeholders::_1);//
	std::function<bool(int)> printInt2 = std::bind(&A::print,pa,std::placeholders::_1);
	printInt(1000);
	printInt2(1002);
	
	
	typedef std::function<bool(int)> FP;

	FP t_fp = FP(print);

	t_fp(12);
	cout<<endl;


	
	//找出不小于12的数据
	std::vector<int> iv(ia,ia+6);
	std::copy_if(iv.begin(),iv.end(),out_it,std::not1(std::bind2nd(std::less<int>(),12)));

	//cout<< std::count_if(ia,ia+6,std::not1(std::bind(f1,12)) )<<endl;


	cout<<endl;

	int nums = std::count_if(
		iv.begin(),iv.end(),
		std::bind(
			std::logical_and<bool>(),
			std::bind(std::less<int>(),std::placeholders::_1,30),
			std::bind(std::greater<int>(),std::placeholders::_1,12)
			)
		);
	cout<<"(12,30):"<<nums<<endl;
	//在循环内部讲执行这样一句代码：_Pred(*first)。_Pred就是函数对象std::bind(std::less<int>(),std::placeholders::_1,12)产生的对象
	int nums2= std::count_if(iv.begin(),iv.end(),std::bind(std::less<int>(),std::placeholders::_1,12));
	cout<<"(-,12):"<<nums2<<endl;



	//使用迭代器输出所以数据
	std::copy(iv.begin(),iv.end(),out_it);
	cout<<endl;
	
	//使用函数名输出
	std::for_each(iv.begin(),iv.end(),print);//类中的函数操作保存在某个位置，我要阅读其他书籍才知道。仿函数对象指的一个仿函数对象，普通函数对象就是函数名，待续
	cout<<endl;

	//使用仿函数对象输出,需要通过指针处理，因为stl函数库的定义使用的模板，不进行类型检查（同时ide也无法完成成员提醒，因为未特化不存在改类型类型相关的代码），
	//只要存在operator()重载即可通过编译。如果使用pa传值，内部调用的()操作符，但是原生指针类型没有进行operator()操作的，会编译报错。
	std::for_each(iv.begin(),iv.end(),(*pa));
	cout<<endl;


	FunPtr fp1 = print; //函数指针和函数名是同一个东西？
	fp1(10);
	cout<<endl;

	//使用函数指针输出
	std::for_each(iv.begin(),iv.end(),fp1);//类中的函数操作保存在某个位置，我要阅读其他书籍才知道。仿函数对象指的一个仿函数对象，普通函数对象就是函数名，待续
	cout<<endl;

	//修饰过一般函数的STL算法，这个东西还没看，一点都不懂
	std::for_each(iv.begin(),iv.end(),std::ptr_fun(fp1));//使用包装好的函数最后调用结构std::ptr_fun(*begin()),里面执行的则是fp1(*begin())
	cout<<endl;


	std::vector<int> iv3;
	std::copy(iv.begin(),iv.end(),std::back_inserter(iv3));//back_inserter 是返回迭代器的函数，就像一个适配器。名为迭代器适配器，和本例子的函数适配器相仿，用于不同的场景

	std::for_each(iv3.begin(),iv3.end(),print);//如果使用非指向函数名的指针，会报错
	cout<<endl;


//	#define _BIND_IMPLICIT1( \
//	TEMPLATE_LIST1, PADDING_LIST1, LIST1, COMMA1, \
//	TEMPLATE_LIST2, PADDING_LIST2, LIST2, COMMA2) \
//template<class _Rx \
//	COMMA1 LIST1(_CLASS_TYPE) \
//	COMMA2 LIST2(_CLASS_TYPEX)> inline \
//	_Bind<true, _Rx, _Rx (* const)(LIST1(_TYPE)) COMMA2 LIST2(_TYPEX)> \
//		bind(_Rx (*_Pfx)(LIST1(_TYPE)) COMMA2 LIST2(_TYPEX_REFREF_ARG)) \
//	{	/* bind a function pointer */ \
//	return (_Bind<true, _Rx, _Rx (* const)(LIST1(_TYPE)) \
//		COMMA2 LIST2(_TYPEX)>(_Pfx COMMA2 LIST2(_FORWARD_ARGX))); \
//	} \

	//注：COMMA：逗号，传参使用的是_TYPEX_REFREF_ARG
	/*
	1: #define _TYPEX_REFREF(NUM)	\
		_VAR_TYPEX(NUM)&&

	2:#define _FORWARD_ARGX(NUM)	\
	_STD forward<_VAR_TYPEX(NUM)>(_VAR_VALX(NUM))

	_VAR_TYPEX 是右值类型，所以再调用跳转的过程中，估计发生了的事情：n被转化为右值类型然后产生了某个副本，

	正解：std::forward<T>(u) 有两个参数：T 与 u。当T为左值引用类型时，u将被转换为T类型的左值，否则u将被转换为T类型右值。
	如此定义std::forward是为了在使用右值引用参数的函数模板中解决参数的完美转发问题。

	*/

	int n = 0;
	std::bind(inc,n)();//源码中使用的是
	print(n);//还是0；
	
	std::bind(inc,std::ref(n))();
	print(n);//终于是1啦。


	Int t1(3),t2(7),t3(20),t4(14),t5(26);

	std::vector<Int> vInt2;
	vInt2.push_back(t1); //cocos2dx中的testcpp有段反射代码，可以考虑用上来，短期险熟悉C++11的stl
	vInt2.push_back(t2);
	vInt2.push_back(t3);
	vInt2.push_back(t4);
	vInt2.push_back(t5);

//mem_fun则是质变算法。当for_each
//两者区别:
//mem_fun_ref的作用和用法跟mem_fun一样，唯一的不同就是：
//当容器中存放的是对象实体的时候用mem_fun_ref，
//当容器中存放的是对象的指针的时候用mem_fun。

	//error C3867: “Int::print1”: 函数调用缺少参数列表；请使用“&Int::print1”创建指向成员的指针
	std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&Int::print1));//如果使用非指向函数名的指针，会报错
	cout<<endl;
	;

	std::for_each(vInt2.begin(),vInt2.end(),std::bind(Int::print3,std::placeholders::_1));//只要保证函数接口的正确性就可以，std::bind构造函数的首参数一定是提供operator()的对象，可以是仿函数和函数名,使用静态是因为由函数操作对象，而不是访问对象的操作


	//std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&std::bind(Int::print3,std::placeholders::_1)));

	cout<<endl;
	
	;

	std::for_each(vInt2.begin(),vInt2.end(),std::bind(std::plus<Int>(),std::placeholders::_1,Int(3)));
	std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&Int::print1));//如果使用非指向函数名的指针，会报错
	cout<<endl;

	//暂时无法使用stlmem_fun_ref实现多参数操作

	

	;

	std::for_each(vInt2.begin(),vInt2.end(),std::bind(std::mem_fun_ref(&Int::addNum),std::placeholders::_1,3));
	std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&Int::print1));//如果使用非指向函数名的指针，会报错
	cout<<endl;


	
	


	std::istream_iterator<int> in_it(cin),eos;//这里有个bug，必须要用多线程解决。程序主线程启动读写，等待输入操作中断置位，vs_stl不提供切换操作。MS的STL中insert_iterator采用主动读取数据的操作，主线程将中断读取操作中。这会导致一个严重的bug，即构建某个IO操作时提前完成了数据操作，而这可能不是我们期望的。

	std::function<bool(int)> f1 = std::bind(std::less<int>(),std::placeholders::_1,13);

	cout<<"less(3,13):"<<f1(3)<<endl;

	while (!(in_it==eos))
	{
		cout<<*in_it;
		cout<<endl;
		++in_it;
	}


	

	return 0;
}

