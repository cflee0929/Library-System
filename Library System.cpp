#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define member_file "member.txt"
#define book_file "book.txt"
#define record_file "record.txt"

time_t t;
struct tm *TIME;

/*資料結構*/
class member{
friend class LinkedList;
private:
    int id;                 //會員編號
    char name[20];          //姓名
    char birthday[20];      //生日
    char id_number[20];     //身份證字號
    char phone[20];         //電話
    char email[40];         //信箱
    int num_borrow;         //已借數量
    member *next;
};

class book{
friend class LinkedList;
private:
    int id;                 //館藏編號
    char name[20];          //書名
    char ISBN[20];          //ISBN
    char publish_date[40];  //出版日期
    char publisher[20];     //出版社
    int state;              //狀態: 0為已借出，1為未借出
    book *next;
};

class record{
friend class LinkedList;
private:
    int id;                 //借還記錄編號
    int member_id;          //會員編號
    int book_id;            //館藏編號
    int time[3];            //借還時間，年月日
    record *next;
};

class LinkedList{
public:
    LinkedList();
    ~LinkedList();

    void load_member();
    void load_book();
    void load_record();

    void save_member();
    void save_book();
    void save_record();
    //登入
    void login_member();
    void login_manager();
    //操作
    void member_interface(member *mp);
    void manager_interface();

    void insert_member();
    void insert_book();
    void insert_record(member *mp);

    void delete_member();
    void delete_book();
    void delete_record(member *mp);

    void show_member();
    void show_book();
    void show_record();

    void check_member_record(member *mp);
    book *find_book_byid(int book_id);
    book *find_book_byname(int book_id);
private:
    FILE *fp;
    member *member_head, *member_tail;
    book *book_head, *book_tail;
    record *record_head, *record_tail;
    int sn_member, sn_book, sn_record;        //編號
    int num_member, num_book, num_record;     //數量
};
//建構解構
LinkedList::LinkedList(){
    member_head = member_tail = NULL;
    book_head = book_tail = NULL;
    record_head = record_tail = NULL;
    num_member = num_book = num_record = 0;
}

LinkedList::~LinkedList(){
    member *mp, *mp4d;
    book *bp, *bp4d;
    record *rp, *rp4d;

    mp4d = member_head;
    while(mp4d=NULL){
        mp = mp4d->next;
        free(mp4d);
        mp4d = mp;
    }

    bp4d = book_head;
    while(bp4d!=NULL){
        bp = bp4d->next;
        free(bp4d);
        bp4d = bp;
    }

    rp4d = record_head;
    while(rp4d!=NULL){
        rp = rp4d->next;
        free(rp4d);
        rp4d = rp;
    }
}
//讀取
void LinkedList::load_member(){
    member *new_member;

    fp = fopen(member_file, "r");
    fscanf(fp, "%d", &num_member);
    if(fp){
        for(int i=0;i<num_member;i++){
            new_member = (member *)malloc(sizeof(member));

            fscanf(fp, "%d", &new_member->id);
            fscanf(fp, "%s", new_member->name);
            fscanf(fp, "%s", new_member->birthday);
            fscanf(fp, "%s", new_member->id_number);
            fscanf(fp, "%s", new_member->phone);
            fscanf(fp, "%s", new_member->email);
            fscanf(fp, "%d", &new_member->num_borrow);
            new_member->next = NULL;

            if(member_head == NULL){
                member_head = member_tail = new_member;
            }else{
                member_tail->next = new_member;
                member_tail = new_member;
            }
        }
    }
    fclose(fp);
}

void LinkedList::load_book(){
    book *new_book;

    fp = fopen(book_file, "r");
    fscanf(fp, "%d", &num_book);
    if(fp){
        for(int i=0;i<num_book;i++){
            new_book = (book *)malloc(sizeof(book));

            fscanf(fp, "%d", &new_book->id);
            fscanf(fp, "%s", new_book->name);
            fscanf(fp, "%s", new_book->ISBN);
            fscanf(fp, "%s", new_book->publish_date);
            fscanf(fp, "%s", new_book->publisher);
            fscanf(fp, "%d", &new_book->state);
            new_book->next = NULL;

            if(book_head == NULL){
                book_head = book_tail = new_book;
            }else{
                book_tail->next = new_book;
                book_tail = new_book;
            }
        }
    }
    fclose(fp);
}

