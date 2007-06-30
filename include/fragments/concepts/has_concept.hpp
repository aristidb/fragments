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

#ifndef FRAGMENTS_CONCEPTS_HAS_CONCEPT_HPP
#define FRAGMENTS_CONCEPTS_HAS_CONCEPT_HPP

#include "supports_concept.hpp"

#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/not.hpp>

namespace fragments {
namespace concepts {

template<typename FragmentList, typename Concept>
struct has_concept
  : boost::mpl::not_<
      boost::is_same<
        typename boost::mpl::find_if<
          FragmentList,
          supports_concept<boost::mpl::_1, Concept>
        >::type,
        typename boost::mpl::end<FragmentList>::type
      >
    >::type
{ };

}}

#endif
