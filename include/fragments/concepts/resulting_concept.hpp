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

#ifndef FRAGMENTS_CONCEPTS_RESULTING_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_RESULTING_CONCEPT_HPP

#include "detail.hpp"

#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/or.hpp>

namespace fragments {
namespace concepts {
  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(shadow)

    template<typename Concept, bool x = has_shadow<Concept>::value>
    struct is_shadowing
      : Concept::shadow
    { };

    template<typename Concept>
    struct is_shadowing<Concept, false>
      : boost::mpl::false_
    { };

    template<typename ConceptSeq,
             typename Iterator = typename boost::mpl::begin<ConceptSeq>::type>
    struct is_shadowing_concept_seq;

    template<typename Concept, bool x = has_implies<Concept>::value>
    struct is_shadowing_implies
      : is_shadowing_concept_seq<typename Concept::implies>
    { };

    template<typename Concept>
    struct is_shadowing_implies<Concept, false>
      : boost::mpl::false_
    { };

    template<typename ConceptSeq, typename Iterator>
    struct is_shadowing_concept_seq
    : boost::mpl::eval_if<
        is_shadowing<
          typename boost::mpl::deref<Iterator>::type
        >,
        boost::mpl::true_,
        boost::mpl::or_<
          typename is_shadowing_implies<
            typename boost::mpl::deref<Iterator>::type
          >::type,
          typename is_shadowing_concept_seq<
            ConceptSeq,
            typename boost::mpl::next<Iterator>::type
          >::type
        >
      >::type
    { };
    
    template<typename ConceptSeq>
    struct is_shadowing_concept_seq
           <
             ConceptSeq,
             typename boost::mpl::end<ConceptSeq>::type
           >
      : boost::mpl::false_
    { };

    template<typename Concept>
    struct is_shadowing_concept
      : boost::mpl::or_<
      typename is_shadowing<Concept>::type,
      typename is_shadowing_implies<Concept>::type
        >::type
    { };
    
    template<
      typename Sequence,
      typename Iterator = typename boost::mpl::begin<Sequence>::type
    >
    struct do_shadowing {
      typedef typename
        boost::mpl::eval_if<
          is_shadowing_concept<typename boost::mpl::deref<Iterator>::type>,
          do_shadowing<
            boost::mpl::iterator_range<
              Iterator,
              typename boost::mpl::end<Sequence>::type
            >,
            typename boost::mpl::next<Iterator>::type
          >,
          do_shadowing<
            Sequence,
            typename boost::mpl::next<Iterator>::type
          >
        >::type type;
    };
    
    template<typename Sequence>
    struct do_shadowing<
      Sequence,
      typename boost::mpl::end<Sequence>::type
    >
    {
      typedef Sequence type;
    };

    template<typename FragmentSeq,
             bool empty = boost::mpl::empty<FragmentSeq>::value >
    struct get_list_of_concepts {
      typedef typename concat<
          typename boost::mpl::front<FragmentSeq>::type::concept,
          typename get_list_of_concepts<
            typename boost::mpl::pop_front<FragmentSeq>::type
          >::type
        >::type
        type;
    };

    template<typename FragmentSeq>
    struct get_list_of_concepts<FragmentSeq, true> {
      typedef FragmentSeq type;
    };
  }
  template<typename FragmentSeq>
  struct resulting_concept {
    typedef typename flatten<
      typename detail::do_shadowing<
        typename detail::get_list_of_concepts<FragmentSeq>::type
        >::type
      >::type
    type;
  };
}}

#endif
