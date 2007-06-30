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

#include <fragments/detail/apply_default_fragments.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

using namespace boost;

struct container {};

struct vector {
  typedef mpl::vector1<container> concept;
};

struct list {
  typedef mpl::vector1<container> concept;
};

struct stack {
  typedef mpl::vector0<> concept;
  typedef mpl::vector1<container> require_before;
  typedef mpl::map1<
      mpl::pair<
        container,
        vector
      >
    > default_fragment;
};

int main() {
  {
    typedef mpl::vector1<stack> seq;
    typedef fragments::detail::apply_default_fragments<seq>::type res;
    BOOST_STATIC_ASSERT((mpl::size<res>::value == 2));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 0>::type, stack>::value));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 1>::type, vector>::value));
  }
  {
    typedef mpl::vector2<stack, list> seq;
    typedef fragments::detail::apply_default_fragments<seq>::type res;
    BOOST_STATIC_ASSERT((mpl::size<res>::value == 2));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 0>::type, stack>::value));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 1>::type, list>::value));
  }
  {
    typedef mpl::vector2<list, stack> seq;
    typedef fragments::detail::apply_default_fragments<seq>::type res;
    BOOST_STATIC_ASSERT((mpl::size<res>::value == 2));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 0>::type, list>::value));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 1>::type, stack>::value));
  }
  {
    typedef mpl::vector2<stack, vector> seq;
    typedef fragments::detail::apply_default_fragments<seq>::type res;
    BOOST_STATIC_ASSERT((mpl::size<res>::value == 2));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 0>::type, stack>::value));
    BOOST_STATIC_ASSERT((is_same<mpl::at_c<res, 1>::type, vector>::value));
  }
}
