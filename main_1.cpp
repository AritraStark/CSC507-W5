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

vector<int> arr;
atomic<int> currIdx(0);
vector<Node*> nodes;
Node* head = nullptr;

void insert(int idx, int val) {
    nodes[idx] = new Node(val);
}

void* insertWorker(void* arg) {
    for(;;) {
        int idx = currIdx.fetch_add(1);
        if (idx >= static_cast<int>(arr.size()))
            break;
        insert(idx, arr[idx]);
    }
    return nullptr;
}

bool verifyList(Node* head, const vector<int>& arr) {
    Node* curr = head;
    for (int i = 0; i < (int)arr.size(); i++) {
        if (!curr || curr->data != arr[i]) return false;
        curr = curr->next;
    }
    if (curr != nullptr) return false;
    return true;
}

int main() {
    ifstream fin("input.txt");
    if (!fin) {
        cerr << "Error: Could not open input.txt\n";
        return 1;
    }

    int val;
    while (fin >> val && (int)arr.size() < N) {
        arr.push_back(val);
    }
    fin.close();

    if ((int)arr.size() < N) {
        cerr << "Warning: File has fewer than N numbers!\n";
    }

    nodes.resize(arr.size(), nullptr);

    pthread_t threads[M];
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < M; i++) 
        pthread_create(&threads[i], nullptr, insertWorker, nullptr);
    for (int i = 0; i < M; i++) 
        pthread_join(threads[i], nullptr);

    for (int i = 0; i < (int)arr.size()-1; i++) {
        nodes[i]->next = nodes[i+1];
    }
    head = nodes[0];

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - start;

    bool check = verifyList(head, arr);

    cout << "Verification: " << (check ? "PASS" : "FAIL") << endl;
    cout << "Execution Time: " << time.count() << " seconds" << endl;

    return 0;
}
