#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/12

/*

Purpose:

    製作 N 階 (N為奇數) 的魔方陣(Magic Square)

    魔方陣：方陣中的各個橫列(row)，直行(column)，對角斜線(diagonal)上之數字，加起來的和均相等

*/

void MagicSquare(const int N, ofstream& out){

    // 此函數會依據 N 的大小來產生相對應階數的魔方陣
    // 並輸出至 .out 檔

    int curRow = 0, curCol = (N / 2);                 // 目前所在的位置 (起始為第一列的正中間)
    int cnt = 0;                                      // 紀錄放入幾個數字
    vector<vector<int>> array(N, vector<int>(N, -1)); // 方陣(起始值為 1)

    while(cnt < N * N){

        // 將數值放入陣列中 (依照 cnt 大小)
        array[curRow][curCol] = ++cnt;

        // 下一個位置 (取 MOD 能確保位置在0 ~ N-1內循環)
        // +N 可以解決負數取 MOD 的問題(轉正後再取 MOD)
        int nxtRow = (curRow - 1 + N) % N;
        int nxtCol = (curCol - 1 + N) % N;

        // 若下一個位置已經占用，改至原位置的下面一格
        if(array[nxtRow][nxtCol] != -1){

            nxtRow = (curRow + 1) % N;
            nxtCol = curCol;

        }

        // 更新位置
        curRow = nxtRow;
        curCol = nxtCol;

    }

    // 輸出至 .out 檔
    for(int i = 0 ; i < N ; i++){

        for(int j = 0 ; j < N ; j++){

            out << setw(3) << array[i][j]; // setw() 每個數字設定寬度為 3

        }

        out << "\n";

    }

    out << "\n";

}

int main(){

    // 開啟輸出檔
    ofstream out("hw1a-B123040049.out"); 

    // 依據題目需求產生 1, 3, 5, 7, 9 階的魔方陣
    for(int N = 1 ; N <= 9 ; N += 2){

        MagicSquare(N, out); // 製作 N 階的魔方陣

    }

    // 關閉檔案
    out.close();

    return 0;
}