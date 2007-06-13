#ifndef PPTEST
#include <iostream>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#endif
#include <boost/preprocessor.hpp>
#include <typeinfo>

#ifndef L
#define L 20
#endif

template<
  typename Seq,
  typename Add
> struct next {
  typedef typename boost::mpl::eval_if<
      boost::mpl::is_sequence<Add>,
      boost::mpl::insert_range<
        Seq,
        typename boost::mpl::end<Seq>::type,
        Add
      >,
      boost::mpl::push_back<
        Seq,
        Add
      >
    >::type type;
};

template<
  BOOST_PP_ENUM_BINARY_PARAMS(
    L,
    typename P,
    = boost::mpl::vector0<> BOOST_PP_INTERCEPT
  )
>
struct combined {
  typedef boost::mpl::vector0<> type0;
  #define XNEXT(z,n,t) \
    typedef typename next< \
        BOOST_PP_CAT(type, n), \
        BOOST_PP_CAT(P, n) \
      >::type  BOOST_PP_CAT(type, BOOST_PP_INC(n)); \
      /**/
  BOOST_PP_REPEAT(L, XNEXT, ~)
  #undef XNEXT
  typedef BOOST_PP_CAT(type, L) type;
};

// ----
// create_vector from greatcontainer3
  template<typename Sequence, int Size>
  struct create_vector_impl;

  template<typename Sequence>
  struct create_vector {
    typedef
      typename create_vector_impl<Sequence, boost::mpl::size<Sequence>::value >::type
      type;
  };

  #define ELEMENT_AT(z, n, t) \
    BOOST_PP_COMMA_IF(n) \
    typename boost::mpl::at_c<Sequence, n>::type

  #define CREATE_VECTOR_IMPL(z, n, t) \
    template<typename Sequence> \
    struct create_vector_impl<Sequence, n> { \
      typedef BOOST_PP_CAT(boost::mpl::vector, n)< \
        BOOST_PP_REPEAT(n, ELEMENT_AT, ~) \
      > type; \
    };

  BOOST_PP_REPEAT(20, CREATE_VECTOR_IMPL, ~)

  #undef CREATE_VECTOR_IMPL
  #undef ELEMENT_AT
// ----

int main() {
  typedef combined<int, char>::type x1;
  std::cout << "_Z1x" << typeid(create_vector<x1>::type).name() << std::endl;
}
