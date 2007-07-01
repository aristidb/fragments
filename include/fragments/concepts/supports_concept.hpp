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

#ifndef FRAGMENTS_CONCEPTS_SUPPORTS_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_SUPPORTS_CONCEPT_HPP

#include "detail/has_implies.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/deref.hpp>

namespace fragments {
namespace concepts {

namespace detail {
  template<typename, typename, typename>
  struct checker2;

  template<
    typename Seq,
    typename Concept
  >
  struct checker {
    typedef typename checker2<
        typename boost::mpl::begin<Seq>::type,
        typename boost::mpl::end<Seq>::type,
        Concept
      >::type type;
  };

  template<
    typename Concept,
    typename CheckedConcept,
    bool = has_implies<CheckedConcept>::value
  >
  struct implies_checker {
    typedef typename checker<
        typename CheckedConcept::implies,
        Concept
      >::type type;
  };

  template<typename Concept, typename CheckedConcept>
  struct implies_checker<Concept, CheckedConcept, false> {
    typedef boost::mpl::false_ type;
  };

  template<
    typename First,
    typename Last,
    typename Concept
  >
  struct checker2 {
    typedef typename boost::mpl::deref<First>::type checked_concept;

    typedef typename boost::mpl::next<First>::type next;

    typedef typename boost::mpl::eval_if<
        boost::mpl::bool_<(
          boost::is_same<
            checked_concept,
            Concept
          >::type::value
          ||
          implies_checker<
            Concept,
            checked_concept
          >::type::value
        )>,
        boost::mpl::true_,
        checker2<
          next,
          Last,
          Concept
        >
      >::type type;
  };

  template<typename End, typename Concept>
  struct checker2<End, End, Concept> {
    typedef boost::mpl::false_ type;
  };
}

template<typename FragmentClass, typename Concept>
struct supports_concept
  : boost::mpl::eval_if<
      boost::is_same<FragmentClass, Concept>,
      boost::mpl::true_,
      detail::checker<typename FragmentClass::concept, Concept>
    >::type
{ };

}}

#endif
