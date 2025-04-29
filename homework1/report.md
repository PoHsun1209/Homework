
# 排序演算法報告 資料結構(二) 作業一

**41243237 陳柏勳**  
APR. 27 / 2025  

---

## 目錄

1. [解題說明](#解題說明)
2. [演算法設計與實作](#演算法設計與實作)
3. [效能分析](#效能分析)
4. [測試與結果](#測試與結果)
5. [結論](#結論)

---

## 解題說明

此報告比較了四種常見排序演算法：插入排序、快速排序、合併排序和堆積排序。在測試中，我選擇了五個不同規模的資料集（500、1000、2000、4000、5000）進行排序操作，並測量了每種演算法在這些資料集下的執行時間。目的是評估這些演算法在不同資料集規模下的效能，以便選擇適合特定應用場景的排序算法。

---

## 演算法設計與實作

### 插入排序 (Insertion Sort)
插入排序是一種簡單的排序算法，其基本思想是將資料分為已排序和未排序兩部分，並從未排序的部分選擇一個元素，將其插入到已排序部分的適當位置。這個過程會一直進行，直到整個資料集排序完成。插入排序在資料量小的情況下表現良好，但對大資料集的排序效率較低。

```cpp
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```
### 快速排序 (Quick Sort)
快速排序是一種基於分治法的排序演算法。其基本思想是從資料集中選擇一個基準元素（pivot），將資料集分成兩個部分，一邊比基準小，另一邊比基準大，然後分別對這兩個部分遞迴進行排序。快速排序的平均時間複雜度為𝑂(𝑛log𝑛)通常比其他排序演算法更高效。

```cpp
void quickSort(vector<int>& arr, int low, int high) {
    if (low + 10 <= high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    } else {
        insertionSort(arr);
    }
}

int medianOfThree(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    swap(arr[mid], arr[high - 1]);
    return arr[high - 1];
}

```

### 合併排序 (Merge Sort)
合併排序也是一種基於分治法的排序演算法，其基本思想是將資料分成兩半，遞迴地對每個子資料集進行排序，然後將兩個已排序的子資料集合併成一個排序好的資料集。合併排序的時間複雜度為 𝑂(𝑛log𝑛)，且它是一個穩定排序演算法。雖然效率較高，但需要額外的空間來存儲中間結果。

```cpp
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);
    int i = 0, j = 0, k = left;
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < leftArr.size()) arr[k++] = leftArr[i++];
    while (j < rightArr.size()) arr[k++] = rightArr[j++];
}
```

### 堆疊排序 (Heap Sort)
堆積排序基於堆積資料結構，它首先將資料轉換成一個最大堆，然後依次將堆頂元素（即最大元素）放到已排序的資料集中，再調整剩下的資料形成新的堆。堆積排序的時間複雜度為 𝑂(𝑛log𝑛)，但由於需要維護堆的結構，實際上其效能通常不如快速排序。

```cpp
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
```

---

## 效能分析

### 時間複雜度與空間複雜度比較

| 演算法 | 最佳情況 | 平均情況 | 最壞情況 | 空間複雜度 |
|---------|---------|---------|---------|---------|
| 插入排序 | O(n) | O(n^2) | O(n^2) | O(1) |
| 選擇排序 | O(n^2) | O(n^2) | O(n^2) | O(1) |
| 快速排序 | O(n log n) | O(n log n) | O(n^2) | O(logn) |
| 合併排序 | O(n log n) | O(n log n) | O(n log n) | O(n) |
| 堆疊排序 | O(n log n) | O(n log n) | O(n log n) | O(1) |


---

## 測試與結果

測試使用不同大小的隨機整數陣列（500 至 50000 元素），使用 `chrono` 計時工具測量每種排序演算法執行時間。

```cpp
void testSorting(vector<int>& arr, void (*sortFunc)(vector<int>&), string sortName) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << sortName << " took " << duration.count() << " ms." << endl;
}
```

## 平均時間

| 資料規模(n) | 插入排序 | 快速排序 | 合併排序 | 堆積排序 |
|----|----|----|----|----|
|500 | 1540 ms | 9 ms     | 555 ms    | 72 ms    |
|1000 | 2414 ms | 17 ms    | 1256 ms    | 58 ms    |
|2000 | 6359 ms | 28 ms    | 1986 ms    | 204 ms    |
|4000 | 18032 ms | 101 ms    | 2876 ms   | 305 ms   |
|5000 | 28391 ms | 78 ms    | 3765 ms   | 442 ms   |

## 最差方案

| 資料規模(n) | 插入排序 | 快速排序 | 合併排序 | 堆積排序 |
|----|----|----|----|----|
|500 | 6160 ms | 37 ms     | 2221 ms    | 288 ms    |
|1000 | 9657 ms | 68 ms    | 5026 ms    | 235 ms    |
|2000 | 25438 ms | 112 ms    | 7944 ms    | 819 ms    |
|4000 | 72131 ms | 405 ms    | 11470 ms   | 1223 ms   |
|5000 | 113566 ms | 314 ms    | 16062 ms   | 1770 ms   |

## 執行結果
n=500
![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/n_500.jpg)

n=1000
![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/n_1000.jpg)

n=2000
![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/n_2000.jpg)

n=4000
![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/n_4000.jpg)

n=5000
![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/n_5000.jpg)

## 排序演算法平均時間對比

![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/avg.jpg)

## 最壞情況執行時間對比

![image](https://github.com/PoHsun1209/NFU-DS-homework1/blob/main/report/image/worst.jpg)

---

## 結論

經過測試和效能分析，快速排序在大部分情況下都表現出最佳的效能，尤其是對於大資料集，插入排序在資料集較小時表現較好，但對於大型資料集並不適用。合併排序適用於穩定性要求較高的場合，而堆積排序則在某些情況下仍具有其應用價值。選擇哪種排序演算法應該根據資料集的大小、對效能的要求以及是否需要穩定性等因素進行綜合考量。

1. 快速排序的優勢
2. 插入排序的侷限性
3. 合併排序的穩定性
4. 堆積排序的空間優勢

---
