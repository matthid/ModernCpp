#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include <deque>
#include <stdexcept>
#include <memory>

#include <string>
#include <ios>
#include <type_traits>
#include <algorithm>


// Templates with compile-time constants
template <unsigned N>
struct Fibonacci
{
	enum
	{
		value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value
	};
};

// Template spezialisierung
template <> // weiter "offene" parameter hier
struct Fibonacci<1> // spezialisierte parameter hier
{
	enum
	{
		value = 1
	};
};

template <>
struct Fibonacci<0>
{
	enum
	{
		value = 0
	};
};

int compile_time_fib()
{
	printf("%d", Fibonacci<15>::value);
	return 0;
}


// Function ptr
void add1(int &v)
{
	v += 1;
}

void add2(int &v)
{
	v += 2;
}

template <void(*T)(int &)>
void doOperation()
{
	int temp = 0;
	T(temp);
	std::cout << "Result is " << temp << std::endl;
}

int functionPtrParameter()
{
	doOperation<add1>();
	doOperation<add2>();

	return 0;
}













// Type parameter
struct MyFuncs
{
	int smallInt() { return 1; }
	unsigned long bigULong() { return 1000; }
};

class OtherFuncs
{
public:
	unsigned long aNumber() { return 555; }
};

// types instead of compile time constants, "typename" tells the compiler that the following name is a "type"
template <typename Object, typename Func>
long long getNum(Object obj, Func getNumber)
{
	return (obj.*getNumber)();
}

int funcObjParameters()
{
	MyFuncs funcs;
	long long num1 = getNum(funcs, &MyFuncs::smallInt);
	long long num2 = getNum(funcs, &MyFuncs::bigULong);

	OtherFuncs other;
	long long num3 = getNum(other, &OtherFuncs::aNumber);

	//long long num4 = getNum(funcs, &OtherFuncs::aNumber);
	return 0;
}

















// type alias, identical to
// typedef std::ios_base::fmtflags flags;
using flags = std::ios_base::fmtflags;
// the name 'flags' now denotes a type:
flags fl = std::ios_base::dec;

// type alias, identical to
// typedef void (*func)(int, int);
using func = void(*) (int, int);
// the name 'func' now denotes a pointer to function:
void example(int, int) {}
func f = example;

// alias template
template<class T>
using ptr = T*;
// the name 'ptr<T>' is now an alias for pointer to T
ptr<int> x;

// type alias used to hide a template parameter 
template<class CharT>
using mystring = std::basic_string<CharT, std::char_traits<CharT>>;
mystring<char> str;

// type alias can introduce a member typedef name
template<typename T>
struct Container { using value_type = T; };
// which can be used in generic programming
template<typename Container>
void g(const Container& c)
{
	typename Container::value_type n;
}

// type alias used to simplify the syntax of std::enable_if
template<typename T>
using Invoke = typename T::type;
template<typename Condition>
using EnableIf = Invoke<std::enable_if<Condition::value>>;
template<typename T, typename = EnableIf<std::is_polymorphic<T>>>
int fpoly_only(T t) { return 1; }

struct S { virtual ~S() {} };

int check_poly_only()
{
	Container<int> c;
	g(c); // Container::value_type will be int in this function
	//fpoly_only(c); // error: enable_if prohibits this
	S s;
	fpoly_only(s); // okay: enable_if allows this

	return 0;
}













// "Tricks" with new explicit, fix some C++ design mistakes
class Testable {
	bool ok_;
	typedef void (Testable::*bool_type)() const;
	void this_type_does_not_support_comparisons() const {}
public:
	// Don't allow implicit conversions
	explicit Testable(bool b = true) :ok_(b) {}

	operator bool_type() const {
		return ok_ ?
			&Testable::this_type_does_not_support_comparisons : 0;
	}
};
//template <typename T>
//bool operator!=(const Testable& lhs, const T&) {
//	lhs.this_type_does_not_support_comparisons();
//	return false;
//}
//template <typename T>
//bool operator==(const Testable& lhs, const T&) {
//	lhs.this_type_does_not_support_comparisons();
//	return false;
//}

