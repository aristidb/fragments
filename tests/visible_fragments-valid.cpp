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

#include <fragments/detail/visible_fragments.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/static_assert.hpp>
#include <typeinfo>
#include <iostream>

struct f0 { typedef boost::mpl::true_ shadow; };
struct f1 {};
struct f2 { typedef boost::mpl::true_ shadow; };
struct f3 { typedef boost::mpl::false_ shadow; };
struct f4 {};

int main() {
  typedef boost::mpl::vector5<f0, f1, f2, f3, f4> seq;
  typedef fragments::detail::visible_fragments<seq>::type vis;
  BOOST_STATIC_ASSERT((boost::mpl::size<vis>::value == 3));
  //std::cout << "_Z1x" << typeid(vis).name() << std::endl;
}
