#include <fragments/concepts/flatten.hpp>

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

  struct conceptiA {
    typedef boost::mpl::vector1<conceptA> implies;
  };

  struct conceptiAiA {
    typedef boost::mpl::vector1<conceptiA> implies;
  };

  struct concept0 {
    typedef boost::mpl::vector0< > implies;
  };
}

int main() {
  bool ret = true;

  {
    typedef boost::mpl::vector1<conceptA> seq;
    CHECK((boost::mpl::size<
           fragments::concepts::flatten<seq>::type
           >::value),
          1)
  }

  // ...

  return ret ? 0 : 1;
}
