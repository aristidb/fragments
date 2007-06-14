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

#ifndef FRAGMENTS_AS_FRAGMENT_HPP
#define FRAGMENTS_AS_FRAGMENT_HPP

#include <fragments/detail/find_fragment.hpp>

template<typename T, typename H>
typename detail::find_fragment<typename H::fragments, T>::type &
as_fragment(H &c) {
  return *static_cast<
      typename detail::find_fragment<typename H::fragments, T>::type *
    >(&c);
}

template<typename T, typename H>
typename detail::find_fragment<typename H::fragments, T>::type const &
as_fragment(H const &c) {
  return *static_cast<
      typename detail::find_fragment<typename H::fragments, T>::type const *
    >(&c);
}

#endif