void LinkedList::load_record(){
    record *new_record;
    int data;

    fp = fopen(record_file, "r");
    fscanf(fp, "%d", &num_record);
    if(fp){
        for(int i=0;i<num_record;i++){
            new_record = (record *)malloc(sizeof(record));
            fscanf(fp, "%d", &new_record->id);
            fscanf(fp, "%d", &new_record->member_id);
            fscanf(fp, "%d", &new_record->book_id);
            for(int i=0;i<3;i++){
                fscanf(fp, "%d/", &data);
                new_record->time[i] = data;
            }
            new_record->next = NULL;

            if(record_head == NULL){
                record_head = record_tail = new_record;
            }else{
                record_tail->next = new_record;
                record_tail = new_record;
            }
        }
    }
    fclose(fp);
}
//儲存
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
        fprintf(fp, "%d\n", ptr->num_borrow);
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
        fprintf(fp, "%s\n", ptr->ISBN);
        fprintf(fp, "%s\n", ptr->publish_date);
        fprintf(fp, "%s\n", ptr->publisher);
        fprintf(fp, "%d\n", ptr->state);
        ptr = ptr->next;
    }
    fclose(fp);
}

void LinkedList::save_record(){
    record *ptr = record_head;

    fp = fopen(record_file, "w");
    fprintf(fp, "%d\n", num_record);

    while(ptr != NULL){
        fprintf(fp, "%d\n", ptr->id);
        fprintf(fp, "%d\n", ptr->member_id);
        fprintf(fp, "%d\n", ptr->book_id);
        for(int i=0;i<3;i++){
            if(i!=2){
                fprintf(fp, "%d/", ptr->time[i]);
            }else{
                fprintf(fp, "%d\n", ptr->time[i]);
            }
        }
        ptr = ptr->next;
    }
    fclose(fp);
}

void LinkedList::login_member(){
    member *mp = member_head;
    int member_account;
    char member_password[20];
    int account_correct = 0, password_correct = 0;   //帳號密碼flag

    printf("輸入帳號(會員編號): ");
    scanf("%d", &member_account);
    printf("輸入密碼(會員身分證末三碼): ");
    scanf("%s", member_password);

    while(mp != NULL){
        if(mp->id == member_account){ //找到會員
            account_correct = 1;
            for(int i=0;i<3;i++){
                if(member_password[i] == mp->id_number[7+i]){   //比對身分證末三碼
                    password_correct = 1;
                }else{
                    password_correct = 0;
                    break;
                }
            }
            break;
        }else{
            mp = mp->next;
        }
    }

    if(!account_correct){
        printf("查無帳號!\n");
        system("pause");
        system("cls");
        return;
    }else if(!password_correct){
        printf("密碼錯誤!\n");
        system("pause");
        system("cls");
        return;
    }else{
        printf("登入成功!\n");
        system("pause");
        system("cls");
        member_interface(mp);
    }
}

void LinkedList::login_manager(){
    char manager_password[] = "1234";
    char password[20];

    printf("請輸入管理員密碼(提示:1234): ");
    scanf("%s", password);

    if(!strcmp(password, manager_password)){
        printf("登入成功!\n");
        system("pause");
        system("cls");
        manager_interface();
    }else{
        printf("密碼錯誤!\n");
        system("pause");
        system("cls");
        return;
    }
}

void LinkedList::insert_member(){
    member *new_member = (member *)malloc(sizeof(member));

    printf("註冊會員\n");
    printf("姓名: ");
    scanf("%s", new_member->name);
    printf("生日: ");
    scanf("%s", new_member->birthday);
    printf("身分證字號: ");
    scanf("%s", new_member->id_number);
    printf("電話: ");
    scanf("%s", new_member->phone);
    printf("信箱: ");
    scanf("%s", new_member->email);
    new_member->num_borrow = 0;
    new_member->next = NULL;

    if(member_head == NULL){
        new_member->id = 1;                            //第一位會員編號設定1
        member_head = member_tail = new_member;
    }else{
        new_member->id = (member_tail->id) + 1;        //會員編號為tail編號+1
        member_tail->next = new_member;
        member_tail = new_member;
    }
    num_member++;
}

