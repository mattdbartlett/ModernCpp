
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>

/**
 * A naive implementation of the seive or Eratosthenes (an algorithm for calculating prime numbers)
 *
 */
class Sieve
{
    public:
        Sieve() = delete; //no Default ctor
        Sieve(Sieve const&) = delete; //no copy ctor
        Sieve(uint64_t max) 
        {
            std::vector<bool> workArea(max+1);
            //fill the work area with true (treat things as prime until we prove they are not
            std::fill(workArea.begin(), workArea.end(), true);

            for (auto count=2ULL; count <= max; ++count)
            {
                if (workArea[count]) //if so, we have reached a prime
                {
                    m_primes.push_back(count); //throw it in our list
                    for (auto idx=count; idx < max; idx+=count)
                    {
                        workArea[idx] = false;
                    }
                }
                //else, skip it
            }
        }

        std::list<uint64_t> GetPrimes() const {return m_primes;}
    private:
        std::list<uint64_t> m_primes;
};


int main(int argc, char* argv[])
{
    uint64_t upperLimit{1000000};
    if (argc == 2)
    {
        upperLimit = std::stoull(argv[1]);
    }
    Sieve sieve{upperLimit};

    std::cout << "Found " << sieve.GetPrimes().size() << " from 0 - " << upperLimit << std::endl;

    return 0;
}
