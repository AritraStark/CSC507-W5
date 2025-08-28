#include <bits/stdc++.h>
#include <pthread.h>
#include <atomic>
using namespace std;

#define N 1000000  
#define M 4         

struct Node {
    int data;
    Node* next;
    Node(int val=0) : data(val), next(nullptr) {}
};

vector<int> numbers;
atomic<int> currentIndex(0);
vector<Node*> nodes;  // storage for nodes
Node* head = nullptr;

void insert(int idx, int val) {
    // create node at the right index position
    nodes[idx] = new Node(val);
}

void* insertWorker(void* arg) {
    while (true) {
        int idx = currentIndex.fetch_add(1);
        if (idx >= (int)numbers.size()) break;
        insert(idx, numbers[idx]);   // use insert() function
    }
    return nullptr;
}

int main() {
    ifstream fin("input.txt");
    if (!fin) {
        cerr << "Error: Could not open input.txt\n";
        return 1;
    }

    int val;
    while (fin >> val && (int)numbers.size() < N) {
        numbers.push_back(val);
    }
    fin.close();

    if ((int)numbers.size() < N) {
        cerr << "Warning: File has fewer than N numbers!\n";
    }

    // Preallocate nodes array
    nodes.resize(numbers.size(), nullptr);

    // Step 2: Create M threads
    pthread_t threads[M];
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < M; i++) pthread_create(&threads[i], nullptr, insertWorker, nullptr);
    for (int i = 0; i < M; i++) pthread_join(threads[i], nullptr);

    // Step 3: Link nodes in correct order
    for (int i = 0; i < (int)numbers.size()-1; i++) {
        nodes[i]->next = nodes[i+1];
    }
    head = nodes[0];

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    // Step 4: Verify correctness
    Node* curr = head;
    bool ok = true;
    for (int i = 0; i < (int)numbers.size(); i++) {
        if (!curr || curr->data != numbers[i]) { ok = false; break; }
        curr = curr->next;
    }
    if (curr != nullptr) ok = false;

    cout << "Verification: " << (ok ? "PASS" : "FAIL") << endl;
    cout << "Execution Time: " << duration.count() << " seconds" << endl;

    return 0;
}
