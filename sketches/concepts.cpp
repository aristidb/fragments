#include <typeinfo>
#include <iostream>
#include <boost/mpl/size.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>//das kommt aber bitte nich in greatcontainer
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/iterator_range.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/inserter.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/erase_key.hpp>

namespace mpl = boost::mpl;

namespace stl {
  namespace concepts {
    struct container { };
    struct random_access_container {
      typedef mpl::vector1<container> implies;
    };

    struct adapter { };
    struct sequence { };

    struct type_holder { };
  }

  struct vector {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef mpl::vector1<concepts::container> concept;
    typedef mpl::vector1<concepts::type_holder> require_before;
  };

  template<typename T>
  struct type {
    template<typename Before, typename After>
    struct fragment : public Before {
      typedef T value_type; // ...
    };

    typedef mpl::vector1<concepts::type_holder> concept;
  };

  struct stack {
    template<typename Before, typename After>
    struct fragment : protected Before { };

    typedef mpl::vector2<concepts::sequence, concepts::adapter> concept;
    typedef mpl::vector2<concepts::type_holder, concepts::container> require_before;
    typedef mpl::map1<mpl::pair<concepts::container, vector> > default_fragment;
    typedef boost::mpl::true_ shadow;
  };
}

namespace concepts {
  struct container {
  };
  struct random_access_container {
    typedef mpl::vector1<container> implies;
  };

  struct foo { 
    typedef boost::mpl::false_ shadow;
  };
  struct bar {
    typedef mpl::vector1<random_access_container> implies;
    typedef boost::mpl::false_ shadow;
  };
  struct baz { typedef mpl::vector0< > implies; };

  struct shads {
    typedef mpl::vector1<random_access_container> implies;
    typedef boost::mpl::true_ shadow;
  };

  struct implies_shads {
    typedef mpl::vector1<shads> implies;
  };
}
using namespace concepts;


struct fragment0 {
  template<typename Before, typename After>
  struct fragment : public Before { };
  
  typedef mpl::vector2<container, foo> concept;
  /*  typedef mpl::vector<X> require;
  typedef mpl::vector<X> require_before;
  typedef mpl::vector<X> require_after;
  typedef mpl::vector<X> demote_concepts;*/
};

struct fragment1 {
  template<typename Before, typename After>
  struct fragment : public Before { };
  
  typedef mpl::vector1<bar> concept;
  /*typedef mpl::vector<container> require;
    typedef mpl::vector<X> require_before;
  typedef mpl::vector<X> require_after;
  typedef mpl::vector<X> demote_concepts;*/
};

struct fragment2 {
  template<typename Before, typename After>
  struct fragment : public Before { };
  
  typedef mpl::vector1<random_access_container> concept;
  /*typedef mpl::vector<container> require;
    typedef mpl::vector<X> require_before;
  typedef mpl::vector<X> require_after;
  typedef mpl::vector<X> demote_concepts;*/
};

struct fragment3 {
  template<typename Before, typename After>
  struct fragment : public Before { };
  
  typedef mpl::vector1<foo> concept;
  typedef mpl::vector1<container> require_before;
  /*  typedef mpl::vector<X> require_before;
  typedef mpl::vector<X> require_after;
  typedef mpl::vector<X> demote_concepts;*/
};

template<typename Sequence>
struct fragment_holder {
  //
};

namespace utils {
  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(implies)

