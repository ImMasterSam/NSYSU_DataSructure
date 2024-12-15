#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/23

/*

Purpose:

    給予一串數字，找出所有出現次數為奇數之數字

    -> 利用 Binary Search Tree 來解決此問題

    檢查每一個數字:

    1. 若找到相等者，表示之前已經出現過奇數次，則刪除該node
    2. 若未找到相等者，表示之前出現過偶數次（含零次），則將該數字插入

*/

class Node{

    public:

        int val;                // 存放數字 (資料)
        Node* left = nullptr;   // 右子節點
        Node* right = nullptr;  // 左子節點

        // 建構式 (資料)
        Node(int value): val(value) {} 

};

// 依照題目需求建立 二元搜尋樹 Binary Search Tree (BST)
class BST{

    private:

        Node* top = nullptr;

    public:

        // insert() 將 key 插入到二元搜尋樹
        void insert(int key){

            Node* curNode = top;         // 目前位置
            Node* prevNode = nullptr;    // 前一個位置

            // 一路往下找到空的位置
            while(curNode != nullptr){
                
                // 若 key < value -> 向左走
                if(key < curNode->val){

                    prevNode = curNode;       // prevNode 走上目前位置
                    curNode = curNode->left;  // curNode  往左邊走

                }
                // 若 key >= value -> 向右走
                else{

                    prevNode = curNode;       // prevNode 走上目前位置
                    curNode = curNode->right;  // curNode  往右邊走

                }

            }

            // 將 key 值放入 curNode
            Node* newNode = new Node(key); // 建立新結點

            // 將父節指向新結點
            // 1. 前一個節點為空 -> BST 為空，直接修改 top
            if(prevNode == nullptr){

                top = newNode;

            }
            // 2. 前一個節點 value <= key -> 將新結點插入到父節點的右邊
            else if(prevNode->val <= key){

                prevNode->right = newNode;

            }
            // 3. 前一個節點 value > key -> 將新結點插入到父節點的右邊
            else{

                prevNode->left = newNode;

            }

        }

        // erase() 將 key 從二元搜尋樹刪除
        void erase(int key){

            Node* curNode = top;        // 紀錄目前位置
            Node* prevNode = nullptr;   // 前一個位置

            while(curNode != nullptr){
                
                // 若 value == key -> 進行刪除動作
                if(curNode->val == key){

                    Node* parentchild = nullptr; // 用來暫存新子節點的位置

                    // 無子節點 -> 刪除即可
                    if(curNode->left == nullptr && curNode->right == nullptr){

                        parentchild = nullptr;
                        
                        // 刪除節點
                        delete curNode;

                    }
                    
                    // 若無左節點 -> 拿右節點遞補
                    else if(curNode->left == nullptr){

                        parentchild = curNode->right;  //設定新子節為右節點
                        
                        // 刪除節點
                        delete curNode;

                    }

                    // 若無右節點 -> 拿左節點遞補
                    else if(curNode->right == nullptr){

                        parentchild = curNode->left;  //設定新子節點為左節點
                        
                        // 刪除節點
                        delete curNode;

                    }
                    else{

                        /*
                        
                        如果兩邊都有子節點，則可以找

                        1. 左子樹最大值 
                        2. 右子樹最小值 <- 目前使用方法

                        */
                        int MINNode = findMin(curNode->right, curNode);
                        curNode->val = MINNode;

                        parentchild = curNode;  //設定新子節點

                    }

                    // 將父節點指向新子節點
                    if(prevNode == nullptr)
                        top = parentchild;
                    else if(key >= prevNode->val)
                        prevNode->right = parentchild;
                    else
                        prevNode->left = parentchild;

                    return;

                }
                // key < value -> 向左走
                else if(key < curNode->val){

                    prevNode = curNode;         // prevNode 走上目前位置
                    curNode = curNode->left;    // curNode 向左走

                }
                // key > value -> 向右走
                else{

                    prevNode = curNode;         // prevNode 走上目前位置
                    curNode = curNode->right;    // curNode 向左走

                }

            }

        }

