#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define member_file "member.txt"
#define book_file "book.txt"
#define data_file "data.txt"

//會員資料結構
class member{
    friend class LinkedList;
private:
    int id;                 //會員編號
    char name[20];          //會員姓名
    char birthday[20];      //會員生日
    char id_number[20];     //會員身份證字號
    char phone[20];         //會員電話
    char email[40];         //會員信箱
    int borrow;             //會員借閱書籍數量，每位會員借閱上限10本
    //char borrowBook[5][20]; //會員借閱書籍書名

    member *pre;
    member *next;
};

//書籍資料結構
class book{
    friend class LinkedList;
private:
    int id;                 //書籍編號
    char name[20];          //書籍書名
    char publisher[20];     //書籍出版社
    char publish_date[40];  //書籍出版日期
    int state;              //書籍可借狀態，0為已借出，1為可借

    book *pre;
    book *next;
};

//借閱資料結構
class data{
    friend class LinkedList;
private:
    int id;                 //借閱資料編號
    char member_name[20];   //借閱資料會員
    int book_number;        //借閱資料書籍數量
    //int book_id[5];         //借閱資料書籍書名

    data *pre;
    data *next;
};

//串列資料處理
class LinkedList{
public:
    LinkedList();
    ~LinkedList();
    void release(member *mp, book *bp, data *dp);

    void load_member();     /*讀檔*/
    void load_book();
    void load_data();
    void save_member();     /*寫檔*/
    void save_book();
    void save_data();
    int member_login();    /*帳號登入*/
    int manager_login();

    void insert_member();   /*新增*/
    void insert_book();
    void insert_data();
    void delete_member();   /*刪除*/
    void delete_book();
    void delete_data();
    void show_member();     /*顯示*/
    void show_book();
    void show_data();

    void find_member();     /*尋找*/
    void find_book();
    void find_data();

private:
    FILE *fp;

    member *member_head, *member_tail;
    book *book_head, *book_tail;
    data *data_head, *data_tail;

    int num_member, num_book, num_data;     //紀錄會員、書籍、借閱資料目前數量
    int sn_member, sn_book, sn_data;        //紀錄會員、書籍、借閱資料編號
};
//construct
LinkedList::LinkedList(){                   //系統建構初始值
    member_head = member_tail = NULL;
    book_head = book_tail = NULL;
    data_head = data_tail = NULL;
    num_member = num_book = num_data = 0;
    sn_member = sn_book = sn_data = 0;
}
//destruct
LinkedList::~LinkedList(){
    release(member_head, book_head, data_head);
}
//release memory
void LinkedList::release(member *mp, book *bp, data *dp){
    member *mp4d;
    book *bp4d;
    data *dp4d;

    while(mp != NULL){
        mp4d = mp->next;
        if(mp4d != NULL){
            mp4d->pre = NULL;    //防野指標
        }
        free(mp);
        mp = mp4d;
    }
    while(bp != NULL){
        bp4d = bp->next;
        if(bp4d != NULL){
            bp4d->pre = NULL;    //防野指標
        }
        free(bp);
        bp = bp4d;
    }
    while(dp != NULL){
        dp4d = dp->next;
        if(dp4d != NULL){
            dp4d->pre = NULL;    //防野指標
        }
        free(dp);
        dp = dp4d;
    }
}

/*讀檔*/
void LinkedList::load_member(){
    member *new_member;

    fp = fopen(member_file, "r");
    fscanf(fp, "%d", &num_member);                      //讀取第一行，得num_member
    for(int i=0;i<num_member;i++){                      //讀取num_member筆會員資料
        new_member = (member *)malloc(sizeof(member));
        fscanf(fp, "%d", &new_member->id);
        fscanf(fp, "%s", new_member->name);
        fscanf(fp, "%s", new_member->birthday);
        fscanf(fp, "%s", new_member->id_number);
        fscanf(fp, "%s", new_member->phone);
        fscanf(fp, "%s", new_member->email);
        fscanf(fp, "%d", &new_member->borrow);
        new_member->pre = NULL;
        new_member->next = NULL;

        if(member_head == NULL){
            member_head = member_tail = new_member;
        }else{
            member_tail->next = new_member;             //連結
            new_member->pre = member_tail;
            member_tail = new_member;
        }
    }
    fclose(fp);
}

