#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/14

/*

Purpose:

    製作一個程式，
    使其能產生西洋棋騎士(knight)走完棋盤的順序。

    -> 實作方式使用stack，而不使用recursion

*/


// 建立 Stack 裡的元素類別 -> Data
class Data{

    public:

        int i; // 紀錄所在位置的 列
        int j; // 紀錄所在位置的 行
        int d; // 紀錄移動至下一元素的方向
        Data* next = nullptr; // 記錄下一個元素的記憶體位置 (Stack 使用)

        // 建構式 -> 依照參數放入相對應的變數中
        Data(int row, int col, int dir){

            i = row;
            j = col;
            d = dir;

        }

};

class Stack{

    private:

        // 建立頭指標 -> 指向最頂端的元素 (預設為空指標)
        Data* head = nullptr;

        // 紀錄 Stack 內元素數量
        int sizeNum = 0;

    public:

        // CreateData 會建立一個全新的 Data 元素並回傳指標
        Data* CreateData(int i, int j, int d, Data* next){

            Data* newData = new Data(i, j, d);  // 在記憶體中開啟一個新元素
            newData->next = next;               // 將新 Data 的下一個元素位置設定為 next

            return newData;                     // 回傳目前位置

        }

        // top() 負責回傳 Stack 中最頂端的元素
        Data* top(){

            // 若頭指標未指向任何元素，則 Stack 為空，回傳空指標
            if(head == nullptr){

                return nullptr;

            }
            else{

                return head; // 回傳第一個元素

            }

        }

        // push() 推入新元素
        void push(int i, int j, int d){
            
            // 使用 CreateData 建立新元素
            // 新元素要指向目前的的最頂端元素
            Data* newData = CreateData(i, j, d, head);

            // 頭指標指向新的元素 -> 取代原本最頂端的元素
            head = newData;

            sizeNum++; // 元素數量 +1

        }

        // pop() 刪除第一個元素
        void pop(){

            // 確定 Stack 不為空再行動
            if(head != nullptr){

                // 取出目前最頂端元素
                Data* old = top(); 

                // 頭指標要指向最頂端元素的下一個元素 (即第二個元素);
                head = old->next;

                // 刪除舊元素
                delete old;

                sizeNum--; // 元素數量 -1

            }

        }

        
        int size() { return sizeNum; }           // size()  回傳目前元素數量
        bool empty() { return sizeNum == 0; }    // empty() 回傳 Stack 是否為空

};

bool dfs(int initRow, int initCol, vector<vector<int>>& array, const int N){

    // 以 Stack 來實現 DFS 演算法 (Back Tracking)
    // 讀取起始位置、array、N (版面大小)

    // 事先建立好方向向量
    const int dir[8][2] = {{-2,  1},
                           {-1,  2},
                           {1 ,  2},
                           {2 ,  1},
                           {2 , -1},
                           {1 , -2},
                           {-1, -2},
                           {-2, -1}};

    // 建立 Stack 物件
    Stack stk;

    // 先推入第一個位置並設置 array (起點位置)
    stk.push(initRow, initCol, -1);
    array[initRow][initCol] = stk.size();

    while(!stk.empty() && stk.size() != N * N){

        // 讀取目前 Stack 最頂端資料
        Data* curData = stk.top();

        int curRow = curData->i;     // 目前元素的   列 位置
        int curCol = curData->j;     // 目前元素的   行 位置
        int curDir = curData->d + 1; // 目前元素的 方向 位置 (檢查下一個方向)

        if(curDir >= 8){

            stk.pop();
            array[curRow][curCol] = 0;
            continue;

        }

        // 檢查下一步是否可以走
        int nextRow = curRow + dir[curDir][0]; // 下一步的 列 位置
        int nextCol = curCol + dir[curDir][1]; // 下一步的 行 位置

        // 若下一步的位置的位置為空且在範圍內則往下走
        if((nextRow >= 0 && nextRow < N) && (nextCol >= 0 && nextCol < N) && !array[nextRow][nextCol]){

            curData->d = curDir; // 更新目前元素的方向
            stk.push(nextRow, nextCol, -1);
            array[nextRow][nextCol] = stk.size();

            // 若已全部填寫完畢則可退出迴圈
            if(stk.size() == N * N){

                break;

            }


        }
        else{

            // 否則則推入下一個方向 (位置與 curData 相同)
            stk.pop();
            stk.push(curRow, curCol, curDir);

        }

    }

    return stk.size() == N * N;

}

int main(){

    // 開啟輸出檔
    ofstream out("hw3-B123040049.out");

    // 依照題目需求輸出 N = 1 ~ 6 的情形
    for(int N = 1 ; N <= 6 ; N++){

        // 建立一個 N * N 的陣列，並設定初始值為 0;
        vector<vector<int>> array(N, vector<int>(N, 0));

        // 若 dfs() 有結果 (true) 則輸出陣列，否則輸出 "no solution"
        if(dfs(0, 0, array, N)){

            for(int i = 0 ; i < N ; i++){

                for(int j = 0 ; j < N ; j++){

                    out << setw(3) << array[i][j];

                }  

                out << "\n";

            }

        }
        else{

            out << "no solution\n";

        }

        out << "\n";

    }
    
    // 關閉檔案
    out.close();

    return 0;
}