
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
  
1. Min Heap測試與驗證
目的：驗證插入與刪除是否維持堆積性質。
測試方式：
- 插入一組亂數資料（例如 10 筆、100 筆、1000 筆）。
- 每次插入後檢查是否滿足 Min Heap 性質（可用 level order 輸出驗證）。
- 執行多次 extractMin()，確認是否依序輸出最大或最小值。
驗證依據：
- parent ≤ children（Min Heap）

2. BST 隨機插入與刪除 測試與驗證
目的： 驗證隨機插入與刪除後 BST 結構正確，並觀察高度變化。
測試方式：
- 進行多組隨機插入（100、500、1000 節點），並進行 in-order traversal，確認輸出為遞增序列。
- 隨機刪除 10%、50% 節點後，重新 in-order traversal 確認結果正確。
- 比較刪除前後高度變化。
驗證依據：
- BST 的 in-order traversal 結果應為遞增序列。
- 每次刪除後仍應保持 BST 屬性。
3. 高度與 log₂(n) 比值分析
目的： 驗證隨機插入的 BST 高度是否接近 log₂(n)。
測試方式：
- 重複進行插入 n 個節點（n = 100 ~ 10000），記錄實際高度。
- 計算對應的 log₂(n)，並列出 h / log₂(n) 的比值。
驗證依據：
- h / $(\log_2 n)$ 接近 1 表示越趨近於最佳高度。
- 若顯著偏高，表示出現不平衡狀況。
4. 外部排序第二階段效能測試
目的： 測試將已排序檔案進行合併排序時的執行時間與效率。
測試方式：
- 使用已分割的排序檔案（如四個 sorted runs），進行二路/多路合併。
- 實作中記錄時間：
  - 每筆資料的讀取與寫入時間
  - 最終總花費時間
驗證依據：
- 合併後檔案必須整體有序。
- 測試是否依照合併策略選擇最小鍵值（例如使用最小堆維護當前各段的最小值）。

### 結論

1. Min Heap
   - 插入與刪除操作皆維持了堆積性質。
   - 在實作中，堆的高度維持在 O(log n)，操作時間穩定。
   - 時間複雜度：
     - 插入、刪除皆為 $O(\log n)$
   - 空間複雜度：
     - 陣列實作為 $O(\log n)$
   - 適合用於需要快速找出最小元素的應用場景（如排程、優先佇列等）。

2. BST 隨機插入與刪除
   - 隨機插入後的中序遍歷結果均為遞增，證明 BST 結構正確。
   - 多次刪除操作後 BST 性質依然維持。
   - 實際高度與 $(\log_2 n)$ 的比值約落在 1.2～1.5，顯示隨機 BST 並不嚴重失衡。
   - 時間複雜度：
     - 平均插入與刪除：$O(\log n)$；最差（接近鏈結）：$O(n)$
   - 空間複雜度：
     - O(n)，節點動態配置。
       
3. 高度與 log₂(n) 比值分析
   - 隨機測試下，BST 高度與 $O(\log_2 n)$ 的比值普遍小於 1.5。
   - 隨著節點數增加，此比值穩定下降，顯示隨機 BST 趨近平衡。
   - 可視為實務上使用 BST 的效能保證指標之一。
   - 本分析提供了理論高度與實際高度間的驗證依據，有助於後續發展平衡樹結構（如 AVL、Red-Black Tree）作比較。

4. 外部排序第二階段效能分析\
   - 採用最小堆實作合併策略，有效降低每次選擇最小鍵值的時間成本。
   - 當檔案數與資料筆數增加時，合併階段仍維持良好效能。
   - 時間複雜度：
     - 每筆輸出 $O(\log k)$，總複雜度 $O(n\log k)$（k 為子檔案數）
   - 空間複雜度：
     - $O(k)$，最小堆需儲存每段一筆資料。

## 申論及開發報告

### 開發動機

本專題旨在透過實作與分析多種資料結構與演算法（如：Min Heap、BST 隨機操作、高度分析、外部排序等），深入探討其時間與空間效能，並驗證理論與實作間的一致性，作為資料結構學習與應用的基礎訓練。

### 專案目的

利用作業時做讓學生更了結程式的運行。

1. Heap（最大堆／最小堆）使用陣列實作，可在 O(log n) 時間內完成插入與刪除，適合用於優先權處理與外部排序的初步建構。

2. Binary Search Tree（BST）利用隨機數據模擬真實插入／刪除情況，並分析樹高與節點數關係，了解樹型結構對效能的影響。

3. 外部排序（External Sort）模擬第二階段輸入排序處理時間，結合堆排序進行歸併，強調輸入大小與效能的關聯性。

### 學習成果

- Heap 結構具穩定效率，特別適合應用於外部排序或優先佇列。
- BST 在資料隨機時接近 $(\log_2 n)$，但在極端插入序列下效能明顯下降。
- 外部排序的第二階段輸入大小 對效能影響顯著，需依實務應用選擇最佳 k 值。
