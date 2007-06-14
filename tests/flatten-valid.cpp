#include <fragments/concepts/flatten.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

/*
  TODO
Problem: equivalent_sequence depends on order. Thus checks depend on the order
         of the result of flatten!
 */

template<typename SeqA,
         typename SeqB,
         bool emptyA = boost::mpl::empty<SeqA>::value,
         bool emptyB = boost::mpl::empty<SeqB>::value>
struct equivalent_sequence
  : boost::mpl::and_<
      typename boost::is_same<
        typename boost::mpl::front<SeqA>::type,
        typename boost::mpl::front<SeqB>::type
      >::type,
      typename equivalent_sequence<
        typename boost::mpl::pop_front<SeqA>::type,
        typename boost::mpl::pop_front<SeqB>::type
     >::type>
{ };

template<typename SeqA,
         typename SeqB>
struct equivalent_sequence<SeqA, SeqB, true, true>
  : boost::mpl::true_
{ };

template<typename SeqA,
         typename SeqB,
         bool x>
struct equivalent_sequence<SeqA, SeqB, x, true>
  : boost::mpl::false_
{ };

template<typename SeqA,
         typename SeqB,
         bool x>
struct equivalent_sequence<SeqA, SeqB, true, x>
  : boost::mpl::false_
{ };

#define CHECK(X, expect)                                           \
  {                                                                \
    bool b = equivalent_sequence<X, expect>::value;                \
    ret = ret && b;                                                \
    if(!b)                                                         \
      std::cerr << __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) ":"   \
        " FAILED: " BOOST_PP_STRINGIZE(X) " not equaivalent to "   \
        BOOST_PP_STRINGIZE(expect) << '\n';                        \
  }

namespace {
  struct conceptA { };

  struct conceptiA {
    typedef boost::mpl::vector1<conceptA> implies;
  };

  struct conceptiAiA {
    typedef boost::mpl::vector1<conceptiA> implies;
  };

  struct concept0 {
    typedef boost::mpl::vector0< > implies;
  };
}

int main() {
  bool ret = true;

  {
    typedef boost::mpl::vector1<conceptA> seq;
    CHECK(fragments::concepts::flatten<seq>::type,
          boost::mpl::vector1<conceptA>)
  }

  {
    typedef boost::mpl::vector1<concept0> seq;
    CHECK(fragments::concepts::flatten<seq>::type,
          boost::mpl::vector1<concept0>)
  }

  {
    typedef boost::mpl::vector0< > seq;
    CHECK(fragments::concepts::flatten<seq>::type,
          seq)
  }

  {
    typedef boost::mpl::vector1<conceptiA> seq;
    typedef boost::mpl::vector2<conceptA, conceptiA> expected;
    CHECK(fragments::concepts::flatten<seq>::type,
          expected)
  }

  {
    typedef boost::mpl::vector1<conceptiAiA> seq;
    typedef boost::mpl::vector3<conceptA, conceptiA, conceptiAiA> expected;
    CHECK(fragments::concepts::flatten<seq>::type,
          expected)
  }

  // ...

  return ret ? 0 : 1;
}
