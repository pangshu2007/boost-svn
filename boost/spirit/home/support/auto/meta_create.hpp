//  Copyright (c) 2001-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_SUPPORT_META_CREATE_NOV_21_2009_0327PM)
#define BOOST_SPIRIT_SUPPORT_META_CREATE_NOV_21_2009_0327PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/unused.hpp>

#include <boost/proto/proto.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/include/fold.hpp>

namespace boost { namespace spirit { namespace traits
{
    ///////////////////////////////////////////////////////////////////////////
    // This is the main dispatching point for meta_create to the correct domain
    template <typename Domain, typename T, typename Enable = void>
    struct meta_create;

    ///////////////////////////////////////////////////////////////////////////
    // This allows to query whether a valid mapping exits for the given data 
    // type to a component in the given domain
    template <typename Domain, typename T, typename Enable = void>
    struct meta_create_exists : mpl::false_ {};
}}}

namespace boost { namespace spirit 
{
    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T>
        struct add_const_ref
          : add_reference<typename add_const<T>::type> {};

        template <typename T>
        struct remove_const_ref
          : remove_const<typename remove_reference<T>::type> {};

        ///////////////////////////////////////////////////////////////////////
        template <typename OpTag, typename Domain>
        struct nary_proto_expr_function
        {
            template <typename T>
            struct result;

            template <typename T1, typename T2>
            struct result<nary_proto_expr_function(T1, T2)>
            {
                typedef typename remove_const_ref<T1>::type left_type;
                typedef typename 
                    spirit::traits::meta_create<Domain, T2>::type
                right_type;

                typedef typename mpl::eval_if<
                    traits::not_is_unused<left_type>
                  , proto::result_of::make_expr<OpTag, left_type, right_type>
                  , mpl::identity<right_type>
                >::type type;
            };

            template <typename T>
            typename result<nary_proto_expr_function(unused_type const&, T)>::type
            operator()(unused_type const&, T) const
            {
                typedef spirit::traits::meta_create<Domain, T> right_type;
                return right_type::call();
            }

            template <typename T1, typename T2>
            typename result<nary_proto_expr_function(T1, T2)>::type
            operator()(T1 const& t1, T2) const
            {
                // we variants to the alternative operator
                typedef spirit::traits::meta_create<Domain, T2> right_type;
                return proto::make_expr<OpTag>(t1, right_type::call());
            }
        };
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename T, typename OpTag, typename Domain>
    struct make_unary_proto_expr
    {
        typedef spirit::traits::meta_create<Domain, T> subject_type;

        typedef typename proto::result_of::make_expr<
            OpTag, typename subject_type::type
        >::type type;

        static type call()
        {
            return proto::make_expr<OpTag>(subject_type::call());
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Sequence, typename OpTag, typename Domain>
    struct make_nary_proto_expr
    {
        typedef detail::nary_proto_expr_function<OpTag, Domain> 
            make_proto_expr;

        typedef typename fusion::result_of::fold<
            Sequence, unused_type, make_proto_expr
        >::type type;

        static type call()
        {
            return fusion::fold(Sequence(), unused, make_proto_expr());
        }
    };
}}

#endif
