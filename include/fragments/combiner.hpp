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

#ifndef FRAGMENTS_COMBINER_HPP
#define FRAGMENTS_COMBINER_HPP

#include <fragments/detail/find_fragment.hpp>
#include <fragments/detail/seq_of_elements.hpp>
#include <fragments/detail/create_vector.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/static_assert.hpp>
#include <boost/preprocessor.hpp>

namespace fragments {

namespace detail {

  template<typename Sequence>
  struct combine {
    typedef typename seq_of_elements<Sequence>::type sequence_;

    // DIAGNOSIS: fragments::combiner<> needs at least one fragment
    BOOST_STATIC_ASSERT(!boost::mpl::empty<sequence_>::value);

    typedef typename create_vector<sequence_>::type sequence;
    typedef typename boost::mpl::back<sequence>::type back;
    typedef typename find_fragment<sequence, back>::type type;
  };
}

template<
  BOOST_PP_ENUM_BINARY_PARAMS(
    BOOST_MPL_LIMIT_VECTOR_SIZE,
    typename Fragment,
    = boost::mpl::vector0<> BOOST_PP_INTERCEPT
  )
>
struct combiner
  : detail::combine<
      BOOST_PP_CAT(boost::mpl::vector, BOOST_MPL_LIMIT_VECTOR_SIZE)
      <
        BOOST_PP_ENUM_PARAMS(
          BOOST_MPL_LIMIT_VECTOR_SIZE,
          Fragment
        )
      >
    >::type
{
};

}

#endif
