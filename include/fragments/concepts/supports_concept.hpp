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

#ifndef FRAGMENTS_CONCEPTS_SUPPORTS_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_SUPPORTS_CONCEPT_HPP

#include "detail/has_implies.hpp"

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>

namespace fragments {
namespace concepts {

namespace detail {
  template<typename FragmentSeq, typename Concept,
           bool empty = boost::mpl::empty<FragmentSeq>::value>
  struct checker {
    typedef typename boost::mpl::front<FragmentSeq>::type checked_concept;

    template<typename CheckedConcept, bool = has_implies<CheckedConcept>::value>
    struct implies {
      static bool const value =
        checker<typename CheckedConcept::implies, Concept>::value;
    };

    template<typename CheckedConcept>
    struct implies<CheckedConcept, false> {
      static bool const value = false;
    };

    static bool const value =
      boost::is_same<checked_concept, Concept>::value ?
      true :
      ( implies<checked_concept>::value ?
        true :
        checker<
          typename boost::mpl::pop_front<FragmentSeq>::type,
          Concept
        >::value);
  };

  template<typename FragmentSeq, typename Concept>
  struct checker<FragmentSeq, Concept, true> {
    static bool const value = false;
  };
}

template<typename FragmentClass, typename Concept>
struct supports_concept
  : boost::integral_constant<
      bool,
      detail::checker<typename FragmentClass::concept, Concept>::value
    > 
{ };

}}

#endif
