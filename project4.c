#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>

//منوی اصلی
void main_menu();//ok

//ورود کاربر 
void sign_in_menu_paint();//ok
void sign_in_menu_username();//ok
void sign_in_menu_password();//ok
int valid_username_password();//ok

void forget_password();//ok

void initialize_users();

void sign_up_menu_paint();//ok
void sign_up_menu_username();//ok
int validusername();//ok
void sign_up_menu_email();//ok
int validemail();//ok
void sign_up_menu_password();//ok
int validpassword();//ok

void random_password();//ok

void save_file();//ok
void save_user();//ok

void update_user_score();//ok
void scoreboard_print();//ok
int load_users();//ok
int compare_scores();//ok
void display_scoreboard();//ok
void display_user_info();//ok

void game_menu();//ok

void reset_game_state();//ok
void load_game_state();//no
void save_game_state();//no


int check_room_collision();//ok
void game_map();//ok
void game_map2();//ok

void connect_rooms();//ok
void connect_rooms_hidden();//ok

void generate_foods();//ok
void collect_foods();//ok
void eat_foods();//ok
void update_hunger();//ok
void show_hunger_bar();//ok

void generate_golds();//ok
void collect_golds();//ok
void show_golds();//ok

void generate_weapons();//ok
void collect_weapons();//ok
void show_weapons();//ok

void generate_spells();//ok
void collect_spells();//ok
void show_spells();//no

void generate_monsters();//ok
void move_monsters();//ok

void stair_loc();//ok

void hidden_door_print();//ok
void map_print();//ok
void trap_print();//ok
void heart_print();//ok
void character_print();//ok

int find_room();//ok
int near_room();//ok
void room_print();//ok

void character_loc();//ok
void regenerate_health();
void heart();//ok

int can_move();//ok
int move_player();//ok
void move_print();//ok
void win();
void message_print();//ok

void setting_menu();//ok
void game_music();//no

void exit_program();//ok

///////////////////////

//ورودی های ورود
char usernamel[100];
char passwordl[100];

//ورودی های ثبت نام
char username[100];
char password[100];
char email[100];

///////////////////////

//scoreboard
#define MAX_USERS 100

char user[100];

typedef struct {
    char username[100];
    int score;
    int games_played;
    time_t last_game_time;
} User;

User users[MAX_USERS];

/////////////////////////

#define WIDTH 120
#define HEIGHT 30
#define MAX_ROOMS 7
#define MIN_ROOMS 6
#define MIN_ROOM_SIZE 6
#define MAX_ROOM_SIZE 8
#define MAX_FOOD 2
#define MAX_GOLD 3
#define MAX_SPELLS 2
#define MAX_WEAPONS 2
#define MAX_MONSTER 1

#define MAX_INVENTORY 10 //حداکثر منو

// نقشه بازی
char map[HEIGHT][WIDTH];

// ساختار غذا
typedef struct {
    int x,y;  
    int type;  
    int health_boost;
} Food;

Food inventory[MAX_INVENTORY];

//ساختار طلا
typedef struct {
    int x,y;      
    int value;    
    int is_black; 
} Gold;

int total_gold = 0;

// ساختار طلسم
typedef struct {
    int x,y;     
    int type;        
} Spell;

Spell inventory_spells[MAX_INVENTORY];
int collected_spell_count = 0;

// ساختار سلاح
typedef struct {
    int x,y;       
    char type[20];
    int damage;
    int ranged;
    int range;
    int ammo;
} Weapon;


Weapon inventory_weapons[MAX_INVENTORY];
int collected_weapon_count = 0;  
int equipped_weapon = 0;  

//ساختار هیولا
typedef struct {
    int x,y;        
    int type;       
    int health;       
    int damage;      
    int chase_limit;
    int alive;     
} Monster;

Monster monsters[10];
int monster_count = 0;


typedef struct {
    int x,y;    
    int width,height;
    int x1,y1; 
    int x2,y2;
    int enter;
    Food foods[MAX_FOOD];
    Gold golds[MAX_GOLD];
    Spell spells[MAX_SPELLS];
    Weapon weapons[MAX_WEAPONS];
    Monster monsters[MAX_MONSTER]; 
    int food_count;
    int gold_count;
    int spell_count;
    int weapon_count;
    int monster_count;
} Room;

Room rooms[MAX_ROOMS];
int room_count_global = 0;

/////////////////////

int selected_difficulty = 0;
int selected_color = 0;
int hide_door1 = 1;
int hide_door2 = 1;
int pd1 = 1;
int pd2 = 1;
int character_color = 1;
int hunger_change = 1;
int health_change = 3;
int hunger = 0;
int health = 100;
int collected_food_count = 0;
int g_pressed = 1;
int f_pressed = 0;
int collect = 1;
int mapp = 1;
int stop = 0;
int ttrap_print = 1;
Food inventory[MAX_INVENTORY];

//////////////////////

//راهرو ها
#define VISION_RADIUS 1 
int seen_map[HEIGHT][WIDTH] = {0};

/////////////////////

int loc_x,loc_y;

//////////////////////

int main() {
    setlocale(LC_ALL, "");
    initscr(); //باز کردن ترمینال
    cbreak(); //بدون نیاز به enter
    noecho(); //عدم نمابش ورودی کیبورد
    keypad(stdscr, TRUE); //فعالسازی کیبورد
    curs_set(0);

    start_color(); //تنظیم رنگ ها
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_BLACK);
    init_pair(7, COLOR_CYAN, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);

    initialize_users();

    main_menu();
    endwin();
    return 0;
}

void initialize_users() {
    for (int i = 0; i < MAX_USERS; i++) {
        users[i].score = 0;
        users[i].games_played = 0;
        users[i].last_game_time = 0;
        strcpy(users[i].username, "");  
    }

    FILE *file = fopen("Scores.txt", "r");
    if (!file) {
        printf("⚠️ فایل `Scores.txt` یافت نشد، کاربران جدید مقداردهی اولیه شدند.\n");
        return;
    }

    int index = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) && index < MAX_USERS) {
        if (strncmp(line, "Username: ", 10) == 0) {
            sscanf(line, "Username: %s", users[index].username);

            fgets(line, sizeof(line), file);
            sscanf(line, "Score: %d", &users[index].score);

            fgets(line, sizeof(line), file);
            sscanf(line, "Games Played: %d", &users[index].games_played);

            fgets(line, sizeof(line), file);
            sscanf(line, "Last Game Time: %ld", &users[index].last_game_time);

            index++;
        }
    }

    fclose(file);
}

void sign_in_menu_paint() {
    clear();
    attron(A_BOLD);
    attron(COLOR_PAIR(5));
    mvprintw(0, 55, "🔑 Sign In Menu");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(5));

    sign_in_menu_username();
}

void sign_in_menu_username() {
    memset(usernamel, 0, sizeof(usernamel));

    mvprintw(5, 35, "Username: ");
    refresh();
    curs_set(1);
    echo();
    mvscanw(5, 45, "%s", usernamel);
    noecho();

    strcpy(user,usernamel);

    int length = strlen(usernamel);

    if (length == 0) {
        attron(COLOR_PAIR(1));
        mvprintw(11, 35, "⚠️ The username cannot be empty!");
        mvprintw(12, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));
        sign_in_menu_username();
    }

    if (strcmp(usernamel, "Back") == 0) {
        curs_set(0);
        main_menu();
    }

    sign_in_menu_password();
}

void sign_in_menu_password() {
    memset(passwordl, 0, sizeof(passwordl));

    mvprintw(7, 35, "Password: ");
    refresh();
    echo();
    mvscanw(7, 45, "%s", passwordl);
    curs_set(0);
    noecho();

    if (strcmp(passwordl, "Forget") == 0) {
        forget_password();
    } 
    
    else {
        int length = strlen(passwordl);
        if (length == 0) {
            attron(COLOR_PAIR(1));
            mvprintw(11, 35, "⚠️ The password cannot be empty!   ");
            mvprintw(12, 35, "Try again please!           ");
            refresh();
            attroff(COLOR_PAIR(1));
            sign_in_menu_password();
        } 
        
        else {
            if (!valid_username_password(usernamel, passwordl)) {
                attron(COLOR_PAIR(1));
                mvprintw(7, 45, "                                ");
                mvprintw(5, 45, "                                ");
                mvprintw(11, 35, "❌ Username and Password do not match!");
                mvprintw(12, 35, "Try again please!           ");
                refresh();
                attroff(COLOR_PAIR(1));
                sign_in_menu_username();
            }

            strcpy(user, usernamel);

            attron(COLOR_PAIR(2));
            mvprintw(11, 35, "✅ Password entered successfully!     ");
            mvprintw(12, 35, "Loading...                  ");
            refresh();
            attroff(COLOR_PAIR(2));

            memset(usernamel, 0, sizeof(usernamel));
            memset(passwordl, 0, sizeof(passwordl));

            napms(3000);
            game_menu();
        }
    }
}