void LinkedList::load_book(){
    book *new_book;

    fp = fopen(book_file, "r");
    fscanf(fp, "%d", &num_book);                        //讀取第一行，得num_book
    for(int i=0;i<num_book;i++){                        //讀取num_book筆書籍資料
        new_book = (book *)malloc(sizeof(book));
        fscanf(fp, "%d", &new_book->id);
        fscanf(fp, "%s", new_book->name);
        fscanf(fp, "%s", new_book->publisher);
        fscanf(fp, "%s", new_book->publish_date);
        fscanf(fp, "%d", &new_book->state);
        new_book->pre = NULL;
        new_book->next = NULL;

        if(book_head == NULL){
            book_head = book_tail = new_book;
        }else{
            book_tail->next = new_book;                 //連結
            new_book->pre = book_tail;
            book_tail = new_book;
        }
    }
    fclose(fp);
}

/*寫檔*/
void LinkedList::save_member(){
    member *ptr = member_head;

    fp = fopen(member_file, "w");
    fprintf(fp, "%d\n", num_member);

    while(ptr != NULL){
        fprintf(fp, "%d\n", ptr->id);
        fprintf(fp, "%s\n", ptr->name);
        fprintf(fp, "%s\n", ptr->birthday);
        fprintf(fp, "%s\n", ptr->id_number);
        fprintf(fp, "%s\n", ptr->phone);
        fprintf(fp, "%s\n", ptr->email);
        fprintf(fp, "%d\n", ptr->borrow);
        ptr = ptr->next;
    }
    fclose(fp);
}

void LinkedList::save_book(){
    book *ptr = book_head;

    fp = fopen(book_file, "w");
    fprintf(fp, "%d\n", num_book);

    while(ptr != NULL){
        fprintf(fp, "%d\n", ptr->id);
        fprintf(fp, "%s\n", ptr->name);
        fprintf(fp, "%s\n", ptr->publisher);
        fprintf(fp, "%s\n", ptr->publish_date);
        fprintf(fp, "%d\n", ptr->state);
        ptr = ptr->next;
    }
    fclose(fp);
}

/*登入*/
int LinkedList::member_login(){
    member *mp = member_head;
    char member_account[20], member_password[20];
    int account_correct = 0, password_correct = 0;   //帳號密碼flag

    printf("請輸入會員身分證字號: ");
    scanf("%s", member_account);
    printf("請輸入會員手機末三碼: ");
    scanf("%s", member_password);

    while(mp != NULL){
        if(!strcmp(mp->id_number, member_account)){
            account_correct = 1;                    //找到帳號(id_number)
            for(int i=0;i<3;i++){                   //比對密碼(phone末三碼)
                if(member_password[i] == mp->phone[7+i]){
                    password_correct = 1;
                }else{
                    password_correct = 0;
                    break;
                }
            }
            break;                                 //假設身份證字號不重複，僅比對一筆即可，提早結束迴圈
        }else{
            mp = mp->next;
        }
    }

    if(!account_correct){
        printf("查無會員!\n");
        system("pause");
        system("cls");
        return -1;
    }else if(!password_correct){
        printf("密碼錯誤!\n");
        system("pause");
        system("cls");
        return -1;
    }else{
        printf("登入成功!\n");
        system("pause");
        system("cls");
        return 1;
    }
}

int LinkedList::manager_login(){
    char manager_password[] = "12345678";
    char password[20];

    printf("請輸入管理員密碼: ");
    scanf("%s", password);

    if(!strcmp(password, manager_password)){
        printf("登入成功!\n");
        system("pause");
        system("cls");
        return 1;
    }else{
        printf("密碼錯誤!\n");
        system("pause");
        system("cls");
        return -1;
    }
}

