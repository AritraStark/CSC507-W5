#include <bits/stdc++.h>
#include <pthread.h>
#include <atomic>
using namespace std;

class LinkedList {
private:
    struct Node {
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    pthread_mutex_t lock;

public:
    LinkedList() : head(nullptr), tail(nullptr) {
        pthread_mutex_init(&lock, nullptr);
    }

    ~LinkedList() {
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
        pthread_mutex_destroy(&lock);
    }

    void insert(int val) {
        Node* newNode = new Node(val);
        pthread_mutex_lock(&lock);
        if (!head) {
            head = newNode;
            tail = newNode;
        } 
        else {
            tail->next = newNode;
            tail = newNode;
        }
        pthread_mutex_unlock(&lock);
    }

    bool verify(const vector<int>& nums) {
        Node* curr = head;
        for (int x : nums) {
            if (!curr || curr->data != x) 
                return false;
            curr = curr->next;
        }
        return curr == nullptr;
    }
};

LinkedList llist;
atomic<int> currIdx(0);
vector<int> arr;


void* insertWorker(void* arg) {
    for(;;) {
        int idx = currIdx.fetch_add(1);
        if (idx >= static_cast<int>(arr.size())) {
            break;
        }
        llist.insert(arr[idx]);
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./program <N> <M>\n";
        return 1;
    }
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);

    arr.clear();
    arr.reserve(N);

    for (int i = 0; i < N; i++) 
        arr.push_back(i);

    pthread_t threads[M];
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < M; i++) 
        pthread_create(&threads[i], nullptr, insertWorker, nullptr);
    for (int i = 0; i < M; i++) 
        pthread_join(threads[i], nullptr);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> time = end - start;

    cout << N << " " << M << " " << time.count()
         << " " << (llist.verify(arr) ? "PASS" : "FAIL") << endl;

    return 0;
}
