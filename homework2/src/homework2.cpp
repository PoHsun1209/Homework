#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std; // 使用標準命名空間

// ===================== 問題 1：最小優先佇列 =====================

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap;

    // 向上調整堆（插入後保持堆的性質）
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] >= heap[parent]) break;
            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

    // 向下調整堆（刪除根節點後保持堆的性質）
    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < size && heap[left] < heap[smallest])
                smallest = left;
            if (right < size && heap[right] < heap[smallest])
                smallest = right;

            if (smallest == index) break;
            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:
    bool IsEmpty() const override {
        return heap.empty();
    }

    const T& Top() const override {
        if (IsEmpty()) throw runtime_error("堆為空");
        return heap[0];
    }

    void Push(const T& element) override {
        heap.push_back(element);
        heapifyUp(heap.size() - 1);
    }

    void Pop() override {
        if (IsEmpty()) throw runtime_error("堆為空");
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0);
        }
    }

    int size() const { return heap.size(); }
};

// ===================== 問題 2：二元搜尋樹 =====================

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    // 插入節點
    BSTNode* insert(BSTNode* node, int key) {
        if (!node) return new BSTNode(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);

        return node;
    }

    // 取得樹的高度
    int getHeight(BSTNode* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // 找到最小值的節點
    BSTNode* findMin(BSTNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // 刪除節點
    BSTNode* deleteNode(BSTNode* node, int key) {
        if (!node) return node;

        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        } else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        } else {
            // 找到要刪除的節點
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }

            // 節點有兩個子節點
            BSTNode* temp = findMin(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

    // 銷毀整棵樹
    void destroyTree(BSTNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        destroyTree(root);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    int getHeight() {
        return getHeight(root);
    }

    void deleteKey(int key) {
        root = deleteNode(root, key);
    }
};

// 分析 BST 高度與 log2(n) 的比值
void analyzeBSTHeight() {
    cout << "\n=== BST 高度分析 ===\n";
    cout << setw(8) << "n" << setw(10) << "高度"
         << setw(12) << "log2(n)" << setw(15) << "高度/log2(n)" << "\n";
    cout << string(45, '-') << "\n";

    random_device rd;
    mt19937 gen(rd());

    vector<int> sizes = {100, 500, 1000, 2000, 3000, 5000, 10000};

    for (int n : sizes) {
        BST bst;
        uniform_int_distribution<> dis(1, n * 10);

        // 插入 n 個隨機值
        for (int i = 0; i < n; i++) {
            bst.insert(dis(gen));
        }

        int height = bst.getHeight();
        double log2n = log2(n);
        double ratio = height / log2n;

        cout << setw(8) << n << setw(10) << height
             << setw(12) << fixed << setprecision(2) << log2n
             << setw(15) << setprecision(3) << ratio << "\n";
    }
}

// BST 刪除操作示範與時間複雜度說明
void demonstrateBSTDeletion() {
    cout << "\n=== BST 刪除示範 ===\n";
    BST bst;

    // 插入一些值
    vector<int> values = {50, 30, 70, 20, 40, 60, 80};
    cout << "插入數值: ";
    for (int val : values) {
        cout << val << " ";
        bst.insert(val);
    }
    cout << "\n插入後高度: " << bst.getHeight() << "\n";

    // 刪除節點
    cout << "刪除節點 30...\n";
    bst.deleteKey(30);
    cout << "刪除後高度: " << bst.getHeight() << "\n";

    cout << "\nBST 刪除操作時間複雜度:\n";
    cout << "- 最佳/平均情況：O(log n)（平衡樹）\n";
    cout << "- 最差情況：O(n)（偏斜樹）\n";
}

// ===================== 問題 3：外部排序分析 =====================

void solveExternalSorting() {
    cout << "\n=== 外部排序分析 ===\n";

    // 已知參數
    int n = 200000;     // 紀錄數量
    int m = 64;         // 初始區段數
    int S = 2000;       // 記憶體可容納紀錄數
    double ts = 0.08;   // 搜尋時間（秒）
    double tl = 0.02;   // 潛伏時間（秒）
    double tr = 1e-3;   // 每筆紀錄的傳輸時間（秒）

    cout << "已知參數:\n";
    cout << "n = " << n << " 筆紀錄\n";
    cout << "m = " << m << " 區段\n";
    cout << "S = " << S << " 筆紀錄記憶體容量\n";
    cout << "ts = " << ts << " 秒（搜尋時間）\n";
    cout << "tl = " << tl << " 秒（潛伏時間）\n";
    cout << "tr = " << tr << " 秒/筆紀錄（傳輸時間）\n\n";

    // (a) 第二階段輸入時間
    cout << "第 (a) 題：第二階段總輸入時間（k 路合併）\n";
    cout << setw(5) << "k" << setw(15) << "輸入時間(秒)" << "\n";
    cout << string(20, '-') << "\n";

    vector<int> k_values = {2, 4, 8, 16, 32, 64};
    for (int k : k_values) {
        if (k > m) continue;

        int passes = static_cast<int>(ceil(log(m) / log(k)));
        double time_per_pass = ts + tl + n * tr;
        double total_input_time = passes * time_per_pass;

        cout << setw(5) << k << setw(15) << fixed << setprecision(2) << total_input_time << "\n";
    }

    // (b) 分析特定值並列出 Pass 數
    cout << "\n第 (b) 題：指定參數分析\n";
    cout << setw(5) << "k" << setw(15) << "輸入時間(秒)"
         << setw(15) << "合併次數" << "\n";
    cout << string(35, '-') << "\n";

    for (int k = 2; k <= 64; k *= 2) {
        if (k > m) break;

        int passes = static_cast<int>(ceil(log(m) / log(k)));
        double total_input_time = passes * (ts + tl + n * tr);

        cout << setw(5) << k << setw(15) << fixed << setprecision(2)
             << total_input_time << setw(15) << passes << "\n";
    }

    // CPU 時間與輸入時間比較
    cout << "\n分析：何時 t_CPU ≈ t_input？\n";
    cout << "依據以下因素而定：\n";
    cout << "1. CPU 合併速度\n";
    cout << "2. 磁碟 I/O 效能\n";
    cout << "3. k 的大小（合併路數）\n";
    cout << "4. 是否使用有效緩衝策略\n\n";
    cout << "隨著 k 增加，Pass 數減少，I/O 時間下降；\n";
    cout << "但 CPU 處理多路合併的負擔增加。\n";
    cout << "最佳 k 應平衡 I/O 與 CPU 使用。\n";
}

// ===================== 主程式入口 =====================

int main() {
    cout << "資料結構專題解題\n";
    cout << "=========================\n";

    // 問題 1：測試 MinHeap
    cout << "\n=== 問題 1：MinHeap 測試 ===\n";
    MinHeap<int> minHeap;

    vector<int> testValues = {15, 10, 20, 8, 25, 5, 12};
    cout << "插入數值: ";
    for (int val : testValues) {
        cout << val << " ";
        minHeap.Push(val);
    }

    cout << "\n取出最小值順序: ";
    while (!minHeap.IsEmpty()) {
        cout << minHeap.Top() << " ";
        minHeap.Pop();
    }
    cout << "\n";

    // 問題 2：BST 高度分析與刪除示範
    analyzeBSTHeight();
    demonstrateBSTDeletion();

    // 問題 3：外部排序分析
    solveExternalSorting();

    return 0;
}
