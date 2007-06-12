#ifndef PPTEST
#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/type_traits/is_same.hpp>
#endif
#include <boost/preprocessor.hpp>

#ifndef FRAGMENT_LIMIT
#define FRAGMENT_LIMIT 5
#endif

namespace detail {
  template<typename Sequence>
  struct nada {
    typedef Sequence fragments;
  };

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

  BOOST_PP_REPEAT(BOOST_PP_INC(FRAGMENT_LIMIT), CREATE_VECTOR_IMPL, ~)

  #undef CREATE_VECTOR_IMPL
  #undef ELEMENT_AT

  template<typename Sequence, typename Before>
  struct iterate_fragments {
    typedef Sequence sequence;
    typedef typename boost::mpl::front<sequence>::type first;

    typedef typename boost::mpl::pop_front<sequence>::type next_sequence_raw;
    typedef typename create_vector<next_sequence_raw>::type next_sequence;
    typedef typename first::template fragment<Before, next_sequence> second;
  };

  template<typename, typename> struct next_fragment;

  template<typename Sequence, typename Fragment, typename Before = detail::nada<Sequence> >
  struct find_fragment {
    typedef iterate_fragments<Sequence, Before> it;
    typedef
      typename boost::mpl::eval_if<
        boost::is_same<Fragment, typename it::first>,
        boost::mpl::identity<typename it::second>,
        next_fragment<it, Fragment>
      >::type type;
  };

  template<typename It, typename Fragment>
  struct next_fragment {
    typedef
      typename find_fragment<
        typename It::next_sequence,
        Fragment,
        typename It::second
      >::type type;
  };

  template<typename Sequence>
  struct combine {
    typedef typename create_vector<Sequence>::type sequence;
    typedef typename boost::mpl::back<sequence>::type back;
    typedef typename find_fragment<sequence, back>::type type;
  };
}

template<typename Sequence>
struct combiner
  : detail::combine<Sequence>::type
{
  // typedef typename resulting_concept<Sequence>::concept concept;
};

template<typename T, typename H>
typename detail::find_fragment<typename H::fragments, T>::type &
as_fragment(H &c) {
  return *static_cast<
      typename detail::find_fragment<typename H::fragments, T>::type *
    >(&c);
}

template<typename T, typename H>
typename detail::find_fragment<typename H::fragments, T>::type const &
as_fragment(H const &c) {
  return *static_cast<
      typename detail::find_fragment<typename H::fragments, T>::type const *
    >(&c);
}

template<typename T, typename Fragment>
struct with_fragment
: boost::mpl::contains<typename T::fragments, Fragment>::type {};

template<typename Fragment, typename T>
bool has_fragment(T const &) {
  return with_fragment<T, Fragment>::value;
}

template<typename T>
struct type {
  template<typename Before, typename After>
  struct fragment : public Before {
    typedef T value_type;
  };
};

template<typename Adapt>
struct adaptor {
  template<typename Before, typename After>
  struct fragment : public Before, public Adapt { };
};

// EXAMPLE:
struct array {
  template<typename Before, typename After>
  struct fragment : public Before {
    void push_back(typename Before::value_type const &) {
      std::cout << "PUSH_BACK\n";
    }
  };
};

struct simple_storage {
  template<typename Before, typename After>
  struct fragment : public Before {
  protected:
    typename Before::value_type storage;
  };
};

struct setter_interface {
  template<typename Before, typename After>
  struct fragment : public Before {
    void set(typename Before::value_type const &n) {
      Before::storage = n;
    }
  };
};

struct getter_x2_interface {
  template<typename Before, typename After>
  struct fragment : public Before {
    typename Before::value_type get() const {
      return Before::storage * 2;
    }
  };
};

struct getter_interface {
  template<typename Before, typename After>
  struct fragment : public Before {
    typename Before::value_type get() const {
      //return Before::storage;
      return as_fragment<getter_x2_interface>(*this).get() / 2;
    }
  };
};

int main() {
  std::cout << "_Z6__type" << typeid(detail::combine<boost::mpl::vector2<type<int>, array> >::type).name() << '\n';
  combiner<boost::mpl::vector2<type<int>, array> > foo;
  foo.push_back(10);

  combiner<boost::mpl::vector5<type<int>, simple_storage, setter_interface, getter_interface, getter_x2_interface> > bar;

  bar.set(99);
  std::cout << bar.get() << '\n';

  std::cout << as_fragment<getter_interface>(bar).get() << '\n';
  std::cout << as_fragment<getter_interface>(as_fragment<getter_x2_interface>(bar)).get() << '\n';
  std::cout << as_fragment<getter_x2_interface>(as_fragment<type<int> >(bar)).get() << '\n';

  std::cout << has_fragment<type<double> >(bar) << ',' << has_fragment<type<int> >(bar) << std::endl;

  //as_fragment<type<double> >(bar); //funny error message

  std::cout << "sizes: " << sizeof(foo) << ", " << sizeof(bar) << std::endl;
}
