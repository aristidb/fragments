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

#include <fragments/concepts/check_concepts.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

#define CHECK(X, expect)                                           \
  ret = ret && ((X) == (expect));                                  \
  if((X) != (expect))                                              \
    std::cerr << __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) ":"     \
      " FAILED: " BOOST_PP_STRINGIZE(X) " not equal to "           \
              << expect << '\n';

namespace {
  struct conceptA { };

  struct fragmentA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;
  };

  struct conceptB { };

  struct fragmentBrbA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
    typedef boost::mpl::vector1<conceptA> require_before;
  };

  struct fragmentBraA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
    typedef boost::mpl::vector1<conceptA> require_after;
  };

  struct fragmentBrA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
    typedef boost::mpl::vector1<conceptA> require;
  };

  struct conceptiA {
    typedef boost::mpl::vector1<conceptA> implies;
  };

  struct fragmentiA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptiA> concept;
  };

  struct fragmentBiA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector2<conceptB, conceptiA> concept;
  };

  struct fragmentBiAA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector3<conceptB, conceptiA, conceptA> concept;
  };

  struct fragment0 {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector0< > concept;
  };
}

int main() {
  bool ret = true;
  {
    typedef boost::mpl::vector2<fragmentA, fragmentBrA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  {
    typedef boost::mpl::vector2<fragmentA, fragmentBrbA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  {
    typedef boost::mpl::vector2<fragmentBraA, fragmentA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  {
    typedef boost::mpl::vector3<fragmentA, fragmentBraA, fragmentA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  {
    typedef boost::mpl::vector2<fragmentA, fragmentBraA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), false);
  }

  {
    typedef boost::mpl::vector2<fragmentBrbA, fragmentA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), false);
  }

  {
    typedef boost::mpl::vector1<fragmentBrbA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), false);
  }

  {
    typedef boost::mpl::vector0< > seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  {
    typedef boost::mpl::vector1<fragment0> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  {
    typedef boost::mpl::vector1<fragmentA> seq;
    CHECK((fragments::concepts::check_concepts<seq>::value), true);
  }

  // ...

  return ret ? 0 : 1;
}
