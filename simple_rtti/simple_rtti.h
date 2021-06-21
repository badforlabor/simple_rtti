#pragma once

/**,
 * Auth :   liubo
 * Date :   2021/06/21 11:07
 * Comment: 简单的RTTI替代方案  
 */

 /*
	 // example

 class ClassBase : public rtti::SimpleRtti
 {
	 DECLARE_RTTI(ClassBase, rtti::SimpleRtti);

 public:
	 virtual void ShowMe()
	 {
		 std::cout << "base" << std::endl;
	 }
 };

 class ClassA : public ClassBase
 {
	 DECLARE_RTTI(ClassA, ClassBase);
 public:
	 virtual void ShowMe() override
	 {
		 std::cout << "A" << std::endl;
	 }
 };

 void Test1()
 {
	ClassBase* A = new ClassA();
	
	assert(A->IsA<ClassA>());

	auto One = A->Cast<ClassA>();
	assert(One != nullptr);
	One->ShowMe();

	assert(TypeOf(A)->ClsName == "ClassA");
	assert(TypeId<ClassA>()->ClsName == "ClassA");
 }

 */

namespace rtti
{
	class RttiType
	{
	protected:

		RttiType(const RttiType* InParent, const char* InClsName = "")
			: TypeId(GetUniqueId()), Parent(InParent), ClsName(InClsName)
		{

		}
		RttiType(const char* InClsName) : RttiType(nullptr, InClsName)
		{

		}
		RttiType() : RttiType(nullptr, "")
		{

		}

		static int GetUniqueId()
		{
			static int Id = 0;
			Id++;
			return Id;
		}

	public:
		int TypeId = 0;
		const RttiType *Parent = nullptr;
		const char* ClsName = "";

		bool IsA(const RttiType* TargetType) const
		{
			if (TypeId == TargetType->TypeId)
			{
				return true;
			}
			if (Parent)
			{
				return Parent->IsA(TargetType);
			}
			return false;
		}

		static const RttiType* New(const char* InClsName, const RttiType* InParent = nullptr)
		{
			return new RttiType(InParent, InClsName);
		}

	};

	class SimpleRtti
	{
	public:
		static const RttiType* StaticClass()
		{
			static auto Inside = RttiType::New("SimpleRtti");
			return Inside;
		}
		virtual const RttiType* GetType() const
		{
			return StaticClass();
		}
	public:
		template<class ChildClass>
		ChildClass* Cast()
		{
			return IsA<ChildClass>() ? static_cast<ChildClass*>(this) : nullptr;
		}
		template<class ChildClass>
		const ChildClass* Cast() const
		{
			return IsA<ChildClass>() ? static_cast<const ChildClass*>(this) : nullptr;
		}

		template<class ChildClass>
		bool IsA() const
		{
			return GetType()->IsA(ChildClass::StaticClass());
		}

	};

	template<class T>
	const RttiType* TypeId()
	{
		return T::StaticClass();
	}
	template<class T>
	const RttiType* TypeOf(T* obj)
	{
		return obj->GetType();
	}
	
	template<class A, class B>
	A* Cast(B* b)
	{
		return b->Cast<A>();
	}
	template<class A, class B>
	const A* Cast(const B* b)
	{
		return b->Cast<A>();
	}

#define DECLARE_RTTI(Cls, ParentCls) \
public: \
	static const rtti::RttiType* StaticClass() \
	{ \
		static auto Inside = rtti::RttiType::New(#Cls, ParentCls::StaticClass()); \
		return Inside; \
	} \
	virtual const rtti::RttiType* GetType() const override \
	{ \
		return Cls::StaticClass(); \
	}

}
