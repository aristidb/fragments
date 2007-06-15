// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:

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

#ifndef FRAGMENTS_DETAIL_FIND_FRAGMENT_HPP
#define FRAGMENTS_DETAIL_FIND_FRAGMENT_HPP

#include <fragments/detail/nada.hpp>
#include <fragments/detail/iterate_fragments.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

namespace fragments { namespace detail {
  template<typename, typename> struct next_fragment;

  template<
    typename Sequence,
    typename Fragment,
    typename Before = nada<Sequence>
  >
  struct find_fragment {
    typedef iterate_fragments<Sequence, Before> it;
    typedef
      typename boost::mpl::eval_if<
        boost::is_same<Fragment, typename it::first>,
        boost::mpl::identity<typename it::second>,
        next_fragment<it, Fragment>
      >::type type;
  };

  template<typename It, typename Fragment>
  struct next_fragment {
    typedef
      typename find_fragment<
        typename It::next_sequence,
        Fragment,
        typename It::second
      >::type type;
  };
}}

#endif
