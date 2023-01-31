#include <iostream>
#include <cstdlib>

using namespace std;
template<typename keytype>
class Heap {
    private: 
    CircularDynamicArray<keytype> *heap;
    int size;
    keytype nil;

    void bottomUp(int i) {
        int l = i * 2;
        int r = i * 2 + 1;
        int smaller;
        if (l<=size && (*heap)[l]<(*heap)[i]) {
            smaller=l;
        }
        else {
            smaller=i;
        }
        if (r<=size && (*heap)[r]<(*heap)[smaller]) {
            smaller=r;
        }
        if ((*heap)[i] > (*heap)[smaller]) {
            keytype temp = (*heap)[i];
            (*heap)[i] = (*heap)[smaller];
            (*heap)[smaller] = temp;
            bottomUp(smaller);
        }
    }
    void deepCopy(const Heap &src) {
        heap = new CircularDynamicArray<keytype>(*src.heap);
        size = src.size;
    }

    public: 
    Heap() {
        heap = new CircularDynamicArray<keytype>();
        size = 0;
        heap->addEnd(nil);
    }
    Heap(keytype k[], int s) {
        heap = new CircularDynamicArray<keytype>();
        size = s;
        heap->addEnd(nil);
        for (int i = 0; i < size; i++) {
            heap->addEnd(k[i]);
        }
        for (int i = size / 2; i > 0; i--) {
           bottomUp(i);
        }
    }

    ~Heap() {
        delete heap;
    }

    Heap& operator=(const Heap &src) { // asignment operator
        if (this != &src) {
            deepCopy(src);
        }
        return *this;
    }

    Heap(const Heap &src) { // copy constructor
        deepCopy(src);
    }

    keytype peekKey() {
        return (*heap)[1];
    }

    keytype extractMin() {
        keytype min = peekKey();
        if (size == 1) {
            heap->delEnd();
            size--;
            return min;
        }
        (*heap)[1] = (*heap)[size];
        heap->delEnd();
        size--;
        bottomUp(1);
        return min;
    }

    void insert(keytype k) {
        size++;
        heap->addEnd(k);
        int a = size;
        for (int i = size / 2; i > 0; i /= 2) {
            if ((*heap)[a] < (*heap)[i]) {
                keytype temp = (*heap)[i];
                (*heap)[i] = (*heap)[a];
                (*heap)[a] = temp;
                a /= 2;
            }
        }
    }

    void printKey() {
        if (size == 0) {
            return;
        }
        for (int i = 1; i <= size; i++) {
            cout << (*heap)[i] << " ";
        }
        cout << endl;
    }
};