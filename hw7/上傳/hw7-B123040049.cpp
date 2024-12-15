#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <stdlib.h>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/29

/*

Purpose:

    測試五種排序 Sorting 演算法的執行效率

    1. A 類排序法
        -> 插入排序法 Insertion Sort
    2. B 類排序法
        -> 合併排序法 Merge Sort
    3. C 類排序法
        -> 快速排序法 Quick Sort
    4. D 類排序法
        -> <stdlib.h> qsort()
    5. E 類排序法
        -> <algorithm> sort()

    給予不同資料量 並 測量其執行時間

*/

// 將 chrono 的 High Resolution Clock 設為 Clock
using Clock = chrono::high_resolution_clock;

// 建立類別對照排序法名稱的表
const string sortingName[5] = {"Insertion Sort", "Merge Sort", "Quick Sort", "Qsort (stdlib.h)", "sort (algorithm)"};
const string inputfile = "input.txt";
const string testTimefile = "Time_test.txt";

// swap(a, b) 將 a, b 交換
void swap(int &a, int &b){

    int t = a; // t 用來協助交換
    a = b;
    b = t;

}

// cmp(a, b) 回傳 a, b 大小比較 -> 給 qsort() 判斷用 (由小排到大)
int cmp(const void* a, const void* b){

    // 先將 a, b 從 void* 轉為 int*
    // 再將 a, b 取值出來
    // 最後回傳結果 (正 : a > b, 負 : a < b, 零 : a == b)

    return *(int*)a - *(int*)b;

}

// A 類排序法 (複雜度 : O(n^2))
void InsertionSort(int array[], const int N){

    // 讀取陣列大小
    int size = N;

    // 依序將數字插入到前面以排序過的位置
    for(int i = 1 ; i < size ; i++){

        // 先將要插入的數字暫存起來
        int temp = array[i], j;

        // 將大於 temp 的數先往後推
        for(j = i - 1 ; j >= 0 && array[j] > temp ; j--){

            array[j + 1] = array[j];

        }

        // 將 temp 插入到空位中
        array[j + 1] = temp;

    }

}

// merge_inplace() 將陣列兩邊合併在一起
void merge_inplace(int array[], vector<int>& temp, const int l_bound, const int r_bound){

    // temp 暫存合併後的結果

    int mid = (l_bound + r_bound) / 2;           // 取得中間位置
    int idx1 = l_bound;                          // 左邊陣列指標
    int idx2 = mid + 1;                          // 右邊陣列指標
    
    for(int k = l_bound ; k <= r_bound ; k++){

        // 若左鎮列還沒用完 且 (右陣列已經用完 或 左邊目前數字 < 右邊目前數字) -> 位置 k 選擇左邊目前數字
        if( idx1 <= mid && (idx2 > r_bound || array[idx1] < array[idx2])){

            temp[k - l_bound] = array[idx1++];

        }
        else{

            temp[k - l_bound] = array[idx2++];

        }

    }

    // 將目標範圍從 temp 複製到 array
    for(int i = l_bound ; i <= r_bound ; i++){

        array[i] = temp[i - l_bound];

    }
    
}

// B 類排序法 (複雜度 : O(nlogn)) -> 分治法
void MergeSort(int array[], vector<int>& temp, const int l_bound, const int r_bound){

    // 若 r <= l -> 不需要排序
    if(r_bound <= l_bound)
        return;

    // 取得中間位置
    int mid = (l_bound + r_bound) / 2;

    // 兩邊各自做 MergeSort
    MergeSort(array, temp, l_bound, mid);
    MergeSort(array, temp, mid + 1, r_bound);

    // 將兩邊 Sort 完的陣列合併
    merge_inplace(array, temp, l_bound, r_bound);

}

