#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/16

/*

Purpose:

    製作一個程式，
    使其能產生西洋棋騎士(knight)走完棋盤的順序。

    -> 實作方式使用 recursion，而不使用 stack

*/


bool dfs(int row, int col, int depth, vector<vector<int>>& array, const int N){

    // 利用 遞迴 來實現 DFS 演算法 (Back Tracking)
    // 讀取位置、遞迴深度、array、N (版面大小)

    // 事先建立好方向向量
    const int dir[8][2] = {{-2,  1},
                           {-1,  2},
                           {1 ,  2},
                           {2 ,  1},
                           {2 , -1},
                           {1 , -2},
                           {-1, -2},
                           {-2, -1}};

    // 走上去後將 array 中的位置設定好
    array[row][col] = depth;


    // 若遞迴深度已經達到 N * N，則已完成題目要求，可回傳 "已完成" (true)
    if(depth == N * N){

        return true;

    }

    else{

        

        // 看過所有 8 個方向，如果可以則往下走
        for(int d = 0 ; d < 8 ; d++){

            int nextRow = row + dir[d][0]; // 下一步的 列 位置
            int nextCol = col + dir[d][1]; // 下一步的 行 位置

            // 若下一步的位置的位置為空且在範圍內則往下走
            if((nextRow >= 0 && nextRow < N) && (nextCol >= 0 && nextCol < N) && !array[nextRow][nextCol]){
                
                // 如果遞迴結果已經回傳 true
                // 則可以回傳 "已完成" (true) 給上一步
                if(dfs(nextRow, nextCol, depth + 1, array, N)){

                    return true;

                }

            }

        }

        array[row][col] = 0;

    }

    return false;

}

int main(){

    // 開啟輸出檔
    ofstream out("hw4-B123040049.out");

    // 依照題目需求輸出 N = 1 ~ 6 的情形
    for(int N = 1 ; N <= 6 ; N++){

        // 建立一個 N * N 的陣列，並設定初始值為 0;
        vector<vector<int>> array(N, vector<int>(N, 0));

        // 若 dfs() 有結果 (true) 則輸出陣列，否則輸出 "no solution"
        if(dfs(0, 0, 1, array, N)){

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