int valid_username_password(const char *username, const char *password) {
    FILE *file = fopen("Data.txt", "r");
    if (!file) {
        return 0;
    }

    char usernameline[100];
    char passwordline[100];

    while (fgets(usernameline, sizeof(usernameline), file)) {
        if (strncmp(usernameline, "Username: ", 10) == 0) {
            char stored_username[100];
            sscanf(usernameline, "Username: %s", stored_username);

            fgets(passwordline, sizeof(passwordline), file);

            char stored_password[100];
            sscanf(passwordline, "Password: %s", stored_password);

            if (strcmp(stored_username, username) == 0 && strcmp(stored_password, password) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

void forget_password() {
    FILE *file = fopen("Data.txt", "r");

    int found = 0;
    char usernameline[100];
    char passwordline[100];
    char stored_username[100];
    char stored_password[100];

    while (fgets(usernameline, sizeof(usernameline), file)) {
        usernameline[strcspn(usernameline, "\n")] = '\0';

        if (strncmp(usernameline, "Username: ", 10) == 0) {
            sscanf(usernameline, "Username: %s", stored_username);

            if (strcmp(stored_username, usernamel) == 0) {
                fgets(passwordline, sizeof(passwordline), file);
                passwordline[strcspn(passwordline, "\n")] = '\0';

                sscanf(passwordline, "Password: %s", stored_password);
                mvprintw(11, 35, "Your password is: %s              ", stored_password);
                mvprintw(12,35,"Please enter username and password");
                refresh();
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        mvprintw(11, 35, "Username not found!\n");
        refresh();
    }
    memset(usernamel, 0, sizeof(usernamel));
    memset(passwordl, 0, sizeof(passwordl));
    fclose(file);
    napms(5000);
    sign_in_menu_paint();
}

///////////////////////

void sign_up_menu_paint() {
    clear();

    attron(A_BOLD);
    attron(COLOR_PAIR(5));
    mvprintw(0, 55, "Sign Up Menu");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(5));

    attron(COLOR_PAIR(5));
    mvprintw(18, 32, "attention:");
    mvprintw(20, 32, "1.");
    mvprintw(20, 75, "!");
    mvprintw(22, 32, "2.");
    mvprintw(22, 92, ",");
    mvprintw(23, 93, "!");
    attroff(COLOR_PAIR(5));

    attron(COLOR_PAIR(3));
    mvprintw(20, 34, "length of the password must be at least 7");
    mvprintw(22, 34, "The password must contain ate least one numberic character");
    mvprintw(23, 32, "one uppercase English letter and one lowercase English letter");
    attroff(COLOR_PAIR(3));

    mvprintw(17, 29, "____________________________________________________________________");
    mvprintw(24, 29, "____________________________________________________________________");

    for (int i = 0; i < 7; i++) {
        mvprintw(18 + i, 29, "|");
        mvprintw(24 - i, 96, "|");
    }

    sign_up_menu_username();
}

void sign_up_menu_username() {

    mvprintw(5, 35, "Username: ");
    refresh();
    echo();
    mvscanw(5, 45, "%s", username);
    noecho();

    strcpy(user,username);

    int length = strlen(username);

    if (length == 0) {

        attron(COLOR_PAIR(1));
        mvprintw(11, 35, "The username cannot be empty!");
        mvprintw(12, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));

        sign_up_menu_username();
    }


    if(strcmp(username, "Back") == 0){
        curs_set(0);
        main_menu();
    }

    if (validusername(username)){

        attron(COLOR_PAIR(1));
        mvprintw(5, 45, "                                            ");
        mvprintw(11, 35, "Username is already taken!    ");
        mvprintw(12, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));

        memset(username, 0, sizeof(username)); //null قرار دادن بافر

        sign_up_menu_username();
    }

    attron(COLOR_PAIR(2));
    mvprintw(11, 35, "Username entered successfully!");
    mvprintw(12, 35, "                        ");
    refresh();
    attroff(COLOR_PAIR(2));

    sign_up_menu_email();
}

int validusername(const char *username){

    FILE *file = fopen("Data.txt", "r");

    char usernameline[100];

    while (fgets(usernameline, sizeof(usernameline), file)){

        if (strncmp(usernameline, "Username: ", 10) == 0){

            char stored_username[100];                            
            sscanf(usernameline, "Username: %s", stored_username);
        
            if (strcmp(stored_username, username) == 0){
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;    
}

void sign_up_menu_email() {

    mvprintw(7, 35, "Email: ");
    refresh();
    echo();
    mvscanw(7, 42, "%s", email);
    noecho();

    int length = strlen(email);

    if (length == 0){

        attron(COLOR_PAIR(1));
        mvprintw(13, 35, "The email cannot be empty!");
        mvprintw(14, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));

        sign_up_menu_email();
    }

    if (!validemail(email)){

        attron(COLOR_PAIR(1));
        mvprintw(7, 42, "                                            ");
        mvprintw(13, 35, "Error Email!              ");
        mvprintw(14, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));

        memset(email, 0, sizeof(email));

        sign_up_menu_email();
    }

    attron(COLOR_PAIR(2));
    mvprintw(13, 35, "email entered successfully!");
    mvprintw(14, 35, "                      ");
    refresh();
    attroff(COLOR_PAIR(2));

    sign_up_menu_password();
}

int validemail(const char *email) {

    int at_count = 0; 
    int dot_count = 0; 
    int i;
    int at_pos = -1;

    if (strlen(email) < 5) {
        return 0;
    }

    for (i = 0; email[i] != '\0'; i++) {
        char c = email[i];

    
        if (!isalnum(c) && c != '@' && c != '.' && c != '-' && c != '_' && c != '+') {
            return 0;
        }

        if (c == '@') {
            at_count++;
            at_pos = i;
        }

        if (c == '.' && at_pos != -1 && i > at_pos) {
            dot_count++;
        }
    }

    if (at_count != 1) {
        return 0;
    }

    if (dot_count < 1) {
        return 0;
    }

    if (email[0] == '@' || email[strlen(email) - 1] == '@') {
        return 0;
    }

    if (email[at_pos + 1] == '.' || email[at_pos - 1] == '.') {
        return 0;
    }

    if (email[0] == '.') {
        return 0;
    }

    for (i = at_pos + 1; email[i] != '\0'; i++) {

        if (email[i] == '-' || email[i] == '_') {

            if (email[i + 1] == '.' || email[i + 1] == '@' || email[i + 1] == '\0') {
                
                return 0;
            }
        }
    }

    return 1;
}

void sign_up_menu_password() {

    random_password();

    mvprintw(9, 35, "Password: ");
    refresh();
    echo();                         
    mvscanw(9, 45, "%s", password);
    noecho();

    int length = strlen(password);

    if (length == 0){

        attron(COLOR_PAIR(1));
        mvprintw(15, 35, "The password cannot be empty!");
        mvprintw(16, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));

        sign_up_menu_password();
    }

    if (!validpassword(password)){

        attron(COLOR_PAIR(1));
        mvprintw(9, 45, "                                ");
        mvprintw(15, 35, "Error password!              ");
        mvprintw(16, 35, "Try again please!");
        refresh();
        attroff(COLOR_PAIR(1));

        memset(password, 0, sizeof(password));

        sign_up_menu_password();
    }

    attron(COLOR_PAIR(2));
    mvprintw(15, 35, "Password entered successfully!");
    mvprintw(16, 35, "Loading...           ");
    refresh();
    attroff(COLOR_PAIR(2));

    save_user(username, password, email);
}

int validpassword(const char *password) {

    int length_password = strlen(password);
    int capital_number = 0, small_number = 0, number_number = 0;

    for (int i = 0; i < length_password; i++) {

        char check = password[i];

        if (check >= '0' && check <= '9') {
            number_number++;
        }
        if (check >= 'a' && check <= 'z') {
            small_number++;
        }
        if (check >= 'A' && check <= 'Z') {
            capital_number++;
        }
    }

    if (length_password < 7) {
        return 0;
    }

    if (capital_number && small_number && number_number) {
        return 1;
    }

    return 0;
}

void random_password() {

    const char *characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$^&*()_-+=<>?";
    int number_characters = strlen(characters);

    srand(time(NULL));

    char randompassword[16];

    for (int i = 0; i < 15; i++){

        randompassword[i] = characters[rand() % number_characters];
    }

    randompassword[15] = '\0';

    attron(COLOR_PAIR(4));
    mvprintw(9, 80, "Random Password %s", randompassword);
    refresh();
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(2));

    mvprintw(9, 95, ":");
    refresh();
    attroff(COLOR_PAIR(2));

}

void save_user(char *username, char *password, char *email) {

    // ذخیره اطلاعات برای ورود
    FILE *file = fopen("Data.txt", "a");
    if (file) {
        fprintf(file, "Username: %s\n", username);
        fprintf(file, "Password: %s\n", password);
        fprintf(file, "Email: %s\n\n", email);
        fclose(file);
    }

    // ذخیره اطلاعات برای امتیاز
    FILE *scoreFile = fopen("Scores.txt", "a");
    if (scoreFile) {
        fprintf(scoreFile, "Username: %s\n", username);
        fprintf(scoreFile, "Score: %d\n", 0);
        fprintf(scoreFile, "Games Played: %d\n", 0);
        fprintf(scoreFile, "Last Game Time: %ld\n\n", 0L);
        fclose(scoreFile);
    }

    memset(username, 0, sizeof(username));
    memset(email, 0, sizeof(email));
    memset(password, 0, sizeof(password));

    napms(3000);
    game_menu();
}

void update_user_score(const char *username, int new_score) {

    FILE *file = fopen("Scores.txt", "r");
    if (!file) return;

    FILE *temp = fopen("TempScores.txt", "w");
    if (!temp) {
        fclose(file);
        return;
    }

    char line[100], stored_username[100];
    int stored_score, stored_games_played;
    time_t stored_last_game_time;

    while (fgets(line, sizeof(line), file)) {

        if (strncmp(line, "Username: ", 10) == 0) {

            sscanf(line, "Username: %s", stored_username);
            fprintf(temp, "%s", line);

            fgets(line, sizeof(line), file);
            sscanf(line, "Score: %d", &stored_score);

            fgets(line, sizeof(line), file);
            sscanf(line, "Games Played: %d", &stored_games_played);

            fgets(line, sizeof(line), file);
            sscanf(line, "Last Game Time: %ld", &stored_last_game_time);

            if (strcmp(stored_username, username) == 0) {
                stored_score += new_score;
                stored_games_played++;
                stored_last_game_time = time(NULL);

                fprintf(temp, "Score: %d\n", stored_score);
                fprintf(temp, "Games Played: %d\n", stored_games_played);
                fprintf(temp, "Last Game Time: %ld\n\n", stored_last_game_time);
            } 
            
            else {
                fprintf(temp, "Score: %d\n", stored_score);
                fprintf(temp, "Games Played: %d\n", stored_games_played);
                fprintf(temp, "Last Game Time: %ld\n\n", stored_last_game_time);
            }
        } 
        
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);
    remove("Scores.txt");
    rename("TempScores.txt", "Scores.txt");
}

///////////////////////

void scoreboard_print() {
    clear();

    int count = load_users(users);

    if (count == 0) {
        attron(COLOR_PAIR(1));
        mvprintw(5, 50, "🚨 No scores available yet!");
        attroff(COLOR_PAIR(1));
        getch();

        game_menu();
        return;
    }

    qsort(users, count, sizeof(User), compare_scores);

    const char *choices[] = {"🏆 View All Scores", "⭐ View My Score", "🔙 Back to Menu"};
    int num_choices = 3;
    int current_choice = 0;

    while (1) {
        clear();

        attron(COLOR_PAIR(5));
        attron(A_BOLD);
        mvprintw(0, 40, "════════════════════════════════════════════════");
        mvprintw(1, 40, "              🏆 GAME SCOREBOARD 🏆              ");
        mvprintw(2, 40, "════════════════════════════════════════════════");
        attroff(COLOR_PAIR(5));
        attroff(A_BOLD);

        for (int i = 0; i < num_choices; i++) {
            if (i == current_choice) {
                attron(A_REVERSE);
            }

            mvprintw(i + 4, 55, "%s", choices[i]);
                attroff(A_REVERSE);
        }

        int ch = getch();

        if (ch == KEY_UP) {

            if (current_choice == 0) {

                current_choice = num_choices - 1;
            }

            else {

                current_choice = current_choice - 1;
            }
        }

        else if (ch == KEY_DOWN) {

            if (current_choice == num_choices - 1) {

                current_choice = 0;
            }

            else {
                current_choice = current_choice + 1;
            }
        }

        else if (ch == 10) {

            if (current_choice == 0) {

                display_scoreboard(users, count);
            }

            if (current_choice == 1) {
                
                display_user_info(users, count, user);
            }

            if (current_choice == 2) {

                game_menu();
            }
        }
    }
}

int load_users(User users[MAX_USERS]) {

    FILE *file = fopen("Scores.txt", "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), file) && count < MAX_USERS) {

        if (strncmp(line, "Username: ", 10) == 0) {
            sscanf(line, "Username: %s", users[count].username);

            fgets(line, sizeof(line), file);
            sscanf(line, "Score: %d", &users[count].score);

            fgets(line, sizeof(line), file);
            sscanf(line, "Games Played: %d", &users[count].games_played);

            fgets(line, sizeof(line), file);
            sscanf(line, "Last Game Time: %ld", &users[count].last_game_time);

            count++;
        }
    }

    fclose(file);
    return count;
}

int compare_scores(const void *a, const void *b) {
    User *userA = (User *)a;
    User *userB = (User *)b;
    return userB->score - userA->score;
}

#define PLAYERS_PER_PAGE 3

void display_scoreboard(User users[MAX_USERS], int user_count, const char *current_username) {
      const char *title = "";
    int page = 0; 
    int total_pages = (user_count + PLAYERS_PER_PAGE - 1) / PLAYERS_PER_PAGE;

    while (1) {
        clear();

        attron(A_BOLD);
        attron(COLOR_PAIR(5));
        mvprintw(0, 53, "🏅 Top Players 🏅");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(5));

        int start = page * PLAYERS_PER_PAGE;
        int end = start + PLAYERS_PER_PAGE;
        if (end > user_count) end = user_count;

        for (int i = start; i < end; i++) {
            
            const char *medal = "";
            if (i == 0) { 
                medal = "🥇"; attron(COLOR_PAIR(2));
                title = "Champion";
            }
            else if (i == 1) { 
                medal = "🥈"; attron(COLOR_PAIR(3));
                title = "Legend";
            }
            else if (i == 2) { 
                medal = "🥉"; attron(COLOR_PAIR(4)); 
                title = "Pro";
            }
            else {
                attron(COLOR_PAIR(8));
            }

            // چک کردن اینکه این سطر متعلق به کاربر فعلی است یا نه
            if (strcmp(users[i].username, user) == 0) {
                attron(A_REVERSE | COLOR_PAIR(4)); // استفاده از رنگ مخصوص کاربر فعلی
            }

            mvprintw(i - start + 2, 25, "%s Rank %d: %-10s | Score: %4d | Games: %2d | Last Played: %s",
                     medal, i + 1, users[i].username, users[i].score, users[i].games_played, ctime(&users[i].last_game_time));

            mvprintw(i-start + 2, 15,"%s",title);
            // خاموش کردن افکت‌ها
            attroff(A_REVERSE);
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(8));
        }

        attron(COLOR_PAIR(5));
        mvprintw(HEIGHT - 4, 56, "Page %d of %d", page + 1, total_pages);
        attroff(COLOR_PAIR(5));
        refresh();

        int ch = getch();

        if (ch == KEY_RIGHT) {
            if (page < total_pages - 1) page++;
        } 
        
        else if (ch == KEY_LEFT) {
            if (page > 0) page--;
        } 
        
        else if (ch == 'q' || ch == 'Q') {
            break;
        }
    }
}


