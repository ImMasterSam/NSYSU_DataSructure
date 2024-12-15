#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <stack>
#include <map>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/23

/*

Purpose:

    以Huffman 演算法製作一個檔案壓縮與解壓縮之軟體
    -> Huffman演算法為無失真壓縮方法


    操作範例:
        huffman –c –i infile –o outfile
            (進行壓縮，infile為輸入檔案，outfile為輸出檔案)
        huffman –u –i infile –o outfile  
            (進行解壓縮，infile為輸入檔案，outfile為輸出檔案)

*/



// getData() 依照檔案名稱讀取二進制資料到 inputData
// Return Value: 
//      0 : Read correctly
//      1 : Error
int getData(const char* filename, vector<uint8_t>& inputData){

    ifstream inputfile(filename, ios::binary);

    // 利用 istreambuf_iterator 一次將整個檔案輸入到 vector 中
    // 變數型態是 char -> 一次 8 個位元
    inputData.assign(istreambuf_iterator(inputfile), istreambuf_iterator<char>());

    // 關閉檔案
    inputfile.close();

    return inputData.size() > 0 ? 0 : 1;

}


// Huffman namespace 負責將檔案加密解密處理
namespace Huffman{

    // Huffman, Heap 需要用到的節點
    struct Node{

        uint8_t val;    // 紀錄 8 bits 的值
        int freq;       // 出現次數
        Node* left;     // 左子節點
        Node* right;    // 右子節點
        
        // 建構式
        Node() {}
        Node(const uint8_t& value, const int& frequency, Node* l = nullptr, Node* r = nullptr){

            val = value;
            freq = frequency;
            left = l;
            right = r;

        }

        // 比較運算符 < Overloading
        const bool operator < (const Node& b) const {

            if(freq != b.freq)
                return freq < b.freq;
            else
                return val < b.val;

        }

    };

    // Min Heap -> 快速讀取最小值
    class minHeap{

        private:

            int num = 0;                // 紀錄 Heap 的大小
            vector<Node* > heapArray;   // 儲存整個 Heap 的陣列

        public:

            // 建構式
            minHeap(){
                
                // 先在記憶體中預留位置，避免動態分配會花費時間
                heapArray.reserve(300);
                
                // 先在 heapArray 中推入一個 Dummy (用不到的節點)
                // 因為 heapArray 會從 1 開始，而不是 0
                heapArray.push_back(new Node);

            }
            
            // 將節點向上送到適當位置
            void shift_up(int curPos){

                int parentPos;                  // 紀錄父節點位置

                // 當上面還有父節點就向上走
                while(curPos > 1){

                    parentPos = curPos >> 1;    // 父節點 = 目前位置 / 2

                    // 檢查 parentPos 是否 < curPos -> 退出
                    if(*heapArray[parentPos] < *heapArray[curPos]){
                        
                        break;

                    }

                    swap(heapArray[curPos], heapArray[parentPos]);  // 與父節點交換
                    curPos = parentPos;                             // 往上走到目前位置

                }

            }

            // 將節點向下送到適當位置
            void shift_down(int curPos){

                int left, right, nxt;           // 紀錄左右子節點、下一個節點

                while(curPos <= num){

                    left = (curPos << 1);       // 左子節點 = 目前位置 * 2 
                    right = (curPos << 1) + 1;  // 右子節點 = 目前位置 * 1

                    if(left > num) break;       // 若左節點已超出範圍 -> 退出

                    nxt = left;

                    // 檢查 right 是否比 left 小
                    if(right <= num && *heapArray[right] < *heapArray[left]){

                        nxt = right;

                    } 

                    // 檢查 nxt 是否比 curPos 大 -> 退出
                    if(*heapArray[curPos] < *heapArray[nxt]) break;

                    swap(heapArray[curPos], heapArray[nxt]);
                    curPos = nxt;

                }

            }

            // push() 節點推入到 Heap 中
            void push(Node* node){

                num++;                          // 大小加一

                heapArray.push_back(node);      // 將節點推入到 Heap 的最底端
                shift_up(num);                  // 將節點向上推到適當的位置

            }

