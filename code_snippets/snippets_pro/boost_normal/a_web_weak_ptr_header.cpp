//网上封装好的 weak_ptr

/*
使用方式：包含头文件，然后在调用 ptr_io_service->post(boost::bind(&test1::fun1,
boost::weak_ptr<test1>(shared_from_this()), i));
*/

//#include "E:\Work\iSpeak\common\sdk\utilities\inc\bind_weak_ptr.hpp"
#ifndef BOOST_BIND_WEAK_PTR_HPP_INCLUDED
#define BOOST_BIND_WEAK_PTR_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <stdexcept>

#ifndef BOOST_BIND_WEAK_PTR_MAX_ARITY
#define BOOST_BIND_WEAK_PTR_MAX_ARITY 10
#endif // !defined(BOOST_BIND_WEAK_PTR_MAX_ARITY)

namespace boost {
namespace detail {
#ifdef BOOST_NO_VOID_RETURNS
struct fake_return_void {};

template <typename T> struct no_void_returns_helper { typedef T type; };

template <> struct no_void_returns_helper<void> {
  typedef fake_return_void type;
};

template <typename R> struct invoker {
  template <typename T, class M>
  R operator()(const boost::shared_ptr<T> &sp, M mp) {
    return sp ? ((*sp).*mp)() : R();
  }

  // 1 .. BOOST_BIND_WEAK_PTR_MAX_ARITY
#define BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP(z, n, txt)                 \
  template <typename T, class M, BOOST_PP_ENUM_PARAMS(n, typename T)>          \
  R operator()(const boost::shared_ptr<T> &sp, M mp,                           \
               BOOST_PP_ENUM_BINARY_PARAMS(n, T, p)) {                         \
    return sp ? ((*sp).*mp)(BOOST_PP_ENUM_PARAMS(n, p)) : R();                 \
  }

  BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_BIND_WEAK_PTR_MAX_ARITY),
                          BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP, _)

#undef BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP
};

template <> struct invoker<void> {
  template <typename T, class M>
  fake_return_void operator()(const boost::shared_ptr<T> &sp, M mp) {
    return sp ? ((*sp).*mp)();
    fake_return_void();
  }

  // 1 .. BOOST_BIND_WEAK_PTR_MAX_ARITY
#define BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP(z, n, txt)                 \
  template <typename T, class M, BOOST_PP_ENUM_PARAMS(n, typename T)>          \
  fake_return_void operator()(const boost::shared_ptr<T> &sp, M mp,            \
                              BOOST_PP_ENUM_BINARY_PARAMS(n, T, p)) {          \
    return sp ? ((*sp).*mp)(BOOST_PP_ENUM_PARAMS(n, p)) : fake_return_void();  \
  }

  BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_BIND_WEAK_PTR_MAX_ARITY),
                          BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP, _)

#undef BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP
};
#endif // BOOST_NO_VOID_RETURNS

////////////////////////////////////////////////////////////////////////
template <typename T, typename R, typename M> class weak_ptr_functor {
public:
#ifdef BOOST_NO_VOID_RETURNS
  typedef no_void_returns_helper<R>::type return_type;
#define INVOKE_0 !sp_ ? return_type() : invoker<R>()(sp_, mp_)
#define INVOKE_N !sp_?return_type():invoker<R>()(sp_, mp_,
#else // BOOST_NO_VOID_RETURNS
  typedef R return_type;
#define INVOKE_0 !sp_ ? return_type() : ((*sp_).*mp_)()
#define INVOKE_N !sp_?return_type():((*sp_).*mp_)(
#endif // BOOST_NO_VOID_RETURNS

  weak_ptr_functor(const boost::weak_ptr<T> &wp, M mp)
      : sp_(wp.lock()), mp_(mp) {}

  // 0
  return_type operator()() { return INVOKE_0; }

  // 1 .. BOOST_BIND_WEAK_PTR_MAX_ARITY
#define BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP(z, n, txt)                 \
  template <BOOST_PP_ENUM_PARAMS(n, typename T)>                               \
  return_type operator()(BOOST_PP_ENUM_BINARY_PARAMS(n, T, p)) {               \
      return INVOKE_N BOOST_PP_ENUM_PARAMS(n, p));                             \
  }

  BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_BIND_WEAK_PTR_MAX_ARITY),
                          BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP, _)

#undef BOOST_BIND_WEAK_PTR_DEFINE_FUNCTION_CALL_OP

private:
  boost::shared_ptr<T> sp_;
  M mp_;
};

////////////////////////////////////////////////////////////////////////
template <typename T> class weak_ptr_proxy {
public:
  weak_ptr_proxy(const boost::weak_ptr<T> &wp) : wp_(wp) {}

  // 0
  template <typename R>
  weak_ptr_functor<T, R, R (T::*)()> operator->*(R (T::*f)()) {
    return weak_ptr_functor<T, R, R (T::*)()>(wp_, f);
  }

  template <typename R>
  weak_ptr_functor<T, R, R (T::*)() const> operator->*(R (T::*f)() const) {
    return weak_ptr_functor<T, R, R (T::*)() const>(wp_, f);
  }

  // 1 .. BOOST_BIND_WEAK_PTR_MAX_ARITY
#define BOOST_BIND_WEAK_PTR_DEFINE_PTR_TO_MEM_OP(z, n, txt)                    \
  template <typename R, BOOST_PP_ENUM_PARAMS(n, typename T)>                   \
  weak_ptr_functor<T, R, R (T::*)(BOOST_PP_ENUM_PARAMS(n, T))> operator->*(    \
      R (T::*f)(BOOST_PP_ENUM_PARAMS(n, T))) {                                 \
    return weak_ptr_functor<T, R, R (T::*)(BOOST_PP_ENUM_PARAMS(n, T))>(wp_,   \
                                                                        f);    \
  }                                                                            \
                                                                               \
  template <typename R, BOOST_PP_ENUM_PARAMS(n, typename T)>                   \
  weak_ptr_functor<T, R, R (T::*)(BOOST_PP_ENUM_PARAMS(n, T)) const>           \
  operator->*(R (T::*f)(BOOST_PP_ENUM_PARAMS(n, T)) const) {                   \
    return weak_ptr_functor<T, R, R (T::*)(BOOST_PP_ENUM_PARAMS(n, T)) const>( \
        wp_, f);                                                               \
  }

  BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_BIND_WEAK_PTR_MAX_ARITY),
                          BOOST_BIND_WEAK_PTR_DEFINE_PTR_TO_MEM_OP, _)

#undef BOOST_BIND_WEAK_PTR_DEFINE_PTR_TO_MEM_OP

private:
  boost::weak_ptr<T> wp_;
};

} // namespace detail

////////////////////////////////////////////////////////////////////////////
template <typename T>
detail::weak_ptr_proxy<T> get_pointer(const boost::weak_ptr<T> &wp) {
  return detail::weak_ptr_proxy<T>(wp);
}

} // namespace boost

#endif // BOOST_BIND_WEAK_PTR_HPP_INCLUDED
