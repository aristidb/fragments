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

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor.hpp>

namespace fragments {

namespace detail {
  template<
    typename Seq,
    typename Add
  >
  struct add_seqel {
    typedef typename boost::mpl::eval_if<
        boost::mpl::is_sequence<Add>,
        boost::mpl::insert_range<
          Seq,
          typename boost::mpl::end<Seq>::type,
          Add
        >,
        boost::mpl::push_back<
          Seq,
          Add
        >
      >::type type;
  };

  template<
    typename Seq,
    typename Prev = boost::mpl::vector0<>,
    bool empty = boost::mpl::empty<Seq>::value
  >
  struct seq_of_elements {
    typedef typename add_seqel<
        Prev,
        typename boost::mpl::front<Seq>::type
      >::type next;
    typedef typename seq_of_elements<
        typename boost::mpl::pop_front<Seq>::type,
        next
      >::type type;
  };

  template<
    typename Seq,
    typename Result
  >
  struct seq_of_elements<Seq, Result, true> {
    typedef Result type;
  };

  template<typename Sequence>
  struct nada {
    typedef Sequence fragments;
  };

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

  template<typename Sequence, typename Before>
  struct iterate_fragments {
    typedef Sequence sequence;
    typedef typename boost::mpl::front<sequence>::type first;

    typedef typename boost::mpl::pop_front<sequence>::type next_sequence_raw;
    typedef typename create_vector<next_sequence_raw>::type next_sequence;
    typedef typename first::template fragment<Before, next_sequence> second;
  };

  template<typename, typename> struct next_fragment;

  template<
    typename Sequence,
    typename Fragment,
    typename Before = detail::nada<Sequence>
  >
  struct find_fragment {
    typedef iterate_fragments<Sequence, Before> it;
    typedef
      typename boost::mpl::eval_if<
        boost::is_same<Fragment, typename it::first>,
        boost::mpl::identity<typename it::second>,
        next_fragment<it, Fragment>
      >::type type;
  };

  template<typename It, typename Fragment>
  struct next_fragment {
    typedef
      typename find_fragment<
        typename It::next_sequence,
        Fragment,
        typename It::second
      >::type type;
  };

  template<typename Sequence>
  struct combine {
    typedef typename create_vector<Sequence>::type sequence;
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
      typename detail::seq_of_elements<
        BOOST_PP_CAT(boost::mpl::vector, BOOST_MPL_LIMIT_VECTOR_SIZE)
        <
          BOOST_PP_ENUM_PARAMS(
            BOOST_MPL_LIMIT_VECTOR_SIZE,
            Fragment
          )
        >
      >::type
    >::type
{
};

}

#endif
