#ifndef C_FUNC_WRAPER_HPP
#define C_FUNC_WRAPER_HPP

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

template<typename CFuncType, typename ClosureSignature>
class c_func_wraper : boost::noncopyable
{
public:
    c_func_wraper()
    {
        wrapped_func_ = new boost::function<ClosureSignature>;
    }

    ~c_func_wraper()
    {
        delete wrapped_func_;
    }

    template<typename T>
    c_func_wraper(const T &bindedfuntor)
    {
        wrapped_func_ = new boost::function<ClosureSignature>;
        *wrapped_func_ = bindedfuntor;
    }

    template<typename T>
    c_func_wraper<CFuncType, ClosureSignature>& operator = (const T &bindedfuntor)
    {
        *wrapped_func_ = bindedfuntor;
        return *this;
    }

    void* c_void_ptr()
    {
        return new boost::function<ClosureSignature>(*wrapped_func_);
    }

    CFuncType c_func_ptr()
    {
        return (CFuncType)wrapperd_callback;
    }
private:
    template<typename R>
    static R wrapperd_callback(void* user_data)
    {
        boost::scoped_ptr<boost::function<ClosureSignature>  > wrapped_func(
                    reinterpret_cast<boost::function<ClosureSignature> *>(user_data));
        return (R)(*wrapped_func)();
    }

    template<typename R, typename ARG1>
    static R wrapperd_callback(ARG1 arg1, void* user_data)
    {
        boost::scoped_ptr<boost::function<ClosureSignature>  > wrapped_func(
                    reinterpret_cast<boost::function<ClosureSignature> *>(user_data));
        return (*wrapped_func)(arg1);
    }

    boost::function<ClosureSignature> * wrapped_func_;
};

#endif
