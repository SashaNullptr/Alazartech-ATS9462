#ifndef ATS9462ENGINE_H
#define ATS9462ENGINE_H

// C System-Headers
//
// C++ System headers
//
// AlazarTech Headers
//
// Boost Headers
#include <boost/bind.hpp>
// Qt Headers
//
// Project Specific Headers
#include "ats9462.h"
#include "../JASPL/jAlgorithm/jalgorithm.h"
#include "../JASPL/jFFT/jfft.h"

class ATS9462Engine : public alazar::ATS9462 {

    typedef std::lock_guard<std::mutex> lock;

  public:
    ATS9462Engine(uint signal_samples , uint num_averages, uint ring_buffer_size );
    ~ATS9462Engine();

    void Start();
    void Stop();
    bool Finished();
    std::vector < float > FinishedSignal();

  protected:
    uint number_averages;
    uint samples_per_average;

    void CallBackUpdate( unsigned long signal_size );
    void CallBackWait( unsigned long signal_size );
    void ThreadCallback( unsigned int num_threads );

  private:

    void UpdateAverage();

    jaspl::RecurseMean< std::vector < float > > average_engine;

    std::vector < float > current_signal;
    jaspl::JFFT fft_er;

    uint samples_half;
    bool ready_flag = false;
    uint pending_avg_index = 0;
    uint num_active_threads = 0;
    const uint thread_limit = 10; // Limits the number of threads that can be alive at any time

    std::vector< std::thread > worker_threads;
    std::vector< std::thread::id > complete_thread_ids;

    void ThreadCleanUp();
    void FinalThreadCleanUp();

    void Rebin( std::vector < float >& to_bin );

//    std::mutex read_monitor;
};

#endif // ATS9462ENGINE_H
