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
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/logical.hpp>

namespace fragments {
namespace concepts {

namespace detail {
  template<
    typename Seq,
    typename Concept,
    bool empty = boost::mpl::empty<Seq>::value
  >
  struct checker;

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

  template<typename Seq, typename Concept, bool /*false*/>
  struct checker {
    typedef typename boost::mpl::front<Seq>::type checked_concept;

    typedef typename boost::mpl::pop_front<Seq>::type next;

    typedef typename boost::mpl::apply1<
        boost::mpl::or_<
          boost::is_same<
            checked_concept,
            boost::mpl::_1
          >,
          implies_checker<
            boost::mpl::_1,
            checked_concept
          >,
          checker<
            next,
            boost::mpl::_1
          >
        >,
        Concept
      >::type type;
  };

  template<typename Seq, typename Concept>
  struct checker<Seq, Concept, true> {
    typedef boost::mpl::false_ type;
  };
}

template<typename FragmentClass, typename Concept>
struct supports_concept
  : detail::checker<typename FragmentClass::concept, Concept>::type
{ };

}}

#endif
