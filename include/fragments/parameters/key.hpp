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

#ifndef FRAGMENTS_PARAMETERS_KEY_HPP
#define FRAGMENTS_PARAMETERS_KEY_HPP

#include <fragments/parameters/keyed_value.hpp>
#include <boost/ref.hpp>

namespace fragments { namespace parameters {

template<typename Key>
struct key {
  template<typename Value>
  keyed_value<Key, boost::reference_wrapper<Value const> >
  operator=(Value const &x) const {
    return keyed_value<Key, boost::reference_wrapper<Value const> >
      (boost::cref(x));
  }

  template<typename Value>
  keyed_value<Key, boost::reference_wrapper<Value> >
  operator=(boost::reference_wrapper<Value> x) const {
    return keyed_value<Key, boost::reference_wrapper<Value> >
      (x);
  }
};

#define FRAGMENTS_PARAMETERS_KEY(name) \
  struct name : ::fragments::parameters::key<name> { \
    using ::fragments::parameters::key<name>::operator=; \
  }; \
  /**/

FRAGMENTS_PARAMETERS_KEY(positional)

}}

#endif
