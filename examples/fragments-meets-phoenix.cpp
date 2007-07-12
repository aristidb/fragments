#include <fragments/combiner.hpp>
using namespace fragments;

#include <boost/spirit/phoenix.hpp>
using namespace phoenix;

#include <boost/mpl/vector.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>

namespace fragment {
  namespace concept {
    struct functor { };

    struct functor_1 { typedef boost::mpl::vector1<functor> implies; };
  }

  struct printer {
    template<typename Before, typename After>
    struct fragment : Before {
      template<typename T>
      struct result {
        typedef void type;
      };

      template<typename T>
      void operator()(T const &o) {
        std::cout << o << '\n';
      }
    };

    typedef boost::mpl::vector1<concept::functor_1> concept;
  };

  namespace concept {
    struct iterate { };
  }

  struct for_each {
    template<typename Before, typename After>
    struct fragment : Before {
      template<typename A, typename B>
      struct result {
        typedef void type;
      };

      template<typename A, typename B>
      void operator()(A begin, B end) {
        for(A i = begin; i != end; ++i)
          Before::operator()(*i);
      }
    };

    typedef boost::mpl::vector1<concept::functor_1> require_before;
    typedef boost::mpl::vector1<concept::iterate> concept;
  };
}

int main() {

  std::vector<int> foo;
  for(int i = 0; i < 10; ++i) {
    foo.push_back(i);
  }

  typedef combiner<fragment::for_each, fragment::printer> print_each_impl;
  function<print_each_impl> const print_each = print_each_impl();

  std::vector<int>::const_iterator begin = foo.begin();
  std::vector<int>::const_iterator end = foo.end();
  print_each(arg1, arg2)(begin, end);
}
