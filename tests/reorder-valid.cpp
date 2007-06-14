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

#include <fragments/concepts/reorder.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>

/*
  TODO
Problem: equivalent_sequence depends on order. Thus checks depend on the order
         of the result of flatten!
 */

template<typename SeqA,
         typename SeqB,
         bool emptyA = boost::mpl::empty<SeqA>::value,
         bool emptyB = boost::mpl::empty<SeqB>::value>
struct equivalent_sequence
  : boost::mpl::and_<
      typename boost::is_same<
        typename boost::mpl::front<SeqA>::type,
        typename boost::mpl::front<SeqB>::type
      >::type,
      typename equivalent_sequence<
        typename boost::mpl::pop_front<SeqA>::type,
        typename boost::mpl::pop_front<SeqB>::type
     >::type>
{ };

template<typename SeqA,
         typename SeqB>
struct equivalent_sequence<SeqA, SeqB, true, true>
  : boost::mpl::true_
{ };

template<typename SeqA,
         typename SeqB,
         bool x>
struct equivalent_sequence<SeqA, SeqB, x, true>
  : boost::mpl::false_
{ };

template<typename SeqA,
         typename SeqB,
         bool x>
struct equivalent_sequence<SeqA, SeqB, true, x>
  : boost::mpl::false_
{ };

#define CHECK(X, expect)                                           \
  {                                                                \
    bool b = equivalent_sequence<X, expect>::value;                \
    ret = ret && b;                                                \
    if(!b)                                                         \
      std::cerr << __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__) ":"   \
        " FAILED: " BOOST_PP_STRINGIZE(X) " not equaivalent to "   \
        BOOST_PP_STRINGIZE(expect) << '\n';                        \
  }

int main() {
  // TODO ...
}