// partition() 會依照 pivot 的大小將 array 分成兩邊，並回傳分組的分割點
int partition(int array[], const int l_bound, const int r_bound){

    int pivot = array[r_bound];  // 設定 pivot 為最右邊的值
    int idx = l_bound;           // idx 為左邊陣列的指標

    // 跑過陣列並分組 -> 將 array[j] <= pivot 與 array[i] 交換 (換到 <= pivot 的那組)
    for(int j = l_bound ; j < r_bound ; j++){

        if(array[j] <= pivot){

            swap(array[idx], array[j]);
            idx++;

        }

    }

    // 將 pivot 換到中間
    swap(array[idx], array[r_bound]);
    
    return idx;

}

// C 類排序法 (複雜度 : 1.最好 O(nlogn) 2. 最差 O(n^2))
void QuickSort(int array[], const int l_bound, const int r_bound){

    // 如果 r_bound <= l_bound -> 不須排列
    if(r_bound <= l_bound)
        return;

    // 分組，並取得分割點
    int part = partition(array, l_bound, r_bound);
    
    // 分兩邊各自做 QuickSort
    QuickSort(array, l_bound, part - 1);
    QuickSort(array, part + 1, r_bound);

}

// Sort 依照類別將 array 排序
void Sort(vector<int>& array, const char category){

    vector<int> temp;   // 給 Merge Sort 使用的 temp Array (不能宣告在 switch 中)
                        // 避免在 Merge inplace 階段才重複宣告
                        // 提前宣告後再傳入到函式中效率比較好

                        // A, B, C排序法均傳入 array.data()
                        // 傳 vector 的參考會影響效能
                        // 直接傳指標會更有效率

    switch(category){

        case 'A':
            InsertionSort(array.data(), array.size());
            break;

        case 'B':
            temp.resize(array.size());  // 重新調整成與 array 相同大小
            MergeSort(array.data(), temp, 0, array.size()-1);
            break;

        case 'C':
            QuickSort(array.data(), 0, array.size()-1);
            break;
        
        case 'D':
            qsort(&array[0], array.size(), sizeof(int), cmp);
            break;

        case 'E':
            sort(array.begin(), array.end());
            break;

    }

}

// outputTime 依照 testCaseSize, table 輸出到 terminal 中 (依照 backspace 刪除先前的輸出)
int outputTime(const vector<int>& testCaseSize, const vector<vector<long long>>& table, const int backline){

    const int space = 20; // 設定一格的大小

    // 刪除表格部分
    for (int i = 0; i < backline; i++) {

        // 使用 ANSI escape code 移動鼠標到上一行，並清除該行
        cout << "\033[A";

    }

    string buffer;                                  // 用來暫存輸出結果
    buffer += string(space / 2, ' ');        // 最左上角為空格

    for(int i = 0 ; i < 5 ; i++){

        string name = sortingName[i];

        // 不滿 20 個字元 -> 補空格在左邊
        while(name.size() < 20)
            name = " " + name;

        buffer += name;

    }

    buffer += "\n";
    
    for(int idx = 0 ; idx < testCaseSize.size() ; idx++){

        string sizeText = to_string(testCaseSize[idx]);          // 資料量
        while(sizeText.size() < space / 2) sizeText += " ";      // 補滿到 space 的一半
        buffer += sizeText;

        for(int c = 0 ; c < 5 ; c++){

            string time;    // 執行時間

            // 若此排序法尚未測量此資料量的執行時間 -> 輸出 '#'
            if(table[c].size() <= idx){

                time = "#";

            }
            else{
                
                // 如果測量時間為 -1 -> TLE!
                if(table[c][idx] == -1)
                    time = "TLE!";
                else
                    time = to_string(table[c][idx] * 1e-6);

            }

            // 補滿 space 個字元
            while(time.size() < space)
                time = " " + time;

            // 輸出到 buffer
            buffer += time;

        }

        buffer += '\n';

    }

    cout << buffer;

    return testCaseSize.size() + 1; // 回傳本次輸出了幾行
}

