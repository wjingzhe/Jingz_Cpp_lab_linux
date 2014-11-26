// functor-adapter_p431.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <algorithm>//Ԫ�ز����㷨
#include <functional> //�����������bind��compose��negate
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
        //std::bind���Ա��ֳ���̬��Ϊ  
        FunObejct f=std::bind(&A::f,this);  
        f();  
    } 
	bool print(int i)
	{
		std::cout<< i << "��";
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
	std::ostream_iterator<int> out_it(cout,",");//����������󶨵�����������,ÿ��������һ��,��print������

	int ia[6] = {2,21,12,7,19,23};

	std::shared_ptr<A>pa(new B()); //����ָ��ָ���������
    pa->init(); //��̬��������ʵ�ֵĶ�̨
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


	
	//�ҳ���С��12������
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
	//��ѭ���ڲ���ִ������һ����룺_Pred(*first)��_Pred���Ǻ�������std::bind(std::less<int>(),std::placeholders::_1,12)�����Ķ���
	int nums2= std::count_if(iv.begin(),iv.end(),std::bind(std::less<int>(),std::placeholders::_1,12));
	cout<<"(-,12):"<<nums2<<endl;



	//ʹ�õ����������������
	std::copy(iv.begin(),iv.end(),out_it);
	cout<<endl;
	
	//ʹ�ú��������
	std::for_each(iv.begin(),iv.end(),print);//���еĺ�������������ĳ��λ�ã���Ҫ�Ķ������鼮��֪�����º�������ָ��һ���º���������ͨ����������Ǻ�����������
	cout<<endl;

	//ʹ�÷º����������,��Ҫͨ��ָ�봦����Ϊstl������Ķ���ʹ�õ�ģ�壬���������ͼ�飨ͬʱideҲ�޷���ɳ�Ա���ѣ���Ϊδ�ػ������ڸ�����������صĴ��룩��
	//ֻҪ����operator()���ؼ���ͨ�����롣���ʹ��pa��ֵ���ڲ����õ�()������������ԭ��ָ������û�н���operator()�����ģ�����뱨��
	std::for_each(iv.begin(),iv.end(),(*pa));
	cout<<endl;


	FunPtr fp1 = print; //����ָ��ͺ�������ͬһ��������
	fp1(10);
	cout<<endl;

	//ʹ�ú���ָ�����
	std::for_each(iv.begin(),iv.end(),fp1);//���еĺ�������������ĳ��λ�ã���Ҫ�Ķ������鼮��֪�����º�������ָ��һ���º���������ͨ����������Ǻ�����������
	cout<<endl;

	//���ι�һ�㺯����STL�㷨�����������û����һ�㶼����
	std::for_each(iv.begin(),iv.end(),std::ptr_fun(fp1));//ʹ�ð�װ�õĺ��������ýṹstd::ptr_fun(*begin()),����ִ�е�����fp1(*begin())
	cout<<endl;


	std::vector<int> iv3;
	std::copy(iv.begin(),iv.end(),std::back_inserter(iv3));//back_inserter �Ƿ��ص������ĺ���������һ������������Ϊ���������������ͱ����ӵĺ�����������£����ڲ�ͬ�ĳ���

	std::for_each(iv3.begin(),iv3.end(),print);//���ʹ�÷�ָ��������ָ�룬�ᱨ��
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

	//ע��COMMA�����ţ�����ʹ�õ���_TYPEX_REFREF_ARG
	/*
	1: #define _TYPEX_REFREF(NUM)	\
		_VAR_TYPEX(NUM)&&

	2:#define _FORWARD_ARGX(NUM)	\
	_STD forward<_VAR_TYPEX(NUM)>(_VAR_VALX(NUM))

	_VAR_TYPEX ����ֵ���ͣ������ٵ�����ת�Ĺ����У����Ʒ����˵����飺n��ת��Ϊ��ֵ����Ȼ�������ĳ��������

	���⣺std::forward<T>(u) ������������T �� u����TΪ��ֵ��������ʱ��u����ת��ΪT���͵���ֵ������u����ת��ΪT������ֵ��
	��˶���std::forward��Ϊ����ʹ����ֵ���ò����ĺ���ģ���н������������ת�����⡣

	*/

	int n = 0;
	std::bind(inc,n)();//Դ����ʹ�õ���
	print(n);//����0��
	
	std::bind(inc,std::ref(n))();
	print(n);//������1����


	Int t1(3),t2(7),t3(20),t4(14),t5(26);

	std::vector<Int> vInt2;
	vInt2.push_back(t1); //cocos2dx�е�testcpp�жη�����룬���Կ�������������������ϤC++11��stl
	vInt2.push_back(t2);
	vInt2.push_back(t3);
	vInt2.push_back(t4);
	vInt2.push_back(t5);

//mem_fun�����ʱ��㷨����for_each
//��������:
//mem_fun_ref�����ú��÷���mem_funһ����Ψһ�Ĳ�ͬ���ǣ�
//�������д�ŵ��Ƕ���ʵ���ʱ����mem_fun_ref��
//�������д�ŵ��Ƕ����ָ���ʱ����mem_fun��

	//error C3867: ��Int::print1��: ��������ȱ�ٲ����б���ʹ�á�&Int::print1������ָ���Ա��ָ��
	std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&Int::print1));//���ʹ�÷�ָ��������ָ�룬�ᱨ��
	cout<<endl;
	;

	std::for_each(vInt2.begin(),vInt2.end(),std::bind(Int::print3,std::placeholders::_1));//ֻҪ��֤�����ӿڵ���ȷ�ԾͿ��ԣ�std::bind���캯�����ײ���һ�����ṩoperator()�Ķ��󣬿����Ƿº����ͺ�����,ʹ�þ�̬����Ϊ�ɺ����������󣬶����Ƿ��ʶ���Ĳ���


	//std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&std::bind(Int::print3,std::placeholders::_1)));

	cout<<endl;
	
	;

	std::for_each(vInt2.begin(),vInt2.end(),std::bind(std::plus<Int>(),std::placeholders::_1,Int(3)));
	std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&Int::print1));//���ʹ�÷�ָ��������ָ�룬�ᱨ��
	cout<<endl;

	//��ʱ�޷�ʹ��stlmem_fun_refʵ�ֶ��������

	

	;

	std::for_each(vInt2.begin(),vInt2.end(),std::bind(std::mem_fun_ref(&Int::addNum),std::placeholders::_1,3));
	std::for_each(vInt2.begin(),vInt2.end(),std::mem_fun_ref(&Int::print1));//���ʹ�÷�ָ��������ָ�룬�ᱨ��
	cout<<endl;


	
	


	std::istream_iterator<int> in_it(cin),eos;//�����и�bug������Ҫ�ö��߳̽�����������߳�������д���ȴ���������ж���λ��vs_stl���ṩ�л�������MS��STL��insert_iterator����������ȡ���ݵĲ��������߳̽��ж϶�ȡ�����С���ᵼ��һ�����ص�bug��������ĳ��IO����ʱ��ǰ��������ݲ�����������ܲ������������ġ�

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

