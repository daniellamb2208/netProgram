2021/4 
netProgramming HW2

Author B073040031 葉星佑

.
├── Makefile
├── Readme.txt
├── huffman
│   ├── huffman.cpp
│   ├── huffman.h
│   └── test.cpp
├── socket
│   ├── client.cpp
│   ├── client.h
│   ├── server.cpp
│   └── server.h
└── testFile
    ├── 1.wav
    ├── Homework3_2021.doc
    ├── SCRIPT.docx
    ├── a.txt
    ├── aa.jfif
    ├── b.txt
    └── hello.txt

3 directories, 16 files

這次作業是要實作一個FTP伺服器，要能夠傳輸檔案，並且搭配霍夫曼壓縮技術。
一開始我先將老師上課的PPT在看過一遍，並且讓客戶端能夠連上伺服器端，然後開始實作霍夫曼壓縮
我使用的C++的priority_queue 裝霍夫曼樹的節點，然後用指標把樹整顆建好，
最後用inorder的方式去把對應的編碼給存在map STL裡面，我幾乎都是用C++的STL來裝我的資料的
但後來想跟socket部分串起來的時候發現自己有點太自作聰明了，其實在傳送的時候都是stream用流的
其實根本不用用那麼多看起來很厲害的東西，簡單來說就是其實可以單純用struct來裝就好了。

我霍夫曼的api有encode跟decode兩個，
剩下的compare class是用來給priority queue比大小的
inorderTraversal是拿來讀取霍夫曼樹的資訊
p_q可以幫我很容易實現兩個節點拿出來合併成一個之後再放回去，
我實現的是variable的huffman，在編碼前先將所有字元掃過一遍，
統計每一個出現的頻率，透過pq實現的tree，得到每一個字元的編碼

My tree example,    left node encode '0'
                    right node encode '1'
        abcd:12
        /     \
       /       \
     abc:7    d:5
     /  \
    /    \
   ab:3  c:4
  /  \
 /    \
a:1   b:2

a 1 -> 000
b 2 -> 001
c 4 -> 01
d 5 -> 1

但是在編碼過程中我發現，原本一個char 大小的 a 居然被編成了 000, 3個 char大小 (驚恐)
所以我的想法就是把他們八個八個為單位，因為都只有 1 or 0，所以八個抓起來當成一個char
例如 aabcd -> 000000001011  
           -> asc(0) 1011
那這後面的 1011 怎麼辦呢，我就用 '0' 將把補滿到8個字元 成為一個 char 大小的 '1'/'0'，
不過就是要把我補了幾個零告訴解碼的人，才不會在檔案最後出現奇怪的字符。

解碼就是將收到的字元當成8個字來處理， 若我收到 '1', 我應該視為他是 49, ascii 表告訴我的
但程式可以使用bitset將他轉成 8 位元的樣子 00110001, 然後我再將所有解碼的 0, 1, 霍夫曼特性 prefix 不會重複 
去我的table裡面找原始的字元，就可以完整地將我的檔案解壓縮回來。

在過程中當然遇到了很多問題，像是我一直卡在memory leak, 原因是因為我推進去pq的東西沒有被delete
後來的解決方法是使用c++的smart pointer, 他會在變數生命週期結束的時候去幫我管理記憶體。

在socket的部分更是痛不欲生，第一次使用如此多的魔法，在接收跟傳送這裡一直卡著，
出了一大堆的神奇報錯，有 stack-buffer-overflow, stack-buffer-underflow dynamic-stack-buffer-overflow
貌似都是我在讀取的時候越界存取了，大概試了4個晚上，無數種排列組合，使用busy waiting, 限制吃的大小
最後誤打誤撞成功了，所以我也不確定在這過程中，為什麼同樣的程式碼有不同樣的結果，一定是我寫出來的程式是 non deterministic
就像梗圖中的鴿子一樣，頭轉一轉成功飛起來了，我最後的解法就是把伺服器端需要的資料塞在一個檔案中，用每次都get a char的方式將它變成類似stream的東西
在傳送之前要先跟伺服器溝通我等等要送東西給你, flag 0 是等等要送資料，你給我先吃一個長度，等等我要送的幾個char
flag 0, 是客戶端結束(leave), 你也可以休息了，所以關閉伺服器。
不得不說這次的作業讓我見識到我對於程式並不了解，不了解就不了解，沒有好像QQ
希望在下次的作業中，能夠學到更多的魔法，雖然過程中有點痛苦就是了。