void display_user_info(User users[MAX_USERS], int user_count, const char *current_username) {
    clear();

    int found = 0;

    for (int i = 0; i < user_count; i++) {

        if (strcmp(users[i].username, current_username) == 0) {

            attron(A_BOLD);
            attron(COLOR_PAIR(6));
            mvprintw(3, 50, "💡 Your Profile:");
            attroff(A_BOLD);
            attroff(COLOR_PAIR(6));

            attron(COLOR_PAIR(3));
            mvprintw(5, 50, "⭐ Your Rank: %d", i + 1);
            mvprintw(6, 50, "🆔 Username: %s", users[i].username);
            mvprintw(7, 50, "🏆 Score: %d", users[i].score);
            mvprintw(8, 50, "🎮 Games Played: %d", users[i].games_played);
            mvprintw(9, 50, "⏳ Last Game: %s", ctime(&users[i].last_game_time));
            attroff(COLOR_PAIR(3));
            found = 1;
            break;
        }
    }

    if (!found) {

        attron(COLOR_PAIR(1));
        mvprintw(5, 50, "❌ User not found in scoreboard!");
        attroff(COLOR_PAIR(1));
    }

    mvprintw(12, 50, "⚡ Press any key to return...");
    refresh();
    getch();
}

//////////////////////

void game_menu() {
    curs_set(0);
    attron(A_BOLD);

    const char *choices[] = {"New Game", "Resume Game", "Scoreboard", "Setting", "Back"};
    int num_choices = 5;
    int current_choices = 0;

    while (1){

        clear();

        for (int i = 0; i < num_choices; i++) {

            if (i == current_choices) {

                attron(COLOR_PAIR(6));
                attron(A_REVERSE);
            }

            else {

                if (i == 0){
                    attron(COLOR_PAIR(2));
                }
                else if (i == 1){
                    attron(COLOR_PAIR(3));
                }
                else if (i == 2){
                    attron(COLOR_PAIR(4));
                }
                else if (i == 3){
                    attron(COLOR_PAIR(7));
                }
                else if (i == 4){
                    attron(COLOR_PAIR(1));
                }

            }

            mvprintw(i + 10, 55, "%s", choices[i]);

            attroff(A_REVERSE);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(6));
            attroff(COLOR_PAIR(7));
        }

        int ch = getch();

        if (ch == KEY_UP){

            if (current_choices == 0) {

                current_choices = num_choices - 1;
            }

            else {

                current_choices = current_choices - 1;
            }
        }

        else if (ch == KEY_DOWN){

            if (current_choices == num_choices - 1) {

                current_choices = 0;
            }

            else {
                current_choices = current_choices + 1;
            }
        }

        else if (ch == 10){

            if (current_choices == 0){
                reset_game_state();
                move_print();
                //save_game_state();
            }

            if (current_choices == 1){
                load_game_state();
                move_print();
            }

            if (current_choices == 2){
                scoreboard_print();
            }

            if (current_choices == 3){
                setting_menu();
            }

            if (current_choices == 4){
                main_menu();
            }
        }

        refresh();
    }
    curs_set(0);
}

void setting_menu() {
    attron(A_BOLD);

    const char *choices1[] = {"Easy", "Meduim", "Hard"};
    int num_choices1 = 3;
    int current_choices1 = 0;

    const char *choices2[] = {"Green", "Magenta", "Blue"};
    int num_choices2 = 3;
    int current_choices2 = 0;

    const char *choices3[] = {"Music1", "Music2", "Music3" , "None"};
    int num_choices3 = 4;
    int current_choices3 = 0;

    int stop = 1;
    int stop1 = 0;
    while (stop) {
    clear();

    attron(COLOR_PAIR(7));
    mvprintw(0,58,"setting");
    mvprintw(8, 55, "Difficulty Level");
    mvprintw(14, 55, "Character's color");
    mvprintw(20, 55, "Music");
    attroff(COLOR_PAIR(7));
    attron(COLOR_PAIR(5));
    mvprintw(8, 71, ":");
    mvprintw(14, 72, ":");
    mvprintw(20, 60, ":");
    attroff(COLOR_PAIR(5));


        if(stop1 == 1){
            attron(COLOR_PAIR(2));
            mvprintw(8,73,"Easy");
            attroff(COLOR_PAIR(2));
        }

        if(stop1 == 2){
            attron(COLOR_PAIR(4));
            mvprintw(8,73,"Meduim");
            attroff(COLOR_PAIR(4));
        }

        if(stop1 == 3){
            attron(COLOR_PAIR(1));
            mvprintw(8,73,"Hard");
            attroff(COLOR_PAIR(1));
        }

        for (int i = 0; i < num_choices1; i++){

            if (i == current_choices1){

                attron(COLOR_PAIR(6));
                attron(A_REVERSE);
            }

            else{

                if (i == 0){
                    attron(COLOR_PAIR(2));
                }
                else if (i == 1){
                    attron(COLOR_PAIR(4));
                }
                else if (i == 2){
                    attron(COLOR_PAIR(1));
                }
            }

            mvprintw(i + 9, 55, "%s", choices1[i]);

            attroff(A_REVERSE);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(6));
        }

        int ch = getch();

        if (ch == KEY_UP){

            if (current_choices1 == 0){

                current_choices1 = num_choices1 - 1;
            }

            else{

                current_choices1 = current_choices1 - 1;
            }
        }

        else if (ch == KEY_DOWN){

            if (current_choices1 == num_choices1 - 1){

                current_choices1 = 0;
            }

            else{
                current_choices1 = current_choices1 + 1;
            }
        }

        else if (ch == 10){

            if (current_choices1 == 0){
                selected_difficulty = 0;
                stop1 = 1;
                stop = 0;
            }

            if (current_choices1 == 1){
                selected_difficulty = 1;
                stop1 = 2;
                stop = 0;
            }

            if (current_choices1 == 2){
                selected_difficulty = 2;
                stop1 = 3;
                stop = 0;
            }
        }

        refresh();
    }

    stop = 1;
    int stop2 = 0;
    while (stop) {
    clear();

    attron(COLOR_PAIR(7));
    mvprintw(0,58,"setting");
    mvprintw(8, 55, "Difficulty Level");
    mvprintw(14, 55, "Character's color");
    mvprintw(20, 55, "Music");
    attroff(COLOR_PAIR(7));
    attron(COLOR_PAIR(5));
    mvprintw(8, 71, ":");
    mvprintw(14, 72, ":");
    mvprintw(20, 60, ":");
    attroff(COLOR_PAIR(5));

        if(stop1 == 1){
            attron(COLOR_PAIR(2));
            mvprintw(8,73,"Easy");
            attroff(COLOR_PAIR(2));
        }

        if(stop1 == 2){
            attron(COLOR_PAIR(4));
            mvprintw(8,73,"Meduim");
            attroff(COLOR_PAIR(4));
        }

        if(stop1 == 3){
            attron(COLOR_PAIR(1));
            mvprintw(8,73,"Hard");
            attroff(COLOR_PAIR(1));
        }

        if(stop2 == 1){
            attron(COLOR_PAIR(2));
            mvprintw(14,74,"Green");
            attroff(COLOR_PAIR(2));
        }

        if(stop2 == 2){
            attron(COLOR_PAIR(5));
            mvprintw(14,74,"Magenta");
            attroff(COLOR_PAIR(5));
        }

        if(stop2 == 3){
            attron(COLOR_PAIR(3));
            mvprintw(14,74,"Blue");
            attroff(COLOR_PAIR(3));
        }

        for (int i = 0; i < num_choices2; i++){

            if (i == current_choices2){

                attron(COLOR_PAIR(6));
                attron(A_REVERSE);
            }

            else{

                if (i == 0){
                    attron(COLOR_PAIR(2));
                }
                else if (i == 1){
                    attron(COLOR_PAIR(5));
                }
                else if (i == 2){
                    attron(COLOR_PAIR(3));
                }
            }

            mvprintw(i + 15, 55, "%s", choices2[i]);

            attroff(A_REVERSE);
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(6));
        }

        int ch = getch();

        if (ch == KEY_UP){

            if (current_choices2 == 0){

                current_choices2 = num_choices2 - 1;
            }

            else{

                current_choices2 = current_choices2 - 1;
            }
        }

        else if (ch == KEY_DOWN){

            if (current_choices2 == num_choices2 - 1){

                current_choices2 = 0;
            }

            else{
                current_choices2 = current_choices2 + 1;
            }
        }

        else if (ch == 10){

            if (current_choices2 == 0){
                selected_color = 0;
                stop2 = 1;
                stop = 0;
            }

            if (current_choices2 == 1){
                selected_color = 1;
                stop2 = 2;
                stop = 0;
            }

            if (current_choices2 == 2){
                selected_color = 2;
                stop2 = 3;
                stop = 0;
            }
        }

        refresh();
    }

    stop = 1;
    int stop3 = 0;
    while (stop) {
    clear();

    attron(COLOR_PAIR(7));
    mvprintw(0,58,"setting");
    mvprintw(8, 55, "Difficulty Level");
    mvprintw(14, 55, "Character's color");
    mvprintw(20, 55, "Music");
    attroff(COLOR_PAIR(7));
    attron(COLOR_PAIR(5));
    mvprintw(8, 71, ":");
    mvprintw(14, 72, ":");
    mvprintw(20, 60, ":");
    attroff(COLOR_PAIR(5));

        if(stop1 == 1){
            attron(COLOR_PAIR(2));
            mvprintw(8,73,"Easy");
            attroff(COLOR_PAIR(2));
        }

        if(stop1 == 2){
            attron(COLOR_PAIR(4));
            mvprintw(8,73,"Meduim");
            attroff(COLOR_PAIR(4));
        }

        if(stop1 == 3){
            attron(COLOR_PAIR(1));
            mvprintw(8,73,"Hard");
            attroff(COLOR_PAIR(1));
        }

        if(stop2 == 1){
            attron(COLOR_PAIR(2));
            mvprintw(14,74,"Green");
            attroff(COLOR_PAIR(2));
        }

        if(stop2 == 2){
            attron(COLOR_PAIR(5));
            mvprintw(14,74,"Magenta");
            attroff(COLOR_PAIR(5));
        }

        if(stop2 == 3){
            attron(COLOR_PAIR(3));
            mvprintw(14,74,"blue");
            attroff(COLOR_PAIR(3));
        }

        for (int i = 0; i < num_choices3; i++){

            if (i == current_choices3){

                attron(COLOR_PAIR(6));
                attron(A_REVERSE);
            }

            else{

                if (i == 0){
                    attron(COLOR_PAIR(3));
                }
                else if (i == 1){
                    attron(COLOR_PAIR(5));
                }
                else if (i == 2){
                    attron(COLOR_PAIR(1));
                }
                else if (i == 3){
                    attron(COLOR_PAIR(4));
                }
            }

            mvprintw(i + 21, 55, "%s", choices3[i]);

            attroff(A_REVERSE);
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(6));
            attroff(COLOR_PAIR(4));
        }

        int ch = getch();

        if (ch == KEY_UP){

            if (current_choices3 == 0){

                current_choices3 = num_choices3 - 1;
            }

            else{

                current_choices3 = current_choices3 - 1;
            }
        }

        else if (ch == KEY_DOWN){

            if (current_choices3 == num_choices3 - 1){

                current_choices3 = 0;
            }

            else{
                current_choices3 = current_choices3 + 1;
            }
        }

        else if (ch == 10){

            if (current_choices3 == 0){
                game_music(1);
                stop3 = 1;
                stop = 0;
            }

            if (current_choices3 == 1){
                game_music(2);
                stop3 = 2;
                stop = 0;
            }

            if (current_choices3 == 2){
                game_music(3);
                stop3 = 3;
                stop = 0;
            }
            if (current_choices3 == 3){
                game_music(0);
                stop3 = 4;
                stop = 0;
            }
        }

        if(stop3 == 1){
            attron(COLOR_PAIR(3));
            mvprintw(20,62,"Music1");
            attroff(COLOR_PAIR(3));
        }

        if(stop3 == 2){
            attron(COLOR_PAIR(5));
            mvprintw(20,62,"Music2");
            attroff(COLOR_PAIR(5));
        }

        if(stop3 == 3){
            attron(COLOR_PAIR(1));
            mvprintw(20,62,"Music3");
            attroff(COLOR_PAIR(1));
        }

        if(stop3 == 4){
            attron(COLOR_PAIR(4));
            mvprintw(20,62,"None");
            attroff(COLOR_PAIR(4));
        }

        refresh();
    }
    napms(2000);
    curs_set(1);
    curs_set(0);
    game_menu();
}

