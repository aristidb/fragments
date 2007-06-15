// Copyright (c) 2007 Aristid Breitkreuz, Ruediger Sonderfeld
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

#ifndef FRAGMENTS_DETAIL_APPLY_DEFAULT_FRAGMENTS_HPP
#define FRAGMENTS_DETAIL_APPLY_DEFUALT_FRAGMENTS_HPP

#include "../concepts/detail/has_require.hpp"
#include "../concepts/detail/concat.hpp"
#include "../concepts/has_concept.hpp"

#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>

#include <boost/static_assert.hpp>

namespace fragments { namespace detail {
  namespace detail {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(default_fragment)

    template<
      typename Fragment,
      bool has = concepts::detail::has_require_before<Fragment>::value
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
      bool has = concepts::detail::has_require_after<Fragment>::value
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
      bool has = concepts::detail::has_require<Fragment>::value
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
        concepts::has_concept<
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

      typedef typename concepts::detail::concat<
        typename get_requirements_before<fragment>::type,
        typename concepts::detail::concat<
          typename get_requirements_after<fragment>::type,
          typename get_requirements<fragment>::type
        >::type
      >::type requirements;

      typedef typename concepts::detail::concat<
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
}}

#endif
