#include <fragments/concepts/has_concept.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

#define CHECK(X, expect)                                           \
  ret = ret && ((X) == (expect));                                  \
  if((X) != (expect))                                              \
    std::cerr << __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) ":"     \
      " FAILED: " BOOST_PP_STRINGIZE(X) " not equal to "           \
              << expect << '\n';

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

  struct conceptC { };

  struct conceptiA {
    typedef boost::mpl::vector1<conceptA> implies;
  };

  struct fragmentiA {
    template<typename Before, typename After>
    struct fragment : public Before { };

    typedef boost::mpl::vector1<conceptiA> concept;
  };
}

int main() {
  bool ret = true;
  {
    typedef boost::mpl::vector3<fragmentN, fragmentA, fragmentB> fragment_seq;
    CHECK((fragments::concepts::has_concept<fragment_seq, conceptA>::value),
          true);
    CHECK((fragments::concepts::has_concept<fragment_seq, conceptB>::value),
          true);
    CHECK((fragments::concepts::has_concept<fragment_seq, conceptC>::value),
          false);
  }

  {
    typedef boost::mpl::vector3<fragmentN, fragmentiA, fragmentB> fragment_seq;
    CHECK((fragments::concepts::has_concept<fragment_seq, conceptA>::value),
          true);
    CHECK((fragments::concepts::has_concept<fragment_seq, conceptB>::value),
          true);
    CHECK((fragments::concepts::has_concept<fragment_seq, conceptC>::value),
          false);
  }

  return ret ? 0 : 1;
}
