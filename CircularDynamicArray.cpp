// Michael Reis
#include <iostream>
#include <cstdlib>


using namespace std;
template <typename T>
class CircularDynamicArray {
    private:
    int size, cap, front, end;
    T *data;
    T outOfBounds;
    void shrink() {
        T *temp = new T[cap / 2];  
        for (int i = 0; i < size; i++) {
            temp[i] = this->operator[](i);  
        }
        delete[] data; 
        data = temp; 
        
        front = 0; 
        end = size - 1; 
        cap /= 2; 
        
        
    }
    void dbl() {
        T *temp = new T[cap * 2];  
        for (int i = 0; i < size; i++) {
            temp[i] = this->operator[](i); 
        }
        delete[] data; 
        data = temp; 
        front = 0; 
        end = size - 1; 
        cap *= 2; 
        
    }
    T Select(T temp[], int s ,int k, bool WC = false) {
        T pivot;
        if (!WC) {
            pivot = temp[rand() % s];
        }
        else if (WC) {
            pivot = wcPivot(temp, s);
        }
        int l = 0;
        int e = 0;
        int g = 0;
        T *L = new T[s];
        T *E = new T[s];
        T *G = new T[s];
        for (int i = 0; i < s; i++) {
            if (temp[i] < pivot) {
                L[l] = temp[i];
                l++;
            }
            else if (temp[i] == pivot) {
                E[e] = temp[i];
                e++;
            }
            else if (temp[i] > pivot) {
                G[g] = temp[i];
                g++;
            }
        }
        if (k <= l) {
            return Select(L, l, k, WC);
        }
        else if (k <= l + e) {
            return pivot;
        }
        else {
            return Select(G, g, k - l - e, WC);
        }
        
    }
    T wcPivot(T temp[], int s) {
        int g = 5;
        if (s <= g) {
            return medianOfGroup(temp, 0, s - 1);
        }
        int medianGroupSize = 0;
        if ((s % g) == 0) {
            medianGroupSize = s / g;
        }
        else  { 
            medianGroupSize = (s / g) + 1;
        }
        T *medianGroup = new T[medianGroupSize];
        for (int i = 0; i < medianGroupSize; i++) {
            medianGroup[i] = medianOfGroup(temp, i * 5, min(i * 5 + 4, s - 1));
        }
        return Select(medianGroup, medianGroupSize, medianGroupSize / 2, true) ;

    }
    T medianOfGroup(T temp[], int first, int last) {
        for(int i = first; i < last; i++) {
            int j = i + 1;
            while(j > first  && temp[j-1] > temp[j]) {
                swap(temp[j-1], temp[j]);
                j--;
            }
        }
        return temp[(first + last) / 2];
    }
    public:
    CircularDynamicArray() { 
        cap = 2;
        size = 0;
        front = 0; 
        end = 1;  
        size = 0;
        data = new T[cap]; 
        
    }
    CircularDynamicArray(int s) { 
        cap = s;
        size = s;
        front = 0; 
        end = size - 1; 
        data = new T[cap]; 
    }
    ~CircularDynamicArray() { 
        delete[] data; 
    }
    CircularDynamicArray& operator=(const CircularDynamicArray &src) { 
        if (this != &src) {
            delete[] data;
            deepCopy(src);
        }
        return *this;
    }
    CircularDynamicArray(const CircularDynamicArray &src) { 
        deepCopy(src);
    }
    void deepCopy(const CircularDynamicArray &src) {
        data = new T[src.cap];
        for (int i = 0; i < src.cap; i++) {
            data[i] = src.data[i];
        }
        size = src.size;
        cap = src.cap;
        front = src.front;
        end = src.end;
    }

    T& operator[](int i) {
        if (i >= size || i < 0 ) {
            cout << i << " is out of bounds" << endl; 
            return outOfBounds;
            }  
        return data[(i + front) % cap]; 
    }
   void addEnd(T v) {
        if (size >= cap) { 
            dbl();
        }
        end = (end + 1) % cap; 
        data[end] = v; 
        size++; 
    }
    void addFront(T v) {
        if (size >= cap) { 
            dbl(); 
        }
        front = (front - 1 + cap) % cap; 
        data[front] = v; 
        size++; 
    }
    void delEnd() {
        data[end] = outOfBounds; 
        size--; 
        end = (end - 1 + cap) % cap; 
        if (size == 0) { 
            return;
        }
        else if (size <= (double)cap / 4) { 
            shrink(); 
        }        
    }
    void delFront() {
        data[front] = outOfBounds; 
        size--; 
        front = (front + 1) % cap; 
        if (size == 0) { 
            return;
        }
        else if (size <= (double)cap / 4) { 
            shrink();
        }

    }
    int length() {
        return size; 
    }
    int capacity() {
        return cap; 
    }
    void clear() { 
        delete[] data;
        cap = 2;
        size = 0;
        front = 0; 
        end = 1;  
        size = 0;
        data = new T[cap]; 
    }
    T QuickSelect(int k) {
        T kthSmallest;
        T *temp = new T[size];
        for (int i = 0; i < size; i++) {
            temp[i] = this->operator[](i);
        }
        kthSmallest = Select(temp, size, k);
        return kthSmallest;

    }
    T WCSelect(int k) {
        T kthSmallest;
        T *temp = new T[size];
        for (int i = 0; i < size; i++) {
            temp[i] = this->operator[](i);
        }
        kthSmallest = Select(temp, size, k, true);
        return kthSmallest;
    }
    void stableSort() {
        T *temp = new T[size];
        for (int i = 0; i < size; i++) {
            temp[i] = this->operator[](i);
        }
        for (int i = 0; i < size; i++) {
            data[i] = temp[i];
        }
        mergesort(data, size);
        front = 0;
        end = size - 1;
    }
    void mergesort(T data[], int n) {
        if (n <= 1) return;
        T *data1 = new T[n/2];
        T *data2 = new T[n-(n/2)];
        for (int i = 0; i < n/2; i++) 
            data1[i] = data[i]; 
        for (int i = n/2; i<n; i++) 
            data2[i - (n/2)] = data[i];   
        mergesort(data1, n/2);
        mergesort(data2, n-(n/2));
        merge(data, data1, data2, n);
    }

    void merge(T data[], T data1[], T data2[], int n) {
        int n1 = n/2, n2 = n-(n/2);
        int p1 = 0, p2 = 0, i = 0;
        while (p1 < n1 && p2 < n2) data[i++] = data1[p1]<data2[p2] ? data1[p1++] : data2[p2++];
        while (p1 < n1) data[i++] = data1[p1++];
        while (p2 < n2) data[i++] = data2[p2++];
   
    }
    int linearSearch(T e) {
        for (int i = 0; i < size; i++) { 
            if (data[(i + front) % cap] == e ) { 
                return i; 
            }
        }
        return -1; 
    }
    int binSearch(T e) { 
        int tempFront = 0;
        int tempEnd = this->size - 1;
        return bSearch(e, tempFront, tempEnd);
    }
    int bSearch(T e, int tempFront, int tempEnd) {
        if (tempFront > tempEnd) {
            return -1;
        }
            int middle = (tempFront + tempEnd) / 2;
                if (e == this->operator[](middle)) {

                    return middle;
                }
                else if (e > this->operator[](middle)) {
                    tempFront = middle + 1;
                    return bSearch(e, tempFront, tempEnd);
                }   
                else {
                    tempEnd = middle - 1;
                    return bSearch(e, tempFront, tempEnd);
                }
        return -1; 
    }
};

