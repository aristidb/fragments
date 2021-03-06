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

#include <fragments/combiner.hpp>
#include <fragments/container/type.hpp>
#include <fragments/container/stack.hpp>
#include <iostream>
#include <string>

struct push2 {
  typedef boost::mpl::vector0<> concept;

  typedef boost::mpl::vector1<fragments::container::stack> require_before;

  template<typename Before>
  struct fragment : Before {
    void push(typename Before::const_reference x) {
      Before::push(x);
      Before::push(x+x);
    }
  };
};

int main() {
  typedef fragments::combiner<
      fragments::container::stack,
      fragments::container::type<std::string>,
      push2,
      push2
    > stack;
  stack s;
  s.push("Hello, stack! ");
  while (!s.empty()) {
    std::cout << s.top() << std::endl;
    s.pop();
  }
  std::cout << "---" << std::endl;
}