void game_music(int a) {

    system("pkill mpg123");
    if (a == 0) {
        curs_set(0);
    }

    if (a == 1) {
        system("mpg123 -q song1.mp3 &");
       curs_set(0);
    }

    if (a == 2) {
        system("mpg123 -q song2.mp3 &");
        curs_set(0);
    }

    if (a == 3) {
        system("mpg123 -q song3.mp3 &");
        curs_set(0);
    }

}

/////////////////////

void save_game_state() {
    char filename[100];
    
    if (strcmp(user, "guest") == 0) {
        strcpy(filename, "save_guest.dat");
    } 
    
    else {
        snprintf(filename, sizeof(filename), "save_%.90s.dat", user);
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        return;
    }

    fwrite(&mapp, sizeof(int), 1, file);
    fwrite(&hide_door1, sizeof(int), 1, file);
    fwrite(&hide_door2, sizeof(int), 1, file);
    fwrite(&pd1, sizeof(int), 1, file);
    fwrite(&pd2, sizeof(int), 1, file);
    fwrite(&character_color, sizeof(int), 1, file);
    fwrite(&hunger_change, sizeof(int), 1, file);
    fwrite(&health_change, sizeof(int), 1, file);
    fwrite(&hunger, sizeof(int), 1, file);
    fwrite(&health, sizeof(int), 1, file);
    fwrite(&total_gold, sizeof(int), 1, file);
    fwrite(&collected_food_count, sizeof(int), 1, file);
    fwrite(&collected_weapon_count, sizeof(int), 1, file);
    fwrite(&equipped_weapon, sizeof(int), 1, file);
    fwrite(&loc_x, sizeof(int), 1, file);
    fwrite(&loc_y, sizeof(int), 1, file);
    fwrite(map, sizeof(map), 1, file);
    fwrite(seen_map, sizeof(seen_map), 1, file);
    fwrite(&collect, sizeof(int), 1, file);

    fclose(file);
}


int is_loading_game = 0;

void load_game_state() {
    char filename[100];

    if (strcmp(user, "guest") == 0) {
        strcpy(filename, "save_guest.dat");
    } 
    
    else {
        snprintf(filename, sizeof(filename), "save_%.90s.dat", user);
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        return;
    }

    fread(&mapp, sizeof(int), 1, file);
    fread(&hide_door1, sizeof(int), 1, file);
    fread(&hide_door2, sizeof(int), 1, file);
    fread(&pd1, sizeof(int), 1, file);
    fread(&pd2, sizeof(int), 1, file);
    fread(&character_color, sizeof(int), 1, file);
    fread(&hunger_change, sizeof(int), 1, file);
    fread(&health_change, sizeof(int), 1, file);
    fread(&hunger, sizeof(int), 1, file);
    fread(&health, sizeof(int), 1, file);
    fread(&total_gold, sizeof(int), 1, file);
    fread(&collected_food_count, sizeof(int), 1, file);
    fread(&collected_weapon_count, sizeof(int), 1, file);
    fread(&equipped_weapon, sizeof(int), 1, file);
    fread(&loc_x, sizeof(int), 1, file);
    fread(&loc_y, sizeof(int), 1, file);
    fread(map, sizeof(map), 1, file);
    fread(seen_map, sizeof(seen_map), 1, file);
    fread(&collect, sizeof(int), 1, file);

    fclose(file);

    is_loading_game = 1;
}

void reset_game_state() {
    hide_door1 = 1;
    hide_door2 = 1;
    pd1 = 1;
    pd2 = 1;
    hunger_change = 1;
    health_change = 3;
    hunger = 0;
    health = 100;
    collected_food_count = 0;
    collected_weapon_count = 0;
    collected_spell_count = 0;
    equipped_weapon = -1;
    total_gold = 0;

    f_pressed = 0;
    g_pressed = 1;
    stop = 0;
    ttrap_print = 1;
    collect = 1;
    mapp = 1;

    memset(inventory, 0, sizeof(inventory));
    memset(inventory_weapons, 0, sizeof(inventory_weapons));
    memset(inventory_spells, 0, sizeof(inventory_spells));

    if (selected_difficulty == 0) {
        hunger_change = 1;
        health_change = 3;
    }
    if (selected_difficulty == 1) {
        hunger_change = 3;
        health_change = 4;
    }
    if (selected_difficulty == 2) {
        hunger_change = 5;
        health_change = 5;
    }

    if (selected_color == 0) {
        character_color = 1;
    }
    if (selected_color == 1) {
        character_color = 2;
    }
    if (selected_color == 2) {
        character_color = 3;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            map[y][x] = ' ';
            seen_map[y][x] = 0;
        }
    }

    for (int i = 0; i < 10; i++) {
        monsters[i] = (Monster){0, 0, 0, 0, 0, 0, 0};
    }
    monster_count = 0;

    game_map();
}

//////////////////////

int check_room_collision(Room new_room) {

    for (int y = new_room.y - 3; y < new_room.y + new_room.height + 3; y++) {
        for (int x = new_room.x - 5; x < new_room.x + new_room.width + 5; x++) {

            if (map[y][x] != ' ') {
                return 1;
            }
        }
    }
    return 0; 
}

void game_map() {

    srand(time(NULL));

    // طراحی نقشه
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
                map[y][x] = ' ';
            
        }
    }

    int room_count = 0;
    int target_room_count = rand() % (MAX_ROOMS - MIN_ROOMS + 1) + MIN_ROOMS; //تعداد اتاق

    while (room_count < target_room_count) {

        Room new_room;
        new_room.width = rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 5) + MIN_ROOM_SIZE;  // عرض
        new_room.height = rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1) + MIN_ROOM_SIZE; // ارتفاع
        new_room.x = rand() % (WIDTH - new_room.width - 7) + 4; // موقعیت x 
        new_room.y = rand() % (HEIGHT - new_room.height - 2) + 6; // موقعیت y 
        new_room.enter = 1;

        // بررسی تداخل
        if (!check_room_collision(new_room)) {
        
            for (int y = new_room.y; y < new_room.y + new_room.height; y++) {
                for (int x = new_room.x; x < new_room.x + new_room.width; x++) {
                    map[y][x] = '.';  // کف اتاق
                }
            }
            
            for (int y = new_room.y - 1; y <= new_room.y + new_room.height; y++) {

                map[y][new_room.x - 1] = '*';  // دیوار چپ
                map[y][new_room.x + new_room.width] = '*';  // دیوار راست

            }

            for (int x = new_room.x - 1; x <= new_room.x + new_room.width; x++) {

                map[new_room.y - 1][x] = '*';  // دیوار بالا
                map[new_room.y + new_room.height][x] = '*';  // دیوار پایین

            }

            for (int i = 0; i < rand() % 2; i++){
                int y = rand() %  (new_room.height) + new_room.y;
                int x = (rand() %  2) * (new_room.width + 1)  + new_room.x - 1;
                map[y][x] = '='; //پنجره
            }

            for (int i = 0; i < rand() % 2 + 1; i++) {
                int y = rand() % (new_room.height - 2) + new_room.y + 1;
                int x = rand() % (new_room.width - 2) + new_room.x + 1;
                map[y][x] = 'O'; //ستون
            }

            for (int i = 0; i < rand() % 3 + 1 ; i++) {
                int y = rand() % (new_room.height - 2) + new_room.y + 1;
                int x = rand() % (new_room.width - 2) + new_room.x + 1;
                map[y][x] = 'T'; //تله
            }

            rooms[room_count] = new_room;
            room_count++;
        }
    }

    //اتاق طلسم
    for (int y = rooms[1].y - 1; y <= rooms[1].y + rooms[1].height; y++) {

        map[y][rooms[1].x - 1] = '3';  // دیوار چپ
        map[y][rooms[1].x + rooms[1].width] = '3';  // دیوار راست

    }

    for (int x = rooms[1].x - 1; x <= rooms[1].x + rooms[1].width; x++) {

        map[rooms[1].y - 1][x] = '3';  // دیوار بالا
        map[rooms[1].y + rooms[1].height][x] = '3';  // دیوار پایین

    }

    //اتاق معمولی

    for (int i = 2; i < room_count ; i++){

        int y = rand() %  (rooms[i].height) + rooms[i].y;
        int x = rooms[i].width + rooms[i].x ;
        map[y][x] = '+'; //در عمودی

        rooms[i].x1 = x;
        rooms[i].y1 = y;

    }

    //اتاق طلسم

        int g = rooms[1].y - 1;
        int h = rand() % (rooms[1].width)  + rooms[1].x;
        map[g][h] = '-'; // در افقی

        rooms[1].x2 = h;
        rooms[1].y2 = g;
    

    // اتاق در مخفی
    
        int c = rooms[0].y - 1;
        int d = rand() % (rooms[0].width) + rooms[0].x;

        map[c][d] = '+';

        rooms[0].x2 = d;
        rooms[0].y2 = c;

        int a = rand() % (rooms[0].height) + rooms[0].y;
        int b = rooms[0].width + rooms[0].x;

        map[a][b] = '1';
        
        rooms[0].x1 = b;
        rooms[0].y1 = a;

    room_count_global = room_count;

    generate_foods(room_count);
    generate_golds(room_count);
    generate_spells(room_count);
    generate_weapons(room_count);
    generate_monsters(room_count);
    character_loc(room_count);
    stair_loc(room_count);

    if (room_count == 6){
    connect_rooms(&rooms[2],&rooms[0]);
    connect_rooms(&rooms[3],&rooms[0]);
    connect_rooms(&rooms[4],&rooms[0]);
    connect_rooms(&rooms[5],&rooms[0]);
    }

    if (room_count == 7){
    connect_rooms(&rooms[2],&rooms[0]);
    connect_rooms(&rooms[3],&rooms[0]);
    connect_rooms(&rooms[4],&rooms[0]);
    connect_rooms(&rooms[5],&rooms[0]);
    connect_rooms(&rooms[6],&rooms[0]);
    }
}

