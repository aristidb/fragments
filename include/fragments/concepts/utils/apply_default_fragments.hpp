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

#ifndef FRAGMENTS_CONCEPTS_UTILS_APPLY_DEFAULT_FRAGMENTS_HPP
#define FRAGMENTS_CONCEPTS_UTILS_APPLY_DEFUALT_FRAGMENTS_HPP

#include "has_concept.hpp"

#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>

namespace fragments {
namespace concepts {
namespace utils {
  namespace detail {
    template<
      typename FragmentSeq,
      typename Concept,
      bool no_fragments_left = boost::mpl::empty<FragmentSeq>::value
    >
    struct get_matching_fragment {
      typedef typename boost::mpl::front<
          FragmentSeq
         >::type front;
      typedef typename boost::mpl::eval_if<
          supports_concept<front, Concept>,
          boost::mpl::identity<front>,
          get_matching_fragment<
            typename boost::mpl::pop_front<
              FragmentSeq
            >::type,
            Concept
          >
        >::type type;
    };

    template<
      typename FragmentSeq,
      typename Concept
    >
    struct get_matching_fragment<
      FragmentSeq,
      Concept,
      true
    >;

    template<
      typename Requirements,
      typename FragmentSeq,
      typename Fragment,
      typename Graph,
      template<typename, typename> class PairMaker,
      bool no_requirements_left = boost::mpl::empty<Requirements>::value
    >
    struct fragment_graph {
      typedef typename PairMaker<
          Fragment,
          typename get_matching_fragment<
            FragmentSeq,
            typename boost::mpl::front<Requirements>::type
          >::type
        >::type pair;
      typedef boost::mpl::pair<
          typename pair::first,
          typename boost::mpl::insert<
            typename boost::mpl::at<Graph, typename pair::first>::type,
            typename pair::second
          >::type
        > real_pair;
      typedef typename boost::mpl::insert<
          Graph,
          real_pair
        >::type graph;
      typedef typename fragment_graph<
          typename boost::mpl::pop_front<Requirements>::type,
          FragmentSeq,
          Fragment,
          graph,
          PairMaker
        >::type type;
    };

    template<
      typename Requirements,
      typename FragmentSeq,
      typename Fragment,
      typename Graph,
      template<typename, typename> class PairMaker
    >
    struct fragment_graph<
      Requirements,
      FragmentSeq,
      Fragment,
      Graph,
      PairMaker,
      true
    > {
      typedef Graph type;
    };

    template<typename From, typename To>
    struct before_pair {
      typedef boost::mpl::pair<From, To> type;
    };

    template<typename To, typename From>
    struct after_pair {
      typedef boost::mpl::pair<From, To> type;
    };

    template<
      typename FragmentSeq,
      typename Fragment,
      typename Graph,
      bool has_require_before = has_require_before<Fragment>::value
    >
    struct fragment_graph_before {
      typedef typename fragment_graph<
          typename Fragment::require_before,
          FragmentSeq,
          Fragment,
          Graph,
          before_pair
        >::type type;
    };

    template<typename FragmentSeq, typename Fragment, typename Graph>
    struct fragment_graph_before<FragmentSeq, Fragment, Graph, false> {
      typedef Graph type;
    };

    template<
      typename FragmentSeq,
      typename Fragment,
      typename Graph,
      bool has_require_after = has_require_after<Fragment>::value
    >
    struct fragment_graph_after {
      typedef typename fragment_graph<
          typename Fragment::require_after,
          FragmentSeq,
          Fragment,
          Graph,
          after_pair
        >::type type;
    };

    template<typename FragmentSeq, typename Fragment, typename Graph>
    struct fragment_graph_after<FragmentSeq, Fragment, Graph, false> {
      typedef Graph type;
    };

    template<
      typename FragmentSeq,
      bool empty = boost::mpl::empty<FragmentSeq>::value
    >
    struct init_graph {
      typedef typename boost::mpl::insert<
          typename init_graph<
            typename boost::mpl::pop_front<FragmentSeq>::type
          >::type,
          boost::mpl::pair<
            typename boost::mpl::front<FragmentSeq>::type,
            boost::mpl::set0<>
          >
        >::type type;
    };

    template<typename FragmentSeq>
    struct init_graph<FragmentSeq, true> {
      typedef boost::mpl::map0<> type;
    };

