#include <fragments/concepts/resulting_concept.hpp>

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

int main() {
  // TODO ...
}
