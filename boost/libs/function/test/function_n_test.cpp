// Boost.Function library

// Copyright (C) 2001 Doug Gregor (gregod@cs.rpi.edu)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org

#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>
#include <boost/function.hpp>
#include <functional>
#include <cassert>
#include <string>

using namespace boost;
using std::string;
using std::negate;

int global_int;

struct write_five_obj { void operator()() const { global_int = 5; } };
struct write_three_obj { int operator()() const { global_int = 3; return 7; }};
static void write_five() { global_int = 5; }
static void write_three() { global_int = 3; }
struct generate_five_obj { int operator()() const { return 5; } };
struct generate_three_obj { int operator()() const { return 3; } };
static int generate_five() { return 5; }
static int generate_three() { return 3; }
static string identity_str(const string& s) { return s; }
static string string_cat(const string& s1, const string& s2) { return s1+s2; }
static int sum_ints(int x, int y) { return x+y; }

struct write_const_1_nonconst_2
{
  void operator()() { global_int = 2; }
  void operator()() const { global_int = 1; }
};

struct add_to_obj
{
  add_to_obj(int v) : value(v) {}

  int operator()(int x) const { return value + x; }

  int value;
};

static void
test_zero_args()
{
  typedef function0<void> func_void_type;

  write_five_obj five;
  write_three_obj three;

  // Default construction
  func_void_type v1;
  BOOST_TEST(v1.empty());

  // Assignment to an empty function 
  v1 = five;
  BOOST_TEST(!v1.empty());

  // Invocation of a function
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 5);

  // clear() method
  v1.clear();
  BOOST_TEST(v1.empty());

  // Assignment to an empty function
  v1 = three;
  BOOST_TEST(!v1.empty());

  // Invocation and self-assignment
  global_int = 0;
  v1 = v1;
  v1();
  BOOST_TEST(global_int == 3);

  // Assignment to a non-empty function
  v1 = five;

  // Invocation and self-assignment
  global_int = 0;
  v1.set(v1);
  v1();
  BOOST_TEST(global_int == 5);

  // clear()
  v1.clear();
  BOOST_TEST(v1.empty());

  // Assignment to an empty function from a free function
  v1 = write_five;
  BOOST_TEST(!v1.empty());

  // Invocation
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v1 = &write_three;
  BOOST_TEST(!v1.empty());

  // Invocation
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 3);

  // Assignment
  v1 = five;
  BOOST_TEST(!v1.empty());

  // Invocation
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v1 = write_three;
  BOOST_TEST(!v1.empty());

  // Invocation
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 3);

  // Construction from another function (that is empty)
  v1.clear();
  func_void_type v2(v1);
  BOOST_TEST(!v2);

  // Assignment to an empty function
  v2 = three;
  BOOST_TEST(!v2.empty());

  // Invocation
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 3);

  // Assignment to a non-empty function
  v2.set(five);

  // Invocation
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 5);

  v2.clear();
  BOOST_TEST(v2.empty());

  // Assignment to an empty function from a free function
  v2.set(&write_five);
  BOOST_TEST(v2);

  // Invocation
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v2 = &write_three;
  BOOST_TEST(!v2.empty());

  // Invocation
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 3);

  // Swapping
  v1 = five;
  swap(v1, v2);
  v2();
  BOOST_TEST(global_int == 5);
  v1();
  BOOST_TEST(global_int == 3);
  swap(v1, v2);
  v1.clear();

  // Assignment
  v2 = five;
  BOOST_TEST(!v2.empty());

  // Invocation
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v2 = &write_three;
  BOOST_TEST(!v2.empty());

  // Invocation
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 3);

  // Assignment to a function from an empty function
  v2 = v1;
  BOOST_TEST(v2.empty()); 
  
  // Assignment to a function from a function with a functor
  v1 = three;
  v2 = v1;
  BOOST_TEST(!v1.empty());
  BOOST_TEST(!v2.empty());

  // Invocation
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 3);
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 3);

  // Assign to a function from a function with a function
  v2 = &write_five;
  v1 = v2;
  BOOST_TEST(!v1.empty());
  BOOST_TEST(!v2.empty());
  global_int = 0;
  v1();
  BOOST_TEST(global_int == 5);
  global_int = 0;
  v2();
  BOOST_TEST(global_int == 5);

  // Construct a function given another function containing a function
  func_void_type v3(v1);

  // Invocation of a function
  global_int = 0;
  v3();
  BOOST_TEST(global_int == 5);
  
  // clear() method
  v3.clear();
  BOOST_TEST(!v3);

  // Assignment to an empty function
  v3 = three;
  BOOST_TEST(!v3.empty());

  // Invocation
  global_int = 0;
  v3();
  BOOST_TEST(global_int == 3);

  // Assignment to a non-empty function
  v3 = five;

  // Invocation
  global_int = 0;
  v3();
  BOOST_TEST(global_int == 5);

  // clear()
  v3.clear();
  BOOST_TEST(v3.empty());

  // Assignment to an empty function from a free function
  v3 = &write_five;
  BOOST_TEST(!v3.empty());

  // Invocation
  global_int = 0;
  v3();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v3 = &write_three;
  BOOST_TEST(!v3.empty());

  // Invocation
  global_int = 0;
  v3();
  BOOST_TEST(global_int == 3);

  // Assignment
  v3 = five;
  BOOST_TEST(!v3.empty());

  // Invocation
  global_int = 0;
  v3();
  BOOST_TEST(global_int == 5);

  // Construction of a function from a function containing a functor
  func_void_type v4(v3);

  // Invocation of a function
  global_int = 0;
  v4();
  BOOST_TEST(global_int == 5);
  
  // clear() method
  v4.clear();
  BOOST_TEST(v4.empty());

  // Assignment to an empty function
  v4 = three;
  BOOST_TEST(!v4.empty());

  // Invocation
  global_int = 0;
  v4();
  BOOST_TEST(global_int == 3);

  // Assignment to a non-empty function
  v4 = five;

  // Invocation
  global_int = 0;
  v4();
  BOOST_TEST(global_int == 5);

  // clear()
  v4.clear();
  BOOST_TEST(v4.empty());

  // Assignment to an empty function from a free function
  v4 = &write_five;
  BOOST_TEST(!v4.empty());

  // Invocation
  global_int = 0;
  v4();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v4 = &write_three;
  BOOST_TEST(!v4.empty());

  // Invocation
  global_int = 0;
  v4();
  BOOST_TEST(global_int == 3);

  // Assignment
  v4 = five;
  BOOST_TEST(!v4.empty());

  // Invocation
  global_int = 0;
  v4();
  BOOST_TEST(global_int == 5);

  // Construction of a function from a functor
  func_void_type v5(five);

  // Invocation of a function
  global_int = 0;
  v5();
  BOOST_TEST(global_int == 5);
  
  // clear() method
  v5.clear();
  BOOST_TEST(v5.empty());

  // Assignment to an empty function
  v5 = three;
  BOOST_TEST(!v5.empty());

  // Invocation
  global_int = 0;
  v5();
  BOOST_TEST(global_int == 3);

  // Assignment to a non-empty function
  v5 = five;

  // Invocation
  global_int = 0;
  v5();
  BOOST_TEST(global_int == 5);

  // clear()
  v5.clear();
  BOOST_TEST(v5.empty());

  // Assignment to an empty function from a free function
  v5 = &write_five;
  BOOST_TEST(!v5.empty());

  // Invocation
  global_int = 0;
  v5();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v5 = &write_three;
  BOOST_TEST(!v5.empty());

  // Invocation
  global_int = 0;
  v5();
  BOOST_TEST(global_int == 3);

  // Assignment
  v5 = five;
  BOOST_TEST(!v5.empty());

  // Invocation
  global_int = 0;
  v5();
  BOOST_TEST(global_int == 5);  

  // Construction of a function from a function
  func_void_type v6(&write_five);

  // Invocation of a function
  global_int = 0;
  v6();
  BOOST_TEST(global_int == 5);

  // clear() method
  v6.clear();
  BOOST_TEST(v6.empty());

  // Assignment to an empty function
  v6 = three;
  BOOST_TEST(!v6.empty());

  // Invocation
  global_int = 0;
  v6();
  BOOST_TEST(global_int == 3);

  // Assignment to a non-empty function
  v6 = five;

  // Invocation
  global_int = 0;
  v6();
  BOOST_TEST(global_int == 5);

  // clear()
  v6.clear();
  BOOST_TEST(v6.empty());

  // Assignment to an empty function from a free function
  v6 = &write_five;
  BOOST_TEST(!v6.empty());

  // Invocation
  global_int = 0;
  v6();
  BOOST_TEST(global_int == 5);

  // Assignment to a non-empty function from a free function
  v6 = &write_three;
  BOOST_TEST(!v6.empty());

  // Invocation
  global_int = 0;
  v6();
  BOOST_TEST(global_int == 3);

  // Assignment
  v6 = five;
  BOOST_TEST(!v6.empty());

  // Invocation
  global_int = 0;
  v6();
  BOOST_TEST(global_int == 5);  

  // Const vs. non-const
  write_const_1_nonconst_2 one_or_two;
  const function0<void> v7(one_or_two);
  function <void> v8(one_or_two);

  global_int = 0;
  v7();
  BOOST_TEST(global_int == 2);
  
  global_int = 0;
  v8();
  BOOST_TEST(global_int == 2);

  // Test return values
  typedef function0<int> func_int_type;
  generate_five_obj gen_five;
  generate_three_obj gen_three;

  func_int_type i0(gen_five);

  BOOST_TEST(i0() == 5);
  i0 = gen_three;
  BOOST_TEST(i0() == 3);
  i0 = &generate_five;
  BOOST_TEST(i0() == 5);
  i0 = &generate_three;
  BOOST_TEST(i0() == 3);
  BOOST_TEST(i0);
  i0.clear();
  BOOST_TEST(!i0);

  // Test return values with compatible types
  typedef function0<long> func_long_type;
  func_long_type i1(gen_five);

  BOOST_TEST(i1() == 5);
  i1 = gen_three;
  BOOST_TEST(i1() == 3);
  i1 = &generate_five;
  BOOST_TEST(i1() == 5);
  i1 = &generate_three;
  BOOST_TEST(i1() == 3);
  BOOST_TEST(i1);
  i1.clear();
  BOOST_TEST(!i1);
}

