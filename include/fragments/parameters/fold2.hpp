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

#ifndef FRAGMENTS_PARAMETERS_FOLD2_HPP
#define FRAGMENTS_PARAMETERS_FOLD2_HPP

#include <fragments/parameters/fold1.hpp>

namespace fragments { namespace parameters {

template<typename T1, typename T2>
struct fold2 {
  typedef fold2<
      T1,
      typename fold1<T2>::type
    > folder;

  typedef typename folder::type type;

  static type fold(T1 const &x, T2 const &y) {
    return folder::fold(
        x,
        fold1<T2>::fold(y)
      );
  }
};

template<
  typename T,
  typename Key,
  typename Value,
  typename Base
>
struct fold2<T, add<Key, Value, Base> > {
  typedef
    boost::tuples::cons<
      T const &,
      typename value_type<
        add<Key, Value, Base>,
        positional
      >::type
    > cons;
  typedef add<
      positional,
      boost::tuples::cons<
        T const &,
        typename value_type<
          add<Key, Value, Base>,
          positional
        >::type
      >,
      add<Key, Value, Base>
    > type;

  static type fold(T const &x, add<Key, Value, Base> const &y) {
    return type(
        cons(
          x,
          getter<add<Key, Value, Base>, positional>::get(y)
        ),
        y
      );
  }
};

template<
  typename T,
  typename Key,
  typename Value,
  typename Base
>
struct fold2<boost::reference_wrapper<T>, add<Key, Value, Base> > {
  typedef
    boost::tuples::cons<
      T &,
      typename value_type<
        add<Key, Value, Base>,
        positional
      >::type
    > cons;
  typedef add<
      positional,
      boost::tuples::cons<
        T &,
        typename value_type<
          add<Key, Value, Base>,
          positional
        >::type
      >,
      add<Key, Value, Base>
    > type;

  static type fold(T &x, add<Key, Value, Base> const &y) {
    return type(
        cons(
          x,
          getter<add<Key, Value, Base>, positional>::get(y)
        ),
        y
      );
  }
};

template<
  typename Key1,
  typename Value1,
  typename Key2,
  typename Value2,
  typename Base
>
struct fold2<keyed_value<Key1, Value1>, add<Key2, Value2, Base> > {
  typedef add<
      Key1,
      Value1,
      add<
        Key2,
        Value2,
        Base
      >
    > type;

  static type fold(
      keyed_value<Key1, Value1> const &x,
      add<Key2, Value2, Base> const &y)
  {
    return type(x.get(), y);
  }
};

}}

#endif
