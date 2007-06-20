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

#ifndef FRAGMENTS_PARAMETERS_CONTAINER_HPP
#define FRAGMENTS_PARAMETERS_CONTAINER_HPP

namespace fragments { namespace parameters {

struct empty {
};

template<typename Key, typename Value, typename Base = empty>
class add : public Base {
public:
  add(Value const &v, Base const &b = Base()) : Base(b), value(v) {}

  Value top() const {
    return value;
  }

private:
  Value value;
};

template<typename T, typename Key>
struct value_type;

template<typename Key1, typename Key2, typename Value, typename Base>
struct value_type<add<Key1, Value, Base>, Key2> {
  typedef typename value_type<Base, Key2>::type type;
};

template<typename Key, typename Value, typename Base>
struct value_type<add<Key, Value, Base>, Key> {
  typedef Value type;
};

template<typename T, typename Key>
struct getter;

template<typename Key1, typename Key2, typename Value, typename Base>
struct getter<add<Key1, Value, Base>, Key2> {
  static
  typename value_type<Base, Key2>::type
  get(Base const &x) {
    return getter<Base, Key2>::get(x);
  }
};

template<typename Key, typename Value, typename Base>
struct getter<add<Key, Value, Base>, Key> {
  static
  Value
  get(add<Key, Value, Base> const &x) {
    return x.top();
  }
};

}}

#endif
