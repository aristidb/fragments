#ifndef FRAGMENTS_CONCEPTS_UTILS_SUPPORTS_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_UTILS_SUPPORTS_CONCEPT_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>

namespace fragments {
namespace concepts {
namespace utils {

namespace detail {
  BOOST_MPL_HAS_XXX_TRAIT_DEF(implies)

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

}}}

#endif