void game_map2() {
    
    srand(time(NULL));

    // طراحی نقشه
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
                map[y][x] = ' ';
            
        }
    }

    int room_count = 0;
    int target_room_count = rand() % (MAX_ROOMS - MIN_ROOMS + 1) + MIN_ROOMS; //تعداد اتاق

    while (room_count < target_room_count) {

        Room new_room;
        new_room.width = rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 5) + MIN_ROOM_SIZE;  // عرض
        new_room.height = rand() % (MAX_ROOM_SIZE - MIN_ROOM_SIZE + 1) + MIN_ROOM_SIZE; // ارتفاع
        new_room.x = rand() % (WIDTH - new_room.width - 7) + 4; // موقعیت x 
        new_room.y = rand() % (HEIGHT - new_room.height - 2) + 6; // موقعیت y 
        new_room.enter = 1;

        // بررسی تداخل
        if (!check_room_collision(new_room)) {
        
            for (int y = new_room.y; y < new_room.y + new_room.height; y++) {
                for (int x = new_room.x; x < new_room.x + new_room.width; x++) {
                    map[y][x] = '.';  // کف اتاق
                }
            }
            
            for (int y = new_room.y - 1; y <= new_room.y + new_room.height; y++) {

                map[y][new_room.x - 1] = '*';  // دیوار چپ
                map[y][new_room.x + new_room.width] = '*';  // دیوار راست

            }

            for (int x = new_room.x - 1; x <= new_room.x + new_room.width; x++) {

                map[new_room.y - 1][x] = '*';  // دیوار بالا
                map[new_room.y + new_room.height][x] = '*';  // دیوار پایین

            }

            for (int i = 0; i < rand() % 2; i++){
                int y = rand() %  (new_room.height) + new_room.y;
                int x = (rand() %  2) * (new_room.width + 1)  + new_room.x - 1;
                map[y][x] = '='; //پنجره
            }

            for (int i = 0; i < rand() % 2 + 1; i++) {
                int y = rand() % (new_room.height - 2) + new_room.y + 1;
                int x = rand() % (new_room.width - 2) + new_room.x + 1;
                map[y][x] = 'O'; //ستون
            }

            for (int i = 0; i < rand() % 3 + 1 ; i++) {
                int y = rand() % (new_room.height - 2) + new_room.y + 1;
                int x = rand() % (new_room.width - 2) + new_room.x + 1;
                map[y][x] = 'T'; //تله
            }

            rooms[room_count] = new_room;
            room_count++;
        }
    }

    //اتاق طلسم
    for (int y = rooms[1].y - 1; y <= rooms[1].y + rooms[1].height; y++) {

        map[y][rooms[1].x - 1] = '4';  // دیوار چپ
        map[y][rooms[1].x + rooms[1].width] = '4';  // دیوار راست

    }

    for (int x = rooms[1].x - 1; x <= rooms[1].x + rooms[1].width; x++) {

        map[rooms[1].y - 1][x] = '4';  // دیوار بالا
        map[rooms[1].y + rooms[1].height][x] = '4';  // دیوار پایین

    }

    //اتاق معمولی

    for (int i = 2; i < room_count ; i++){

        int y = rand() %  (rooms[i].height) + rooms[i].y;
        int x = rooms[i].width + rooms[i].x ;
        map[y][x] = '+'; //در عمودی

        rooms[i].x1 = x;
        rooms[i].y1 = y;

    }

        int y = rand() %  (rooms[1].height) + rooms[1].y;
        int x = rooms[1].width + rooms[1].x ;
        map[y][x] = 'W'; //در عمودی

        rooms[1].x1 = x;
        rooms[1].y1 = y;

    // اتاق در مخفی
    
        int c = rooms[0].y - 1;
        int d = rand() % (rooms[0].width) + rooms[0].x;

        map[c][d] = '+';

        rooms[0].x2 = d;
        rooms[0].y2 = c;

    room_count_global = room_count;

    generate_foods(room_count);
    generate_golds(room_count);
    generate_weapons(room_count);
    generate_monsters(room_count);
    character_loc(room_count);

    if (room_count == 6){
    connect_rooms(&rooms[1],&rooms[0]);
    connect_rooms(&rooms[2],&rooms[0]);
    connect_rooms(&rooms[3],&rooms[0]);
    connect_rooms(&rooms[4],&rooms[0]);
    connect_rooms(&rooms[5],&rooms[0]);
    }

    if (room_count == 7){
    connect_rooms(&rooms[1],&rooms[0]);
    connect_rooms(&rooms[2],&rooms[0]);
    connect_rooms(&rooms[3],&rooms[0]);
    connect_rooms(&rooms[4],&rooms[0]);
    connect_rooms(&rooms[5],&rooms[0]);
    connect_rooms(&rooms[6],&rooms[0]);
    }
}

/////////////////////

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point queue[WIDTH * HEIGHT];
    int front, rear;
} Queue;

void initQueue(Queue *q) {
    q->front = q->rear = 0;
}

void enqueue(Queue *q, int x, int y) {
    q->queue[q->rear].x = x;
    q->queue[q->rear].y = y;
    q->rear++;
}

Point dequeue(Queue *q) {
    return q->queue[q->front++];
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

void connect_rooms(Room *room1, Room *room2) {

    int dx[] = {0, 0, 1, -1}; // جهت‌های حرکت: بالا، پایین، راست، چپ
    int dy[] = {-1, 1, 0, 0};

    bool visited[HEIGHT][WIDTH] = {false};
    Point queue[WIDTH * HEIGHT]; // صف
    Point prev[HEIGHT][WIDTH];

    int front = 0, rear = 0;

    Point door1 = {room1->x1, room1->y1};
    Point door2 = {room2->x2, room2->y2};

    queue[rear++] = door1;
    visited[door1.y][door1.x] = true;

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            prev[i][j] = (Point){-1, -1};
        }
    }

    bool path_found = false;
    while (front < rear) {
        Point current = queue[front++];

        if (current.x == door2.x && current.y == door2.y) {

            path_found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {

            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 1 && ny >= 1 && nx < WIDTH - 2 && ny < HEIGHT - 2 && !visited[ny][nx] &&
                (map[ny][nx] == ' ' || map[ny][nx] == '.' || map[ny][nx] == '+' || map[ny][nx] == '#')) {

                visited[ny][nx] = true;
                queue[rear++] = (Point){nx, ny};
                prev[ny][nx] = current;

                if (rear >= WIDTH * HEIGHT)
                    break;
            }
        }
    }

    if (!path_found) {
        return;
    }

    Point p = door2;

    while (!(p.x == door1.x && p.y == door1.y)) {

        if (map[p.y][p.x] == ' ')
            map[p.y][p.x] = '#';

        p = prev[p.y][p.x];
    }
}

void connect_rooms_hidden(Room *room1, Room *room2) {

    int dx[] = {0, 0, 1, -1};
    int dy[] = {-1, 1, 0, 0};

    bool visited[HEIGHT][WIDTH] = {false};
    Point queue[WIDTH * HEIGHT];
    Point prev[HEIGHT][WIDTH];

    int front = 0, rear = 0;

    Point door1 = {room1->x1, room1->y1};
    Point door2 = {room2->x2, room2->y2};

    queue[rear++] = door1;
    visited[door1.y][door1.x] = true;

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            prev[i][j] = (Point){-1, -1};
        }
    }

    bool path_found = false;
    while (front < rear) {
        Point current = queue[front++];

        if (current.x == door2.x && current.y == door2.y) {

            path_found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {

            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 1 && ny >= 1 && nx < WIDTH - 2 && ny < HEIGHT - 2 && !visited[ny][nx] &&
                (map[ny][nx] == ' ' || map[ny][nx] == '.' || map[ny][nx] == '-' || map[ny][nx] == '#')) {

                visited[ny][nx] = true;
                queue[rear++] = (Point){nx, ny};
                prev[ny][nx] = current;

                if (rear >= WIDTH * HEIGHT)
                    break;
            }
        }
    }

    if (!path_found) {
        return;
    }

    Point p = door2;

    while (!(p.x == door1.x && p.y == door1.y)) {

        if (map[p.y][p.x] == ' ')
            map[p.y][p.x] = '0';

        p = prev[p.y][p.x];
    }
}

/////////////////////
int speed_boost = 0;
int speed_timer = 0;

int damage_boost = 0;
int damage_timer = 0;

void generate_monsters(int room_count) {
    for (int i = 0; i < room_count; i++) { 
        if (i == 1) continue;

        rooms[i].monster_count = 1;  

        int type = rand() % 5;
        int x = rand() % rooms[i].width + rooms[i].x;
        int y = rand() % rooms[i].height + rooms[i].y;

        Monster new_monster;
        new_monster.x = x;
        new_monster.y = y;
        new_monster.type = type;
        new_monster.alive = 1;

        if (type == 0) {
            new_monster.health = 5;
            new_monster.damage = 2;
            new_monster.chase_limit = 0;
        }
        else if (type == 1) {
            new_monster.health = 10;
            new_monster.damage = 4;
            new_monster.chase_limit = 0;
        } 
        else if (type == 2) {
            new_monster.health = 15;
            new_monster.damage = 6;
            new_monster.chase_limit = 6;
        }
        else if (type == 3) {
            new_monster.health = 20;
            new_monster.damage = 5;
            new_monster.chase_limit = 0;
        } 
        else {
            new_monster.health = 30;
            new_monster.damage = 8;
            new_monster.chase_limit = 6;
        } 

        rooms[i].monsters[0] = new_monster;
    }
}

void move_monsters(int hero_x, int hero_y) {

    int room_index = find_room(hero_x, hero_y, room_count_global);
    if (room_index == -1 || room_index == 1) return;

    Room *current_room = &rooms[room_index];

    static char tiles[HEIGHT][WIDTH];

    for (int i = 0; i < current_room->monster_count; i++) {

        Monster *m = &current_room->monsters[i];
        if (!m->alive) continue;

        int dx = hero_x - m->x;
        int dy = hero_y - m->y;
        int distance = abs(dx) + abs(dy);

        int new_x = m->x;
        int new_y = m->y;

        if (tiles[m->y][m->x] == 0) {
            tiles[m->y][m->x] = map[m->y][m->x];
        }

        if (m->type == 3) {

            if (dx > 0 && can_move(m->x + 1, m->y)) new_x++;
            else if (dx < 0 && can_move(m->x - 1, m->y)) new_x--;

            if (dy > 0 && can_move(m->x, m->y + 1)) new_y++;
            else if (dy < 0 && can_move(m->x, m->y - 1)) new_y--;
        }

        else if ((m->type == 2 || m->type == 4)) {

            if (m->chase_limit > 0) {

                if (dx > 0 && can_move(m->x + 1, m->y)) new_x++;
                else if (dx < 0 && can_move(m->x - 1, m->y)) new_x--;

                if (dy > 0 && can_move(m->x, m->y + 1)) new_y++;
                else if (dy < 0 && can_move(m->x, m->y - 1)) new_y--;

                m->chase_limit--;
            }
        }

        else if (distance < 3) {
            if (dx > 0 && can_move(m->x + 1, m->y)) new_x++;
            else if (dx < 0 && can_move(m->x - 1, m->y)) new_x--;

            if (dy > 0 && can_move(m->x, m->y + 1)) new_y++;
            else if (dy < 0 && can_move(m->x, m->y - 1)) new_y--;
        }

        if (new_x != m->x || new_y != m->y) {
            map[m->y][m->x] = tiles[m->y][m->x]; 

            tiles[new_y][new_x] = map[new_y][new_x];

            m->x = new_x;
            m->y = new_y;
        }
        
        if (m->type == 0) map[m->y][m->x] = 'D';
        else if (m->type == 1) map[m->y][m->x] = 'F';
        else if (m->type == 2) map[m->y][m->x] = 'G';
        else if (m->type == 3) map[m->y][m->x] = 'S';
        else if (m->type == 4) map[m->y][m->x] = 'U';
    }
}