// time_test 執行排序法並測量時間
void time_test(){
    
    int lastLine = 0;                                             // 最後一筆輸出的字數     
    vector<int> testCaseSize;                                     // 儲存資料量大小
    vector<vector<long long>> table(5, vector<long long>());      // 儲存每一種演算法的執行時間

    for(char category = 'A' ; category <= 'E' ; category++){

        // 開啟測資檔
        ifstream input(testTimefile);

        // 檢查檔案是否存在
        if(!input.good()){

            cout << testTimefile << " doesn\'t exist!\n";
            return;

        }

        int testSize; // 資料量大小
        int dif;      // 測資量

        vector<int> array;              // 建立相對應資料量的陣列
        array.reserve(500000);          // 在記憶體中預留位置 
        vector<long long> timeArray;    // 紀錄每一組的執行時間

        while(input >> testSize >> dif){

            // 如果資料量大小還沒有到 8 組 -> 再推入
            if(testCaseSize.size() < 8)
                testCaseSize.push_back(testSize);

            array.resize(testSize);     // 陣列依照資料重新調整大小
            timeArray.resize(dif);      // 陣列依照測資數量重新調整大小

            // 一共有 dif 個測資
            for(int testCase = 1 ; testCase <= dif ; testCase++){

                // 輸出目前測資編號 (方便 debug 用)
                cout << "[" << testSize << "]" << " Test Case : " << testCase << " -> Reading ";   

                // 依照資料量大小讀取數據
                for(int i = 0 ; i < testSize ; i++){

                    input >> array[i];

                }

                cout << string(8, '\b') << "Sorting   \r";

                auto start = Clock::now(); // 紀錄開始時間

                // 將 array 排序
                Sort(array, category);

                auto stop = Clock::now(); // 紀錄結束時間
                long long duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();   // 計算運行時間
                timeArray[testCase - 1] = duration;

            }

            long long avg = 0; // avg 為執行時間平均值
            for(const int &t : timeArray)
                avg += t;
            avg /= timeArray.size();

            table[category - 'A'].push_back(avg);               // 將時間記錄到 table 中

            lastLine = outputTime(testCaseSize, table, lastLine); // 輸出表格

        }

        // 關閉檔案
        input.close();

    }

}

// solve 單傳解決排序的題目並產生輸出檔
void solve(const char category){

    string catString(1, category);                       // 將字元轉換為字串
    string filepath = "output" + catString + ".txt";     // 輸出檔名稱
    ofstream output(filepath);                           // 開啟輸出檔
    ifstream input(inputfile);                         // 開啟測資檔

    int testSize;       // 資料量大小
    input >> testSize;    // 輸入測資量大小

    // 輸出演算法名稱 (減去 'A' 即可得到索引值)
    output << sortingName[(const int)(category - 'A')] << ":\n\n";

    vector<int> array(testSize);    // 建立相對應資料量的陣列

    // 依照資料量大小讀取數據
    for(int i = 0 ; i < testSize ; i++){

        input >> array[i];

    }

    // 將 array 排序
    Sort(array, category);

    // 將所有值印出來
    for(const int &i : array){

        output << i << "\n";

    }

    output << endl;

    // 排序法完成
    cout << setw(20) << sortingName[(const int)(category - 'A')] << " complete!\n";

    // 關閉檔案
    output.close();
    input.close();

}


int main(){
    
    ios_base::sync_with_stdio(false);   // IO 加速

    int method; // 紀錄要使用甚麼方法

    // 選取要使用的方法
    cout << "1. Sort input.txt\n";
    cout << "2. Test time\n";
    cout << "Choose an operation: ";
    cin >> method;

    cout << endl;

    switch(method){

        case 1:
            solve('A');
            solve('B');
            solve('C');
            solve('D');
            solve('E');
            break;
        
        case 2:
            time_test();
            break;
        
        default:
            break;

    }

    return 0;
}