static void
test_one_arg()
{
  negate<int> neg;

  function1<int, int> f1(neg);
  BOOST_TEST(f1(5) == -5);

  function1<string, string> id(&identity_str);
  BOOST_TEST(id("str") == "str");

  function1<std::string, const char*> id2(&identity_str);
  BOOST_TEST(id2("foo") == "foo");

  add_to_obj add_to(5);
  function1<int, int> f2(add_to);
  BOOST_TEST(f2(3) == 8);

  const function1<int, int> cf2(add_to);
  BOOST_TEST(cf2(3) == 8);
}

static void
test_two_args()
{
  function2<string, const string&, const string&> cat(&string_cat);
  BOOST_TEST(cat("str", "ing") == "string");  

  function2<int, short, short> sum(&sum_ints);
  BOOST_TEST(sum(2, 3) == 5);
}

static void
test_emptiness()
{
  function0<float> f1;
  BOOST_TEST(f1.empty());

  function0<float> f2;
  f2 = f1;
  BOOST_TEST(f2.empty());

  function0<double> f3;
  f3 = f2;
  BOOST_TEST(f3.empty());
}

struct X {
  X(int v) : value(v) {}

  int twice() const { return 2*value; }
  int plus(int v) { return value + v; }

  int value;
};

static void
test_member_functions()
{

  boost::function1<int, X*> f1(&X::twice);
  
  X one(1);
  X five(5);

  BOOST_TEST(f1(&one) == 2);
  BOOST_TEST(f1(&five) == 10);

  boost::function1<int, X*> f1_2;
  f1_2 = &X::twice;

  BOOST_TEST(f1_2(&one) == 2);
  BOOST_TEST(f1_2(&five) == 10);

  boost::function2<int, X&, int> f2(&X::plus);
  BOOST_TEST(f2(one, 3) == 4);
  BOOST_TEST(f2(five, 4) == 9);
}

struct add_with_throw_on_copy {
  int operator()(int x, int y) const { return x+y; }

  add_with_throw_on_copy() {}

  add_with_throw_on_copy(const add_with_throw_on_copy&)
  {
    throw std::runtime_error("But this CAN'T throw");
  }

  add_with_throw_on_copy& operator=(const add_with_throw_on_copy&)
  {
    throw std::runtime_error("But this CAN'T throw");
  }
};

static void
test_ref()
{
  add_with_throw_on_copy atc;
  try {
    boost::function2<int, int, int> f(ref(atc));
    BOOST_TEST(f(1, 3) == 4);
  }
  catch(std::runtime_error e) {
    BOOST_ERROR("Nonthrowing constructor threw an exception");
  }
};

int test_main(int, char* [])
{
  test_zero_args();
  test_one_arg();
  test_two_args();
  test_emptiness();
  test_member_functions();
  test_ref();
  return 0;
}
