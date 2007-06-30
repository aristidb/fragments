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

#ifndef FRAGMENTS_CONCEPTS_RESULTING_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_RESULTING_CONCEPT_HPP

#include "detail/has_implies.hpp"
#include "detail/concat.hpp"
#include "flatten.hpp"

#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/or.hpp>

namespace fragments { namespace concepts {
  namespace detail {
    template<typename FragmentSeq,
             bool empty = boost::mpl::empty<FragmentSeq>::value >
    struct get_list_of_concepts {
      typedef typename concat<
          typename boost::mpl::front<FragmentSeq>::type::concept,
          typename get_list_of_concepts<
            typename boost::mpl::pop_front<FragmentSeq>::type
          >::type
        >::type type;
    };

    template<typename FragmentSeq>
    struct get_list_of_concepts<FragmentSeq, true> {
      typedef FragmentSeq type;
    };
  }

  template<typename FragmentSeq>
  struct resulting_concept {
    typedef typename flatten<
        typename detail::get_list_of_concepts<FragmentSeq>::type
      >::type type;
  };
}}

#endif
