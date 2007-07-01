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

#include <fragments/concepts/supports_concept.hpp>

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

  struct fragmentB {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
  };

  struct fragmentBA {
    template<typename Before>
    struct fragment : public Before { };

    typedef boost::mpl::vector2<conceptB, conceptA> concept;
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
  CHECK((fragments::concepts::supports_concept<fragmentA, conceptA>::value),
        true);
  CHECK((fragments::concepts::supports_concept<fragmentA, conceptB>::value),
        false);
  CHECK((fragments::concepts::supports_concept<fragmentBA, conceptA>::value),
        true);
  CHECK((fragments::concepts::supports_concept<fragmentBA, conceptB>::value),
        true);
  CHECK((fragments::concepts::supports_concept<fragmentB, conceptA>::value),
        false);
  CHECK((fragments::concepts::supports_concept<fragment0, conceptA>::value),
        false);

  CHECK((fragments::concepts::supports_concept<fragmentiA, conceptA>::value),
        true);
  CHECK((fragments::concepts::supports_concept<fragmentBiA, conceptA>::value),
        true);
  CHECK((fragments::concepts::supports_concept<fragmentBiAA, conceptA>::value),
        true);
  CHECK((fragments::concepts::supports_concept<fragmentiA, conceptB>::value),
        false);
  return ret ? 0 : 1;
}