    template<
      typename Graph,
      typename First = typename boost::mpl::begin<Graph>::type,
      typename Last = typename boost::mpl::end<Graph>::type,
      typename Result = boost::mpl::map0<>
    >
    struct clean_graph {
      typedef typename boost::mpl::deref<First>::type::first key;
      typedef typename boost::mpl::if_<
          typename boost::mpl::has_key<
            Result,
            key
          >::type,
          Result,
          typename boost::mpl::insert<
            Result,
            boost::mpl::pair<
              key,
              typename boost::mpl::at<
                Graph,
                key
              >::type
            >
          >::type
        >::type next;
      typedef typename clean_graph<
          Graph,
          typename boost::mpl::next<First>::type,
          Last,
          next
        >::type type;
    };

    template<typename Graph, typename First, typename Result>
    struct clean_graph<Graph, First, First, Result> {
      typedef Result type;
    };

    template<
      typename FragmentSeq,
      typename Graph = typename init_graph<FragmentSeq>::type,
      bool = boost::mpl::empty<FragmentSeq>::value
    >
    struct order_graph {
      typedef typename boost::mpl::front<FragmentSeq>::type fragment;

      typedef typename fragment_graph_before<
          FragmentSeq,
          fragment,
          Graph
        >::type before;

      typedef typename fragment_graph_after<
          FragmentSeq,
          fragment,
          before
        >::type after;

      typedef typename order_graph<
          typename boost::mpl::pop_front<FragmentSeq>::type,
          after
        >::type type;
    };

    template<typename FragmentSeq, typename Graph>
    struct order_graph<FragmentSeq, Graph, true> {
      typedef typename clean_graph<Graph>::type type;
    };

    template<
      typename Result,
      typename After,
      typename First,
      typename Last
    >
    struct add_edges {
      typedef typename boost::mpl::deref<First>::type x;
      typedef typename boost::mpl::if_<
          typename boost::mpl::has_key<Result, x>::type,
          typename boost::mpl::at<Result, x>::type,
          boost::mpl::set0<>
        >::type old;
      typedef boost::mpl::pair<
          x,
          typename boost::mpl::insert<
            old,
            After
          >::type
        > new_pair;
      typedef typename boost::mpl::insert<
          Result,
          new_pair
        >::type new_result;
      typedef typename add_edges<
          new_result,
          After,
          typename boost::mpl::next<First>::type,
          Last
        >::type type;
    };

    template<typename Result, typename After, typename First>
    struct add_edges<Result, After, First, First> {
      typedef Result type;
    };

    template<
      typename First,
      typename Last,
      typename Result
    >
    struct reverse_graph {
      typedef typename boost::mpl::deref<First>::type x;
      typedef typename x::first after;
      typedef typename x::second before;
      typedef typename reverse_graph<
          typename boost::mpl::next<First>::type,
          Last,
          typename add_edges<
            Result,
            after,
            typename boost::mpl::begin<before>::type,
            typename boost::mpl::end<before>::type
          >::type
        >::type type;
    };

    template<typename First, typename Result>
    struct reverse_graph<First, First, Result> {
      typedef typename clean_graph<Result>::type type;
    };

    template<
      typename FragmentSeq,
      typename Fragments,
      typename Graph,
      typename Result = boost::mpl::vector0<>,
      typename NIt = typename boost::mpl::find_if<
          Fragments,
          boost::mpl::empty<
            boost::mpl::at<
              Graph,
              boost::mpl::_1
            >
          >
        >::type,
      typename End = typename boost::mpl::end<Fragments>::type
    >
    struct reorder {
      typedef typename boost::mpl::deref<NIt>::type N;
      typedef typename boost::mpl::push_back<
          Result,
          N
        >::type next;
      typedef typename reverse_graph<
          typename boost::mpl::begin<Graph>::type,
          typename boost::mpl::end<Graph>::type,
          boost::mpl::map0<>
        >::type reverse;
      typedef typename boost::mpl::erase_key<
          reverse,
          N
        >::type deconnect_N;
      typedef typename reverse_graph<
          typename boost::mpl::begin<deconnect_N>::type,
          typename boost::mpl::end<deconnect_N>::type,
          typename init_graph<FragmentSeq>::type
        >::type next_graph;
      typedef typename reorder<
          FragmentSeq,
          typename boost::mpl::erase_key<
            Fragments,
            N
          >::type,
          next_graph,
          next
        >::type type;
    };

    template<
      typename FragmentSeq,
      typename Fragments,
      typename Graph,
      typename Result,
      typename It
    >
    struct reorder<FragmentSeq, Fragments, Graph, Result, It, It> {
      // check for cycles
      BOOST_STATIC_ASSERT((
        boost::is_same<
          typename boost::mpl::find_if<
            Graph,
            boost::mpl::not_<
              boost::mpl::empty<
                boost::mpl::second<
                  boost::mpl::_1
                >
              >
            >
          >::type,
          typename boost::mpl::end<Graph>::type
        >::value
      ));
      typedef Result type;
    };
  }

