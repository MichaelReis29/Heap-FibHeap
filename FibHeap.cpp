// Michael Reis
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
  enum Mark {
            unmarked, 
            marked
        };
 template <typename keytype>
        struct FibHeapNode {
            keytype key;
            FibHeapNode<keytype> *p;
            FibHeapNode<keytype> *c;
            FibHeapNode<keytype> *lSib;
            FibHeapNode<keytype> *rSib;
            int rank;
            Mark mark = unmarked;
        };
template <typename keytype>
class FibHeap{
    private:

        int size;
        FibHeapNode<keytype> *nil;
        FibHeapNode<keytype> *min;
        FibHeapNode<keytype> *head;
        FibHeapNode<keytype> *tail;

        void addRootList(FibHeapNode<keytype> *x) {
            if (min == nil) {
                x->lSib = x;
                x->rSib = x;
                head = x;
                tail = x;
                min = x;
            }
            else {
                x->lSib = tail;
                x->rSib = head;
                head->lSib = x;
                tail->rSib = x;
                tail = x;
                if (x->key < min->key) {
                    min =x;
                }
            }
        }
        void removeRootList(FibHeapNode<keytype> *x) {
            (x->lSib)->rSib = x->rSib;
            (x->rSib)->lSib = x->lSib;
            if (x == head && size > 1) {
                head = x->rSib;
            }
            else if (x == tail && size > 1) {
                tail = x->lSib;
            }
            if (size == 1) {
                head = nil;
                tail = nil;
            }
        }

        void consolidate() {
            double phi = (1 + sqrt(5)) / 2;
            int aSize = log2(size)/log2(phi);
            FibHeapNode<keytype>* A[aSize];

            for (int i = 0; i < aSize; i++) {
                A[i] = nil;
            }
            FibHeapNode<keytype>* x = head;
            do {
                int d = x->rank;
                while (A[d] != nil && A[d]->key != x->key) {
                    FibHeapNode<keytype>* y = A[d];
                    if (x->key > y->key) {
                        FibHeapNode<keytype>* temp = x;
                        x = y;
                        y = temp;
                    }
                    fibHeapLink(y, x);
                    A[d] = nil;
                    d++;
                }
                A[d] = x;
                x = x->rSib;

            } while (x != head);
                min = nil;
                head = nil;
                tail = nil;
                for (int i = 0; i < aSize; i++) {
                    if (A[i] != nil) {
                        addRootList(A[i]);
                    }
                }
        }

        void fibHeapLink(FibHeapNode<keytype> *y, FibHeapNode<keytype> *x) {
            removeRootList(y);
            if (x->rank == 0) {
                y->p = x;
                x->c = y;
                y->rSib = y;
                y->lSib = y;
            }
            else {
                y->p = x;
                y->lSib = x->c->lSib;
                y->rSib = x->c;
                (x->c->lSib)->rSib = y;
                x->c->lSib = y;
                x->c = y;
            }
            x->rank++;
            y->mark = unmarked;
        }

        void recursePreorder(FibHeapNode<keytype> *x, FibHeapNode<keytype> *child, FibHeapNode<keytype> *head)  {
            if (x == nil) {
                return;
            }
            cout << x->key << " ";
            recursePreorder(x->c, x->c, head);
            if (x->rSib == child || x == head) {
                return;
            }
            recursePreorder(x->rSib, child, head);

        }

        void recurseDelete(FibHeapNode<keytype> *x, FibHeapNode<keytype> *child) {
            if (x == nil) {
                return;
            }
            recurseDelete(x->c, x->c);
            if (x->rSib != child) {
                recurseDelete(x->rSib, child);
            }
            delete x;

        }

        void cut(FibHeapNode<keytype> *x, FibHeapNode<keytype> *y) {
            if (y->c == x && x == x->rSib) {
                y->c = nil;
            }
            else if (y->c == x) {
                y->c = x->rSib;  
            }
            (x->lSib)->rSib = x->rSib;
            (x->rSib)->lSib = x->lSib;
            y->rank--;
            addRootList(x);
            x->p = nil;
            x->mark = unmarked;
        }

