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

#ifndef FRAGMENTS_DETAIL_VISIBLE_FRAGMENTS_HPP
#define FRAGMENTS_DETAIL_VISIBLE_FRAGMENTS_HPP

#include <fragments/detail/create_vector.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/iterator_range.hpp>

namespace fragments { namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF(shadow)

template<typename T, bool = has_shadow<T>::value>
struct test_shadow
  : T::shadow
{};

template<typename T>
struct test_shadow<T, false>
  : boost::mpl::false_
{};

template<
  typename Seq,
  typename First = typename boost::mpl::begin<Seq>::type,
  typename Last = typename boost::mpl::end<Seq>::type
>
struct visible_fragments {
  typedef typename boost::mpl::deref<First>::type first;

  typedef typename boost::mpl::next<First>::type next;

  typedef typename boost::mpl::eval_if<
      test_shadow<first>,
      visible_fragments<
        boost::mpl::iterator_range<First, Last>,
        next,
        Last
      >,
      visible_fragments<
        Seq,
        next,
        Last
      >
    >::type type;
};

template<typename Seq, typename End>
struct visible_fragments<Seq, End, End> {
  typedef typename create_vector<Seq>::type type;
};

}}

#endif
