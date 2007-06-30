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

#ifndef FRAGMENTS_ADAPTOR_HPP
#define FRAGMENTS_ADAPTOR_HPP

#include <boost/mpl/vector.hpp>

namespace fragments {
  template<typename AdaptThis, typename ConceptSeq,
           typename RequireBefore = boost::mpl::vector0< >,
           typename Require = boost::mpl::vector0< >,
           typename RequireAfter = boost::mpl::vector0< >,
           typename Shadow = boost::mpl::false_>
  struct adapter {
    template<typename Before, typename After>
    struct fragment : Before, AdaptThis { };

    typedef ConceptSeq concept;
    typedef RequireBegore require_before;
    typedef Require require;
    typedef RequireAfter require_after;
    typedef Shadow shadow;
  };
}

#endif
