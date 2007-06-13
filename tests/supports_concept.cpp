#include <fragments/concepts/supports_concept.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

namespace {
  struct conceptA { };

  struct fragmentA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;
  };
}

int main() {
  return !fragments::concepts::supports_concept<fragmentA, conceptA>::value;
}
