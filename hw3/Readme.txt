我是使用thread的方法去實現伺服器和客戶端

伺服器(server)的主thread用來accept新的連接，並讓他們各自成為一個新的thread
每一個客戶端連到伺服器的時候都會有一個thread去接應他
每一個用戶的名字和基本資料都會存在 record 這個全域變數的 user 中，user中有許多處理回饋需要的資料
而thread的function裡面的功能有，
1.0 初始化user
1.1 若有offline訊息 則會在上線後收到別人的留言
1.2 提示其他使用者，有使用者上線

2.0 接收及處理client的request
2.1 從clinet 接受一個opcode來辨別我要執行的通訊  (設計最初的理念是收到幾個參數 
2.2 opcode (0) 代表client 切斷了連線 bye command 當用戶切斷連線，會廣播給所有還在線上的用戶
2.3 unicast(2)      chat A "msg"
2.4 multicast(3)    chat A B "msg" 僅支援一次對兩個人送訊息
2.5 在relay訊息的時候 有可能遇到對方是不在線上的，所以伺服器就要幫sender pending訊息，直到receiver 上線後收到
2.6 有可能遇到輸入的receiver是不存在的，所以要告知client那個用戶不存在

note:
在做通訊 read() write() 時，3rd 參數要對稱要做好(接收跟發送的量盡量要一樣)，不然很容易整個程式都爛掉。

客戶端(client)
main function 主要用來接收用戶的input，然後還會分岔出一個thread去負責接收來自伺服器的回饋
getmsg() 則是thread裡面執行的函式，這邊也是用opcode的概念，伺服器會依不同狀況先回傳一個opcode，來告知我現在需要怎麼收訊息

// 1 for normal communication
// 2 for off the line reminder
// 3 for offline msg
// 4 for not exist
// 5 for online reminder
// 6 for remind chatter is offline, will pending the message
1 有個人跟我講話，我要收寄件者跟訊息 兩個read()
2 有人下線，收那個人的名字 一個read()
3 我離線過，在離線期間有人傳訊息給我被pending在伺服器，所以這個要收比較多資訊 寄件者 訊息 發送時間 三個read()  
4 我發送給一個不存在的用戶 一個read() 
5 上線通知 一個read()
6 我發送出去一個訊息，但接收者不在線上，伺服器端pending我的訊息 一個read()

command

--help
    叫出幫忙的訊息
chat [name] [name](option) "message"
    第一個接收者 若只有一個接收者則 unicast
    第二個接收者(可選)             multicast
    訊息需要用 " 包起來 因為我的做法是用 " 去判斷他是不是訊息
bye
    結束連線、終止程式

Demo (usage from homework announcement pdf)

[terminal server]
> make
> ./server
--------------------------
[terminal A]
>./client
    > connect 127.0.0.1 2208 A
[terminal B]
>./client
    > connect 127.0.0.1 2208 B
[terminal C]
>./client
    > connect 127.0.0.1 2208 C

if do this respectively, A will get B and C are online, B will get C is online message
--------------------------
then unicast

[terminal A]
chat C "hello, c"

[terminal C] 
A says "hello, c"
--------------------------
then multicast

[terminal A] 
chat B C "hello, guys"

[terminal B] 
A says "hello, guys"

[terminal C] 
A says "hello, guys"
--------------------------
then DNE user

[terminal A]
chat H "hello, how r u"
User H is not a existing user
--------------------------
then offline message

[terminal C] 
bye 
Bye Bye

[terminal A] 
chat C "Lend me $"
C is offline, the message will pending until he/she is back

[terminal C] 
> ./client
    > connect 127.0.0.1 2208 C 
now everyone is online will get C is online message 
and C will get the pending message from A 

A left a message to you when (Wed May 19 01:14:52 2021) time when message sent
"Lend me $"
--------------------------
server 那邊會顯示用戶端們通訊的log