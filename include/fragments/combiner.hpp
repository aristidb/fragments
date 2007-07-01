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

#ifndef FRAGMENTS_COMBINER_HPP
#define FRAGMENTS_COMBINER_HPP

#include <fragments/config.hpp>
#include <fragments/concepts/check_concepts.hpp>
#include <fragments/concepts/resulting_concept.hpp>
#include <fragments/parameters/fold_wrap.hpp>
#include <fragments/detail/reorder.hpp>
#include <fragments/detail/apply_default_fragments.hpp>
#include <fragments/detail/find_fragment.hpp>
#include <fragments/detail/seq_of_elements.hpp>
#include <fragments/detail/create_vector.hpp>
#include <fragments/detail/visible_fragments.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/static_assert.hpp>
#include <boost/preprocessor.hpp>

namespace fragments {

namespace detail {
  template<typename Seq, typename Derived>
  struct combiner_base;

  template<typename SeqSeq, typename Derived>
  class combine {
    typedef typename seq_of_elements<SeqSeq>::type raw_sequence;

    // DIAGNOSIS: fragments::combiner<> needs at least one fragment
    BOOST_STATIC_ASSERT(!boost::mpl::empty<raw_sequence>::value);

    typedef typename apply_default_fragments<
        raw_sequence
      >::type complete_sequence;

    typedef typename reorder<complete_sequence>::type ordered_sequence;

    typedef typename create_vector<ordered_sequence>::type sequence;

    // DIAGNOSIS: missing / misplaced fragment / concept
    BOOST_STATIC_ASSERT((concepts::check_concepts<sequence>::value));

    typedef typename boost::mpl::back<sequence>::type back;

  public:
    typedef typename find_fragment<
        sequence,
        back,
        combiner_base<sequence, Derived>
      >::type type;
  };
}

template<
  BOOST_PP_ENUM_BINARY_PARAMS(
    FRAGMENTS_LIMIT,
    typename F,
    = boost::mpl::vector0<> BOOST_PP_INTERCEPT
  )
>
struct combiner
  : detail::combine<
      BOOST_PP_CAT(boost::mpl::vector, FRAGMENTS_LIMIT)<
        BOOST_PP_ENUM_PARAMS(FRAGMENTS_LIMIT, F)
      >,
      combiner<
        BOOST_PP_ENUM_PARAMS(FRAGMENTS_LIMIT, F)
      >
    >::type
{
  typedef typename detail::combine<
      BOOST_PP_CAT(boost::mpl::vector, FRAGMENTS_LIMIT)<
        BOOST_PP_ENUM_PARAMS(FRAGMENTS_LIMIT, F)
      >,
      combiner<
        BOOST_PP_ENUM_PARAMS(FRAGMENTS_LIMIT, F)
      >
    >::type base;

  // DIAGNOSIS: topmost fragment fails to propagate "access" definition
  typedef typename base::access access;

  typedef typename concepts::resulting_concept<
      typename access::visible_fragments
    >::type concept;

  combiner() {}

#define FRAGMENTS_COMBINER_CONSTRUCTOR(z, n, t) \
  template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
  combiner(BOOST_PP_ENUM_BINARY_PARAMS(n, T, const &x)) \
    : base(parameters::fold_wrap(BOOST_PP_ENUM_PARAMS(n, x))) \
  {} \
  /**/

  BOOST_PP_REPEAT_FROM_TO(
    1,
    BOOST_PP_INC(FRAGMENTS_LIMIT),
    FRAGMENTS_COMBINER_CONSTRUCTOR,
    ~
  )

#undef FRAGMENTS_COMBINER_CONSTRUCTOR
};

namespace detail {
  template<typename Seq, typename Derived>
  struct combiner_base {
    struct access {
      typedef Seq fragments;

      typedef typename visible_fragments<Seq>::type visible_fragments;
      typedef Derived derived;
      typedef combiner_base root;
    };
  };
}

}

/*
struct my_fragment {
  typedef boost::mpl::vector0<> concept;
  typedef boost::mpl::true_ shadow; //das muss ich noch mit ruediger besprechen
  template<typename Before, typename After>
  class fragment {
  public:
    typedef typename Before::access access; //<-- wichtig!!!
  private:
    Before x;
  };
};
*/

#endif
