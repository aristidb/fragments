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

#ifndef FRAGMENTS_PARAMETRS_WRAP_HPP
#define FRAGMENTS_PARAMETRS_WRAP_HPP

#include <fragments/parameters/container.hpp>

namespace fragments { namespace parameters {

template<typename T>
class wrap {
public:
  wrap() {}

  wrap(T const &x) : wrapped(x) {}

  template<typename K>
  typename value_type<T, K>::type
  get() const {
    return getter<T, K>::get(wrapped);
  }

  template<typename K, typename V>
  wrap<add<K, V, T> >
  set(V const &v) const {
    return add<K, V, T>(v, wrapped);
  }

  template<typename K, typename V>
  struct set_f {
    typedef wrap<add<K, V, T> > type;
  };

private:
  T wrapped;
};

}}

#endif
