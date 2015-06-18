#ifndef STDAFX_H
#define STDAFX_H

#define SINGLE_PRECISION

#ifdef SINGLE_PRECISION
    #define FFT_COMPLEX fftwf_complex
    #define FFT_EXECUTE fftwf_execute
    #define FFT_PLAN fftwf_plan
    #define FFT_CREATE_PLAN fftwf_plan_dft_1d
    #define FFT_FREE fftwf_free
    #define FFT_MALLOC fftwf_malloc
    #define FFT_DESTROY_PLAN fftwf_destroy_plan
    #define FFT_INIT_THREADS fftwf_init_threads
    #define T_REAL float
#else
    #define FFT_COMPLEX fftw_complex
    #define FFT_EXECUTE fftw_execute
    #define FFT_PLAN fftw_plan
    #define FFT_CREATE_PLAN fftw_plan_dft_1d
    #define FFT_FREE fftw_free
    #define FFT_MALLOC fftw_malloc
    #define FFT_DESTROY_PLAN fftw_destroy_plan
    #define FFT_INIT_THREADS fftw_init_threads
    #define T_REAL double
#endif




//remove undefs to enable logging. using both types of logging at the same time is ok.
#define DUMP_RAW
#undef DUMP_RAW
#define DUMP_FFT
#undef DUMP_FFT

#endif // STDAFX_H
