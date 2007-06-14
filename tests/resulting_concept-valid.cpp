// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:

// Copyright (c) 2007 Aristid Breitkreuz, Ruediger Sonderfeld
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

#include <fragments/concepts/resulting_concept.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/size.hpp>
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
  struct conceptiA { typedef boost::mpl::vector1<conceptA> implies; };
  struct conceptiAiA { typedef boost::mpl::vector1<conceptiA> implies; };
  struct conceptB { };
  struct conceptC { };

  struct fragmentA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;
  };

  struct fragmentAiA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector2<conceptA, conceptiA> concept;
  };

  struct fragmentiA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptiA> concept;
  };

  struct fragmentiAiA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptiAiA> concept;
  };

  struct fragmentB {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
  };

  struct fragmentC {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptC> concept;
  };

  struct fragment0 {
    template<typename Before, typename After>
    struct fragment : public Before { };
    
    typedef boost::mpl::vector0< > concept;
  };
}

int main() {
  bool ret = true;
  {
    typedef boost::mpl::vector0< > seq;
    typedef boost::mpl::vector0< > result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector1<fragment0> seq;
    typedef boost::mpl::vector0< > result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector1<fragmentA> seq;
    typedef boost::mpl::vector1<conceptA> result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector3<fragmentA, fragmentB, fragmentC> seq;
    typedef boost::mpl::vector3<conceptA, conceptB, conceptC> result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector1<fragmentiA> seq;
    typedef boost::mpl::vector2<conceptA, conceptiA> result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector3<fragmentB, fragmentiA, fragmentC> seq;
    typedef boost::mpl::vector4<conceptB, conceptA, conceptiA, conceptC> result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector1<fragmentiAiA> seq;
    typedef boost::mpl::vector3<conceptA, conceptiA, conceptiAiA> result;

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  {
    typedef boost::mpl::vector1<fragmentAiA> seq;
    typedef boost::mpl::vector3<conceptA, conceptA, conceptiA> result;

    /*
      should this be rather vector2<conceptA, conceptiA>?
     */

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }

  {
    typedef boost::mpl::vector2<fragmentA, fragmentiA> seq;
    typedef boost::mpl::vector3<conceptA, conceptA, conceptiA> result;

    /*
      should this be rather vector2<conceptA, conceptiA>?
     */

    CHECK(fragments::concepts::resulting_concept<seq>::type, result)
  }
  return ret ? 0 : 1;
}
