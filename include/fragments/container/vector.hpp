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

#ifndef FRAGMENTS_CONTAINER_VECTOR_HPP
#define FRAGMENTS_CONTAINER_VECTOR_HPP

#include <vector>
#include <fragments/container/concepts/container.hpp>
#include <fragments/container/concepts/type_provider.hpp>

namespace fragments { namespace container {
  struct vector {
    typedef boost::mpl::true_ shadow;
    typedef boost::mpl::vector1<concepts::container> concept;
    typedef boost::mpl::vector1<concepts::type_provider> require_before;

    template<typename Before, typename>
    struct fragment :
      Before,
      std::vector<typename Before::value_type> 
    {
    };
  };
}}

#endif