    template<typename FragmentSeq, typename Concept,
             bool empty = mpl::empty<FragmentSeq>::value>
    struct checker {
      typedef typename mpl::front<FragmentSeq>::type checked_concept;

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
          checker<typename mpl::pop_front<FragmentSeq>::type, Concept>::value);
    };

    template<typename FragmentSeq, typename Concept>
    struct checker<FragmentSeq, Concept, true> {
      static bool const value = false;
    };
  }

  template<typename FragmentClass, typename Concept>
  struct supports_concept
  : boost::integral_constant<bool, detail::checker<typename FragmentClass::concept, Concept>::value> 
  {};
  
  template<typename FragmentList, typename Concept>
  struct has_concept
  : boost::mpl::not_<
      boost::is_same<
        typename boost::mpl::find_if<
          FragmentList,
          supports_concept<boost::mpl::_1, Concept>
        >::type,
        typename boost::mpl::end<FragmentList>::type
      >
    >::type
  { };

  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(require)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(require_before)
    BOOST_MPL_HAS_XXX_TRAIT_DEF(require_after)

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

  template<typename Sequence,
           typename Iterator = typename boost::mpl::begin<Sequence>::type>
  struct check_concepts;

  namespace detail {
    template<typename Sequence, typename Iterator>
    struct check_concepts_impl
    {
      typedef
      boost::mpl::and_<
        check_concepts<
          Sequence,
          typename boost::mpl::next<Iterator>::type
        >,
        boost::mpl::eval_if<
          detail::has_require<
            typename boost::mpl::deref<Iterator>::type
          >,
          detail::check_requirements_all<Sequence, Iterator>,
          boost::mpl::true_
        >,
        boost::mpl::eval_if<
          detail::has_require_before<
            typename boost::mpl::deref<Iterator>::type
          >,
          detail::check_requirements_before<Sequence, Iterator>,
          boost::mpl::true_
        >,
        boost::mpl::eval_if<
          detail::has_require_after<
            typename boost::mpl::deref<Iterator>::type
          >,
          detail::check_requirements_after<Sequence, Iterator>,
          boost::mpl::true_
        >
      >
      type;
    };
  }

  template<typename Sequence,
           typename Iterator>
  struct check_concepts
  : boost::mpl::eval_if<
      boost::is_same<Iterator, typename boost::mpl::end<Sequence>::type>,
      boost::mpl::true_,
      detail::check_concepts_impl<Sequence, Iterator>
    >::type
  { };


  namespace detail {
    template<typename First, typename Last, typename Result>
    struct flatten {
      typedef typename boost::mpl::deref<First>::type deref;

      template<typename T>
      struct subflatten {
        typedef
          typename flatten<
            typename boost::mpl::begin<typename T::implies>::type,
            typename boost::mpl::end<typename T::implies>::type,
            boost::mpl::vector0<>
          >::type type;
      };

      typedef typename boost::mpl::insert_range<
          Result,
          typename boost::mpl::end<Result>::type,
          typename boost::mpl::eval_if<
            has_implies<deref>,
            subflatten<deref>,
            mpl::vector0<>
          >::type
        >::type with_implies;
      typedef typename boost::mpl::insert<
          with_implies,
          typename boost::mpl::end<with_implies>::type,
          deref
        >::type with_deref;

      typedef typename flatten<
          typename boost::mpl::next<First>::type,
          Last,
          with_deref
        >::type type;
    };

    template<typename First, typename Result>
    struct flatten<First, First, Result> {
      typedef Result type;
    };
  }

  template<typename Sequence>
  struct flatten {
    typedef typename detail::flatten<
        typename boost::mpl::begin<Sequence>::type,
        typename boost::mpl::end<Sequence>::type,
        boost::mpl::vector0<>
      >::type
    type;
  };

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

    template<typename SeqA, typename SeqB>
    struct concat {
      typedef typename boost::mpl::insert_range<
          SeqA,
          typename boost::mpl::end<SeqA>::type,
          SeqB
        >::type type;
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

// ----
// create_vector from greatcontainer3
  template<typename Sequence, int Size>
  struct create_vector_impl;

  template<typename Sequence>
  struct create_vector {
    typedef
      typename create_vector_impl<Sequence, boost::mpl::size<Sequence>::value >::type
      type;
  };

  #define ELEMENT_AT(z, n, t) \
    BOOST_PP_COMMA_IF(n) \
    typename boost::mpl::at_c<Sequence, n>::type

  #define CREATE_VECTOR_IMPL(z, n, t) \
    template<typename Sequence> \
    struct create_vector_impl<Sequence, n> { \
      typedef BOOST_PP_CAT(boost::mpl::vector, n)< \
        BOOST_PP_REPEAT(n, ELEMENT_AT, ~) \
      > type; \
    };

  BOOST_PP_REPEAT(20, CREATE_VECTOR_IMPL, ~)

  #undef CREATE_VECTOR_IMPL
  #undef ELEMENT_AT
// ----
}

