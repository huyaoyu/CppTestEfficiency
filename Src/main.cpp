#include <cmath>
#include <iostream>
#include <limits> 

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;
namespace bpt = boost::posix_time;

const size_t LOOP_NUM = 100000000;

#define COUT_INTEGRAL(_t) \
    cout << "Size of " << #_t << " = " << sizeof(_t) << ", " \
         << "[ " << numeric_limits<_t>::min() << ", " \
         << numeric_limits<_t>::max() << " ]." << endl;

#define COUT_FLOATING_POINT(_t) \
    cout << "Size of " << #_t << " = " << sizeof(_t) << ", " \
         << scientific << "[ " << numeric_limits<_t>::min() << ", " \
         << numeric_limits<_t>::max() << " ]." << endl;

static void show_types(void)
{
    COUT_INTEGRAL(int);
    COUT_INTEGRAL(short);
    COUT_INTEGRAL(long);

    COUT_FLOATING_POINT(float);
    COUT_FLOATING_POINT(double);
    COUT_FLOATING_POINT(long double);
}

class Runnable
{
public:
    Runnable() { };
    virtual ~Runnable() { };

    virtual void prepare(void) = 0;
    virtual void run(int loops) = 0;
    virtual void finalize(void) = 0;
};

template <typename _T> 
class FloatingPointMuliplification : public Runnable
{
public:
    FloatingPointMuliplification( ) : Runnable() { };
    virtual ~FloatingPointMuliplification() { };

    virtual void prepare(void) { };
    virtual void finalize(void) { };

    virtual void run(int loops)
    {
        _T temp = (_T)( 0 );
        _T val  = (_T)( 2 );

        for ( int i = 0; i < loops; ++i )
        {
            temp = val * val;
        }
    }
};

template <typename _T> 
class CMathExponential : public Runnable
{
public:
    CMathExponential() { };
    ~CMathExponential() { };

    virtual void prepare(void) { };
    virtual void finalize(void) { };

    virtual void run( int loops )
    {
        _T temp = (_T)( 0 );
        const _T exponent = (_T)( 2 );

        for ( int i = 0; i < loops; ++i )
        {
            temp = exp( exponent );
        }
    }
};

template <typename _T> 
class ArrayAccess : public Runnable
{
public:
    ArrayAccess(size_t size) : mSize(size), mArray(NULL) { };
    virtual ~ArrayAccess()
    {
        destroy();
    }

    virtual void prepare(void)
    {
        destroy();
        mArray = new _T[mSize];
    }

    virtual void finalize(void)
    {
        destroy();
    }

    virtual void run(int loops)
    {
        size_t p = 0;

        _T temp = (_T)( 0 );

        for ( int i = 0; i < loops; ++i )
        {
            temp = mArray[p];
            p++;

            if ( p == mSize )
            {
                p = 0;
            }
        }
    }

private:
    void destroy(void)
    {
        if ( NULL != mArray )
        {
            delete [] mArray; mArray = NULL;
        }
    }

private:
    const size_t mSize;
    _T* mArray;
};

class RunnableOperator
{
public:
    RunnableOperator() { };
    ~RunnableOperator() { };

    void profile( Runnable* rnb, int loops )
    {
        rnb->prepare();

        bpt::ptime start = bpt::microsec_clock::universal_time();

        rnb->run(loops);

        bpt::ptime end = bpt::microsec_clock::universal_time();
        bpt::time_duration td = end - start;

        rnb->finalize();

        cout << "Total time: " << td.total_milliseconds() << " ms." << endl;
    }
};

int main(void)
{
    cout << "Hello efficiency!" << endl;

    cout << "Show the maximum representable values of various types. " << endl;

    show_types();

    Runnable* rnbFloat     = new FloatingPointMuliplification<float>;
    Runnable* rnbDouble    = new FloatingPointMuliplification<double>;
    Runnable* rnbExpFloat  = new CMathExponential<float>;
    Runnable* rnbExpDouble = new CMathExponential<double>;
    Runnable* rnbAAFloat   = new ArrayAccess<float>(1000000);
    Runnable* rnbAADouble  = new ArrayAccess<double>(1000000);

    RunnableOperator ro;

    cout << "Executing time of various operations." << endl;

    ro.profile( rnbFloat,  LOOP_NUM );
    ro.profile( rnbDouble, LOOP_NUM );
    ro.profile( rnbExpFloat,  LOOP_NUM );
    ro.profile( rnbExpDouble, LOOP_NUM );
    ro.profile( rnbAAFloat, LOOP_NUM );
    ro.profile( rnbAADouble, LOOP_NUM );

    delete rnbAADouble; rnbAADouble = NULL;
    delete rnbAAFloat; rnbAAFloat = NULL;
    delete rnbExpDouble; rnbExpDouble = NULL;
    delete rnbExpFloat; rnbExpFloat = NULL;
    delete rnbDouble; rnbDouble = NULL;
    delete rnbFloat; rnbFloat = NULL;

    return 0;
}