/////////////////////

void show_hunger_bar() {

    attron(COLOR_PAIR(5));
    mvprintw(0, 50, "Hunger: [       ]");
    mvprintw(0, 70, "Health: [       ]");
    attroff(COLOR_PAIR(5));

    attron(COLOR_PAIR(3));
    mvprintw(0, 59, "%d/100", hunger);
    mvprintw(0, 79, "%d/100", health);
    attroff(COLOR_PAIR(3));
}

void eat_foods() {
    if (collected_food_count == 0) {
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "No food to eat!              ");
        attroff(COLOR_PAIR(1));
        refresh();
        getch();
        return;
    }

    int choice = 0;

    while (1) {
        clear();

        attron(COLOR_PAIR(5));
        mvprintw(2, 48, "╔═════════════════════════════╗");
        mvprintw(3, 48, "║           Eat Food          ║");
        mvprintw(4, 48, "╠═════════════════════════════╣");
        attroff(COLOR_PAIR(5));

        for (int i = 0; i < collected_food_count; i++) {
            if (i == choice) attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            mvprintw(6 + i, 50, "Food %d | Boost: %d", i + 1, inventory[i].health_boost);
            attroff(A_REVERSE);
            attroff(COLOR_PAIR(4));
        }

        // نمایش دستورالعمل پایین منو
        attron(COLOR_PAIR(5));
        mvprintw(HEIGHT - 4, 48, "╠══════════════════════════╣");
        mvprintw(HEIGHT - 3, 48, "║ ENTER: Eat | Q: Exit     ║");
        mvprintw(HEIGHT - 2, 48, "╚══════════════════════════╝");
        attroff(COLOR_PAIR(5));
        refresh();

        int ch = getch();
        if (ch == KEY_UP && choice > 0) choice--;
        if (ch == KEY_DOWN && choice < collected_food_count - 1) choice++;
        if (ch == 'q') break;
        if (ch == 10) {
            hunger -= inventory[choice].health_boost;
            if (hunger < 0) hunger = 0;

            for (int j = choice; j < collected_food_count - 1; j++) {
                inventory[j] = inventory[j + 1];
            }
            collected_food_count--;

            mvprintw(HEIGHT - 8, 50, "Food eaten!");
            refresh();
            getch();
            break;
        }
    }
}

void update_hunger() {

    hunger += hunger_change;
    if (hunger > 100) {
        hunger = 0;
        health -= health_change; 

        if (health <= 0) {
            attron(COLOR_PAIR(1));
            mvprintw(0, 0, "You died of hunger! Game Over.");
            attroff(COLOR_PAIR(1));
            refresh();
        }
    }
}

void generate_foods(int room_count) {

    for (int i = 0; i < room_count; i++) { 

        if (i == 1) {
            continue;
        }

        rooms[i].food_count = rand() % 2 + 1; 

        for (int j = 0; j < rooms[i].food_count; j++) {
            
            Food new_food;
            new_food.x = rand() % rooms[i].width + rooms[i].x;
            new_food.y = rand() % rooms[i].height + rooms[i].y;
            new_food.type = rand() % 2;

            if(new_food.type == 0) {

            new_food.health_boost = 50;
            map[new_food.y][new_food.x] = 'f';
            }

            if(new_food.type == 1) {

            new_food.health_boost = 100;
            map[new_food.y][new_food.x] = 'p';
            }

            rooms[i].foods[j] = new_food; 

        }
    }
}

void collect_foods(int player_x, int player_y) {

    int room_index = find_room(player_x, player_y, room_count_global);
    
    if (room_index == -1 || room_index == 1) return;

    Room *current_room = &rooms[room_index];

    for (int i = 0; i < current_room->food_count; i++) {

        if (current_room->foods[i].x == player_x && current_room->foods[i].y == player_y) {
            
            if (collected_food_count < MAX_INVENTORY) { 

                inventory[collected_food_count] = current_room->foods[i]; 
                collected_food_count++;

                map[player_y][player_x] = '.'; 
                current_room->foods[i] = current_room->foods[current_room->food_count - 1]; 
                current_room->food_count--;
                i--;

                attron(COLOR_PAIR(2));
                mvprintw(0, 0, "Food collected!              ");
                attroff(COLOR_PAIR(2));
            } 
            
            else {
                attron(COLOR_PAIR(1));
                mvprintw(0, 0, "Inventory full!              ");
                attroff(COLOR_PAIR(1));
            }

            break;
        }
    }
}

/////////////////////

void generate_golds(int room_count) {

    for (int i = 0; i < room_count; i++) { 
        if (i == 1){
            continue;
        }

        rooms[i].gold_count = rand() % MAX_GOLD + 1;

        for (int j = 0; j < rooms[i].gold_count; j++) {
            Gold new_gold;

            new_gold.x = rand() % rooms[i].width + rooms[i].x;
            new_gold.y = rand() % rooms[i].height + rooms[i].y;

            if (rand() % 3 == 0) {
                new_gold.is_black = 1;
                new_gold.value = 5;
            } 
            
            else {
                new_gold.is_black = 0;
                new_gold.value = 1;
            }

            rooms[i].golds[j] = new_gold;

            if (new_gold.is_black) {
                map[new_gold.y][new_gold.x] = 'b';
            } 
            
            else {
                map[new_gold.y][new_gold.x] = 'g';
            }
        }
    }
}

void collect_golds(int player_x, int player_y) {

    int room_index = find_room(player_x, player_y, room_count_global);
    
    if (room_index == -1 || room_index == 1) return;

    Room *current_room = &rooms[room_index];

    for (int i = 0; i < current_room->gold_count; i++) {

        if (current_room->golds[i].x == player_x && current_room->golds[i].y == player_y) {
            
            total_gold += current_room->golds[i].value;

            map[player_y][player_x] = '.';

            current_room->golds[i] = current_room->golds[current_room->gold_count - 1];
            current_room->gold_count--;

            attron(COLOR_PAIR(4));
            mvprintw(0, 0, "You collected %d gold!       ", current_room->golds[i].value);
            attroff(COLOR_PAIR(4));

            break;
        }
    }
}

void show_golds() {
    attron(COLOR_PAIR(5));
    mvprintw(0, 40, "Gold:   ");
    attroff(COLOR_PAIR(5));

    attron(COLOR_PAIR(3));
    mvprintw(0, 46, "%d",total_gold);
    attroff(COLOR_PAIR(3));
}

/////////////////////

void generate_spells(int room_count) {

    for (int i = 0; i < room_count; i++) { 

        if (i == 1) {
            rooms[i].spell_count = 4;
        }
        
        else {
            rooms[i].spell_count = rand() % 2 + 1;
        }

        for (int j = 0; j < rooms[i].spell_count; j++) {

            Spell new_spell;
            new_spell.x = rand() % rooms[i].width + rooms[i].x;
            new_spell.y = rand() % rooms[i].height + rooms[i].y;
            new_spell.type = rand() % 3;

            if (new_spell.type == 0) {
                map[new_spell.y][new_spell.x] = 'h';
            } 
            
            else if (new_spell.type == 1) {
                map[new_spell.y][new_spell.x] = 's';
            } 
            
            else {
                map[new_spell.y][new_spell.x] = 'd';
            }

            rooms[i].spells[j] = new_spell;
        }
    }
}

void collect_spells(int player_x, int player_y) {

    int room_index = find_room(player_x, player_y, room_count_global);
    
    if (room_index == -1) {
        return;
    }

    Room *current_room = &rooms[room_index];

    for (int i = 0; i < current_room->spell_count; i++) {
        if (current_room->spells[i].x == player_x && current_room->spells[i].y == player_y) {
            
            if (collected_spell_count < MAX_INVENTORY) { 
                inventory_spells[collected_spell_count] = current_room->spells[i];
                collected_spell_count++;

                map[player_y][player_x] = '.';
                

                current_room->spells[i] = current_room->spells[current_room->spell_count - 1];
                current_room->spell_count--;

                attron(COLOR_PAIR(2));
                mvprintw(0, 0, "Spell collected!");
                attroff(COLOR_PAIR(2));
            } 
            else {
                attron(COLOR_PAIR(1));
                mvprintw(0, 0, "Inventory full!");
                attroff(COLOR_PAIR(1));
            }
            break;
        }
    }
}

void show_spells() {

    if (collected_spell_count == 0) {

        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "No spells collected!         ");
        attroff(COLOR_PAIR(1));
        refresh();

        getch();
        return;
    }

    int choice = 0;
    while (1) {
        clear();

        attron(COLOR_PAIR(5));
        mvprintw(2, 50, "╔══════════════════════════╗");
        mvprintw(3, 50, "║    Collected Spells      ║");
        mvprintw(4, 50, "╠══════════════════════════╣");
        attroff(COLOR_PAIR(5));

        const char *spell_names[] = {"Health Boost", "Speed Boost", "Attack Boost"};

        for (int i = 0; i < collected_spell_count; i++) {

            if (i == choice) attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            mvprintw(6 + i, 50, "Spell %d | Type: %s", i + 1, spell_names[inventory_spells[i].type]);
            attroff(A_REVERSE);
            attroff(COLOR_PAIR(4));
        }

        attron(COLOR_PAIR(5));
        mvprintw(HEIGHT - 4, 50, "╠══════════════════════════╣");
        mvprintw(HEIGHT - 3, 50, "║  ENTER: Use  |  Q: Exit  ║");
        mvprintw(HEIGHT - 2, 50, "╚══════════════════════════╝");
        attroff(COLOR_PAIR(5));
        refresh();

        int ch = getch();
        if (ch == KEY_UP && choice > 0) choice--;
        if (ch == KEY_DOWN && choice < collected_spell_count - 1) choice++;
        if (ch == 'q') break;
        if (ch == 10) {
            int spell_type = inventory_spells[choice].type;

            if (spell_type == 0) {
                health += 5;
                if (health > 100) health = 100;
            } 

            else if (spell_type == 1) { // طلسم سرعت
                speed_boost = 1;
                speed_timer = 0;
            } 

            else if (spell_type == 2) { // طلسم قدرت
                damage_boost = 1;
                damage_timer = 0;
            }

            for (int j = choice; j < collected_spell_count - 1; j++) {
                inventory_spells[j] = inventory_spells[j + 1];
            }
            collected_spell_count--;

            attron(COLOR_PAIR(3));
            mvprintw(HEIGHT - 8, 50, "Used Spell: %s", spell_names[spell_type]);
            attroff(COLOR_PAIR(3));
            refresh();
            getch();
            break;
        }
    }
}

/////////////////////

void generate_weapons(int room_count) {
    for (int i = 0; i < room_count; i++) { 
        if (i == 1) continue;  // جلوگیری از تولید سلاح در اتاق طلسم

        rooms[i].weapon_count = rand() % 2 + 1;  // ۱ یا ۲ سلاح در هر اتاق

        for (int j = 0; j < rooms[i].weapon_count; j++) {
            Weapon new_weapon;
            new_weapon.x = rand() % rooms[i].width + rooms[i].x;
            new_weapon.y = rand() % rooms[i].height + rooms[i].y;
            int a = rand() % 5;  // انتخاب تصادفی یکی از ۵ سلاح

            if (a == 0) {
                strcpy(new_weapon.type, "Dagger");
                new_weapon.damage = 12;
                new_weapon.ranged = 1;
                new_weapon.range = 5;
                new_weapon.ammo = 10;
                map[new_weapon.y][new_weapon.x] = '6';
            }
            else if (a == 1) {
                strcpy(new_weapon.type, "Magic Wand");
                new_weapon.damage = 15;
                new_weapon.ranged = 1;
                new_weapon.range = 10;
                new_weapon.ammo = 8;
                map[new_weapon.y][new_weapon.x] = '7';
            }
            else if (a == 2) {
                strcpy(new_weapon.type, "Normal Arrow");
                new_weapon.damage = 5;
                new_weapon.ranged = 1;
                new_weapon.range = 5;
                new_weapon.ammo = 20;
                map[new_weapon.y][new_weapon.x] = '8';
            }
            else if (a == 3) {
                strcpy(new_weapon.type, "Sword");
                new_weapon.damage = 10;
                new_weapon.ranged = 0;
                new_weapon.range = 0;
                new_weapon.ammo = -1;
                map[new_weapon.y][new_weapon.x] = '9';
            }

            rooms[i].weapons[j] = new_weapon;
        }
    }
}

