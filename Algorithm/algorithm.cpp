
/**
 * General experiements with std Algorithm
 */
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <iterator>
#include <random>


template <typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> const& rhs)
{
    std::for_each(std::cbegin(rhs), std::cend(rhs), [&](auto value){os << value << " ";});
    return os;
}

int main(void)
{
    const std::vector<double> work{1.0f, 2.0f, 3.3f, 3.5f, 4.5f, 6.9f};
    const std::vector<double> workRef{1.0f, 2.0f, 3.2f, 3.5f, 4.5f, 6.9f};


    auto isOdd = [](auto value) {return (value % 2) == 1;};
    auto isEven = [](auto value) {return (value % 2) == 0;};

    auto printItem = [](auto& val) {std::cout << std::fixed << std::setprecision(1) << std::setw(3) << std::setfill('0') << val << std::endl;};
    auto printInt = [](auto&val) {std::cout << val << std::endl;};

    //std::foreach
    std::for_each(std::cbegin(work), std::cend(work), printItem);

    auto ref=0.5l;
    auto gt=[&](auto const& val) {return val > ref;};
    auto lt=[&](auto const& val) {return val < ref;};

    //std::all_of
    std::cout << "All items in list are " << (std::all_of(std::cbegin(work), std::cend(work), lt) ? "" : "not ") << "less than " << ref << std::endl;
    std::cout << "All items in list are " << (std::all_of(std::cbegin(work), std::cend(work), gt) ? "" : "not ") << "greater than " << ref << std::endl;

    //std::any_of
    ref=3.0f;
    std::cout << (std::any_of(std::cbegin(work), std::cend(work), gt) ? "Some" : "None") << " of the items in the list are greater than " << ref << std::endl;
    ref=8.0f;
    std::cout << (std::any_of(std::cbegin(work), std::cend(work), gt) ? "Some" : "None") << " of the items in the list are greater than " << ref << std::endl;


    //std::count_if
    {
        ref = 4.0f;
        std::cout << "Found " << std::count_if(std::cbegin(work), std::cend(work), gt) << " items in the list greater than " << ref << std::endl;
        auto min{2.0f};
        auto max{4.0f};
        auto number = std::count_if(std::cbegin(work), std::cend(work), [&](auto value){ return value < max && value > min;});
        std::cout << " There are " << number << " elements between " << min << " and " << max << std::endl;
    }

    //std::mismatch
    {
        auto result = std::mismatch(std::cbegin(work), std::cend(work), std::cbegin(workRef), std::cend(workRef));
        assert(*(result.first) != *(result.second));
        assert(result.first != work.end());
    }

    //std::find_end
    {
        std::string source{"Kalamazoo"};
        std::string ref{"a"};
        auto position = std::find_end(std::cbegin(source), std::cend(source), std::cbegin(ref), std::cend(ref));
        std::cout << "Value at last position is " << *position << " (offset of " << std::distance(source.cbegin(), position) << ")" <<  std::endl;
    }

    //std::find_first_of
    {
        std::string source{"Kalamazoo"};
        std::string ref{"lzo"};
        auto position = std::find_first_of(std::cbegin(source), std::cend(source), std::cbegin(ref), std::cend(ref));
        std::cout << "First instance is " << *position << " (offset of " << std::distance(source.cbegin(), position) << ")" <<  std::endl;
    }

    //std::adjacent_find
    {
        std::string source{"Kalamazoo"};
        auto position = std::adjacent_find(std::cbegin(source), std::cend(source));
        std::cout << "First set of adjacent characters is " << *position << " (offset of " << std::distance(source.cbegin(), position) << ")" <<  std::endl;
    }

    //std::search
    {
        std::string source{"Kalamazoo"};
        std::string ref{"maz"};
        auto position = std::search(std::cbegin(source), std::cend(source), std::cbegin(ref), std::cend(ref));
        std::cout << "First instance is " << *position << " (offset of " << std::distance(source.cbegin(), position) << ")" <<  std::endl;
    }

    //std::copy_if
    {
        std::vector<double> dest;
        std::copy_if(std::cbegin(work), std::cend(work), std::back_inserter(dest), [](auto value){return value >= 2.0f && value <= 4.0f;});
        std::cout << "destination now contains " << dest.size() << " elements" << std::endl;
        assert(size_t{3} == dest.size());
    }

    //std::fill
    {
        std::vector<int> toFill(20);
        std::fill(std::begin(toFill), std::end(toFill), 15);
        assert(std::count(std::begin(toFill), std::end(toFill), 15) == 20);
    }

    auto doubleIt = [](auto value) {return value*2;};
    auto isDouble = [](auto lhs, auto rhs) {return lhs*2 == rhs;};

    //std::transform
    {
        std::vector<int> ref{1,2,3,4,5};
        std::vector<int> result;

        std::transform(std::cbegin(ref), std::cend(ref), std::back_inserter(result), doubleIt);

        assert(std::equal(std::cbegin(ref), std::cend(ref), std::cbegin(result), isDouble));
    }

    //std::generate
    {
        std::vector<char> alphabet(26);
        std::generate(std::begin(alphabet), std::end(alphabet), [val='a']() mutable {return val++;});
        assert(std::string("abcdefghijklmnopqrstuvwxyz") == std::string(std::cbegin(alphabet), std::cend(alphabet)));
    }

    //std::remove_if
    {
        std::vector<int> ref(20);
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});
        //remove odd numbers
        auto tail = std::remove_if(std::begin(ref), std::end(ref), isOdd);
        ref.erase(tail, ref.end());
        std::cout << "Modified vector contains " << ref.size() << " elements" << std::endl;
        std::for_each(std::cbegin(ref), std::cend(ref), printItem);
        assert(size_t{10} == ref.size());
    }

    //std::remove_copy_if
    {
        std::vector<int> ref(20);
        std::vector<int> dest;
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});
        //remove even numbers
        auto tail = std::remove_copy_if(std::cbegin(ref), std::cend(ref), std::back_inserter(dest), isEven);

        assert(size_t{20} == ref.size());
        assert(size_t{10} == dest.size());
        std::cout << "First element in destination is " << dest[0] << std::endl;
        assert(1 == dest[0]);

    }

    //std::replace_if
    {
        std::vector<int> ref(20);
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});
        //replace all even values with 0
        std::replace_if(std::begin(ref), std::end(ref), isEven, 0);

        assert(size_t{10} == std::count(std::cbegin(ref), std::cend(ref), 0));
    }

    //std::replace_copy_if
    {
        std::vector<int> ref(20);
        std::vector<int> dest;
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});
        //replace multiples of 5 with 0
        std::replace_copy_if(std::cbegin(ref), std::cend(ref), std::back_inserter(dest), [](auto value) {return (value % 5) == 0;}, 0);

        assert(size_t{20} == ref.size());
        assert(size_t{20} == dest.size());
        auto zeroes = std::count(std::cbegin(dest), std::cend(dest), 0);
        std::cout << "Number of zeroes is " << zeroes << std::endl;
        assert(size_t{4} ==  zeroes);
    }

    //std::sample
    {   
        std::vector<char> alpha(26);
        std::generate(std::begin(alpha), std::end(alpha), [val='a']() mutable {return val++;});

        std::string sample;
        auto sampleSize = size_t{7};
        std::sample(std::cbegin(alpha), std::cend(alpha), std::back_inserter(sample), sampleSize, std::mt19937{std::random_device{}()});

        std::cout << "Sample is \"" << sample << "\"" << std::endl;
        assert(sampleSize == sample.size());
        
    }

    //std::shuffle
    {
        std::vector<char> alpha(26);
        std::generate(std::begin(alpha), std::end(alpha), [val='a']() mutable {return val++;});
        std::string copy(alpha.begin(), alpha.end());

        std::shuffle(std::begin(copy), std::end(copy), std::mt19937());

        std::cout << "Shuffled value is \"" << copy << "\"" << std::endl;
        assert(!std::equal(std::cbegin(alpha), std::cend(alpha), std::cbegin(copy)));
        assert(alpha.size() == copy.size());
    }

    //std::partition
    {
        std::vector<int> ref(20);
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});

        auto boundary = std::partition(std::begin(ref), std::end(ref), isEven);

        assert(size_t{10} == std::distance(std::begin(ref), boundary));
        assert(std::is_partitioned(std::cbegin(ref), std::cend(ref), isEven));
        assert(boundary == std::partition_point(std::cbegin(ref), std::cend(ref), isEven));
    }

    //std::sort
    {
        std::vector<int> ref(20);
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});
        std::shuffle(std::begin(ref), std::end(ref), std::mt19937());

        assert(!std::is_sorted(std::cbegin(ref), std::cend(ref)));
        std::sort(std::begin(ref), std::end(ref));
        assert(std::is_sorted(std::cbegin(ref), std::cend(ref)));
    }

    //std::nth_element
    {
        std::vector<int> ref(20);
        std::generate(std::begin(ref), std::end(ref), [val=0]() mutable {return val++;});
        std::shuffle(std::begin(ref), std::end(ref), std::mt19937());

        auto midIt = ref.begin()+10;
        auto midpoint = *midIt;
        std::nth_element(std::begin(ref), midIt, std::end(ref));


        std::cout << "Start n is " << midpoint << std::endl;
        std::cout << "End n is " << *midIt << std::endl;
        std::for_each(std::cbegin(ref), std::cend(ref), printInt);
        assert(std::is_partitioned(std::cbegin(ref), std::cend(ref), [&](auto value){return value < *midIt;}));
    }

    //std::merge
    {
        std::cout << "merge" << std::endl;
        std::vector<int> sorted(20);
        std::generate(std::begin(sorted), std::end(sorted), [val=0]() mutable {return val++;});

        std::vector<int> odd(10);
        std::generate(std::begin(odd), std::end(odd), [val=1]() mutable {auto temp=val; val+=2; return temp;});
        std::vector<int> even(10);
        std::generate(std::begin(even), std::end(even), [val=0]() mutable {auto temp=val; val+=2; return temp;});

        std::vector<int> result;
        std::merge(std::cbegin(odd), std::cend(odd), std::cbegin(even), std::cend(even), std::back_inserter(result));
        std::cout << result << std::endl;

        assert(std::equal(std::cbegin(result), std::cend(result), std::cbegin(sorted)));
    }

    //set operations
    {
        std::vector<int> sorted(20);
        std::iota(std::begin(sorted), std::end(sorted), 0);

        decltype(sorted) sub = {1,5,6};

        assert(std::includes(std::cbegin(sorted), std::cend(sorted), std::cbegin(sub), std::cend(sub)));

        decltype(sorted) result;
        std::set_difference(std::cbegin(sorted), std::cend(sorted), std::cbegin(sub), std::cend(sub), std::back_inserter(result));

        assert(size_t{17} == result.size());
        assert(!std::includes(std::cbegin(result), std::cend(result), std::cbegin(sub), std::cend(sub)));

    }

    //std::set_intersection
    {
        std::vector<int> r1 = {1,2,3,6,8,22};
        std::vector<int> r2 = {1,2,4,6,9,22};

        decltype(r2) result;

        std::set_intersection(std::cbegin(r1), std::cend(r1), std::cbegin(r2), std::cend(r2), std::back_inserter(result));

        auto exp = std::vector<int>{1,2, 6, 22};
        assert(exp == result);
    }

    //std::set_symetrical_difference
    {
        std::vector<int> r1 = {1,2,3,6,8,22};
        std::vector<int> r2 = {1,2,4,6,9,22};

        decltype(r2) result;

        std::set_symmetric_difference(std::cbegin(r1), std::cend(r1), std::cbegin(r2), std::cend(r2), std::back_inserter(result));

        auto exp = std::vector<int>{3,4,8,9};
        assert(exp == result);
    }




    return 0;
}
