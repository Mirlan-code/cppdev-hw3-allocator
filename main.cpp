#include "allocator_without_deallocation.h"
#include "custom_list.h"
#include "pool_allocator.h"

#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <map>


using namespace std;
using namespace std::chrono;


struct A{
    A(int i) {
        x = i;
    }
    int x;
};
int main(int, char *[]) {

    // Map with custom allocator
    const int n = 1e1;
    map<int, int, less<int>, AllocatorWithoutDeallocation<pair<const int, int>, n>> mapWithCustomAllocator;
    map<int, int> mapWithStdAllocator;
    int cur = 1;
    for (int i = 0; i < n; i++) {
        mapWithCustomAllocator[i] = cur;
        mapWithStdAllocator[i] = cur;
        cur *= (i + 1);
    }

    for (int i = 0; i < n; i++) {
        cout << i << ": custom allocator value = " << mapWithCustomAllocator[i]
             << ", std allocator value = " << mapWithStdAllocator[i] << endl;
    }

    // Custom list container
    CustomList<int, AllocatorWithoutDeallocation<int, n>> listWithCustomAllocator;
    CustomList<int> listWithStdAllocator;

    for (int i = 0; i < n; i++) {
        listWithCustomAllocator.Add(i);
        listWithStdAllocator.Add(i);
    }

    cout << "values in custom list with custom allocator: ";
    for (auto i : listWithCustomAllocator) {
        cout << i.val << " ";
    }
    cout << endl << "values in custom list with std allocator: ";
    for (auto i : listWithStdAllocator) {
        cout << i.val << " ";
    }
    cout << endl;

    const int n2 = 1e2;
    vector<A, PoolAllocator<A, 3 * n2>> vectorWithPoolAllocator;
    for (int i = 0; i < n2; i++) {
        vectorWithPoolAllocator.push_back({i});
    }

    // To check out that allocate is done on the indexes, that are already freed
    // You can see "allocate at 0 50" in the output
    for (int i = 0; i < n2 / 2; i++) {
        vectorWithPoolAllocator.pop_back();
    }
    vectorWithPoolAllocator.shrink_to_fit();

    for (A i : vectorWithPoolAllocator) {
        cout << i.x << " ";
    }

    return 0;
}