void LinkedList::insert_book(){
    book *new_book = (book *)malloc(sizeof(book));

    printf("註冊書籍\n");
    printf("書名: ");
    scanf("%s", new_book->name);
    printf("ISBN: ");
    scanf("%s", new_book->ISBN);
    printf("出版社: ");
    scanf("%s", new_book->publisher);
    printf("出版日期: ");
    scanf("%s", new_book->publish_date);
    new_book->state = 0;
    new_book->next = NULL;

    if(book_head == NULL){
        book_head = book_tail = new_book;
        new_book->id = 1;                           //第一本書籍編號設定1
    }else{
        book_tail->next = new_book;
        book_tail = new_book;
        new_book->id = (book_tail->id) + 1;         //書籍編號為tail編號+1
    }
    num_book++;
    printf("註冊成功!\n");
}

void LinkedList::insert_record(member *mp){
    book *bp;
    int book_id;
    time(&t);
    TIME = localtime(&t);

    printf("要借閱的書籍編號: ");
    scanf("%d", &book_id);                  //手動輸入書籍編號，依id找書
    bp = find_book_byid(book_id);
    if(!bp){
        printf("找不到書籍編號，借閱失敗\n");
    }else{
        record *new_record = (record *)malloc(sizeof(record));
        new_record->member_id = mp->id;                     //會員編號帶入
        new_record->book_id = bp->id;                       //書籍編號帶入
        new_record->time[0] = TIME->tm_year+1900;                //年月日
        new_record->time[1] = TIME->tm_mon+1;
        new_record->time[2] = TIME->tm_mday;
        new_record->next = NULL;

        mp->num_borrow++;                                   //修改會員已借數量、書籍狀態
        bp->state = 1;

        if(record_head == NULL){
            record_head = record_tail = new_record;
            new_record->id = 1;                             //第一筆借閱編號設定1
        }else{
            record_tail->next = new_record;
            record_tail = new_record;
            new_record->id = (record_tail->id) + 1;         //書籍編號為tail編號+1
        }
        num_record++;
        printf("借閱成功!\n");
    }
}

void LinkedList::delete_member(){
    member *mp, *pre;
    int delete_target;
    int found = 0;

    printf("要刪除的會員編號: ");
    scanf("%d", &delete_target);

    mp = member_head;
    while(mp != NULL){
        if(mp->id==delete_target){
            found = 1;
            break;
        }else{
            pre = mp;
            mp = mp->next;
        }
    }
    if(found){
        if(mp == member_head){
            member_head = member_head->next;
        }else{
            if(mp == member_tail){
                member_tail = pre;
                member_tail->next = NULL;
            }else{
                pre->next = mp->next;
            }
        }
        free(mp);
        num_member--;
        printf("找到會員，已刪除!\n");
    }else{
        printf("找不到會員，刪除失敗!\n");
    }
}

void LinkedList::delete_book(){
    book *bp, *pre;
    int delete_target;
    int found = 0;

    printf("要刪除的書籍編號: ");
    scanf("%d", delete_target);

    bp = book_head;
    while(bp != NULL){
        if(bp->id==delete_target){
            found = 1;
            break;
        }else{
            pre = bp;
            bp = bp->next;
        }
    }
    if(found){
        if(bp == book_head){
            book_head = book_head->next;
        }else{
            if(bp == book_tail){
                book_tail = pre;
                book_tail->next = NULL;
            }else{
                pre->next = bp->next;
            }
        }
        free(bp);
        num_book--;
        printf("找到書籍，已刪除!");
    }else{
        printf("找不到書籍，刪除失敗!\n");
    }
}

void LinkedList::delete_record(member *mp){
    record *rp, *pre;
    book *bp;
    int book_id;

    printf("要歸還的書籍編號: ");
    scanf("%d", &book_id);

    bp = book_head;                  //手動輸入書籍編號，依id找書
    while(bp!=NULL){
        if(bp->id == book_id){
            break;
        }else{
            bp = bp->next;
        }
    }

    rp = record_head;
    while(rp!=NULL){
        if(rp->book_id == book_id){
            break;
        }else{
            pre = rp;
            rp = rp->next;
        }
    }

    if(rp){
        if(rp == record_head){
            record_head = record_head->next;
        }else{
            if(rp == record_tail){
                record_tail = pre;
                record_tail->next = NULL;
            }else{
                pre->next = rp->next;
            }
        }
        free(rp);
        mp->num_borrow--;
        bp->state = 0;
        num_record--;
        printf("找到借閱紀錄，歸還成功!\n");

    }else{
        printf("找不到借閱紀錄，歸還失敗\n");
    }
}

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
        printf("已借數量: %d\n", ptr->num_borrow);
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
        printf("ISBN: %s\n", ptr->ISBN);
        printf("出版日期: %s\n", ptr->publish_date);
        printf("出版社: %s\n", ptr->publisher);
        printf("狀態: %s\n", (ptr->state) ? "已借出，無法借閱":"尚未借出，可借閱");
        printf("====================\n");
        ptr = ptr->next;
    }
}

