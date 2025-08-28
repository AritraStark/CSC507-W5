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
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        pthread_mutex_unlock(&lock);
    }

    bool verify(const vector<int>& nums) {
        Node* curr = head;
        for (int x : nums) {
            if (!curr || curr->data != x) return false;
            curr = curr->next;
        }
        return (curr == nullptr);
    }
};

LinkedList llist;
vector<int> numbers;
atomic<int> currentIndex(0);

void* insertWorker(void* arg) {
    while (true) {
        int idx = currentIndex.fetch_add(1);
        if (idx >= (int)numbers.size()) break;
        llist.insert(numbers[idx]);
    }
    return nullptr;
}

// -------------------- MAIN --------------------
int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./program <N> <M>\n";
        return 1;
    }
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);

    numbers.clear();
    numbers.reserve(N);
    for (int i = 0; i < N; i++) numbers.push_back(i);

    auto start = chrono::high_resolution_clock::now();

    pthread_t threads[M];
    for (int i = 0; i < M; i++) pthread_create(&threads[i], nullptr, insertWorker, nullptr);
    for (int i = 0; i < M; i++) pthread_join(threads[i], nullptr);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << N << " " << M << " " << duration.count()
         << " " << (llist.verify(numbers) ? "PASS" : "FAIL") << endl;

    return 0;
}