        // find() 回傳 key 值是否存在在二元搜尋樹中
        bool find(int key){

            Node* curNode = top; // 目前位置

            // 一路向下找
            while(curNode != nullptr){

                // 找到相同的值 -> 回傳 true
                if(curNode->val == key){

                    return true;

                }

                // 若 key < val -> 向左走
                if(key < curNode->val){

                    curNode = curNode->left;

                }
                // 若 key > val -> 向右走
                else{
                    
                    curNode = curNode->right;

                }

            }

            // 找到底都找不到 -> 回傳 false
            return false;

        }

        // findMax() 由 cur 開始往下找到最大的節點 val -> 一路向右 
        int findMin(Node* curNode, Node* parent){
            
            // 若右邊還有節點則向右走
            if(curNode->left != nullptr){

                return findMin(curNode->left, curNode);

            }

            // 若無右節點 -> 已經找到最大值 
            else{

                // 將父節點的目標子節點刪除
                if(curNode->val >= parent->val)
                    parent->right = curNode->right;
                else
                    parent->left = curNode->right;

                int res = curNode->val;     // 紀錄要傳的值 (curNode 的 value)
                delete curNode;             // 刪除節點

                return res;

            }

        }

        // travel() 會遍歷過整棵樹 (使用 中序遍歷)
        void travel(vector<vector<int>>& data, Node* cur) const{

            // 往左邊走
            if(cur->left != nullptr){

                travel(data, cur->left);

            }

            // 推入節點值 (data[0])
            data[0].push_back(cur->val);

            // 推入左節點 (data[1])
            if(cur->left != nullptr)
                data[1].push_back(cur->left->val);
            else
                data[1].push_back(0);

            // 推入右節點 (data[2])
            if(cur->right != nullptr)
                data[2].push_back(cur->right->val);
            else
                data[2].push_back(0);

            // 往右邊走
            if(cur->right != nullptr){

                travel(data, cur->right);

            }

        }

        // operator << Overloading (只能透過 friend 來使用全域函式)
        friend ostream& operator << (ostream& out, const BST& T){
            
            // 若 T 的 top 是空指標 -> 整棵樹是空的
            if(T.top == nullptr){

                out << "Empty Tree!\n";
                return out;

            }
            
            // 開一個三列的二維陣列來儲存資料
            // data[0] : 節點值
            // data[1] : 左節點
            // data[2] : 右節點
            vector<vector<int>> data (3, vector<int>());

            T.travel(data, T.top); // 利用 travel 取得資料值

            // 輸出節點值
            out << "Node  :";
            for(int i=0;i<data[0].size();i++){

                out << setw(3) << data[0][i];

            }

            // 輸出左節點
            out << "\nLeft  :";
            for(int i=0;i<data[1].size();i++){

                out << setw(3) << data[1][i];

            }

            // 輸出左節點
            out << "\nRight :";
            for(int i=0;i<data[2].size();i++){

                out << setw(3) << data[2][i];

            }

            out << "\n";
            
            return out;
            
        }

};

int main(){

    // 開啟輸出檔
    ofstream out("hw6-B123040049.out");

    int n;  // 用來讀取測資
    
    // 讀取測資直到 EOF
    while(cin >> n){

        BST T;  // 建立二元搜尋樹 BST

        do{
            
            // 如果 n 已經出現過 -> 刪除節點
            if(T.find(n)){

                T.erase(n);

            }

            // 若沒有出現過 -> 插入新結點
            else{

                T.insert(n);

            }

            cin >> n;

        }while(n != -1); // n == -1 時退出迴圈

        // 輸出 BST
        out << T << endl;

    }

    // 關閉檔案
    out.close();

    return 0;
}

/* 測資:

10 7 12 8 11 4 -1
3 3 5 9 5 3 4 8 5 3 10 10 8 -1
2 1 3 2 -1

*/