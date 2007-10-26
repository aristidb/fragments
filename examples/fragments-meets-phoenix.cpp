// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:

// Copyright (c) 2007 Aristid Breitkreuz, Ruediger Sonderfeld, Christian Uhrhan
//
// Distributed under the Boost Software License, Version 1.0 and under the MIT
// License.
//
// See accompanying LICENSES.txt or
// <http://fragments.sourceforge.net/LICENSES.txt> or, for the Boost Software
// License, <http://www.boost.org/LICENSE_1_0.txt>.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <climits>

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

    struct functor_1 {
      typedef boost::mpl::vector1<functor> implies;
    };
  }

  struct printer {
    template<typename Before>
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
    struct iterate {
      typedef boost::mpl::vector1<functor> implies;
    };
  }

  namespace {
    template<typename T>
    struct is_void {
      static bool const value = false;
    };

    template<>
    struct is_void<void> {
      static bool const value = true;
    };

    template<typename Vector,
             bool empty = boost::mpl::empty<Vector>::value>
    struct last {
      typedef typename boost::mpl::back<Vector>::type type;
    };

    template<typename Vector>
    struct last<Vector, true> {
      typedef boost::mpl::void_ type;
    };
  }

  struct for_each {
    template<typename Before>
    struct fragment : private Before {
      typedef typename Before::access access;

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
    typedef boost::mpl::true_ shadow;
  };
}

int main() {
  std::vector<int> foo;
  for(int i = 0; i < 10; ++i) {
    foo.push_back(i);
  }
  
  typedef combiner2<fragment::for_each, fragment::printer> print_each_impl;
  function<print_each_impl> const print_each = print_each_impl();
  std::vector<int>::const_iterator begin = foo.begin();
  std::vector<int>::const_iterator end = foo.end();
  print_each(arg1, arg2)(begin, end);
}