            // pop() 排除最頂端節點並回傳節點指標
            Node* pop(){

                Node* res = heapArray[1];       // 紀錄要回傳的節點
                heapArray[1] = heapArray[num];  // 將最後一個節點傳到最上方 (後面會再 Shift Down)

                num--;                          // 大小減一
                heapArray.pop_back();           // 將最後一個節點刪除

                shift_down(1);                  // 將被傳到最上方的節點向下送

                return res;

            }
            
            const int size() const { return heapArray.size() - 1; } // size() 回傳 Heap 大小

    };

    // Huffman Tree 
    class Tree{

        private:

            minHeap heap;           // 建立 Huffman Tree 需要的 minHeap
            Node* top = nullptr;    // 指向 Huffman Tree 的最頂端元素

        public:

            // 將資料處理成出現頻率後回傳 pair 陣列
            // Format : { 8 bits data, Frequency }
            vector<pair<uint8_t, int>> handleData(vector<uint8_t>& inputData){

                vector<int> freq(256, 0);           // 紀錄資料的出現次數
                vector<pair<uint8_t, int>> res;     // 回傳用的陣列

                // 計算出現頻率
                for(const uint8_t& data : inputData){

                    freq[data]++;

                }

                // 檢查每個資料是否出現過
                for(int i=0;i<256;i++){
                    
                    if(freq[i]){
                        
                        // 若出現過則放入到 res 中
                        res.push_back({i, freq[i]});

                    }

                }

                return res;

            }

            // 建立 Huffman Tree (從原始檔案)
            void build(vector<uint8_t>& inputData){

                // 將 inputData 處理成出現頻率
                vector<pair<uint8_t, int>> freq = handleData(inputData);

                // 將頻率資料製作成 Node* 推入到 minHeap 中
                for(const pair<uint8_t, int>& f : freq){

                    heap.push(new Node(f.first, f.second));

                }

                // Huffman algorithm
                //  將 Heap 中最小的兩個節點合併至新的父節點
                while(heap.size() > 1){

                    Node* left = heap.pop();
                    Node* right = heap.pop();

                    // 將字典續最小的放到左節點
                    if(left->val > right->val)
                        swap(left, right);

                    heap.push(new Node(left->val, (left->freq + right->freq), left, right));

                }

                // 整理後將 top 指向最後唯一的父節點 (Root)
                top = heap.pop();

            }

            // 建立 Huffman Tree (從後序遍歷)
            void buildFrom_postorder(vector<uint8_t>& postorder){
                
                stack<Node*> stk;                   // 復原後序遍歷的 Stack
                int idx = 0;                        // postorder 的索引

                while(idx < postorder.size()){
                    
                    // 若遇到 1 則將後面的資料推入史疊克中
                    if(postorder[idx] == 1){

                        idx++;
                        stk.push(new Node(postorder[idx], 0));

                    }
                    // 若遇到 0 -> 代表父節點，從 Stack 中取出 2 個子節點 
                    else if(postorder[idx] == 0){

                        Node* right = stk.top();
                        stk.pop();
                        Node* left = stk.top();
                        stk.pop();

                        stk.push(new Node(left->val, 0, left, right));

                    }

                    idx++;

                }
                
                top = stk.top(); // 將 top 設定為最後一個父節點 (Root)
                stk.pop();

            }

            // 將建立好的 Huffman Tree 依照編碼儲存至陣列中 (利用 DFS 演算法)
            void genCodeList(vector<string>& codeList, Node* curPos, string code){

                // 檢查 Huffman Tree 是否為空
                if(top == nullptr){

                    cout << "No Data!\n";
                    return;

                }

                //  若已經是葉節點 -> 輸出資料與編碼到 codeList
                if(curPos->left == nullptr && curPos->right == nullptr){

                    codeList[curPos->val] = code;
                    return;

                }
                
                // 向左走 -> code + "0"
                if(curPos->left){

                    genCodeList(codeList, curPos->left, code + "0");

                }

                // 向右走 -> code + "1"
                if(curPos->right){

                    genCodeList(codeList, curPos->right, code + "1");

                }

            }