namespace tests {
  template<typename Seq, typename ShadowConcept>
  void test_do_shadowing() {
    typedef typename utils::detail::do_shadowing<Seq>::type results;

    std::cout << "shadowing: " << boost::mpl::size<results>::value << '\n';
    BOOST_STATIC_ASSERT((boost::is_same<
                         typename boost::mpl::front<results>::type,
                         ShadowConcept>::value));
  }

  template<typename Seq, typename Has, bool expect>
  void has_concept() {
    std::cout << "has_concept\n";
    BOOST_STATIC_ASSERT((utils::has_concept<Seq, Has>::value == expect));
  }

  template<typename Fragment, typename Concept, bool expect>
  void supports_concept() {
    std::cout << "supports_concept\n";
    BOOST_STATIC_ASSERT((utils::supports_concept<Fragment, Concept>::value
                         == expect));
  }

  template<typename Seq, bool expect>
  void check_concepts() {
    std::cout << "check_concepts\n";
    BOOST_STATIC_ASSERT((utils::check_concepts<Seq>::value == expect));
  }

  template<typename Seq>
  void flatten() {
    typedef typename utils::flatten<Seq>::type result;
    std::cout << "flatten: " << boost::mpl::size<result>::value << '\n';
  }

  template<typename FragmentSeq>
  void resulting_concept() {
    typedef typename utils::resulting_concept<FragmentSeq>::type result_;
    typedef typename utils::create_vector<result_>::type result;
    std::cout << "resulting concept: " << boost::mpl::size<result>::value;
    std::cout << " _Z1x" << typeid(result).name() << '\n';
  }
}

int main() {
  tests::supports_concept<fragment1, concepts::container, true>();

  tests::has_concept<mpl::vector2<fragment3, fragment1>, concepts::container,
    true>();
  tests::has_concept<mpl::vector0<>, concepts::container, false>();

  tests::check_concepts<mpl::vector2<fragment3, fragment1>, false>();
  tests::check_concepts<mpl::vector0< >, true>();
  tests::check_concepts<mpl::vector2<fragment1, fragment3>, true>();

  tests::test_do_shadowing<mpl::vector4<bar, foo, shads, baz>, shads>();
  tests::test_do_shadowing<mpl::vector4<bar, foo, implies_shads, baz>,
    implies_shads>();
  tests::test_do_shadowing<mpl::vector0< >, boost::mpl::void_>();

  tests::flatten<mpl::vector2<bar, bar> >();
  tests::flatten<mpl::vector0< > >();
  std::cout << "_Z1x" << typeid(utils::create_vector<utils::flatten<mpl::vector2<bar, bar> >::type>::type).name() << '\n';

  tests::resulting_concept<mpl::vector2<fragment1, fragment3> >();

  typedef mpl::vector3<stl::stack, stl::vector, stl::type<int> > c3;
  typedef utils::detail::order_graph<c3>::type graph;
  typedef utils::detail::clean_graph<
      mpl::map3<
        mpl::pair<stl::vector, mpl::set1<stl::type<int> > >
        ,      mpl::pair<stl::stack, mpl::set2<stl::vector, stl::type<int> > >
        ,      mpl::pair<stl::type<int>, mpl::set0<> >
      >
    >::type graph2;
  typedef utils::detail::init_graph<c3>::type ig;
  typedef utils::detail::reverse_graph<
      mpl::begin<graph>::type,
      mpl::end<graph>::type,
      ig
    >::type rg;
 typedef utils::detail::reverse_graph<
      mpl::begin<graph2>::type,
      mpl::end<graph2>::type,
      ig
    >::type rg2;

  std::cout << "_Z1a" << typeid(graph).name() << '\n';
  std::cout << "_Z1b" << typeid(graph2).name() << '\n';
  std::cout << boost::mpl::size<graph>::value << ',' << boost::mpl::size<graph2>::value << '\n';
  std::cout << "_Z1A" << typeid(rg).name() << '\n';
  std::cout << "_Z1B" << typeid(rg2).name() << '\n';
  std::cout << boost::mpl::size<rg>::value << ',' << boost::mpl::size<rg2>::value << '\n';

  std::cout << "_Z1r" << typeid(utils::create_vector<utils::reorder<c3>::type>::type).name() << '\n';

  std::cout << "_Z1x" << typeid(utils::create_vector<utils::apply_default_fragments<mpl::vector2<stl::stack, stl::type<int> > >::type>::type).name() << '\n';
}
