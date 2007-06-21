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

#include <fragments/parameters/foldN.hpp>
#include <fragments/parameters/container.hpp>
#include <fragments/parameters/key.hpp>

using namespace fragments::parameters;

FRAGMENTS_PARAMETERS_KEY(k1)

int main() {
  {
    typedef add<int, int> t1;
    t1 x1(4);
    typedef add<char, int, t1> t2;
    t2 x2(7,x1);

    if (getter<t2, int>::get(x2) != 4)
      return 1;
    if (getter<t2, char>::get(x2) != 7)
      return 1;

    if ((k1() = 4).get() != 4)
      return 1;
  }

  {
    typedef fold2<keyed_value<k1, boost::reference_wrapper<int const> >, int> f;
    f::type z = f::fold(k1() = 1, 2);

    if (getter<f::type, k1>::get(z) != 1)
      return 1;

    if (getter<f::type, positional>::get(z).get<0>() != 2)
      return 1;
  }

  {
    typedef fold5<int, int, int, int, int> f;
    f::type z = f::fold(0, 1, 2, 3, 4);

    if (getter<f::type, positional>::get(z).get<3>() != 3)
      return 1;
   }

  {
    typedef fold6<
        int,
        int,
        keyed_value<k1, boost::reference_wrapper<char const [1]> >,
        int,
        int,
        int
      > f;
    f::type z = f::fold(0, 1, k1() = "", 2, 3, 4);

    if (getter<f::type, positional>::get(z).get<3>() != 3)
      return 1;
   }

  return 0;
}
