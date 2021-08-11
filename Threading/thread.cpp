
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
 * A simple thread pool using std::thread and mutex/condition for synchronization
 */
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
        {
            std::lock_guard<std::mutex> guard{m_mutex};
            m_workList.push_back(work);
        }
        m_cond.notify_all();
    }

    void Start()
    {
        std::lock_guard<std::mutex> guard{m_mutex};
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
                std::unique_lock<std::mutex> guard{m_mutex};
                while (m_workList.empty())
                {
                    m_cond.wait(guard);
                }
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
    size_t m_maxThreads;
    std::list<std::thread> m_threads;
    std::mutex  m_mutex;
    std::list<WorkFunction> m_workList;
    std::condition_variable m_cond;
    std::atomic<size_t> m_availableThreads{0};
    std::atomic<size_t> m_totalThreads{0};
    std::atomic<size_t> m_totalWork{0};
};

uint32_t factorial(uint32_t val)
{
    if (val == 0)
    {
        return 1;
    }
    else
    {
        return val*factorial(val-1);
    }
}

/**
 * A class for testing and storing prime numbers
 */
class FactorPrimes
{
public:
    FactorPrimes() = default;
    FactorPrimes(FactorPrimes const&) = delete;
    void CheckPrime(uint32_t val)
    {
        //check if prime
        if (IsPrime(val))
        {
            //if prime, add it to the list
            std::lock_guard<std::mutex> guard(m_mutex);
            m_primes.push_back(val);
        }
    }

    /**
     * This algorithm is not important for the example, except that it provides heavy computational 
     * load
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

    std::list<uint32_t> GetPrimes() const 
    {
        std::lock_guard<std::mutex> guard(m_mutex);
        return m_primes;
    }
private:
    mutable std::mutex m_mutex;
    std::list<uint32_t> m_primes;
};

/**
 * A simple main that takes 2 arguments (exactly). The first is the number of threads, the second is the maximum value
 *
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
