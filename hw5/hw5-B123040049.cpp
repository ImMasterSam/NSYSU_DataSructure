#include <iostream>
#include <fstream>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/19

/*

Purpose:

    設計一個程式，可以處理兩個多項式之加法與乘法

    -> 每個多項式需用一個 linked list 來儲存

*/


// 建立給 linked list 使用的 Node
class Node{

    public:
        
        int coe;        // 負責儲存 係數
        int exp;        // 負責儲存 指數
        Node* next;     // 負責儲存 下一格元素的記憶體位置

        //建構式 (係數、次方、下一個記憶體位置)
        Node(int c, int e, Node* nxt){

            coe = c;
            exp = e;
            next = nxt;

        }

};

// 依照題目需求建立 Poly (多項式) 類別
class Poly{

    private:

        Node* head = nullptr; // 頭指標指向第一個元素

    public:

        // createNode() 在記憶體中創建一個 Node 來使用並回傳位置
        Node* createNode(int coe, int exp, Node* next){

            Node* newNode = new Node(coe, exp, next); // 在記憶中建立新的 Node

            // 回傳指標
            return newNode;

        }

        // trim() 負責刪除係數為零的項
        void trim(){

            Node* cur = head;      // 設定目前位置為第一個 Node
            Node* prev = nullptr;  // 設定前一個 Node 為空 Node

            while(cur != nullptr){
                
                // 若 Node 的係數為 0 -> 刪除
                if(cur->coe == 0){

                    Node* temp = cur; // 先將目前位置儲存起來

                    cur = cur->next;  // 將 cur 更改至下一個位置

                    // 將前一個元素的下一個位置設為 cur
                    if(prev != nullptr){
                        
                        prev->next = cur;

                    }
                    else{

                        // 若 prev 為空指標 -> 沒有前一個 Node 直接更改 head
                        head = cur;

                    }
                    
                    delete temp; // 刪除掉目前元素

                }
                else{
                    
                    // 向下走
                    prev = cur;       // 設定上一個元素為 cur
                    cur = cur->next;  // 設定 cur 至下一個 Node

                }

            }

        }

        // insert() 將讀取到的 係數 與 次方 插入到多項式 (自動合併同次方項) 
        void insert(int coe, int exp){

            Node* cur, *prev; // cur 紀錄目前走到的 Node、 prev 紀錄上一個 Node (串接用)

            prev = nullptr; // 設定 prev 為空指標
            cur = head;     // 設定 cur  為第一個 Node
            
            // 若沒有走到空地址 或者 目前次方大於要插入的次方則往下走
            while(cur != nullptr && cur->exp > exp){

                prev = cur;
                cur = cur->next;

            }

            
            if(cur == nullptr){

                // 如果目前所在位置 與 前一位置 皆沒有東西 -> 多項式為空
                if(prev == nullptr){

                    Node* newNode = createNode(coe, exp, head); // 建立新 Node
                    head = newNode; // 頭指標指向新 Node

                }
                
                // 目前處在多項式的最尾巴 -> 可直接插入到最後面
                else{

                    Node* newNode = createNode(coe, exp, nullptr); // 建立新 Node (尾巴)
                    prev->next = newNode; // 將最後一個元素指向新元素

                }

            }
            else{

                // 若次方相同則可直接加入到原多項式
                if(cur->exp == exp){

                    cur->coe += coe;

                }
                else{

                    Node* newNode = createNode(coe, exp, cur); // 建立新 Node 

                    // 將前一個元素指向自己
                    if(prev != nullptr){

                        prev->next = newNode;

                    }
                    else{

                        // 若前一個 Node 為空 -> 目前位在第一格 (處理 head)
                        head = newNode;

                    }


                }

            }

            trim(); // 修剪掉係數為零的項

        }

        // operator + Overloading -> 將兩個多項式相加
        Poly operator + (const Poly& p){

            Poly res; // 建立回傳用的多項式

            // 要 Travel 過兩個 多項式
            
            Node* cur = this->head; // 設定目前位置為第一個多項式的頭

            while(cur != nullptr){

                res.insert(cur->coe, cur->exp);    // 插入到 res 多項式中
                cur = cur->next;                   // 更改 cur 至下一個元素

            }

            cur = p.head; // 設定目前位置為第二個多項式的頭

            while(cur != nullptr){

                res.insert(cur->coe, cur->exp);    // 插入到 res 多項式中
                cur = cur->next;                   // 更改 cur 至下一個元素

            }

            res.trim(); // 修剪掉係數為零的項
            return res;    

        }

        // operator + Overloading -> 將兩個多項式相加
        Poly operator * (const Poly& p){

            Poly res; // 建立回傳用的多項式

            // 要 Travel 過兩個 多項式 -> 所有配對 (雙層迴圈)
            
            Node* curPos_1 = this->head; // 設定目前位置為第一個多項式的頭

            while(curPos_1 != nullptr){

                Node* curPos_2 = p.head; // 設定目前位置為第二個多項式的頭

                while(curPos_2 != nullptr){

                    res.insert(curPos_1->coe * curPos_2->coe, curPos_1->exp + curPos_2->exp);    // 將相乘項插入到 res 多項式中
                    curPos_2 = curPos_2->next;    // 更改 cur 至下一個元素

                }
                
                curPos_1 = curPos_1->next;    // 更改 cur 至下一個元素

            }

            res.trim(); // 修剪掉係數為零的項
            return res;    

        }

        bool input(){
            
            int num;      // 紀錄項數
            int x, y;     // 用來讀取係數、次方

            cin >> num;

            if(num == 0) return false; // 零多項式

            // 讀取 num 項資料
            for(int i=0;i<num;i++){

                cin >> x >> y;

                insert(x, y);  // 插入 X * x^Y

            }

            return true;

        }

        // operator << Overloading (只能透過 friend 來使用全域函式)
        friend ostream& operator << (ostream& out, const Poly& p){
            
            // 若 P 為零多項式 -> 直接輸出 0
            if(p.empty()){

                out << "0" << "\n";
                return out;

            }

            Node* cur = p.head; // 設定目前位置式第一個 Node

            // Travel 過所有的元素
            while(cur != nullptr){

                out << cur->coe << " " << cur->exp << "\n"; // 輸出
                cur = cur->next;                            // 更改 cur 至下一個元素

            }

            return out;

        }

        bool empty() const { return head == nullptr; } // empty() 回傳多項式是否為 0

};

int main(){

    // 開啟輸出檔
    ofstream out("hw5-B123040049.out");

    int testCase = 0; // 紀錄測資量

    while(true){

        Poly P, Q; // 建立 P, Q 多項式

        bool input1 = P.input(); // 輸入 P
        bool input2 = Q.input(); // 輸入 Q

        if(!input1 && !input2) // 若 P, Q 皆讀到 0 -> 退出
            break;

        Poly add = P + Q;   // 相加後的多項式
        Poly mult = P * Q;  // 相乘後的多項式

        out << "Case " << ++testCase << ":\n"; // 輸出第幾筆資料

        out << "ADD\n" << add << "\n"; // 輸出 ADD
        out << "MULTIPLY\n" << mult << "\n"; // 輸出 ADD

        out << "\n";

    }
    
    // 關閉檔案
    out.close();

    return 0;
}

/* 測資

1
1 1
3
1 2
1 1
1 0
3
3 5
1 1
1 1
2
-2 1
1 2
2
5 1
-7 0
2
-5 1
7 0
0
0

*/