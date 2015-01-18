/*!
@file
Defines `boost::hana::Either`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_HPP
#define BOOST_HANA_EITHER_HPP

#include <boost/hana/fwd/either.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return equal(x.value, y.value); }

        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return less(x.value, y.value); }

        template <typename T, typename U>
        static constexpr auto apply(_left<T> const&, _right<U> const&)
        { return true_; }

        template <typename T, typename U>
        static constexpr auto apply(_right<T> const&, _left<U> const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct fmap_impl<Either> {
        template <typename E, typename F>
        static constexpr decltype(auto) apply(E&& e, F&& f) {
            return either(left,
                compose(right, detail::std::forward<F>(f)),
                detail::std::forward<E>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<Either> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return right(detail::std::forward<X>(x)); }
    };

    template <>
    struct ap_impl<Either> {
        template <typename E, typename X>
        static constexpr decltype(auto) apply(E&& e, X&& x) {
            return either(left,
                partial(fmap, detail::std::forward<X>(x)),
                detail::std::forward<E>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<Either> {
        template <typename E>
        static constexpr decltype(auto) apply(E&& e)
        { return either(left, id, detail::std::forward<E>(e)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Either> {
        template <typename T, typename F>
        static constexpr decltype(auto) apply(_left<T> const&, F&& f)
        { return detail::std::forward<F>(f)(); }


        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T> const& x, F&& f)
        { return detail::std::forward<F>(f)(x.value); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>& x, F&& f)
        { return detail::std::forward<F>(f)(x.value); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>&& x, F&& f)
        { return detail::std::forward<F>(f)(detail::std::move(x.value)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct traverse_impl<Either> {
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_left<T> const& e, F&& f)
        { return lift<A>(e); }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_left<T>& e, F&& f)
        { return lift<A>(e); }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_left<T>&& e, F&& f)
        { return lift<A>(detail::std::move(e)); }

        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_right<T> const& e, F&& f)
        { return fmap(detail::std::forward<F>(f)(e.value), right); }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>& e, F&& f)
        { return fmap(detail::std::forward<F>(f)(e.value), right); }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>&& e, F&& f)
        { return fmap(detail::std::forward<F>(f)(detail::std::move(e.value)), right); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_HPP
