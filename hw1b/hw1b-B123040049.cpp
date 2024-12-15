#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/12

/*

Purpose:

    對於每一個 N，
    印出 1 到 N 之間所有整數之階乘(共有N列)，
    每列印出一個階乘 (可進行大數運算)。

*/

struct BigN: vector<int>{

    // 建立一個大數資料型態
    // 並繼承 vector<int> 得屬性進行改造

    const int base = 10; // base 會確定每隔元素的最大值

    BigN(){

        push_back(1); // 預設值為 1;

    }

    // carry() 函數是負責進位的函式
    void carry(){

        for(int i = 0 ; i < size() ; i++){

            if(at(i) >= base){

                int r = at(i) % base; // 除以 base 的餘數

                // 如果已經是最後一格必須 push_back() 新格子
                // 否則直接把溢位的數字加至下一格
                if(i == size() - 1){

                    push_back(at(i) / base);

                }
                else{

                    at(i+1) += at(i) / base;

                }

                // 最後第 i 格只保留餘數
                at(i) = r;

            }

        }

    }

    // 建立 *= 號 Operator Overloading
    void operator *= (const int mult){

        // 把每格元素乘上乘數
        for(int i = 0 ; i < size() ; i++){

            at(i) *= mult;

        }

        carry();

    }

    // operator << 只能透過 friend 來使用全域函式
    friend ostream& operator << (ostream& out , const BigN& num){

        // 將 num 逆著格子輸出至 .out 檔中

        for(int i = num.size() - 1 ; i >= 0 ; i--){

            out << num[i];

        }

        return out;

    }

};

int main(){

    // 開啟輸出檔
    ofstream out("hw1b-B123040049.out");

    int N; // 測資

    // 重複讀入測資 N ，直到 N = 0;
    while(cin >> N, N){

        BigN num;

        // 產生 1! ~ N! ，並輸出至 .out 檔
        for(int i=1;i<=N;i++){

            num *= i;
            out << i << "! = ";
            out << num << "\n";

        }

        out << "\n";

    }

    // 關閉檔案
    out.close();
    
    return 0;
}