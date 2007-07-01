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

#ifndef FRAGMENTS_CONCEPTS_CHECK_CONCEPTS_HPP
#define FRAGMENTS_CONCEPTS_CHECK_CONCEPTS_HPP

#include "detail/has_require.hpp"
#include "has_concept.hpp"

#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/and.hpp>

namespace fragments {
namespace concepts {

namespace detail {
  template<
    typename Requirements,
    typename Sequence,
    bool empty = boost::mpl::empty<Requirements>::value
  >
  struct check_requirements
    : boost::mpl::and_<
    typename has_concept<
      Sequence,
      typename boost::mpl::front<Requirements>::type
      >::type,
    typename check_requirements<
      typename boost::mpl::pop_front<Requirements>::type,
      Sequence
      >::type
      >
  { };
    
  template<
    typename Requirements,
    typename Sequence
  >
  struct check_requirements<Requirements, Sequence, true>
    : boost::mpl::true_ { };

  template<typename Sequence, typename Iterator>
  struct check_requirements_all {
    typedef typename detail::check_requirements<
      typename boost::mpl::deref<Iterator>::type::require,
      Sequence
      >::type type;
};

template<typename Sequence, typename Iterator>
struct check_requirements_before {
  typedef typename detail::check_requirements<
    typename boost::mpl::deref<Iterator>::type::require_before,
    boost::mpl::iterator_range<
      typename boost::mpl::begin<Sequence>::type,
      Iterator
      >
    >::type type;
};

  template<typename Sequence, typename Iterator>
  struct check_requirements_after {
    typedef typename detail::check_requirements<
      typename boost::mpl::deref<Iterator>::type::require_after,
      boost::mpl::iterator_range<
        typename boost::mpl::next<Iterator>::type,
        typename boost::mpl::end<Sequence>::type
        >
    >::type type;
};
}

template<
  typename Sequence,
  typename Iterator = typename boost::mpl::begin<Sequence>::type
>
struct check_concepts;

namespace detail {
  template<typename Sequence, typename Iterator>
  struct check_concepts_impl
  {
    typedef boost::mpl::and_<
        check_concepts<
          Sequence,
          typename boost::mpl::next<Iterator>::type
        >,
        boost::mpl::eval_if<
          detail::has_require<
            typename boost::mpl::deref<Iterator>::type
          >,
          detail::check_requirements_all<
            Sequence,
            Iterator
          >,
          boost::mpl::true_
        >,
        boost::mpl::eval_if<
          detail::has_require_before<
            typename boost::mpl::deref<Iterator>::type
          >,
          detail::check_requirements_before<
            Sequence,
            Iterator
          >,
          boost::mpl::true_
        >,
        boost::mpl::eval_if<
          detail::has_require_after<
            typename boost::mpl::deref<Iterator>::type
          >,
          detail::check_requirements_after<
            Sequence,
            Iterator
          >,
          boost::mpl::true_
        >
      > type;
};
}

template<
  typename Sequence,
  typename Iterator
>
struct check_concepts
  : boost::mpl::eval_if<
      boost::is_same<
        Iterator,
        typename boost::mpl::end<Sequence>::type
      >,
      boost::mpl::true_,
      detail::check_concepts_impl<Sequence, Iterator>
  >::type
{ };

}}

#endif
