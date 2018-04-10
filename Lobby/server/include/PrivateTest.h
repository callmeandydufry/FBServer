#ifndef __PRIVATE_TEST_H__
#define __PRIVATE_TEST_H__

#include <string>
#include <iostream>
#include <windows.h>
#include <process.h>

using std::cout;
using std::endl;
using std::cin;

namespace PrivateTt
{
	template<unsigned int n>
	struct Sumsln
	{
		enum eValue
		{
			N = Sumsln<n - 1>::N + n,
		};
	};

	template<>
	struct Sumsln<1>
	{
		enum eValue
		{
			N = 1,
		};
	};

	template<>
	struct Sumsln<0>
	{
		enum eValue
		{
			N = 1,
		};
	};

	class Test1
	{
	public:
		void showTest()
		{
			cout << Sumsln<0>::N << endl;
		}
	};

	class BaseSln;
	BaseSln* tagSln[2];

	class BaseSln
	{
	public:
		virtual int Sum(unsigned int n)
		{
			return 0;
		}
	};

	class DeriveSln : public BaseSln
	{
	public:
		int Sum(unsigned int n)
		{
			return tagSln[!!n]->Sum(n - 1) + n;
		}
	};

	struct Test2
	{
		void showTest(unsigned int n)
		{
			BaseSln bs;
			DeriveSln ds;
			tagSln[0] = &bs;
			tagSln[1] = &ds;
			cout << tagSln[1]->Sum(n) << endl;
		}
	};

	int32 gNum = 0;

	unsigned int __stdcall ThreadFun(LPVOID pM)
	{
		int32 nThreadNum = *(int32*)pM;
		Sleep(50);
		gNum++;
		printf("子线程的nThreadNum为：%d\n, 全局输出gNum = %d\n", nThreadNum, gNum);
		return 0;
	}

	const int32 MaxThreadNum = 10;
	class ThreadTest
	{
	public:
		ThreadTest()
		{
			for (int32 i = 0; i < MaxThreadNum; ++i)
			{
				mThreadHand[i] = NULL;
			}
		}

		void CreateMyThread()
		{
			for (int i = 0; i < MaxThreadNum; ++i)
			{
				mThreadHand[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, &i, 0, NULL);
				i++;
			}
		}

		HANDLE mThreadHand[MaxThreadNum];
	};

	class VBDesctructor
	{
	public:
		~VBDesctructor() { cout << "VBDestructor " << endl; }
	};

	class VDDestructor : public VBDesctructor
	{
	public:
		~VDDestructor() { cout << "VDDestructor " << endl; }
	};

	class testVirtualDestructor
	{
	public:
		void test()
		{
			VDDestructor vd;
		}
	};

	class Base0
	{
	public:
		Base0(int nNum)
		{
			cout << "Base0" << endl;
		}

		virtual void bar()
		{
			cout << "bar" << endl;
		}
	};
};
#endif // __PRIVATE_TEST_H__
