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
#include <fragments/as_concept_each.hpp>
#include <boost/ref.hpp>

unsigned n = 0;

struct c {};

template<unsigned x>
struct f1 {
  typedef boost::mpl::vector1<c> concept;

  template<typename Before>
  struct fragment : Before {
    void action() {
      n |= x;
    }
  };
};

struct f2 {
  typedef boost::mpl::vector0<> concept;
  template<typename Before>
  struct fragment : Before {};
};

struct a {
  template<typename X>
  void operator() (X &x) const {
    x.action();
  }
};

int main() {
  fragments::combiner<f1<2>, f2, f1<4> > x;
  fragments::as_concept_each<c>(x, boost::cref(a()));
  return n != 6;
}
