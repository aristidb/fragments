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

#ifndef FRAGMENTS_PARAMETERS_FOLDN_HPP
#define FRAGMENTS_PARAMETERS_FOLDN_HPP

#include <fragments/parameters/fold2.hpp>
#include <boost/preprocessor.hpp>
#include <fragments/config.hpp>

namespace fragments { namespace parameters {

#define FRAGMENTS_PARAMETERS_FOLD(z, n, t) \
  template< \
    BOOST_PP_ENUM_PARAMS(n, typename T) \
  > \
  struct BOOST_PP_CAT(fold, n) { \
    typedef BOOST_PP_CAT(fold, BOOST_PP_DEC(n))< \
        BOOST_PP_ENUM_SHIFTED_PARAMS(n, T) \
      > sub; \
    typedef fold2< \
        T0, \
        typename sub::type \
      > folder; \
    typedef typename folder::type type; \
    static type fold( \
        BOOST_PP_ENUM_BINARY_PARAMS(n, T, const &x) \
      ) \
    { \
      return folder::fold( \
          x0, \
          sub::fold( \
            BOOST_PP_ENUM_SHIFTED_PARAMS(n, x) \
          ) \
        ); \
    } \
  }; \
  /**/

BOOST_PP_REPEAT_FROM_TO(
  3,
  BOOST_PP_INC(FRAGMENTS_LIMIT),
  FRAGMENTS_PARAMETERS_FOLD,
  ~
)

#undef FRAGMENTS_PARAMETERS_FOLD

}}

#endif
