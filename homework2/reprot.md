
# 41243237

陳柏勳

作業二

## 解題說明

#### 題目一

1. 仿照ADT5.2 MaxPQ 寫出一個抽象類別 MinPQ。
2. 實作一個繼承自 MinPQ的具體類別 MinHeap，還有所以純虛函式，且時間複雜度與 MaxHeap 相同。

#### 題目二

##### (a)

- 建立一棵 BST，進行 n 筆隨機插入，觀察並畫出： ratio = height/ $$\log_2 n$$ ，並對不同的 n 繪圖，檢查是否約為常數 (理論值約為 4 ~  2，依分布狀況略變)。

##### (b)

- 實作刪除節點的函式，並分析其時間複雜度。

#### 題目三

##### (a) 

計算第二階段外部排序的輸入總時間 f_input，假設使用 k-way merge，並使用 Program 7.21 的緩衝區重疊技術。

##### (b) 給定參數:

- t_seek = 80ms, t_latency = 20ms
- t_transmit = 10⁻³ 秒/紀錄
- n = 200,000, m = 64, S = 2000，求出 f_input 與 k 的關係圖，並探討是否存在 tcpu = f_input 的 k 值。


### 解題策略

#### 題目一

1. 建立 MinPQ 抽象類別，與MaxPQ 類似，但操作應以最小值為優先。
2. 實作 MinHeap 類別。
- 使用陣列模擬堆積。
- 實作 Push (插入)、Pop (刪除最小值)、Top (取出最小值)、IsEmpty。
- 操作時間複雜度為 $O(\log n)$（插入與刪除），Top 和 IsEmpty 為 $O(1)$。

#### 題目二

##### (a)

1. 建立空 BST。
2. 使用隨機數產生器插入 n 筆資料（無重複）。
3. 實作函式計算 BST 高度。
4. 計算 $$\log_2 n$$ ，輸出高度除以 $$\log_2 n$$ 。
5. 重複測試 n = 100, 500, 1000, ..., 10000。
6. 使用工具（例如 Excel 或 Python）繪製圖表。

##### (b)

- BST 中刪除節點的情況：
  1. 無子節點：直接刪除。
  2. 一個子節點：用該子節點取代。
  3. 兩個子節點：用「右子樹中的最小節點」或「左子樹中的最大節點」取代。

#### 題目三

1. 計算流程

- 每個 buffer 大小 : B = S / k + 1
- 每次 I/O 的時間（t_IO）： t_io = t_seek + t_latency + B · t_transmit = 0.08 + 0.02 + (2000/(k+1)) · 0.001
- 每個 run 含紀錄數： n / m = 200000 / 64 = 3125
- 每個 run 所需 I/O 次數： 3125 / B = 3125(k+1) / 2000
- 所有 runs 總輸入時間： f_input(k) = m ·  3125(k+1) / 2000 · t_io
- 整理後公式為： f_input(k) = 100(k+1)

2. 數值代入與結果表格

| k | f_input(k)計算過程 | 輸入總時間(秒) |
|---|-------------------|---------------|
|2|10*3+200|230|
|4|10*5+200|250|
|8|10*9+200|290|
|16|10*17+200|370|
|32|10*33+200|530|
|63|10*64+200|840|

3. 趨勢分析圖（f_input vs. k）

## 程式實作

### 問題 1：最小優先佇列（MinHeap）

說明
使用 C++ 實作一個最小優先佇列（Min Priority Queue），採用最小堆（Min Heap）資料結構。主要操作有：
- Push(): 插入新元素，透過向上調整（heapify up）維持堆性質。
- Pop(): 移除最小元素，透過向下調整（heapify down）維持堆性質。
- Top(): 取得最小元素（不移除）。
- IsEmpty(): 檢查是否為空堆。

```cpp
template <class T>
class MinHeap : public MinPQ<T> {
private:
    vector<T> heap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index] >= heap[parent]) break;
            swap(heap[index], heap[parent]);
            index = parent;
        }
    }

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
    bool IsEmpty() const override { return heap.empty(); }
    const T& Top() const override { return heap[0]; }
    void Push(const T& element) override {
        heap.push_back(element);
        heapifyUp(heap.size() - 1);
    }
    void Pop() override {
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
    }
};
```

### 問題 2：二元搜尋樹（BST）高度與刪除分析

說明
- 動態插入隨機整數節點。
- 透過遞迴取得 BST 高度：getHeight()。
- 用 $$\log2 n$$ 與實際高度作比較，分析隨機插入的效率。
- 刪除節點時考慮：
  - 無子節點（直接刪除）
  - 一個子節點（接上子節點）
  - 兩個子節點（找右子樹最小值代替）

```cpp

// 計算樹的高度（遞迴）
int getHeight(BSTNode* node) {
    if (!node) return 0;
    return 1 + max(getHeight(node->left), getHeight(node->right));  // ✅ 重點：高度定義
}

// 刪除節點的遞迴處理
BSTNode* deleteNode(BSTNode* node, int key) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = deleteNode(node->left, key);
    } else if (key > node->key) {
        node->right = deleteNode(node->right, key);
    } else {
        // ✅ 重點：找到目標節點並處理三種情況
        if (!node->left) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (!node->right) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }
        // 兩個子節點：找右子樹最小節點
        BSTNode* temp = findMin(node->right);
        node->key = temp->key;
        node->right = deleteNode(node->right, temp->key);
    }
    return node;
}

```

### 問題三：外部排序第二階段輸入時間分析

說明
- 模擬磁碟合併排序的輸入時間。
- 以不同 k 值（合併路數）計算 Pass 數與總時間。
- 越大的 k → Pass 數少，但 CPU 合併負擔加重。

```cpp

int passes = ceil(log(m) / log(k));  //  重點：合併所需的 Pass 數
double time_per_pass = ts + tl + n * tr;
double total_input_time = passes * time_per_pass;

```

## 效能分析

### 題目一：MinHeap 效能分析

| 題目 | 操作 | 時間複雜度 | 空間複雜度 | 備註 |
|--|--|--|--|--|
| Min Heap | 插入 (Push) | $O(\log n)$ | $O(n)$ | Heap 使用陣列實作，節省空間 |
|          | 取出最小值 (Pop) | $O(\log n)$ | $O(n)$ | heapifyDown 時最多 log n 步 |
|          | 取得最小值 (Top) | $O(1)$      | $O(1)$ | 直接讀取 heap[0] |
| BST 隨機插入 / 刪除 | 插入 (Insert) | $O(\log n)$ (平均) $O(n)$  (最差) | $O(n)$ | 平均情況為平衡樹；最差為鏈狀結構 |
|                    | 刪除 (Delete) | $O(\log n)$ (平均) $O(n)$  (最差) | $O(n)$ | 需要尋找前驅 / 後繼節點 |
|                    | 查詢高度 | $O(n)$ | $O(1)$ | 遞迴呼叫會用到額外堆疊記憶體 |
| BST 高度分析 | 建樹後計算高度 | $O(n)$ | $O(1)$ | 單次 traversal |
|             | $O(\log_2 n)$ | $O(1)$ | $O(1)$ | 數學運算，不需額外空間 |
| 外部排序（第二階段）| 合併排序（每 pass）| $O(n\log_k m)$ | $O(k)$ + $O(n)$ | k：合併路數；n：每筆資料長度 |
|                   | 每次輸入 | $O(n)$ | $O(n)$ | 須暫存每段開頭資料 |

## 測試與驗證

### 題目一：MinHeap 效能分析

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
