#pragma once

#include "Generated/ExampleClass.refureku.h"

namespace exnamespace
{
	class RFKClass() ParentParentParentClass
	{
		RFKParentParentParentClass_GENERATED
	};

	class RFKClass() ParentParentClass : private ParentParentParentClass
	{
		RFKParentParentClass_GENERATED
	};

	class RFKClass() ParentClass : protected ParentParentClass
	{
		RFKParentClass_GENERATED
	};

	class RFKClass() ExampleClass : public ParentClass
	{
		public:
			RFKField()
			int		someInt		= 42;

			RFKField()
			float	someFloat	= 42.42f;

			RFKMethod()
			int		method4(void* ptr);

			RFKMethod()
			void	method1();

		protected:
			RFKMethod()
			void	method2() const;

			RFKMethod()
			int		method3();

			RFKMethod()
			static	int		staticMethod3(char const* param);

		private:
			RFKMethod()
			static	int	staticMethod2();

			RFKMethod()
			static void	staticMethod1();

			RFKMethod()
			static	float	staticMethod3(int param) noexcept;

			RFKMethod()
			float	method3(int i) noexcept;

		public:
			RFKMethod()
			ExampleClass()						= default;
			ExampleClass(ExampleClass const&)	= default;
			ExampleClass(ExampleClass&&)		= default;
			~ExampleClass()						= default;

		//GENERATED

		//static refureku::Type const& staticGetType() noexcept	
		//{	
		//	static bool				initialized = false;	
		//	static refureku::Type	type("ExampleClass", 6613751926088865067, static_cast<refureku::Type::ECategory>(0));	
		//	
		//	if (!initialized)	
		//	{
		//		//std::shared_ptr<refureku::MemberFunction<ExampleClass, int()>>(new refureku::MemberFunction<ExampleClass, int()>(&ExampleClass::method3));
		//		//std::shared_ptr<refureku::MemberFunction<ExampleClass, float(int)>>(new refureku::MemberFunction<ExampleClass, float(int)>(&ExampleClass::method3));

		//		//std::make_shared<refureku::MemberFunction<ExampleClass, int()>>(&ExampleClass::method3);
		//		//std::make_shared<refureku::MemberFunction<ExampleClass, float(int)>>(&ExampleClass::method3);

		//		initialized = true;	
		//	}	
		//		
		//	return type;	
		//}	
		//	
		//refureku::Type const& getType() const noexcept	
		//{	
		//	return ExampleClass::staticGetType();	
		//}

		//template <typename... ArgTypes>
		//static ExampleClass* instantiate(ArgTypes... args) noexcept
		//{
		//	return new ExampleClass(std::forward<ArgTypes>(args)...);
		//}
		static ExampleClass* __RFKinstantiate() noexcept
		{
			//std::cout << "Use this instantiate" << std::endl;
			return new ExampleClass();
		}

		RFKExampleClass_GENERATED
	};
}