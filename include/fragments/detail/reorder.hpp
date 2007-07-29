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

#ifndef FRAGMENTS_DETAIL_REORDER_HPP
#define FRAGMENTS_DETAIL_REORDER_HPP

#include <fragments/concepts/detail/has_require.hpp>
#include <fragments/concepts/supports_concept.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/remove.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

namespace fragments { namespace detail {
  namespace reorder_detail {
    template<
      typename FragmentSeq,
      typename Concept,
      typename Result = boost::mpl::vector0<>,
      bool no_fragments_left = boost::mpl::empty<FragmentSeq>::value
    >
    struct get_matching_fragments {
      typedef typename boost::mpl::front<
          FragmentSeq
         >::type front;
      typedef typename boost::mpl::if_<
          concepts::supports_concept<front, Concept>,
          typename boost::mpl::push_back<Result, front>::type,
          Result
        >::type next;
      typedef typename get_matching_fragments<
          typename boost::mpl::pop_front<FragmentSeq>::type,
          Concept,
          next
        >::type type;
    };

    template<typename FragmentSeq, typename Concept, typename Result>
    struct get_matching_fragments<FragmentSeq, Concept, Result, true> {

      // DIAGNOSIS: required fragment missing
      BOOST_STATIC_ASSERT((!boost::mpl::empty<Result>::value));

      typedef Result type;
    };

    template<
      typename First,
      typename Last,
      typename Graph
    >
    struct insert_pairs {
      typedef typename boost::mpl::deref<First>::type pair;
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
      typedef typename insert_pairs<
          typename boost::mpl::next<First>::type,
          Last,
          graph
        >::type type;
    };

    template<
      typename End,
      typename Graph
    >
    struct insert_pairs<End, End, Graph> {
      typedef Graph type;
    };

    template<
      typename Requirements,
      typename FragmentSeq,
      typename Fragment,
      typename Graph,
      template<typename, typename> class PairMaker,
      bool no_requirements_left = boost::mpl::empty<Requirements>::value
    >
    struct fragment_graph {
      typedef typename boost::mpl::front<Requirements>::type concept;
      typedef typename get_matching_fragments<
          FragmentSeq,
          concept
        >::type matching_fragments_incl;
      typedef typename boost::mpl::remove<
          matching_fragments_incl,
          Fragment
        >::type matching_fragments;
      typedef typename boost::mpl::transform<
          matching_fragments,
          PairMaker<Fragment, boost::mpl::_1>
        >::type pairs;
      typedef typename insert_pairs<
          typename boost::mpl::begin<pairs>::type,
          typename boost::mpl::end<pairs>::type,
          Graph
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
      bool has_require_before =
        concepts::detail::has_require_before<Fragment>::value
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
      bool has_require_after =
        concepts::detail::has_require_after<Fragment>::value
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
      typename First = typename boost::mpl::begin<FragmentSeq>::type,
      typename Last = typename boost::mpl::end<FragmentSeq>::type
    >
    struct order_graph {
      typedef typename boost::mpl::deref<First>::type fragment;

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
          FragmentSeq,
          after,
          typename boost::mpl::next<First>::type,
          Last
        >::type type;
    };

    template<typename FragmentSeq, typename Graph, typename End>
    struct order_graph<FragmentSeq, Graph, End, End> {
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
          typename boost::mpl::erase<
            Fragments,
            typename boost::mpl::find<
              Fragments,
              N
            >::type
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
                boost::mpl::second<boost::mpl::_1>
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
    typedef typename reorder_detail::order_graph<FragmentSeq>::type graph;
    typedef typename reorder_detail::reorder<
        FragmentSeq,
        FragmentSeq,
        graph
      >::type type;
  };
}}

#endif
