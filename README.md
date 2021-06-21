# simple_rtti
c++ rtti 替代方案。这样就可以不开启rtti了。



# 只有一个头文件

simple_rtti.h



# 使用方法

### 关键函数

- Cast，代替dynamic_cast
- TypeOf，代替typeof
- TypeId，代替typeid
- 使用宏DECLARE_RTTI，代替手写

### 例子

```
	
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
	
void Test()
{
	ClassBase* A = new ClassA();
	
	assert(A->IsA<ClassA>());

	auto One = A->Cast<ClassA>();
	assert(One != nullptr);
	One->ShowMe();

	assert(TypeOf(A)->ClsName == "ClassA");
	assert(TypeId<ClassA>()->ClsName == "ClassA");
}
```



# 测试

smoke.cpp