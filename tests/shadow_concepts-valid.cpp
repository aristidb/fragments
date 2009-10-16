// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:

// Copyright (c) 2007,2009 Aristid Breitkreuz, Ruediger Sonderfeld, Christian Uhrhan
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

#include <fragments/combiner.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

struct conceptA { };

struct fragmentA {
  template<typename Before>
  struct fragment : public Before { };

  typedef boost::mpl::vector1<conceptA> concept;
};

struct fragmentB {
  template<typename Before>
  struct fragment {
    typedef typename Before::access access;
  };

  typedef boost::mpl::vector1<conceptA> require_before;
  typedef boost::mpl::bool_<true> shadow;
  typedef boost::mpl::vector0<conceptA> concept;
};

struct fragmentC {
  template<typename Before>
  struct fragment : public Before { };

  typedef boost::mpl::vector1<conceptA> require_before;
  typedef boost::mpl::vector0<> concept;
};

int main() {
  fragments::combiner<fragmentA, fragmentB, fragmentC> x;
}