            // 產生後序遍歷 -> 解壓縮時使用
            void genPostOrder(Node* curPos, vector<uint8_t>& postorder){

                if(curPos->left == nullptr && curPos->right == nullptr){

                    postorder.push_back(1);
                    postorder.push_back(curPos->val);

                }
                else{

                    if(curPos->left){

                        genPostOrder(curPos->left, postorder);

                    }
                    if(curPos->right){

                        genPostOrder(curPos->right, postorder);

                    }

                    postorder.push_back(0);

                }

            }

            Node* getTop() const { return top; }  // getTop() 回傳最頂端的元素

    };

    // genTable() 按照 Code List 產生 編碼對應資料 的 map
    map<string, uint8_t> genTable(vector<string>& codeList){

        map<string, uint8_t> bitsTable;

        for(int i=0;i<256;i++){

            if(codeList[i] != ""){

                bitsTable[codeList[i]] = i;

            }

        }

        return bitsTable;

    }

    // encode() 依照 Code List 將 inputData 壓縮至 encodeData
    void encode(const vector<uint8_t>& inputData, const vector<string>& codeList, vector<uint8_t>& encodeData){

        string bitString;    // bitString 儲存加密後的資料 (以字串儲存)

        // 轉換為 bitArray
        for(const uint8_t& data : inputData){

            bitString += codeList[data];

        }

        uint8_t buffer = 0; // 暫存 8 bits Data
        int curbit = 0;     // 紀錄寫到哪一個 bit

        // 將 bit String 轉換為位元組資料 -> encodeData
        for(const char& c : bitString){

            buffer = (buffer << 1) | (c == '1' ? 1 : 0);
            curbit++;
            
            // 位元組滿了 -> flush 
            if(curbit == 8){
                
                encodeData.push_back(buffer);
                curbit = 0;
                buffer = 0;

            }

        }

        // 將剩下的 bits 補上 0 後放進 encode Data
        if(buffer){

            while(curbit < 8){

                buffer <<= 1;
                curbit++;

            }

            encodeData.push_back(buffer);

        }
        
    }

    // decode() 依照 Code List 將 enecodeData 還原成原本的資料
    void decode(vector<string>& codeList, vector<uint8_t>& encodeData, vector<uint8_t>& resultdata, const unsigned int originalSize){

        // 利用 codeList 產生 bitsTable
        map<string, uint8_t> bitsTable = genTable(codeList);

        string buffer = ""; // 存放

        // 讀取所有 bits;
        for(uint8_t& data : encodeData){

            // 讀取資料中的 8 個 bit
            for(int i=7;i>=0;i--){

                buffer += ((data & (1 << i)) ? "1" : "0");

                // 若遇到相對應的編碼 -> 推入 resultData
                if(bitsTable.count(buffer)){

                    resultdata.push_back(bitsTable[buffer]);
                    buffer = "";

                    // 資料已經足夠即可退回
                    if(resultdata.size() == originalSize){

                        return;

                    }

                }

            }

        }

    }

