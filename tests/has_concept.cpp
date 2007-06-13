#include <fragments/concepts/has_concept.hpp>

#include <boost/mpl/vector.hpp>
#include <iostream>

namespace {
  struct conceptA { };

  struct fragmentA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;
  };

  struct conceptB { };

  struct fragmentB {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
  };

  struct fragmentN {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector0< > concept;
  };
}

int main() {
  typedef boost::mpl::vector3<fragmentN, fragmentA, fragmentB> fragment_seq;
  return !fragments::concepts::has_concept<fragment_seq, conceptA>::value;
}
