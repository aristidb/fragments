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

#ifndef FRAGMENTS_PARAMETERS_FOLD1_HPP
#define FRAGMENTS_PARAMETERS_FOLD1_HPP

#include <fragments/parameters/key.hpp>
#include <fragments/parameters/container.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/ref.hpp>

namespace fragments { namespace parameters {

template<typename T>
struct fold1 {
  typedef add<
      positional,
      boost::tuples::cons<
        T const &,
        boost::tuples::null_type
      >
    > type;

  static type fold(T const &x) {
    return x;
  }
};

template<typename T>
struct fold1<boost::reference_wrapper<T> > {
  typedef add<
      positional,
      boost::tuples::cons<
        T &,
        boost::tuples::null_type
      >
    > type;

  static type fold(boost::reference_wrapper<T> x) {
    return x;
  }
};

template<typename Key, typename Value>
struct fold1<keyed_value<Key, Value> > {
  typedef add<Key, Value> type;

  static type fold(keyed_value<Key, Value> const &x) {
    return x.get();
  }
};

template<typename Key, typename Value, typename Base>
struct fold1<add<Key, Value, Base> > {
  typedef add<Key, Value, Base> type;

  static type const &fold(add<Key, Value, Base> const &x) {
    return x;
  }
};

}}

#endif
