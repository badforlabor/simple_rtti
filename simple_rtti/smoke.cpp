// simple_rtti.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <assert.h>
#include "simple_rtti.h"
//using namespace rtti;
#define USEMacro 1

class ClassBase : public rtti::SimpleRtti
{
#if USEMacro
	DECLARE_RTTI(ClassBase, rtti::SimpleRtti);
#else
public:
	static const rtti::RttiType* StaticClass()
	{
		static auto Inside = rtti::RttiType::New("ClassBase", rtti::SimpleRtti::StaticClass());
		return Inside;
	}
	virtual const rtti::RttiType* GetType() const override
	{
		return ClassBase::StaticClass();
	}
#endif

public:
	virtual void ShowMe()
	{
		std::cout << "base" << std::endl;
	}

};

class ClassA : public ClassBase
{
#if USEMacro
	DECLARE_RTTI(ClassA, ClassBase);
#else
public:
	static const rtti::RttiType* StaticClass()
	{
		static auto Inside = rtti::RttiType::New("ClassA", ClassBase::StaticClass());
		return Inside;
	}
	virtual const rtti::RttiType* GetType() const override
	{
		return ClassA::StaticClass();
	}
#endif
public:
	virtual void ShowMe() override
	{
		std::cout << "A" << std::endl;
	}
};

class ClassAA : public ClassA
{
#if USEMacro
	DECLARE_RTTI(ClassAA, ClassA);
#else
public:
	static const rtti::RttiType* StaticClass()
	{
		static auto Inside = rtti::RttiType::New("ClassAA", ClassA::StaticClass());
		return Inside;
	}
	virtual const rtti::RttiType* GetType() const override
	{
		return ClassAA::StaticClass();
}
#endif
public:
	
	virtual void ShowMe() override
	{
		std::cout << "AA" << std::endl;
	}
};

class ClassB : public ClassBase
{
#if USEMacro
	DECLARE_RTTI(ClassB, ClassBase);
#else
public:
	static const rtti::RttiType* StaticClass()
	{
		static auto Inside = rtti::RttiType::New("ClassB", ClassBase::StaticClass());
		return Inside;
	}
	virtual const rtti::RttiType* GetType() const override
	{
		return ClassB::StaticClass();
	}
#endif
public:

	virtual void ShowMe() override
	{
		std::cout << "B" << std::endl;
	}
};


int TestAll()
{
    std::cout << "Smoke Test...\n";

	assert(ClassBase::StaticClass()->ClsName == "ClassBase");
	assert(ClassA::StaticClass()->ClsName == "ClassA");
	assert(ClassAA::StaticClass()->ClsName == "ClassAA");

	ClassBase* Base = new ClassBase();
	ClassBase* A = new ClassA();
	ClassBase* AA = new ClassAA();
	ClassBase* B = new ClassB();

	{
		auto One = Base->Cast<ClassA>();
		assert(One == nullptr);
	}
	{
		auto One = Base->Cast<ClassAA>();
		assert(One == nullptr);
	}
	{
		auto One = A->Cast<ClassA>();
		assert(One != nullptr);
		One->ShowMe();
	}
	{
		auto One = A->Cast<ClassAA>();
		assert(One == nullptr);
	}
	{
		auto One = AA->Cast<ClassA>();
		assert(One != nullptr);
		One->ShowMe();
	}
	{
		auto One = AA->Cast<ClassAA>();
		assert(One != nullptr);
		One->ShowMe();
	}
	{
		auto One = B->Cast<ClassB>();
		assert(One != nullptr);
		One->ShowMe();
	}
	{
		auto One = B->Cast<ClassAA>();
		assert(One == nullptr);
	}
	{
		auto One = B->Cast<ClassA>();
		assert(One == nullptr);
	}

	{
		assert(AA->IsA<ClassAA>());
		assert(AA->IsA<ClassA>());
		assert(AA->IsA<ClassBase>());
		assert(!AA->IsA<ClassB>());

		assert(!A->IsA<ClassAA>());
		assert(A->IsA<ClassA>());
		assert(A->IsA<ClassBase>());
		assert(!A->IsA<ClassB>());

		assert(!Base->IsA<ClassAA>());
		assert(!Base->IsA<ClassA>());
		assert(Base->IsA<ClassBase>());
		assert(!Base->IsA<ClassB>());

		assert(!B->IsA<ClassAA>());
		assert(!B->IsA<ClassA>());
		assert(B->IsA<ClassBase>());
		assert(B->IsA<ClassB>());
	}

	std::cout << "--------------------" << std::endl;

	{
		assert(rtti::TypeId<ClassBase>()->ClsName == "ClassBase");
		assert(rtti::TypeId<ClassA>()->ClsName == "ClassA");
		assert(rtti::TypeId<ClassAA>()->ClsName == "ClassAA");
	}

	{
		assert(rtti::TypeOf(Base)->ClsName == "ClassBase");
		assert(rtti::TypeOf(A)->ClsName == "ClassA");
		assert(rtti::TypeOf(AA)->ClsName == "ClassAA");
	}
	
#if 0
	{
		// 验证const
		rtti::TypeOf(Base)->ClsName = "1";
		rtti::TypeOf(Base)->Parent = nullptr;
		rtti::TypeOf(Base)->rtti::TypeId = 1;
	}
#endif

	{
		auto One = rtti::Cast<ClassA>(A);
		assert(One != nullptr);
		One->ShowMe();
	}
	{
		auto One = rtti::Cast<ClassAA>(A);
		assert(One == nullptr);
	}
	{
		auto One = rtti::Cast<ClassA>(AA);
		assert(One != nullptr);
		One->ShowMe();
	}
	{
		auto One = rtti::Cast<ClassAA>(AA);
		assert(One != nullptr);
		One->ShowMe();
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
