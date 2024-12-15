#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Author: B123040049 劉育希
// Date  : 2024/09/24

/*

Purpose:

    產生 HW7 所需要的測試檔案

    -> 每組資料量都要有 10 組側資

*/

int main(){

    // 開啟測資檔
    ofstream in("input.txt");          // 測資檔
    ofstream test("Time_test.txt");    // 時間測試檔

    // 設定亂數種子
    srand(time(NULL));

    // 1.產生一班測資檔
    const int size = 250;   // 測資大小
    in << size << "\n";     // 輸出測資量

    // 產生 size 筆測資
    for(int i = 0 ; i < size ; i++){

        in << rand() * rand() << "\n";

    }


    // 2. 產生時間測試所需的大測資
    // 總共 8 組資料量
    const int numSize = 8;
    const int testCaseSize[numSize] = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000};
    const int dif = 10; // 同一組資料量需要 10 組不同的測資

    // 跑過所有測資量
    for(const int &size : testCaseSize){

        test << size << " " << dif << "\n"; // 輸出 size dif 在每組資料量的第一排

        // 產生 dif 組測資 
        for(int loop = 0 ; loop < dif ; loop++){


            // 依照資料量產生測資
            for(int i = 0 ; i < size ; i++){

                test << rand()*rand() << " ";

            } 

            test << endl;

        } 

    }

    cout << "Output Complete!\n";

    // 關閉檔案
    in.close();
    test.close();

    return 0;
}