void collect_weapons(int player_x, int player_y) {
    int room_index = find_room(player_x, player_y, room_count_global);
    
    if (room_index == -1 || room_index == 1) return;

    Room *current_room = &rooms[room_index];

    for (int i = 0; i < current_room->weapon_count; i++) {
        if (current_room->weapons[i].x == player_x && current_room->weapons[i].y == player_y) {
            
            if (collected_weapon_count < MAX_INVENTORY) { 
                if (strcmp(current_room->weapons[i].type, "Sword") == 0) {

                } 

                else {
                    inventory_weapons[collected_weapon_count] = current_room->weapons[i];
                    collected_weapon_count++;
                }

                map[player_y][player_x] = '.';
                current_room->weapons[i] = current_room->weapons[current_room->weapon_count - 1];
                current_room->weapon_count--;

                attron(COLOR_PAIR(2));
                mvprintw(0, 0, "Weapon collected!");
                attroff(COLOR_PAIR(2));
            } 
            else {
                attron(COLOR_PAIR(1));
                mvprintw(0, 0, "Inventory full!");
                attroff(COLOR_PAIR(1));
            }
            break;
        }
    }
}

void show_weapons() {
    if (collected_weapon_count == 0) {
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "❌ No weapons collected!");
        attroff(COLOR_PAIR(1));
        getch();
        refresh();
        return;
    }

    int choice = 0;
    while (1) {
        clear();

        attron(COLOR_PAIR(5));
        mvprintw(2, 48, "╔════════════════════════════╗");
        mvprintw(3, 48, "║       Choose Weapon       ║");
        mvprintw(4, 48, "╠════════════════════════════╣");
        attroff(COLOR_PAIR(5));

        for (int i = 0; i < collected_weapon_count; i++) {
            if (i == choice) attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            mvprintw(6 + i, 50, "🗡️ %s | Damage: %d", inventory_weapons[i].type, inventory_weapons[i].damage);
            if (i == choice) attroff(A_REVERSE);
            attroff(COLOR_PAIR(4));
        }

        attron(COLOR_PAIR(5));
        mvprintw(HEIGHT - 4, 48, "╠══════════════════════════╣");
        mvprintw(HEIGHT - 3, 48, "║ ENTER: Equip | Q: Exit  ║");
        mvprintw(HEIGHT - 2, 48, "╚══════════════════════════╝");
        attroff(COLOR_PAIR(5));
        refresh();

        int ch = getch();

        if (ch == KEY_UP && choice > 0) choice--;
        if (ch == KEY_DOWN && choice < collected_weapon_count - 1) choice++;
        if (ch == 'q') break;

        if (ch == 10) {  // Enter key
            equipped_weapon = choice;
            mvprintw(HEIGHT - 6, 50, "✅ Equipped: %s", inventory_weapons[equipped_weapon].type);
            refresh();
            getch();
            break;
        }
    }
}

/////////////////////

void character_loc(int room_count) {

    int i = rand() % (room_count - 2) + 2;
    loc_x = rooms[i].x + rand() % rooms[i].width;
    loc_y = rooms[i].y + rand() % rooms[i].height;

    if (map[loc_y][loc_x] != '.') {
         
        character_loc(room_count_global);
    }
}

void regenerate_health() {
    static time_t last_regen_time = 0;
    time_t current_time = time(NULL);

    if (current_time - last_regen_time >= 15) {
        if (health < 100) {
            health++;
        }

        last_regen_time = current_time;
    }
}

/////////////////////

void stair_loc(int room_count) {

    int i = rand() % (room_count - 2) + 2;
    int x = rooms[i].x + 1 + rand() % (rooms[i].width - 2);
    int y = rooms[i].y + 1 + rand() % (rooms[i].height - 2);

    int a = find_room(loc_x,loc_y,room_count_global);
    int b = find_room(x,y,room_count_global);

    if (map[y][x] == '.' && !(a == b)) {

        map[y][x] = '<';
    }

    else {

        stair_loc(room_count);
    }
}

/////////////////////

void hidden_door_print(int x,int y,int loc_x,int loc_y) {
    //در مخفی ۱ دیوار چاپ می شود
    if (map[y][x] == '1' && hide_door1) {

        attron(COLOR_PAIR(7));
        mvaddch(y, x, '*'); 
        attroff(COLOR_PAIR(7));
    }
    //نزدیک در مخفی۱
    if(map[y][x] == '1' && y == loc_y && (x == loc_x - 1 || x == loc_x + 1)) {

        if(pd1) {

        attron(COLOR_PAIR(2));
        mvprintw(0,0,"hidden door open!            ");
        mvaddch(y,x,'?');
        attroff(COLOR_PAIR(2));
        hide_door1 = 0;
        }

        pd1 = 0;
    }
    //چاپ در مخفی۱
    if(map[y][x] == '1' && !hide_door1) {

        connect_rooms_hidden(&rooms[0],&rooms[1]);
        attron(COLOR_PAIR(2));
        mvaddch(y,x,'?');
        attroff(COLOR_PAIR(2));
    }
}

void map_print(int x,int y) {

    wchar_t mace[] = L"⚒";
    wchar_t dagger[] = L"🗡";
    wchar_t arrow[] = L"➳";
    wchar_t sword[] = L"⚔";
    //دیوار و ستون
    if (map[y][x] == '*' || map[y][x] ==  'O') {
        attron(COLOR_PAIR(7));
        mvaddch(y, x, map[y][x]); 
        attroff(COLOR_PAIR(7));
    }
    //در و پنجره و راهرو
    else if (map[y][x] == '+' || map[y][x] == '=' || map[y][x] == 'W') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, map[y][x]);
        attroff(COLOR_PAIR(2));
    }
    //کف و تله
    else if (map[y][x] == '.' || map[y][x] == 'T') {
        mvaddch(y, x, '.');
    }
    //پله
    else if (map[y][x] == '<') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, '<');
        attroff(COLOR_PAIR(2));
    }
    //غذا
    else if (map[y][x] == 'p' || map[y][x] == 'f') {
        attron(COLOR_PAIR(3));
        mvaddch(y, x, map[y][x]);
        attroff(COLOR_PAIR(3));
    }
    //دیوار اتاق طلسم
    else if (map[y][x] == '3') {
        attron(COLOR_PAIR(1));
        mvaddch(y,x,'*');
        attroff(COLOR_PAIR(1));
    }
    // در اتاق طلسم
    else if (map[y][x] == '-') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, '+');
        attroff(COLOR_PAIR(2));
    }
    // دیوار اتاق گنج
    else if (map[y][x] == '4') {
        attron(COLOR_PAIR(4));
        mvaddch(y,x,'*');
        attroff(COLOR_PAIR(4));
    }
    //طلا
    else if (map[y][x] == 'g') {
        attron(COLOR_PAIR(4)); 
        mvaddch(y, x, 'g');
        attroff(COLOR_PAIR(4));
    }
    //طلا سیاه
    else if (map[y][x] == 'b') {
        attron(COLOR_PAIR(4));
        mvaddch(y, x, 'b');
        attroff(COLOR_PAIR(4));
    }
    //طلسم سلامت
    else if (map[y][x] == 'h') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, 'h');
        attroff(COLOR_PAIR(2));
    }
    //طلسم اسیب
    else if (map[y][x] == 'd') {
        attron(COLOR_PAIR(1));
        mvaddch(y, x, 'd');
        attroff(COLOR_PAIR(1));
    }
    //طلسم سرعت
    else if (map[y][x] == 's') {
        attron(COLOR_PAIR(5));
        mvaddch(y, x, 's');
        attroff(COLOR_PAIR(5));
    }

    else if (map[y][x] == '6') {

    mvprintw(y, x, "%ls", dagger);
    }

    else if (map[y][x] == '7') {

        mvaddch(y,x,'|');
    }

    else if (map[y][x] == '8') {

    mvprintw(y, x, "%ls", arrow);
    }

    else if (map[y][x] == '9') {

    mvprintw(y, x, "%ls", sword);
    }

    if (map[y][x] == 'D' || map[y][x] == 'F' || map[y][x] == 'G' || map[y][x] == 'S' || map[y][x] == 'U') {
        attron(COLOR_PAIR(1));
        mvaddch(y, x, map[y][x]);
        attroff(COLOR_PAIR(1));
    }

}

void map_print2(int x,int y) {

    wchar_t mace[] = L"⚒";
    wchar_t dagger[] = L"🗡";
    wchar_t arrow[] = L"➳";
    wchar_t sword[] = L"⚔";
    //دیوار و ستون
    if (map[y][x] == '*' || map[y][x] ==  'O') {
        attron(COLOR_PAIR(7));
        mvaddch(y, x, map[y][x]); 
        attroff(COLOR_PAIR(7));
    }
    //در و پنجره و راهرو
    else if (map[y][x] == '+' || map[y][x] == '=' || map[y][x] == 'W') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, map[y][x]);
        attroff(COLOR_PAIR(2));
    }
    //کف و تله
    else if (map[y][x] == '.' || map[y][x] == 'T') {
        mvaddch(y, x, '.');
    }
    //پله
    else if (map[y][x] == '<') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, '<');
        attroff(COLOR_PAIR(2));
    }
    //غذا
    else if (map[y][x] == 'p' || map[y][x] == 'f') {
        attron(COLOR_PAIR(3));
        mvaddch(y, x, map[y][x]);
        attroff(COLOR_PAIR(3));
    }
    //دیوار اتاق طلسم
    else if (map[y][x] == '3') {
        attron(COLOR_PAIR(1));
        mvaddch(y,x,'*');
        attroff(COLOR_PAIR(1));
    }
    // در اتاق طلسم
    else if (map[y][x] == '-') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, '+');
        attroff(COLOR_PAIR(2));
    }
    // دیوار اتاق گنج
    else if (map[y][x] == '4') {
        attron(COLOR_PAIR(4));
        mvaddch(y,x,'*');
        attroff(COLOR_PAIR(4));
    }
    //طلا
    else if (map[y][x] == 'g') {
        attron(COLOR_PAIR(4)); 
        mvaddch(y, x, 'g');
        attroff(COLOR_PAIR(4));
    }
    //طلا سیاه
    else if (map[y][x] == 'b') {
        attron(COLOR_PAIR(4));
        mvaddch(y, x, 'b');
        attroff(COLOR_PAIR(4));
    }
    //طلسم سلامت
    else if (map[y][x] == 'h') {
        attron(COLOR_PAIR(2));
        mvaddch(y, x, 'h');
        attroff(COLOR_PAIR(2));
    }
    //طلسم اسیب
    else if (map[y][x] == 'd') {
        attron(COLOR_PAIR(1));
        mvaddch(y, x, 'd');
        attroff(COLOR_PAIR(1));
    }
    //طلسم سرعت
    else if (map[y][x] == 's') {
        attron(COLOR_PAIR(5));
        mvaddch(y, x, 's');
        attroff(COLOR_PAIR(5));
    }

    else if (map[y][x] == '6') {

    mvprintw(y, x, "%ls", dagger);
    }

    else if (map[y][x] == '7') {

        mvaddch(y,x,'|');
    }

    else if (map[y][x] == '8') {

    mvprintw(y, x, "%ls", arrow);
    }

    else if (map[y][x] == '9') {

    mvprintw(y, x, "%ls", sword);
    }

    if (map[y][x] == 'D' || map[y][x] == 'F' || map[y][x] == 'G' || map[y][x] == 'S' || map[y][x] == 'U') {
        attron(COLOR_PAIR(1));
        mvaddch(y, x, map[y][x]);
        attroff(COLOR_PAIR(1));
    }

    else if (map[y][x] == '#') {
        attron(COLOR_PAIR(2));
        mvaddch(y,x,'#');
        attroff(COLOR_PAIR(2));
    }

    else if (map[y][x] == '0') {
        attron(COLOR_PAIR(1));
        mvaddch(y,x,'#');
        attroff(COLOR_PAIR(1));
    }
}

