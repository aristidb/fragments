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

#include <fragments/detail/reorder.hpp>

namespace {
  struct conceptA { };
  struct conceptB { };
  struct conceptC { };

  struct fragmentA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;

    typedef boost::mpl::vector1<conceptC> require_before;
  };

  struct fragmentB {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;

    typedef boost::mpl::vector1<conceptA> require_before;
  };

  struct fragmentC {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptC> concept;

    typedef boost::mpl::vector1<conceptB> require_before;
  };
}

int main() {
  typedef fragments::detail::reorder<
    boost::mpl::vector3<fragmentC, fragmentA, fragmentB>
      >::type result;
}
