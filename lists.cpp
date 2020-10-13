#include <iostream>
#include <forward_list>
#include <string>
#include <array>
#include <limits>

using namespace std;

//generates random single linked list
forward_list<int> random_list(int a)
{
    forward_list<int> list1;
    for (int i = 0; i < a; i++)
    {
        list1.push_front(rand() % 100);
    }
    return list1;
}

void print_list(const forward_list<int> &lista)
{
    cout << "( ";
    for (int i : lista)
    {
        cout << i << " ";
    }
    cout << ")" << endl;
}

forward_list<int> map_aux(forward_list<int>::const_iterator itCurrent, forward_list<int>::const_iterator itEnd, std::function<int(int i)> functiontest)
{
    forward_list<int> listb;
    if (itCurrent == itEnd)
    {
        return listb;
    }
    listb = map_aux(std::next(itCurrent), itEnd, functiontest);
    listb.push_front(functiontest(*itCurrent));
    return listb;
}

forward_list<int> map(const forward_list<int> &lista, std::function<int(int i)> functiontest)
{
    forward_list<int>::const_iterator itStart = lista.begin();
    forward_list<int>::const_iterator itEnd = lista.end();
    forward_list<int> listc = map_aux(itStart, itEnd, functiontest);
    return listc;
}

forward_list<int> filter_aux(forward_list<int>::const_iterator itCurrent, forward_list<int>::const_iterator itEnd, std::function<bool(int i)> pred)
{
    forward_list<int> listb;
    if (itCurrent == itEnd)
    {
        return listb;
    }
    listb = filter_aux(std::next(itCurrent), itEnd, pred);
    if (pred(*itCurrent))
    {
        listb.push_front(*itCurrent);
    }
    return listb;
}

//filter using predicate pred
forward_list<int> filter(const forward_list<int> &lista, std::function<bool(int i)> pred)
{
    forward_list<int>::const_iterator itStart = lista.begin();
    forward_list<int>::const_iterator itEnd = lista.end();
    forward_list<int> listc = filter_aux(itStart, itEnd, pred);
    return listc;
}

int reduce(const forward_list<int> &lista, int i, std::function<int(int j, int k)> functionb)
{
    int current;
    current = functionb(i, lista.front());
    auto vi = lista.begin();
    advance(vi, 1);
    for (; vi != lista.end(); ++vi)
    {
        current = functionb(current, *vi);
    }
    return current;
}

//Bonus part

int fold_left_aux(forward_list<int>::const_iterator itCurrent, forward_list<int>::const_iterator itEnd, int i, std::function<int(int j, int k)> functionb)
{
    if (itCurrent == itEnd)
    {
        return i;
    }
    else
    {
        return fold_left_aux(std::next(itCurrent), itEnd, functionb(i, *itCurrent), functionb);
    }
}

int fold_left(const forward_list<int> &lista, int i, std::function<int(int j, int k)> functionb)
{
    forward_list<int>::const_iterator itStart = lista.cbegin();
    forward_list<int>::const_iterator itEnd = lista.cend();
    return fold_left_aux(itStart, itEnd, i, functionb);
}

void test_3()
{
    srand(static_cast<unsigned int>(time(0))); // Initialize random number generator
    forward_list<int> a = random_list(10);
    printf("Random generated forward list:"); 
    print_list(a);

    using namespace std::placeholders;
    int fact1;
    fact1 = rand() % 3;
    printf("\nrandom factor for map function: %d:\n", fact1);
    auto multiplyByFact = std::bind( std::multiplies< int >(), fact1, _1);
    forward_list<int> b = map(a, multiplyByFact);
    printf("\nForward list after mapping:");
    print_list(b);

    auto keepEven = std::bind( std::equal_to< bool >(), 0, std::bind( std::modulus< int >(), _1, 2)); // Nested use of bind. Currification to find even numbers
    forward_list<int> c = filter(a, keepEven);
    printf("\nForward list keeping only even numbers");
    print_list(c);
    printf("\nEnd of test_3:\n");
}

void test_4()
{
    srand(static_cast<unsigned int>(time(0))); // Initialize random number generator
    forward_list<int> d = random_list(10);
    int m, m2, m3;
    print_list(d);

    m = reduce(d, numeric_limits<int>::max(), [](int i, int j) -> int { return (min(i, j)); });
    printf("The minimum calculated using reduce is: %d\n", m);

    m3 = fold_left(d, numeric_limits<int>::max(), [](int i, int j) -> int { return (min(i, j)); });
    printf("The minimum calculated using fold_left is: %d\n", m3);

    int local_min = numeric_limits<int>::max();
    int local_max = 0;
    m2 = reduce(d, numeric_limits<int>::max(), [&local_min, &local_max](int i, int j) -> int {if( j > local_max) { local_max = j; } if (j < local_min) {local_min = j;} return 0; });
    printf("The minimum calculated using one pass is: %d and the maximum is %d\n", local_min, local_max);
}

int main()
{
    printf("Start of test_3:\n");
    test_3();
    printf("Start of test_4:\n");

    test_4();
    return 0;
}