void LinkedList::show_record(){
    record *ptr = record_head;
    printf("共有 %d 筆借閱資料\n", num_record);
    while(ptr != NULL){
        printf("借閱編號: %d\n", ptr->id);
        printf("會員編號: %d\n", ptr->member_id);
        printf("書籍編號: %d\n", ptr->book_id);
        printf("====================\n");
        ptr = ptr->next;
    }
}

void LinkedList::check_member_record(member *mp){
    record *rp;

    rp = record_head;
    printf("會員已借 %d 本書籍\n", mp->num_borrow);
    while(rp!=NULL){
        if(rp->member_id == mp->id){
            printf("書籍編號: %d\n", rp->book_id);
            printf("借閱時間: ");
            for(int i=0;i<3;i++){
                if(i!=2){
                    printf("%d/", rp->time[i]);
                }else{
                    printf("%d", rp->time[i]);
                }
            }
            printf("==========\n");
        }
        rp = rp->next;
    }
}

book *LinkedList::find_book_byid(int book_id){
    book *ptr = book_head;

    while(ptr!=NULL){
        if(ptr->id==book_id){
            break;
        }
        ptr = ptr->next;
    }
    return ptr;
}

//操作
void LinkedList::member_interface(member *mp){
    char op;
    int log_out=0;

    while(1){
        printf("(1)借書\n");
        printf("(2)還書\n");
        printf("(3)查詢借閱狀況\n");
        printf("(Q)登出\n");
        scanf(" %c", &op);
        switch(op){
            case '1':
                insert_record(mp);
                break;
            case '2':
                delete_record(mp);
                break;
            case '3':
                check_member_record(mp);
                break;
            case 'Q':
            case 'q':
                log_out = 1;
                printf("已登出會員\n");
                break;
            default:
                printf("指令錯誤!\n");
                break;
        }
        if(log_out) break;
        system("pause");
        system("cls");
    }
}

void LinkedList::manager_interface(){
    char op;
    int log_out=0;

    while(1){
        printf("會員管理:\n");
        printf("(1)新增\t");
        printf("(2)刪除\t");
        printf("(3)顯示\n");

        printf("書籍管理:\n");
        printf("(4)新增\t");
        printf("(5)刪除\t");
        printf("(6)顯示\n");

        printf("借閱管理:\n");
        //printf("(7)新增\t");
        //printf("(8)刪除\t");
        printf("(9)顯示\n");

        printf("(Q)登出管理員\n");
        printf("指令: ");
        scanf(" %c", &op);
        switch(op){
            case '1':
                insert_member();
                break;
            case '2':
                delete_member();
                break;
            case '3':
                show_member();
                break;
            case '4':
                insert_book();
                break;
            case '5':
                delete_book();
                break;
            case '6':
                show_book();
                break;
            /*
            case '7':
                break;
            case '8':
                break;
            */
            case '9':
                show_record();
                break;
            case 'Q':
            case 'q':
                log_out = 1;
                printf("已登出會員\n");
                break;
            default:
                printf("指令錯誤!\n");
                break;
        }
        if(log_out) break;
        system("pause");
        system("cls");
    }
}

int main(){
    LinkedList Library;
    char op;

    Library.load_member();
    Library.load_book();
    Library.load_record();

    while(1){
        printf("<<圖書館借閱系統>>\n");
        printf("(0)查詢本館庫藏\n");
        printf("(1)註冊會員\n");
        printf("(2)會員登入\n");
        printf("(3)管理員登入\n");
        printf("(4)結束系統\n");
        printf("指令: ");
        scanf(" %c", &op);
        switch(op){

        case '0':
            Library.show_book();
            break;
        case '1':
            Library.insert_member();
            printf("\n註冊會員成功!\n");
            break;
        case '2':
            Library.login_member();
            break;
        case '3':
            Library.login_manager();
            break;
        case '4':
            Library.save_member();
            Library.save_book();
            Library.save_record();
            printf("已結束系統\n");
            return 0;
        default:
            printf("指令錯誤!\n");
            break;
        }
        system("pause");
        system("cls");
    }

    return 0;
}
