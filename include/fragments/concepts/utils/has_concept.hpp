#ifndef FRAGMENTS_CONCEPTS_UTILS_HAS_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_UTILS_HAS_CONCEPT_HPP

#include "supports_concept.hpp"

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>

namespace fragments {
namespace concepts {
namespace utils {

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

}}}

#endif
