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

#ifndef FRAGMENTS_DETAIL_SEQ_OF_ELEMENTS_HPP
#define FRAGMENTS_DETAIL_SEQ_OF_ELEMENTS_HPP

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

namespace fragments { namespace detail {
  template<
    typename Seq,
    typename Add
  >
  struct add_seqel {
    typedef typename boost::mpl::eval_if<
        boost::mpl::is_sequence<Add>,
        boost::mpl::insert_range<
          Seq,
          typename boost::mpl::end<Seq>::type,
          Add
        >,
        boost::mpl::push_back<
          Seq,
          Add
        >
      >::type type;
  };

  template<
    typename Seq,
    typename Prev = boost::mpl::vector0<>,
    bool empty = boost::mpl::empty<Seq>::value
  >
  struct seq_of_elements {
    typedef typename add_seqel<
        Prev,
        typename boost::mpl::front<Seq>::type
      >::type next;
    typedef typename seq_of_elements<
        typename boost::mpl::pop_front<Seq>::type,
        next
      >::type type;
  };

  template<
    typename Seq,
    typename Result
  >
  struct seq_of_elements<Seq, Result, true> {
    typedef Result type;
  };
}}

#endif