class AnotherTestable {
	bool ok_;
	typedef void (AnotherTestable::*bool_type)() const;
	bool_type t;
	void this_type_does_not_support_comparisons() const {}
public:
	explicit AnotherTestable(bool b = true) :ok_(b) {}

	operator bool_type() const {
		return ok_ ?
			&AnotherTestable::this_type_does_not_support_comparisons : 0;
	}
};
//template <typename T>
//bool operator!=(const AnotherTestable& lhs, const T&) {
//	lhs.this_type_does_not_support_comparisons();
//	return false;
//}
//template <typename T>
//bool operator==(const AnotherTestable& lhs, const T&) {
//	lhs.this_type_does_not_support_comparisons();
//	return false;
//}

int otherfunc(void)
{
	Testable t1;
	AnotherTestable t2;
	if (t1) {} // Works as expected
	if (t2) {} // Works as expected
	//if (t2 == t1) {} // Fails to compile
	//if (t1 < 0) {} // Fails to compile

	return 0;
}




typedef std::tuple <int, double, long &, const char *> test_tuple;






// Wir wollen "Funktionsobjekte" (wie clear) mit einem template "wrappen"
struct Clear {
	int    operator()(int) const { return 1; }    // The parameter type is
	double operator()(double) const { return 1; } // equal to the return type.
};

template <class Obj>
class Calculus {
public:
	// "template<class Arg> Arg" ist ein Problem
	template<class Arg> Arg operator()(Arg& a) const {
		return member(a);
	}
private:
	Obj member;
};

struct Confused {
	double operator()(int) const { return 1; }     // The parameter type is not
	int    operator()(double) const { return 1; }  // equal to the return type.
};

int testwarning(void)
{
	Calculus<Confused> conf; //-> C4244
							 //CalculusVer2<Confused> conf;
	int in = 1;
	double t = conf(in);

	return 0;
}

template< class Obj >
class CalculusVer2 {
public:
	template<class Arg>
	typename std::result_of<Obj(Arg)>::type operator()(Arg& a) const {
		return member(a);
	}
private:
	Obj member;
};

int testwarning_fixed(void)
{
	CalculusVer2<Confused> conf;
	int in = 1;
	double t = conf(in);

	return 0;
}

template< class Obj >
class CalculusVer3 {
private:
	Obj member;
public:
	template<class Arg>
	auto operator()(Arg& a) -> decltype(member(a)) const {
		return member(a);
	}
};

int testwarning_fixed2(void)
{
	CalculusVer3<Confused> conf;
	int in = 1;
	double t = conf(in);

	return 0;
}




// Template policy based programming
template <typename OutputPolicy, typename LanguagePolicy>
class HelloWorld : private OutputPolicy, private LanguagePolicy
{
	using OutputPolicy::print;
	using LanguagePolicy::message;

public:
	// Behaviour method
	void run() const
	{
		// Two policy methods
		print(message());
	}
};

class OutputPolicyWriteToCout
{
protected:
	template<typename MessageType>
	void print(MessageType const &message) const
	{
		std::cout << message << std::endl;
	}
};

class LanguagePolicyEnglish
{
protected:
	std::string message() const
	{
		return "Hello, World!";
	}
};

class LanguagePolicyGerman
{
protected:
	std::string message() const
	{
		return "Hallo Welt!";
	}
};

int policy_based_design()
{
	/* Example 1 */
	typedef HelloWorld<OutputPolicyWriteToCout, LanguagePolicyEnglish> HelloWorldEnglish;

	HelloWorldEnglish hello_world;
	hello_world.run(); // prints "Hello, World!"

					   /* Example 2
					   * Does the same, but uses another language policy */
	typedef HelloWorld<OutputPolicyWriteToCout, LanguagePolicyGerman> HelloWorldGerman;

	HelloWorldGerman hello_world2;
	hello_world2.run(); // prints "Hallo Welt!"

	return 0;
}






// nested parameter
template <typename T, template <typename> class TNEST> void nestedTypeParameter(TNEST<T>) {

}

// http://www.java2s.com/Tutorial/Cpp/0260__template/Nestedtemplatetype.htm

