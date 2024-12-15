#include <iostream>
#include <fstream>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/13

/*

Purpose:

    以C++製作一個「集合」類別 (set class)，
    使其能進行「聯集」、「交集」、「差集」、「包含」、「屬於」等五種運算。

    1. A 與 B 聯集 (union)          記做 A +  B
    2. A 與 B 交集 (intersection)   記做 A *  B
    3. A 與 B 差集 (difference)     記做 A -  B
    4. A 包含 (contain) B           記做 A >= B 
    5. x 屬於 (belong to) A         記做 x in A

    依照題目需求輸出相對應答案:

    1.  A   
    2.  B   
    3.  A + B 
    4.  A * B 
    5.  A – B 
    6.  B – A 
    7.  if A contains B
    8.  if B contains A
    9.  if X is in A
    10. if X is in B

*/

// 建立「集合」類別 -> TSet
class TSet{

    public:

        bool array[256]; // array 紀錄每一種字元是否出現

        // reset 函式會把所有字元的狀態設為 "未出現"
        void reset(){

            for(int i = 0 ; i < 256 ; i++){

                array[i] = false;

            }

        }

        // 預設建構式
        TSet(){
            
            reset(); // 重置 array

        }

        // 建構式 (透過字串)
        TSet(const string& s){

            reset(); // 重置 array

            // 將所有 s 裡含有的的字元標視為 "出現"
            for(const char &c : s){

                array[c] = true;

            }

        }

        // operator + Overloading -> 聯集 (union)
        TSet operator + (const TSet& T){

            TSet res; // 建立負責回傳用的空集合

            // 迭代所有的自己與 T 的陣列
            // 將自己 "或" T 有出現過的字元標示入 res 中
            for(int i = 0 ; i < 256 ; i++){

                if(array[i] || T.array[i]){

                    res.array[i] = true;

                }

            }

            return res;

        }

        // operator * Overloading -> 交集 (intersection)
        TSet operator * (const TSet& T){

            TSet res; // 建立負責回傳用的空集合

            // 迭代所有的自己與 T 的陣列
            // 將自己有出現 "且" T 也有出現過的字元標示入 res 中
            for(int i = 0 ; i < 256 ; i++){

                if(array[i] && T.array[i]){

                    res.array[i] = true;

                }

            }

            return res;

        }

        // operator - Overloading -> 差集 (difference)
        TSet operator - (const TSet& T){

            TSet res = *this; // 建立負責回傳用的 A 集合複本

            // 迭代自己的陣列
            // 將自己有出現 "且" T 也有出現過的字元從 res 中刪除
            for(int i = 0 ; i < 256 ; i++){

                if(array[i]){
                    
                    if(T.array[i]){

                        res.array[i] = false;

                    }

                }

            }

            return res;

        }

        // operator >= Overloading -> 包含 (contain)
        bool operator >= (const TSet& T){

            bool res = true; // 建立負責紀錄是否包含的結果的布林值

            // 迭代 T 的陣列
            // 若 T 有字元出現但自己的紀錄沒有出現，則可回傳 false
            for(int i = 0 ; i < 256 ; i++){

                if(T.array[i]){
                    
                    if(!array[i]){

                        res = false;

                    }

                }

            }

            return res;

        }

        // operator in -> 屬於 (belong to)
        bool in(const char& c){

            // 若出現 array 則可直接回傳 true，否則回傳 false
            if(array[c]){

                return true;

            }
            else{

                return false;

            }

        }

        // operator << Overloading (只能透過 friend 來使用全域函式)
        friend ostream& operator << (ostream& out, const TSet& T){

            out << "{";

            // 將所有標示為 "出現" 的字元輸出出來 (從 ASCII 最小開始)
            for(int i = 0 ; i < 256 ; i++){

                if(T.array[i]){

                    out << (char)i;

                }

            }

            out << "}";

            return out;

        }

};

int main(){

    // 開啟輸出檔
    ofstream out("hw2-B123040049.out");

    int testCase; //測資數量
    cin >> testCase;
    cin.ignore(1); // (忽略測資數量 (int) 後的換行以便讀入後續測資)

    // 依照測資數量執行迴圈
    for(int test = 1 ; test <= testCase ; test++) {

        string stringA, stringB, stringX; // 測資 (字串 A、字串 B)
        char X;                           // 測資 (字元 X)
        cin >> stringA >> stringB >> X;

        TSet A = stringA;   // A 集合
        TSet B = stringB;   // B 集合
        TSet C = A + B;     // C 集合 (A + B) 
        TSet D = A * B;     // D 集合 (A * B)
        TSet E = A - B;     // E 集合 (A - B) 
        TSet F = B - A;     // F 集合 (B - A) 

        // 依照題目需求輸出相對應答案
        out << "Test Case " << test << ":\n";
        out << "  A  : " << A << "\n";
        out << "  B  : " << B << "\n";
        out << "A + B: " << C << "\n";
        out << "A * B: " << D << "\n";
        out << "A - B: " << E << "\n";
        out << "B - A: " << F << "\n";
        out << "A " << ((A >= B) ? "contains" : "does not contain") << " B\n";
        out << "B " << ((B >= A) ? "contains" : "does not contain") << " A\n";
        out << "\'" << X << "\' is" << (A.in(X) ? "" : " not") << " in A\n";
        out << "\'" << X << "\' is" << (B.in(X) ? "" : " not") << " in B\n";

        out << "\n";

    }
    
    // 關閉檔案
    out.close();

    return 0;
}