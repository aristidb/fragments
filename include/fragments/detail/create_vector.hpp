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

#ifndef FRAGMENTS_DETAIL_CREATE_VECTOR_HPP
#define FRAGMENTS_DETAIL_CREATE_VECTOR_HPP

#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>

namespace fragments { namespace detail {
  template<typename Sequence, int Size>
  struct create_vector_impl;

  template<typename Sequence>
  struct create_vector {
    typedef
      typename create_vector_impl<
        Sequence,
        boost::mpl::size<Sequence>::value
      >::type type;
  };

  #define FRAGMENTS_DETAIL_ELEMENT_AT(z, n, t) \
    BOOST_PP_COMMA_IF(n) \
    typename boost::mpl::at_c<Sequence, n>::type

  #define FRAGMENTS_DETAIL_CREATE_VECTOR_IMPL(z, n, t) \
    template<typename Sequence> \
    struct create_vector_impl<Sequence, n> { \
      typedef BOOST_PP_CAT(boost::mpl::vector, n)< \
        BOOST_PP_REPEAT(n, FRAGMENTS_DETAIL_ELEMENT_AT, ~) \
      > type; \
    };

  BOOST_PP_REPEAT(
    BOOST_PP_INC(BOOST_MPL_LIMIT_VECTOR_SIZE),
    FRAGMENTS_DETAIL_CREATE_VECTOR_IMPL,
    ~
  )

  #undef FRAGMENTS_DETAIL_CREATE_VECTOR_IMPL
  #undef FRAGMENTS_DETAIL_ELEMENT_AT
}}

#endif
