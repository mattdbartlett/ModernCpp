
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <list>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

/**
 * An RAII style class implementing a spin-lock using std::atomic_flag
 */
class WaitOnFlag
{
    public:
        WaitOnFlag()=delete;
        WaitOnFlag(WaitOnFlag const&) = delete;
        WaitOnFlag(std::atomic_flag& flag) : m_flag(flag) 
        {
            while (m_flag.test_and_set()) {} //block till we win
        }
        ~WaitOnFlag()
        {
            m_flag.clear();
        }
    private:
        std::atomic_flag& m_flag;
};

class ThreadPool
{
public:
    using WorkFunction = std::function<void()>;

    ThreadPool(size_t maxThreads) : m_maxThreads{maxThreads}
    {
    }
    ThreadPool() = delete;
    ThreadPool(ThreadPool const&) = delete;

    void AddWork(WorkFunction work)
    {
        WaitOnFlag wait(m_flag);
        m_workList.push_back(work);
    }

    void Start()
    {
        WaitOnFlag wait(m_flag);
        if (!m_threads.empty())
        {
            std::cout << "ThreadPool is already running" << std::endl;
            return;
        }

        for (auto count=0; count < m_maxThreads; ++count)
        {
            m_threads.push_front(std::thread(std::bind(&ThreadPool::Run, this)));
        }
    }

    void Stop()
    {
        AddWork(nullptr);
        for (auto& cur: m_threads)
        {
            cur.join();
        }
        std::cout << "Total work " << m_totalWork << std::endl;
    }

private:
    void Run() //worker function
    {
        size_t threadWork{0};
        ++m_totalThreads;
        ++m_availableThreads;

        bool cont{true};
        while (cont)
        {
            WorkFunction cur;

            { //critical section
                WaitOnFlag wait(m_flag);
                if (m_workList.empty())
                {
                    continue;
                }

                cur = m_workList.front();
                if (cur == nullptr)
                {
                    cont = false;
                    continue;
                }
                m_workList.pop_front();
            }

            --m_availableThreads;
            cur();
            ++m_availableThreads;
            ++threadWork;
            ++m_totalWork;
        }
        --m_totalThreads;
        --m_availableThreads;

        std::ostringstream msg;
        msg << "Thread Exiting, total work for this thread is " << threadWork << std::endl;
        std::cout << msg.str();
    }

private:
    size_t m_maxThreads; //how many threads to create
    std::list<std::thread> m_threads; //active threads
    std::list<WorkFunction> m_workList; //a queue of work to distribute to your threads

    std::atomic<size_t> m_availableThreads{0}; //how many threads are waiting for work
    std::atomic<size_t> m_totalThreads{0}; //number of threads that are alive
    std::atomic<size_t> m_totalWork{0}; //Number of WorkFunctions executed across all threads in the pool
    std::atomic_flag m_flag{ATOMIC_FLAG_INIT}; //used for synchronization
};


/**
 * A class for testing and storing prime numbers. The algorithm is simple, and was stolen from Google.
 * It is not important to the example, it is just a computationally intensive function (well, for larger numbers)
 *
 */
class FactorPrimes
{
public:
    /**
     * Only default constructable (no copy ctor)
     */
    FactorPrimes() = default;
    FactorPrimes(FactorPrimes const&) = delete;

    /**
     * Check if the argument is prime, if it is, store it in the list. 
     * Only takes the flag if a prime is found
     */
    void CheckPrime(uint32_t val)
    {
        //check if prime
        if (IsPrime(val))
        {
            //if prime, add it to the list
            WaitOnFlag wait(m_flag);
            m_primes.push_back(val);
        }
    }

    /**
     * Returns true if a number is prime
     */
    bool IsPrime(uint32_t val)
    {
        if (val == 0 || val == 1)
        {
            return false;
        }

        for (auto count=2; count <= val/2; ++count)
        {
            if (val % count == 0)
            {
                return false;
            }
        }
        return true;
    }

    /**
     * Get the current list of primes we have found
     */
    std::list<uint32_t> GetPrimes() const 
    {
        WaitOnFlag wait(m_flag);
        return m_primes;
    }
private:
    mutable std::atomic_flag m_flag{ATOMIC_FLAG_INIT}; //use this for synchronization
    std::list<uint32_t> m_primes; //if IsPrime is called with a number that is prime, that number will be stored here
};

/**
 * A simple main which takes 2 arguments (exactly), the first is the number of threads, the second is the upper limit
 * for calculating primes.
 */
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        return -1;
    }

    size_t poolSize = std::stol(argv[1]);
    size_t maxValue = std::stol(argv[2]);

    ThreadPool pool{poolSize};

    pool.Start();

    FactorPrimes factor;

    for (auto count=1; count <= maxValue; ++count)
    {
        pool.AddWork(std::bind(&FactorPrimes::CheckPrime, &factor, count));
    }

    pool.Stop();
    std::cout << "Prime numbers from 0 - " << maxValue << std::endl;
    for (auto& val: factor.GetPrimes())
    {
        std::cout << "|" << std::setw(16) << val << " | " << std::endl;
    }
    std::cout << factor.GetPrimes().size() << " primes found from 0 - " << maxValue << std::endl;
    return 0;
}
