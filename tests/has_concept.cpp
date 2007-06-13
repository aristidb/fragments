#include <fragments/concepts/utils/has_concept.hpp>

#include <boost/mpl/vector.hpp>
#include <iostream>

namespace {
  struct conceptA { };

  struct policyA {
    template<typename Before, typename After>
    struct policy : public Before { };

    typedef boost::mpl::vector1<conceptA> concept;
  };

  struct conceptB { };

  struct policyB {
    template<typename Before, typename After>
    struct policy : public Before { };

    typedef boost::mpl::vector1<conceptB> concept;
  };

  struct policyN {
    template<typename Before, typename After>
    struct policy : public Before { };

    typedef boost::mpl::vector0< > concept;
  };
}

int main() {
  typedef boost::mpl::vector3<policyN, policyA, policyB> policy_seq;
  std::cout << fragments::concepts::utils::has_concept<
    policy_seq,
    conceptA>::value
            << '\n';
}
