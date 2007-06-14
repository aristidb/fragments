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

#ifndef FRAGMENTS_AS_CONCEPT_EACH_HPP
#define FRAGMENTS_AS_CONCEPT_EACH_HPP

#include <fragments/as_fragment.hpp>
#include <fragments/filter_concept.hpp>
#include <boost/mpl/for_each.hpp>

namespace fragments {

namespace detail {
  template<typename P, typename Fun>
  struct invoker {
    invoker(P ptr, Fun fun) : ptr(ptr), fun(fun) {}

    P ptr;
    Fun fun;

    template<typename Fragment>
    void operator() (Fragment const &) {
      fun(as_fragment<Fragment>(*ptr));
    }
  };

  template<typename P, typename Fun>
  invoker<P, Fun> make_invoker(P ptr, Fun fun) {
    return invoker<P, Fun>(ptr, fun);
  }
}

template<typename C, typename H, typename F>
void as_concept_each(H &h, F fun) {
  typedef typename filter_concept_lazy<typename H::fragments, C>::type filtered;
  boost::mpl::for_each<filtered>(detail::make_invoker(&h, fun));
}

template<typename C, typename H, typename F>
void as_concept_each(H const &h, F fun) {
  typedef typename filter_concept_lazy<typename H::fragments, C>::type filtered;
  boost::mpl::for_each<filtered>(detail::make_invoker(&h, fun));
}

}

#endif