        void cascadingCut(FibHeapNode<keytype> *y) {
            FibHeapNode<keytype> *z = y->p;
            if (z != nil) {
                if (y->mark == unmarked) {
                    y->mark = marked;
                }
                else {
                    cut(y, z);
                    cascadingCut(z);
                }
            }
        }

        FibHeapNode<keytype>* deleteMin() {
            FibHeapNode<keytype> *z = min;
            FibHeapNode<keytype> *x;
            FibHeapNode<keytype> *r;
            if (min != nil) {
                if (z->c != nil) {
                    x = z->c;
                    for (int i = 0; i < z->rank; i++) {
                        r = x->rSib;
                        addRootList(x);
                        x->p = nil;
                        x = r;
                    }
                }
                removeRootList(z);
                if (z == z->rSib) {
                    min = nil;
                }
                else {
                    min = z->rSib;
                    consolidate();
                }
                size--;
            }
            return z;
        }

    public:

        FibHeap() {
            size = 0;
            nil = nullptr;
            min = nil;
            head = nil;
            tail = nil;
        }

        FibHeap(keytype k[], int s, CircularDynamicArray<FibHeapNode<keytype> *>&handle) {
            size = 0;
            nil = nullptr;
            min = nil;
            head = nil;
            tail = nil;
            for(int i = 0; i < s; i++) {
                handle.addEnd(insert(k[i]));
            }
            consolidate();
        }
        ~FibHeap () {
            if (size == 0) {
                return;
            }
            FibHeapNode<keytype> *x = head;
            recurseDelete(x, x);
        }

        keytype peekKey() {
            return min->key;
        }

        keytype extractMin() {
            FibHeapNode<keytype> *z = min;
            FibHeapNode<keytype> *x;
            FibHeapNode<keytype> *r;
            if (min != nil) {
                if (z->c != nil) {
                    x = z->c;
                    for (int i = 0; i < z->rank; i++) {
                        r = x->rSib;
                        addRootList(x);
                        x->p = nil;
                        x = r;
                    }
                }
                removeRootList(z);
                if (z == z->rSib) {
                    min = nil;
                }
                else {
                    min = z->rSib;
                    consolidate();
                }
                size--;
            }
            return z->key;
        }

        FibHeapNode<keytype>* insert(keytype k) {
            FibHeapNode<keytype> *x = new FibHeapNode<keytype>;
            x->key = k;
            x->p = nil;
            x->c = nil;
            x->lSib = nil;
            x->rSib = nil;
            x->rank = 0;
            x->mark = unmarked;
            size++;
            addRootList(x);
            return x;

        }

        bool decreaseKey(FibHeapNode<keytype> *h, keytype k) {
            if (k > h->key) {
                return false;
            }
            h->key = k;
            FibHeapNode<keytype> *y = h->p;
            if (y != nil && h->key < y->key) {
                cut(h, y);
                cascadingCut(y);
            }
            if (h->key < min->key) {
                min = h;
            }
            return true;
        }

        void merge(FibHeap<keytype> &H2) {
            tail->rSib = H2.head;
            H2.head->lSib = tail;
            head->lSib = H2.tail;
            H2.tail->rSib = head;
            tail = H2.tail;
            if (min == nil || (H2.min != nil && H2.min->key < min->key)) {
                min = H2.min;
            }
            size = size + H2.size;
            H2.min = nil;
            H2.head = nil;
            H2.tail = nil;
            H2.size = 0;
        }

        void printKey() {
            if (size == 0) {
                return;
            }
            FibHeapNode<keytype> *x = head;
            do {
                cout << "Rank " << x->rank << endl;
                recursePreorder(x, x, x);
                cout << endl;
                x = x->rSib;
                if (x != head) {
                    cout << endl;
                }
            } while (x != head);
        }
       
};