  /*
  while (N = find node without incoming edge (Graph) )
    output N
    Graph = reverse<insert<reverse<Graph>, pair<N, set0<> > > > -- remove all edges E from N to M in Graph
  */
  template<typename FragmentSeq>
  struct reorder {
    typedef typename boost::mpl::copy<
        FragmentSeq,
        boost::mpl::inserter<
          boost::mpl::set0<>,
          boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>
        >
      >::type fragments;
    typedef typename detail::order_graph<FragmentSeq>::type graph;
    typedef typename detail::reorder<FragmentSeq, fragments, graph>::type type;
  };

  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(default_fragment)

    template<
      typename Fragment,
      bool has = has_require_before<Fragment>::value
    >
    struct get_requirements_before {
      typedef typename Fragment::require_before type;
    };

    template<typename Fragment>
    struct get_requirements_before<Fragment, false> {
      typedef boost::mpl::vector0< > type;
    };

    template<
      typename Fragment,
      bool has = has_require_after<Fragment>::value
    >
    struct get_requirements_after {
      typedef typename Fragment::require_after type;
    };

    template<typename Fragment>
    struct get_requirements_after<Fragment, false> {
      typedef boost::mpl::vector0< > type;
    };

    template<
      typename Fragment,
      bool has = has_require<Fragment>::value
    >
    struct get_requirements {
      typedef typename Fragment::require type;
    };

    template<typename Fragment>
    struct get_requirements<Fragment, false> {
      typedef boost::mpl::vector0< > type;
    };

    template<
      typename Fragment,
      typename Concept,
      typename DefaultFragment = typename Fragment::default_fragment
    >
    struct default_fragment {
      BOOST_STATIC_ASSERT((boost::mpl::has_key<DefaultFragment, Concept>::value));
      typedef typename boost::mpl::at<DefaultFragment, Concept>::type type;
    };

    template<
      typename Fragment,
      typename Concept
    >
    struct get_default_for {
      BOOST_STATIC_ASSERT((has_default_fragment<Fragment, Concept>::value));
      typedef typename default_fragment<Fragment, Concept>::type type;
    };

    template<typename Seq, typename Fragment, typename Requirement>
    struct add_default {
      typedef typename boost::mpl::push_back<
          Seq,
          typename get_default_for<
            Fragment,
            Requirement
          >::type
        >::type type;
    };

    template<
      typename Fragment,
      typename Requirements,
      typename FragmentSeq,
      bool no_requirements_left = boost::mpl::empty<Requirements>::value
    >
    struct match_or_apply_default_fragment {
      typedef typename boost::mpl::front<Requirements>::type requirement;
      typedef typename boost::mpl::pop_front<Requirements>::type requ_seq;

      typedef typename match_or_apply_default_fragment<
        Fragment,
        requ_seq,
        FragmentSeq
      >::type next_default;
      typedef typename boost::mpl::eval_if<
        has_concept<
          FragmentSeq,
          requirement
        >,
        boost::mpl::identity<next_default>,
        add_default<next_default, Fragment, requirement>
      >::type type;
    };

    template<
      typename Fragment,
      typename Requirements,
      typename FragmentSeq
    >
    struct match_or_apply_default_fragment<
      Fragment,
      Requirements,
      FragmentSeq,
      true
    >
    {
      typedef boost::mpl::vector0< > type;
    };

    template<
      typename FragmentSeq,
      long i = 0,
      long n = boost::mpl::size<FragmentSeq>::value
    >
    struct apply_default_fragments {
      typedef typename boost::mpl::at_c<FragmentSeq, i>::type fragment;

      typedef typename concat<
        typename get_requirements_before<fragment>::type,
        typename concat<
          typename get_requirements_after<fragment>::type,
          typename get_requirements<fragment>::type
        >::type
      >::type requirements;

      typedef typename concat<
        FragmentSeq,
        typename match_or_apply_default_fragment<
          fragment,
          requirements,
          FragmentSeq
        >::type
      >::type new_fragment_seq;

      typedef typename apply_default_fragments<
        new_fragment_seq,
        i + 1
      >::type type;
    };

    template<
      typename FragmentSeq,
      long n
    >
    struct apply_default_fragments<FragmentSeq, n, n> {
      typedef FragmentSeq type;
    };
  }

  template<typename FragmentSeq>
  struct apply_default_fragments {
    typedef typename detail::apply_default_fragments<FragmentSeq>::type type;
  };
}}}

#endif