void print_corridor(int loc_x, int loc_y, int x, int y) {
        
    int dx = x - loc_x;
    int dy = y - loc_y;
    int distance = sqrt(dx * dx + dy * dy);
        
    if (map[y][x] == '#') {


        if (distance <= VISION_RADIUS) {

            seen_map[y][x] = 1;
        }

        if (seen_map[y][x]) { 

            attron(COLOR_PAIR(2));
            mvaddch(y, x, '#');
            attroff(COLOR_PAIR(2));
        } 
        
        else {
            mvaddch(y, x, ' '); 
        }

    }

    if (map[y][x] == '0') { 


        if (distance <= VISION_RADIUS) {
            
            seen_map[y][x] = 1;
        }

        if (seen_map[y][x]) {  

            attron(COLOR_PAIR(1));
            mvaddch(y, x, '#');
            attroff(COLOR_PAIR(1));
        } 
        
        else {
            mvaddch(y, x, ' ');
        }
    }
}

void trap_print(int x,int y,int loc_x,int loc_y){
        //چاپ تله
        if(map[y][x] == 'T' && y == loc_y && x == loc_x){
            attron(COLOR_PAIR(1));
            mvaddch(y,x,'^');
            attroff(COLOR_PAIR(1));
            ttrap_print = 0;
            health -= 5;
        }
}

void character_print(int loc_x,int loc_y){

    if (character_color == 2){
        attron(COLOR_PAIR(5));
    }

    else if (character_color == 1){
        attron(COLOR_PAIR(2));
    }

    else if (character_color == 3){
        attron(COLOR_PAIR(3));
    }

    if (ttrap_print){
        mvprintw(loc_y, loc_x, "@");
    refresh();
    }

    ttrap_print = 1;

    attroff(COLOR_PAIR(3));
    attroff(COLOR_PAIR(5));
    attroff(COLOR_PAIR(4));
}

////////////////////

int find_room(int player_x, int player_y, int room_count_global) {

    for (int i = 0; i < room_count_global; i++) {

        Room current_room = rooms[i];

        if (player_x >= current_room.x && player_x < current_room.x + current_room.width &&
            player_y >= current_room.y && player_y < current_room.y + current_room.height) {
            return i;
        }
    }

    return -1;
}

int near_room(int player_x, int player_y, int room_count_global) {

    for (int i = 0; i < room_count_global; i++) {

        Room current_room = rooms[i];

        if (player_x >= current_room.x - 2 && player_x <= current_room.x + current_room.width + 1 &&
            player_y >= current_room.y - 2 && player_y <= current_room.y + current_room.height + 1) {
            return i;
        }
    }
    return -1;
}

void room_print(){

    attron(COLOR_PAIR(5));
    mvprintw(0, 90, "Room: [  ]");
    attroff(COLOR_PAIR(5));

    attron(COLOR_PAIR(3));
    mvprintw(0, 97, "%d",near_room(loc_x,loc_y,room_count_global));
    attroff(COLOR_PAIR(3));

}

//////////////////////

int can_move(int new_x, int new_y) {
    
    if (new_x < 0 || new_x >= WIDTH || new_y < 1 || new_y >= HEIGHT) {
        return 0;
    }
    
    if (map[new_y][new_x] == '*' || map[new_y][new_x] == 'O' || map[new_y][new_x] == '=' || map[new_y][new_x] == ' ' || map[new_y][new_x] == '3' || map[new_y][new_x] == '4') {
        return 0;
    }

    return 1;
}

int move_player(int *lastx, int *lasty, int ch) {
    int x = *lastx;
    int y = *lasty;
    int moved = 0;

    if (ch == 'f') {
        f_pressed = 1;
        return 1; 
    }

    if (f_pressed) {
        switch (ch) {
            case KEY_UP:
                while (can_move(x, y - 1)) {
                    y--;
                    moved = 1;
                }
                break;
            case KEY_DOWN:
                while (can_move(x, y + 1)) {
                    y++;
                    moved = 1;
                }
                break;
            case KEY_LEFT:
                while (can_move(x - 1, y)) {
                    x--;
                    moved = 1;
                }
                break;
            case KEY_RIGHT:
                while (can_move(x + 1, y)) {
                    x++;
                    moved = 1;
                }
                break;
        }
        f_pressed = 0;
        *lastx = x;
        *lasty = y;
        return moved;
    } 
    
    else {
        switch (ch) {
            case '1': x--; y++; moved = 1; break;
            case '2': y++; moved = 1; break;
            case '3': x++; y++; moved = 1; break;
            case '4': x--; moved = 1; break;
            case '6': x++; moved = 1; break;
            case '7': x--; y--; moved = 1; break;
            case '8': y--; moved = 1; break;
            case '9': x++; y--; moved = 1; break;
            case KEY_UP: y--; moved = 1; break;
            case KEY_DOWN: y++; moved = 1; break;
            case KEY_LEFT: x--; moved = 1; break;
            case KEY_RIGHT: x++; moved = 1; break;
            case 'q': game_menu(); return 0;
        }

        if (can_move(x, y)) {
            *lastx = x;
            *lasty = y;

            
            if (map[y][x] == '<' && mapp == 3) {
                return 2;
            }

            else if (map[y][x] == '<') {
                mapp++;
                return 3;
            }

            if (map[y][x] == 'W') {
                return 4;
            }

            return 1;
        }
    }
    return 0; // اگر حرکت امکان‌پذیر نباشد
}

void move_print() {

    int stop = 0;
    int g_pressed = 1;
    int stop_hunger = 1;
    int stop_tem = 0;

    if (!is_loading_game) {

        game_map();
    }

    is_loading_game = 0;

    while (1) {
        clear();
        regenerate_health();

        if (speed_boost) {
            speed_timer++;
            if (speed_timer > 20) { 
                speed_boost = 0;
                speed_timer = 0;
                attron(COLOR_PAIR(1));
                mvprintw(0,0, "Speed boost wore off!        ");
                attroff(COLOR_PAIR(1));
            }
        }
        
        if (damage_boost) {
            damage_timer++;
            if (damage_timer > 20) { 
                damage_boost = 0;
                damage_timer = 0;
                attron(COLOR_PAIR(1));
                mvprintw(0, 0, "Damage boost wore off!       ");
                attroff(COLOR_PAIR(1));
            }
        }

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                hidden_door_print(x,y,loc_x,loc_y);
                map_print(x,y);
                print_corridor(loc_x,loc_y,x,y);
                trap_print(x,y,loc_x,loc_y);
                collect_golds(loc_x, loc_y);

                if (g_pressed) {
                    collect_foods(loc_x, loc_y);
                    collect_weapons(loc_x, loc_y);

                    if (collect) {
                        collect_spells(loc_x, loc_y);
                    }
                }
                

                if(stop){
                    map_print2(x,y);
                }

            }
        }

        show_hunger_bar();
        room_print();
        show_golds();
        message_print(loc_x,loc_y);
        character_print(loc_x,loc_y);
        refresh();

        int ch = getch();

        if (ch == 'q') {
            save_game_state();
            game_menu();
        }

        if (ch == 'p'){
            main_menu();
        }

        if (ch == 'm'){
            stop ^= 1;
        }

        if (ch == 'e') {
            eat_foods();
        }

        if (ch == 't') {
            show_spells();
        }

        if (ch == 'w') {
            show_weapons();
        }

        if (ch == 'g') {
            g_pressed ^= 1;
        }

        int move_count = 1;
        if (speed_boost) move_count = 2;

        for (int i = 0; i < move_count; i++) {

            int a = move_player(&loc_x, &loc_y, ch);

            if (move_count == 2) {
                stop_hunger ^= 1;
            }

            if (a == 2) {
                collect = 0;
                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        seen_map[y][x] = 0;
                    }
                }
                game_map2();
            }

            if (a == 3) {

                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        seen_map[y][x] = 0;
                
                    }
                }
                hide_door1 = 1;
                pd1 = 1;
                game_map();
            }

            if (a == 4) {
                napms(1000);
                win();
            }

            if (a && stop_hunger) {
                update_hunger();
            }

            if (a) {
                move_monsters(loc_x, loc_y);
            }
        }
    }
    game_menu();
}

void win() {

    while (1){
        clear();
        attron(COLOR_PAIR(2));
        mvprintw(0,50,"YOU WIN!");
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(4));
        mvprintw(3,50,"Your score in this game : %d",total_gold);
        attroff(COLOR_PAIR(4));
        int ch = getch();
        if (ch == 'q') {
            break;
        }

        update_user_score(user,total_gold);
        game_menu();
    }
}

void message_print(int loc_x,int loc_y){

    int a = find_room(loc_x,loc_y,room_count_global);

    if (a > -1){
        if (rooms[a].enter){
            attron(COLOR_PAIR(2));
            mvprintw(0,0,"You entered a new room!      ");
            attroff(COLOR_PAIR(2));
            rooms[a].enter = 0;
        }
    }

    if (map[loc_y][loc_x] == 'T') {
    attron(COLOR_PAIR(1));
    mvprintw(0,0,"you dropped in Trap!         ");
    attroff(COLOR_PAIR(1));
    }
}

/////////////////////

void exit_program() {
    clear();
    attron(A_BOLD);
    attron(COLOR_PAIR(1));
    mvprintw(15, 50, "Exiting Program...");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));
    refresh();
    attron(COLOR_PAIR(5));
    mvprintw(27, 95, "Thanks for playing");
    mvprintw(28, 95, "Amirreza :)");
    attroff(COLOR_PAIR(5));
    getch();
    endwin();
    exit(0);
}

void main_menu() {

    attron(A_BOLD);

    const char *choices[] = {"Guest", "Sign In", "Sign Up", "Exit"};
    int num_choices = 4;
    int current_choices = 0;

    while (1){

        clear();

        for (int i = 0; i < num_choices; i++) {

            if (i == current_choices){

                attron(COLOR_PAIR(6));
                attron(A_REVERSE);
            }

            else {

                if (i == 0){
                    attron(COLOR_PAIR(2));
                }
                else if (i == 1){
                    attron(COLOR_PAIR(3));
                }
                else if (i == 2){
                    attron(COLOR_PAIR(3));
                }
                else if (i == 3){
                    attron(COLOR_PAIR(1));
                }
            }

            mvprintw(i + 12, 55, "%s", choices[i]);

            attroff(A_REVERSE);
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(2));
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(6));
        }

        int ch = getch();

        if (ch == KEY_UP) {

            if (current_choices == 0) {

                current_choices = num_choices - 1;
            }

            else {

                current_choices = current_choices - 1;
            }
        }

        else if (ch == KEY_DOWN) {

            if (current_choices == num_choices - 1) {

                current_choices = 0;
            }

            else {
                current_choices = current_choices + 1;
            }
        }

        else if (ch == 10) {

            if (current_choices == 0) {
                move_print();
            }

            if (current_choices == 1) {
                sign_in_menu_paint(); 
            }

            if (current_choices == 2) {
                sign_up_menu_paint();
            }

            if (current_choices == 3) {
                exit_program();
            }
        }

        refresh();
    }
}
