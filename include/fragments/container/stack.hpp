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

#ifndef FRAGMENTS_CONTAINER_STACK_HPP
#define FRAGMENTS_CONTAINER_STACK_HPP

#include <fragments/container/concepts/container.hpp>
#include <fragments/container/concepts/type_provider.hpp>
#include <fragments/container/vector.hpp>

namespace fragments { namespace container {
  struct stack {
      typedef boost::mpl::true_ shadow;
      typedef boost::mpl::vector0<> concept;//für stack haben wir (noch?) kein eigenes concept
      typedef boost::mpl::vector2<
          concepts::container,
          concepts::type_provider
        > require_before;
      typedef boost::mpl::map1<
          boost::mpl::pair<concepts::container, vector>
        > default_fragment;

      template<typename Before, typename>
      struct fragment {
          typedef typename Before::access access;
          // type_provider types
          typedef typename Before::value_type value_type;
          typedef typename Before::reference reference;
          typedef typename Before::const_reference const_reference;
          // container types
          typedef typename Before::size_type size_type;
        private:
          Before sequence;
        public:
          fragment() {
          }

          bool empty() const {
            return sequence.empty();
          }

          size_type size() const {
            return sequence.size();
          }

          reference top() {
            return sequence.back();
          }

          const_reference top() const {
            return sequence.back();
          }

          void push(const_reference vt) {
            sequence.push_back(vt);
          }

          void pop() {
            sequence.pop_back();
          }
      }; //end fragment
  };//end stack
}}

#endif
