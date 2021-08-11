
#include <utility>
#include <iostream>
#include <cstring>

/**
 * Explore R-value references and move semantics
 */

class Foo
{
    public:
        Foo()
        {
            std::cout << "Default CTOR" << std::endl;
        }

        Foo(Foo const& ref)
        {
            std::cout << "Copy Ctor" << std::endl;
            m_size = ref.m_size;
            m_msg = new char[m_size+1];
            strncpy(m_msg, ref.m_msg, m_size);
        }
        Foo(Foo&& ref)
        {
            std::cout << "Move CTOR" << std::endl;
            m_size = ref.m_size;
            m_msg = ref.m_msg;
            ref.m_size = 0;
            ref.m_msg = nullptr;
        }

        Foo(const char* msg)
        {
            std::cout << "CTOR" << std::endl;
            m_size = strlen(msg);
            m_msg = new char[m_size+1];
            strncpy(m_msg, msg, m_size);
        }


        Foo& operator=(Foo const& ref)
        {
            std::cout << "Assignment Operator" << std::endl;
            if (m_msg)
            {
                delete [] m_msg;
            }
            m_size = ref.m_size;
            m_msg = new char[m_size+1];
            strncpy(m_msg, ref.m_msg, m_size);

            return *this;
        }

        Foo& operator=(Foo&& ref)
        {
            std::cout << "Move Operator" << std::endl;
            if (m_msg)
            {
                delete [] m_msg;
            }
            m_size = ref.m_size;
            m_msg = ref.m_msg;
            ref.m_size = 0;
            ref.m_msg = nullptr;
            return *this;
        }


        void Print(std::ostream& os) const {if (m_size) os << m_msg;}

        ~Foo()
        {
            if (m_msg) 
            {
                delete m_msg;
            }
        }

    private:
        size_t m_size{0};
        char* m_msg{nullptr};
};
std::ostream& operator<<(std::ostream& os, Foo const& ref) { ref.Print(os); return os;}


int main(void)
{
    //Playing with constructors
    Foo item1{"asfljaldfskjalkdsjflakdsjflakjdsflkdsajf"};
    Foo item2{item1}; //copy ctor
    Foo item3; //default ctor
    Foo item4{Foo("abracadabra")}; //Move ctor
                                   // std::move is not needed here, because our temporary object
                                   //is already an r-value
    Foo item5{std::move(item4)}; //Move ctor (again)
                                 // std::move is needed here, because item4 is not an r-value
                                 // std::move lets us convert item4 to an r-value, and item5
                                 // "steals" the resources from item4
    Foo item6;

    //see how we did
    std::cout << "Item1 " << item1 << std::endl;
    std::cout << "Item2 " << item2 << std::endl;
    std::cout << "Item3 " << item3 << std::endl;
    std::cout << "Item4 " << item4 << std::endl;
    std::cout << "Item5 " << item5 << std::endl;
    std::cout << "Item6 " << item6 << std::endl;

    //Test assignment and move operators
    item3 = item1; //assignment operator
    item4 = Foo("SomethingNew"); //Move operator
    item6 = item5; //assignment operator
    item1 = std::move(item2); //Move operator


    //See how we did
    std::cout << "Item1 " << item1 << std::endl;
    std::cout << "Item2 " << item2 << std::endl;
    std::cout << "Item3 " << item3 << std::endl;
    std::cout << "Item4 " << item4 << std::endl;
    std::cout << "Item5 " << item5 << std::endl;
    std::cout << "Item6 " << item6 << std::endl;
    return 0;
}