/*新增*/
void LinkedList::insert_member(){
    member *new_member = (member *)malloc(sizeof(member));

    printf("[註冊會員] 請填寫以下資料\n");
    printf("*姓名: ");
    scanf("%s", new_member->name);
    printf("*生日: ");
    scanf("%s", new_member->birthday);
    printf("*身分證字號: ");
    scanf("%s", new_member->id_number);
    printf("*電話: ");
    scanf("%s", new_member->phone);
    printf("*信箱: ");
    scanf("%s", new_member->email);

    new_member->borrow = 0;                            //會員借閱數量預設為0
    new_member->pre = NULL;
    new_member->next = NULL;

    if(member_head == NULL){
        new_member->id = 1;                            //第一位會員編號設定1
        member_head = member_tail = new_member;
    }else{
        new_member->id = (member_tail->id) + 1;        //會員編號為上次編號+1
        member_tail->next = new_member;                //連結
        new_member->pre = member_tail;
        member_tail = new_member;
    }
    printf("\n會員註冊成功!\n\n");
    num_member++;                                      //會員總數+1
}
void LinkedList::insert_book(){
    book *new_book = (book *)malloc(sizeof(book));

    printf("<< 註冊書籍 >>\n");
    printf("*請輸入書名: ");
    scanf("%s", new_book->name);
    printf("*請輸入出版社: ");
    scanf("%s", new_book->publisher);
    printf("*請輸入出版日期: ");
    scanf("%s", new_book->publish_date);

    new_book->state = 0;                       //書籍狀態預設為可借
    new_book->pre = NULL;
    new_book->next = NULL;

    if(book_head == NULL){
        new_book->id = 1;                      //第一本書籍編號設定1
        book_head = book_tail = new_book;
    }else{
        new_book->id = (book_tail->id) + 1;    //書籍編號為最新編號+1
        book_tail->next = new_book;            //連結
        new_book->pre = book_tail;
        book_tail = new_book;
    }
    printf("書本註冊成功!\n");
    num_book++;                                //書籍總數+1
}

void LinkedList::insert_data(){                     //尋找會員，確認會員狀態，尋找書籍，確認書籍狀態
    data *new_data;
    member *mp = member_head;
    book *bp = book_head;
    int member_id;
    int book_id;
    int found = 0;

    printf("請輸入會員編號: ");                    //尋找會員
    scanf("%d", &member_id);
    while(mp != NULL){
        if(mp->id == member_id){
            found = 1;
        }else{
            mp = mp->next;
        }
    }
    if(found){

    }else{
        printf("找不到該會員，請重新輸入!\n");
        return;
    }

    scanf("%d", &book_id);

    new_data = (data *)malloc(sizeof(data));        //建立借閱資料
}

/*刪除*/
void LinkedList::delete_member(){
    member *ptr = member_head;
    member *previous_position;
    char target_name[20];
    int found = 0;

    printf("輸入要刪除的會員姓名: ");
    scanf("%s", target_name);

    while(ptr != NULL){                                 //依名字尋找位置
        if(!strcmp(ptr->name, target_name)){
            found = 1;
            break;
        }else{
            ptr = ptr->next;
        }
    }

    if(found){                                          //ptr位置的刪除case
        if(ptr == member_head){                         //頭
            member_head = member_head->next;
            if(member_head != NULL){                    //防野指標
                member_head->pre = NULL;
            }
        }else{
            if(ptr == member_tail){                     //尾
                member_tail = member_tail->pre;
                member_tail->next = NULL;
            }else{                                      //中間
                previous_position = ptr->pre;
                previous_position->next = ptr->next;
            }
        }

        free(ptr);
        printf("刪除成功!\n");
        num_member--;                                   //會員總數-1
    }else{
        printf("找不到該會員姓名，無法刪除!\n");
    }
}

void LinkedList::delete_book(){
    book *ptr = book_head;
    book *previous_position;
    char target_name[20];
    int found = 0;

    printf("請輸入書名: ");
    scanf("%s", target_name);

    while(ptr != NULL){                                 //依名字尋找位置
        if(!strcmp(ptr->name, target_name)){
            found = 1;
            break;
        }else{
            ptr = ptr->next;
        }
    }

    if(found){                                          //ptr位置的刪除case
        if(ptr == book_head){                           //頭
            book_head = book_head->next;
            book_head->pre = NULL;
        }else{
            if(ptr == book_tail){                       //尾
                book_tail = book_tail->pre;
                book_tail->next = NULL;
            }else{                                      //中間
                previous_position = ptr->pre;
                previous_position->next = ptr->next;
            }
        }

        free(ptr);
        printf("刪除成功!\n");
        num_book--;                                   //書籍總數-1
    }else{
        printf("找不到該書名，無法刪除!\n");
    }
}

//void LinkedList::delete_data();


/*顯示*/
void LinkedList::show_member(){
    member *ptr = member_head;
    printf("共有 %d 筆會員資料\n", num_member);
    while(ptr != NULL){
        printf("會員編號: %d\n", ptr->id);
        printf("姓名: %s\n", ptr->name);
        printf("生日: %s\n", ptr->birthday);
        printf("身分證字號: %s\n", ptr->id_number);
        printf("電話: %s\n", ptr->phone);
        printf("信箱: %s\n", ptr->email);
        printf("====================\n");
        ptr = ptr->next;
    }
}