// stack template
template <typename T,
	template <typename ELEM,
		typename = std::allocator<ELEM> >
	class CONT = std::deque>
	class Stack {
	private:
		CONT<T> elems;         // elements

	public:
		void push(T const&);   // push element
		void pop();            // pop element
		T top() const;         // return top element
		bool empty() const {   // return whether the stack is empty
			return elems.empty();
		}

		// assign stack of elements of type T2
		template<typename T2,
			template<typename ELEM2,
			typename = std::allocator<ELEM2>
		>class CONT2>
			Stack<T, CONT>& operator= (Stack<T2, CONT2> const&);
};

template <typename T, template <typename, typename> class CONT>
void Stack<T, CONT>::push(T const& elem)
{
	elems.push_back(elem);    // append copy of passed elem
}

template<typename T, template <typename, typename> class CONT>
void Stack<T, CONT>::pop()
{
	if (elems.empty()) {
		throw std::out_of_range("Stack<>::pop(): empty stack");
	}
	elems.pop_back();         // remove last element
}

template <typename T, template <typename, typename> class CONT>
T Stack<T, CONT>::top() const
{
	if (elems.empty()) {
		throw std::out_of_range("Stack<>::top(): empty stack");
	}
	return elems.back();      // return copy of last element
}

template <typename T, template <typename, typename> class CONT>
template <typename T2, template <typename, typename> class CONT2>
Stack<T, CONT>&
Stack<T, CONT>::operator= (Stack<T2, CONT2> const& op2)
{
	if ((void*)this == (void*)&op2) {    // assignment to itself?
		return *this;
	}

	Stack<T2, CONT2> tmp(op2);        // create a copy of the assigned stack

	elems.clear();                   // remove existing elements
	while (!tmp.empty()) {           // copy all elements
		elems.push_front(tmp.top());
		tmp.pop();
	}
	return *this;
}


int nested_templates()
{
	try {
		Stack<int>   intStack;       // stack of ints
		Stack<float> floatStack;     // stack of floats

									 // manipulate int stack
		intStack.push(42);
		intStack.push(7);

		// manipulate float stack
		floatStack.push(7.7f);

		// assign stacks of different type
		floatStack = intStack;

		// print float stack
		std::cout << floatStack.top() << std::endl;
		floatStack.pop();
		std::cout << floatStack.top() << std::endl;
		floatStack.pop();
		std::cout << floatStack.top() << std::endl;
		floatStack.pop();
	}
	catch (std::exception const& ex) {
		std::cerr << "Exception: " << ex.what() << std::endl;
	}

	// stack for ints using a vector as an internal container
	Stack<int, std::vector> vStack;
	//...
	vStack.push(42);
	vStack.push(7);
	std::cout << vStack.top() << std::endl;
	vStack.pop();

	return 0;
}













// Smart Pointer

struct Foo { // object to manage
	Foo() { std::cout << "Foo ctor\n"; }
	Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
	Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
	~Foo() { std::cout << "~Foo dtor\n"; }
};

// See http://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr
int smart_pointer ()
{
	{
		std::unique_ptr<Foo> ptr(new Foo);
		// use foo
	} // foo is deleted

	{
		// https://www.codeproject.com/Articles/820931/Using-std-unique-ptr-RAII-with-malloc-and-free
		auto data =
			std::unique_ptr<double, decltype(free)*> {
			reinterpret_cast<double*>(malloc(sizeof(double) * 50)),
			free };
		double* ptr = data.get();
		// Use Data/ptr

	} // Data is deleted

	return 0;
}



int reorder_unsafe() {
	//f(std::unique_ptr<Foo>(new Foo), std::unique_ptr<Foo>(new Foo));

	// RAII is the single most important idiom of the C++ programming language. Use it!

	
	return 0;
}













// Lambdas
void func5(std::vector<double>& v, const double& epsilon) {
	std::transform(v.begin(), v.end(), v.begin(),
		[=,&epsilon](double d) -> double {
		//[&](double d) -> double {
		//[=](double d) -> double {
		//[](double d) -> double {
		if (d < epsilon) {
			return 0;
		}
		else {
			return d;
		}
	});
}