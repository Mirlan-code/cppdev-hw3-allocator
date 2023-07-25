#include "allocator_without_deallocation.h"
#include "custom_list.h"

#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <map>


using namespace std;
using namespace std::chrono;

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
        cout << i << ": custom allocator value = " << mapWithCustomAllocator[i] << ", std allocator value = " << mapWithStdAllocator[i] << endl;
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
    return 0;
}
