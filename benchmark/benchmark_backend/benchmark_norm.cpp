#include <Fastor.h>

using namespace Fastor;

#define NITER 1000000UL

template<typename T, size_t M, size_t N>
inline T norm_scalar(const T *__restrict__ in) {
    T sum = 0;
    for (size_t i=0; i<M; ++i)
        for (size_t j=0; j<N; ++j)
            sum += in[i*N+j]*in[i*N+j];
    return std::sqrt(sum);
}

template<typename T, size_t M, size_t N>
void iterate_over_scalar(const T *__restrict__ in) {
    size_t iter = 0;
    for (; iter<NITER; ++iter) {
        T out = norm_scalar<T,M,N>(in);
        unused(out);
    }
}

template<typename T, size_t M, size_t N>
void iterate_over_fastor(const T *__restrict__ in) {
    size_t iter = 0;
    for (; iter<NITER; ++iter) {
        T out = _norm<T,M*N>(in);
        unused(out);
    } 

    // manual unrolling of iterations, helps GCC for smaller arrays
    // for (; iter<NITER; iter+=4) {
    //     T out0 = _norm<T,M*N>(in);
    //     T out1 = _norm<T,M*N>(in);
    //     T out2 = _norm<T,M*N>(in);
    //     T out3 = _norm<T,M*N>(in);
    //     unused(out0); unused(out1);
    //     unused(out2); unused(out3);
    // }    
}



template<typename T, size_t M, size_t N>
void run() {

    T *in  = static_cast<T*>(_mm_malloc(sizeof(T) * M*N, 32));
    std::iota(in,in+M*N,1);

    timeit(static_cast<void (*)(const T*)>(&iterate_over_scalar<T,M,N>),in);
    timeit(static_cast<void (*)(const T*)>(&iterate_over_fastor<T,M,N>),in);

    _mm_free(in);
}


int main() {


    print("Single precision benchmark");
    run<float,2,2>();
    run<float,3,3>();
    run<float,4,4>();
    run<float,8,8>();
    run<float,16,16>();
    run<float,32,32>();
    run<float,64,64>();
    print("Double precision benchmark");
    run<double,2,2>();
    run<double,3,3>();
    run<double,4,4>();
    run<double,8,8>();
    run<double,16,16>();
    run<double,32,32>();
    run<double,64,64>();

    return 0;
}