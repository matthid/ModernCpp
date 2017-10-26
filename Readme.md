Neu in C++11 https://en.wikipedia.org/wiki/C%2B%2B11


1. Templates

	- Nicht mehr wegzudenken
	- Performance
	- "Generics"

"With respect to C++, the syntax and idioms of template metaprogramming are esoteric compared to conventional C++ programming, and template metaprograms can be very difficult to understand. [6][7]"
 - https://en.wikipedia.org/wiki/Template_metaprogramming

	  - http://www.yolinux.com/TUTORIALS/Cpp-Templates.html
	  - http://en.cppreference.com/w/cpp/language/type_alias
	  - http://en.cppreference.com/w/cpp/language/template_argument_deduction
	  - 
	
	  a) static_assert (http://en.cppreference.com/w/cpp/language/static_assert)
	  b) type traits (http://en.cppreference.com/w/cpp/types)
	
	
2. Type inference
	 Auto & decltype (https://en.wikipedia.org/wiki/C%2B%2B11#constexpr_.E2.80.93_Generalized_constant_expressions)
	
3. Lambda functions & Expressions
	 [capture](parameters) -> return_type { function_body }
	 
	https://en.wikipedia.org/wiki/Anonymous_function#C.2B.2B_.28since_C.2B.2B11.29
	https://stackoverflow.com/questions/7627098/what-is-a-lambda-expression-in-c11
	
4. New function syntax
	 auto funcname(parameters) -> return_type { function_body }
	
	
5. Uniform initialization and Object construction
		- new type_name{}
		- delegate constructors (members, this(), super() via using)
	

6. Strong typed enumerations - Enum classes


7. Type/Template aliases
           using type_name = other_type<params> 

8. (Custom suffixes)

9. (= default and = delete)

10. (Attributes)

	int [[attr1]] i [[attr2, attr3]];
	
	[[attr4(arg1, arg2)]] if (cond)
	{
	    [[vendor::attr5]] return i;
	}

	[[deprecated("g() is thread-unsafe. Use h() instead")]] // C++ 14

11. Zusätzliche Konstrukturtypen: Move Constructor, inherited and delegating constructors
	https://blog.smartbear.com/c-plus-plus/c11-tutorial-introducing-the-move-constructor-and-the-move-assignment-operator/


12. CPP Core Guidelines
	https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md
	
13. Zukunft CPP = Boost Library
	http://www.boost.org/

14. RAII - Smart Pointer
		- http://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr

Ressourcen:
	- Workshop-Unterlagen (HLRS, interner Sharepoint Documents/Schulungsunterlagen)

	- https://github.com/matthid/ModernCpp (Code)
	- http://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
	- http://www.yolinux.com/TUTORIALS/Cpp-Templates.html 
	- http://en.cppreference.com/w/cpp/language/type_alias
	- http://en.cppreference.com/w/cpp/language/template_argument_deduction
	- [Microsoft "Welcome Back to C++ (Modern C++)"](https://msdn.microsoft.com/en-us/library/hh279654.aspx)
