#include <fragments/concepts/supports_concept.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

namespace {
  struct conceptA { };

  struct policyA {
    template<typename Before, typename After>
    struct policy : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;
  };
}

int main() {
  std::cout <<
    fragments::concepts::supports_concept<
     policyA,
     conceptA
       >::value
            << '\n';
}
