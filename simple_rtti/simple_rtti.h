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
#define DEBUG_RTTI_TYPE 0

	class RttiType
	{
	protected:

		RttiType(const RttiType* InParent, int UniqueId, const char* InClsName = "")
			: TypeId(UniqueId), ClsName(InClsName), Parent(InParent)
		{

		}
		RttiType(const char* InClsName) : RttiType(nullptr, GetUniqueId(), InClsName)
		{

		}
		RttiType() : RttiType("")
		{

		}
		
	public:
		int TypeId = 0;
		const char* ClsName = "";
		const RttiType *Parent = nullptr;

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

		static const RttiType* New(const char* InClsName, const RttiType* InParent, int UniqueId)
		{
			return new RttiType(InParent, UniqueId, InClsName);
		}
		static const RttiType* New(const char* InClsName, const RttiType* InParent)
		{
			return New(InClsName, InParent, GetUniqueId());
		}
		static const RttiType* New(const char* InClsName)
		{
			return New(InClsName, nullptr);
		}

		static int GetUniqueId()
		{
			static int Id = 0;
			Id++;
			return Id;
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
#if DEBUG_RTTI_TYPE
		static const RttiType* StaticClassId;
#endif
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

	// 加入一些可调式信息
#if DEBUG_RTTI_TYPE
	#define DECLARE_RTTI_DEBUG(Cls, ParentCls) \
	static const rtti::RttiType* StaticClassId;
#else
	#define DECLARE_RTTI_DEBUG(Cls, ParentCls)
#endif

#define DECLARE_RTTI(Cls, ParentCls) \
public: \
	DECLARE_RTTI_DEBUG(Cls, ParentCls) \
	static const rtti::RttiType* StaticClass() \
	{ \
		static auto Inside = rtti::RttiType::New(#Cls, ParentCls::StaticClass()); \
		return Inside; \
	} \
	virtual const rtti::RttiType* GetType() const override \
	{ \
		return Cls::StaticClass(); \
	}

#if DEBUG_RTTI_TYPE
#define IMPLEMENT_RTTI(Cls) \
	const rtti::RttiType* Cls::StaticClassId = Cls::StaticClass();
#else
#define IMPLEMENT_RTTI(Cls)
#endif
}