void LinkedList::show_book(){
    book *ptr = book_head;
    printf("共有 %d 筆書籍資料\n", num_book);
    while(ptr != NULL){
        printf("書籍編號: %d\n", ptr->id);
        printf("書名: %s\n", ptr->name);
        printf("出版社: %s\n", ptr->publisher);
        printf("出版日期: %s\n", ptr->publish_date);
        printf("狀態: %s\n", (ptr->state==0) ? "尚未借出，可借閱":"已借出，無法借閱");
        printf("====================\n");
        ptr = ptr->next;
    }
}


int main(){
    char op;
    int user, manager, logout;          //flag
    LinkedList Library_System;

    //開啟，讀檔
    Library_System.load_member();
    Library_System.load_book();

    while(1){
        printf("+------------------------------------------------+\n");
        printf("+               圖書館書籍借閱系統               +\n");
        printf("+------------------------------------------------+\n");
        printf("+      (M)會員登入       |      (R)註冊會員      +\n");
        printf("+------------------------------------------------+\n");
        printf("+      (S)管理員登入     |      (Q)結束系統      +\n");
        printf("+------------------------------------------------+\n");
        printf("操作> ");
        scanf(" %c", &op);

        switch(op){
            //會員登入
            case 'M':
            case 'm':
                user = Library_System.member_login();
                if(user == -1){
                    main();
                }else{
                    while(1){
                        printf("[會員]\n");
                        printf("(1)借書\n");
                        printf("(2)還書\n");
                        printf("(3)登出\n");
                        scanf(" %c", &op);
                        switch(op){
                            case '1':
                                break;
                            case '2':
                                break;
                            case '3':
                                printf("\n已登出!\n");
                                system("pause");
                                system("cls");
                                main();
                                break;
                            default:
                                printf("輸入指令有誤，請重新輸入\n");
                                system("pause");
                                system("cls");
                                break;
                        }
                    }
                }
                break;
            //會員註冊
            case 'R':
            case 'r':
                Library_System.insert_member();
                break;

            //管理員登入
            case 'S':
            case 's':
                manager = Library_System.manager_login();
                if(manager == -1){
                    main();
                }else{
                    logout = 0;
                    while(1){
                        printf("[管理員]\n");
                        printf("\n會員管理:\n");
                        printf("(1)新增會員\n");
                        printf("(2)刪除會員\n");
                        printf("(3)顯示所有會員\n");

                        printf("\n書籍管理:\n");
                        printf("(4)新增書籍\n");
                        printf("(5)刪除書籍\n");
                        printf("(6)顯示所有書籍\n");

                        printf("\n借閱管理:\n");
                        printf("(7)新增借閱\n");
                        printf("(8)刪除借閱\n");
                        printf("(9)顯示所有借閱\n");

                        printf("\nQ.登出\n> ");

                        scanf(" %c", &op);
                        switch(op){
                            //會員管理
                            case '1':
                                Library_System.insert_member();
                                break;

                            case '2':
                                Library_System.delete_member();
                                break;

                            case '3':
                                Library_System.show_member();
                                break;

                            //書籍管理
                            case '4':
                                Library_System.insert_book();
                                break;

                            case '5':
                                Library_System.delete_book();
                                break;

                            case '6':
                                Library_System.show_book();
                                break;

                            //資料管理
                            case '7':
                                break;

                            case '8':
                                break;

                            case '9':
                                break;

                            case 'Q':
                            case 'q':
                                Library_System.save_member();              //登出，寫檔
                                Library_System.save_book();
                                logout = 1;
                                printf("\n已登出!\n");
                                break;

                            default:
                                printf("輸入指令有誤，請重新輸入\n");
                                break;
                        }
                        if(logout == 1){
                            break;
                        }
                        system("pause");
                        system("cls");
                    }
                }
                break;

            //結束，寫檔
            case 'Q':
            case 'q':
                Library_System.save_member();
                Library_System.save_book();
                printf("\n結束系統!\n");
                return 0;
                break;

            default:
                printf("輸入指令有誤，請重新輸入\n");
                break;
        }
        system("pause");
        system("cls");
    }

    return 0;
}