    // compress 依照 inputData 把資料壓縮並輸出到 filename
    void compress(const char* inputFilename, const char* outputFilename){

        vector<uint8_t> inputData;                                      // 紀錄二進制資料
        getData(inputFilename, inputData);                              // 讀取二進制資料

        Tree HuffmanTree;                                               // 建立 Huffman Tree
        HuffmanTree.build(inputData);                                   // 利用 inputData 製作 Huffman Tree

        vector<string> codeList(256, "");                               // 紀錄編碼資料
        HuffmanTree.genCodeList(codeList, HuffmanTree.getTop(), "");    // 產生編碼資料 -> codeList

        vector<uint8_t> encodeData;                                     // 儲存加密後檔案
        encode(inputData, codeList, encodeData);                        // 利用 inputData, codeList 產生加密資料 -> encodeData

        vector<uint8_t> postorder;                                      // 紀錄後序遍歷
        HuffmanTree.genPostOrder(HuffmanTree.getTop(), postorder);      // 產生後序遍歷路徑

        ofstream outputfile(outputFilename, ios::binary);               // 開啟輸出檔

        // 將壓縮後位元大小、壓縮前位元大小、後序遍歷大小寫入
        unsigned int compressedSize = encodeData.size();
        unsigned int originalSize = inputData.size();
        unsigned int headerSize = postorder.size();
        outputfile.write((char* ) &compressedSize, sizeof(unsigned int));
        outputfile.write((char* ) &originalSize, sizeof(unsigned int));
        outputfile.write((char* ) &headerSize, sizeof(unsigned int));

        // 將後序遍歷資料寫入
        for(uint8_t order : postorder){

            outputfile.write((char* ) &order, sizeof(uint8_t));

        }

        // 將壓縮資料寫入
        for(uint8_t data : encodeData){

            outputfile.write((char* ) &data, sizeof(uint8_t));

        }
        
        // 關閉輸出檔
        outputfile.close();

        // 輸出編碼表
        cout << "Encode Table : \n";
        for(int i=0;i<256;i++){

            if(codeList[i] != "")
                cout << i << " = " << codeList[i] << "\n";

        }  
        cout << "\n";

        double compressRatio = (double)compressedSize / originalSize;    // 壓縮率
        cout << "Compression Ratio : " << fixed << setprecision(3) << compressRatio*100 << "%\n";
        cout << "Compressed Size   : " << compressedSize << "\n";
        cout << "Uncompressed Size : " << originalSize << "\n";

    }

    // decompress 將 filename 解壓縮
    void decompress(const char* filename, const char* outputFilename){

        ifstream inputfile(filename, ios::binary);                          // 開啟(壓縮)輸入檔

        // 下列三個資料皆佔了 4 bytes
        unsigned int compressedSize ;                                       // 壓縮檔大小
        unsigned int originalSize;                                          // 原檔案大小
        unsigned int headerSize;                                            // 標都資訊(邊碼表)大小
        inputfile.read((char* ) &compressedSize, sizeof(unsigned int));
        inputfile.read((char* ) &originalSize, sizeof(unsigned int));
        inputfile.read((char* ) &headerSize, sizeof(unsigned int));

        cout << "Compressed Size   : " << compressedSize << "\n";
        cout << "Uncompressed Size : " << originalSize << "\n";

        // 讀入 postorder 資料
        vector<uint8_t> postorder;
        for(int i=0;i<headerSize;i++){

            uint8_t order;
            inputfile.read((char* ) &order, sizeof(uint8_t));
            postorder.push_back(order);

        }

        // 建立 Huffman Tree
        Tree HuffmanTree;
        HuffmanTree.buildFrom_postorder(postorder);

        // 產生 codeList
        vector<string> codeList(256, "");
        HuffmanTree.genCodeList(codeList, HuffmanTree.getTop(), "");

        // 讀取壓縮過的資料
        vector<uint8_t> encodeData;
        for(int i=0;i<compressedSize;i++){

            uint8_t data;
            inputfile.read((char* ) &data, sizeof(uint8_t));
            encodeData.push_back(data);

        }
        
        // 還原利用 codeList 還原資料
        vector<uint8_t> resultData;
        decode(codeList, encodeData, resultData, originalSize);

        ofstream outputfile(outputFilename, ios::binary);

        for(uint8_t& data : resultData){

            outputfile.write((char*) &data, sizeof(uint8_t));

        }

    }

}



// main() 依照參數處理檔案
// Return Value
//      0 : Success
//      1 : Error
int main(int argc, char* argv[]){

    if(argc == 6){

        // -c 壓縮
        if(strcmp(argv[1], "-c") == 0){

            Huffman::compress(argv[3], argv[5]);
            return 0;

        }
        else if(strcmp(argv[1], "-u") == 0){

            Huffman::decompress(argv[3], argv[5]);
            return 0;

        }
        else{

            cout << "Error : Unknown argument " << argv[1] << "\n";
            return 1;

        }

    }
    else{

        cout << "Error : Wrong format\n";
        return 1;

    }

    return 0;
}