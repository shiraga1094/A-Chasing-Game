#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define UP    119
#define LEFT  97
#define RIGHT 100
#define DOWN  115
#define ESC 7

char World_Map[22][22]; // Size=21*21
int Obstacle_Map[10][22][22]={};
bool Check_Map[22][22]={};
int Item_Map[22][22]={};
bool Bomb_Map[22][22]={};
int MapNo=0;
deque<string> Ability_Name[4];

bool OutofRange(int x, int y){
    return x>=22 or x<=0 or y>=22 or y<=0 or Obstacle_Map[MapNo][x][y];
}
void gotoxy(int x, int y)
{ // Allows to move inside the terminal using coordinates
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x; // Starts from 0
    dwPos.Y = y; // Starts from 0
    SetConsoleCursorPosition(hCon, dwPos);
}
void HideCursor()
{ // Hides the cursor :3
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}
void SetColor(int color = 7)
{
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole,color);
}

void DrawWorldMap(int x, int y, int tx, int ty){
    char ch=World_Map[x+1][y+1];
    switch(ch){
        case 'C': SetColor(11); break;
        case 'E': SetColor(12); break;
        case 'I': SetColor(10); break;
        case 'O': SetColor(13); break;
        case 'S': SetColor(14); break;
        case 'P': SetColor(2);  break;
        case 'W': SetColor(136); break;
    }
    gotoxy(tx,ty);
    cout << ch;
    SetColor();
}
void DrawWhiteSpace(int a_x, int a_y, int b_x, int b_y)
{ // To clean a certain space in the terminal
for(int i = a_x; i < b_x; i++)
{
for(int j = a_y; j < b_y; j++)
{
gotoxy(i,j); printf(" ");
}
}
}
void DrawWindowFrame(int a_x, int a_y, int b_x, int b_y, bool DrawWhite)
{ // This will draw a rectangular frame defined by two points
a_y+=3; b_y+=3;
if(DrawWhite) DrawWhiteSpace(a_x,a_y,b_x,b_y);
for(int y=a_y; y<=b_y; y++){
    gotoxy(a_x,y);  cout << "|";
    gotoxy(b_x,y); cout << "|";
}
for(int x=a_x; x<=b_x; x++){
    gotoxy(x,a_y); cout << "-";
    gotoxy(x,b_y); cout << "-";
}
for(int i=0; i<21; i++){
    for(int s=0; s<21; s++){
        //gotoxy(a_x+1+i*2, a_y+1+s);
        DrawWorldMap(i,s,a_x+1+i*2, a_y+s+1);
    }
}
}
void DrawGameLimits(bool st=0)
{ // Draws the game limits, and information that doesn't have to be printed repeatedly
DrawWindowFrame(0,0,43,22,1); // The default size of the Windows terminal is 25 rows x 80 columns
gotoxy(2,1);
if(st) printf("STAGE:");
else printf("BOMB:");

gotoxy(12,1); printf("ENEMY:");
gotoxy(21,1); printf("SCORE:");
gotoxy(31,1); printf("LIFE:");
}

void WelcomeMessage(){
    int x=17, y=10;
    gotoxy(x,y); cout << "A chasing game.";
    gotoxy(x-4, y+1); cout <<"Press any key to continue.";
    getch();
    DrawWhiteSpace(0,0,80,25);
}
void TipsMessage(){
    int x=24, y=10;
    gotoxy(x,y); cout << "How to play:";
    gotoxy(x-3,y+1); cout << "Use WASD to move.";
    gotoxy(x-5, y+2); cout << "Press space to set bomb.";
    gotoxy(x-20, y+3); cout << "'C'=You, 'I'=Bomb, 'E'=Enemy, 'S'=Score, 'O'=Set Bomb";
    gotoxy(x-22, y+4); cout << "Game ends when enemies catch you or you touch the set bomb.";
    gotoxy(x+1, y+5); cout << "Have fun.";
    gotoxy(x-6, y+6); cout <<"(Press any key to start.)";
    getch();
    DrawWhiteSpace(0,0,80,25);
}
void DrawGameMode(){
    int x=17, y=10;
    gotoxy(x,y); cout << "Select Gamemode:";
    gotoxy(x+4,y+1); cout << "Easy";
    gotoxy(x+4,y+2); cout << "Medium";
    gotoxy(x+4, y+3); cout << "Hard";
    gotoxy(x+4, y+4); cout << "Stage Mode";
    gotoxy(x+4, y+5); cout << "Map Select: " << MapNo;
    gotoxy(x-3,y+6); cout << "(Press space to start.)";
}
void DrawGameModeMove(int last, int now){
    int x=19, y=11;
    gotoxy(x, y+last); cout << ' ';
    gotoxy(x, y+now);
    SetColor(11);
    cout << 'O';
    SetColor();
    gotoxy(32,15); cout << "   ";
    gotoxy(32,15); cout << MapNo;
}
void DrawAbilityMode(){
    int x=15, y=10;
    gotoxy(x,y); cout << "Choose Your Ability:";
    gotoxy(x+3,y+1); cout << "Level: ";
    gotoxy(x+3,y+2); cout << "Name: ";
}
void DrawAbilityModeMove(int now, int newlevel, int name){
    int colorno[4]={10,12,3,4};
    int x=16, y=11;
    DrawWhiteSpace(23,12,60,13);
    gotoxy(x,y+(now^1)); cout << ' ';
    gotoxy(x,y+now);
    SetColor(11); cout << 'O';
    gotoxy(24,11); SetColor(colorno[newlevel]); cout << newlevel+1;
    gotoxy(23,12); cout << Ability_Name[newlevel][name];
    SetColor();

}
class Game_Mode{
    private:
        int mode;
        int enemyrandom[4]={50,30,15,INT_MAX};
        int enemylimit[4]={2,3,4,0};
        int bombrandom[4]={20,50,50,20};
        int bomblimit[4]={2,1,1,1};
        int restrandom[4]={50,15,18,INT_MAX};
        bool ai;
        bool stage;
        int ability_mode=0;
    public:
        GameMode(int _mode){mode=_mode; ai=0; stage=0;}
        int AB(){return ability_mode;}

        int ER(){return enemyrandom[mode];}
        int BR(){return bombrandom[mode];}
        int EL(){return enemylimit[mode];}
        int BL(){return bomblimit[mode];}
        int RR(){return restrandom[mode];}
        bool AI(){return ai;}
        bool Stage(){return stage;}

        void SetGameMode(){
            DrawGameMode();
            char key;
            int Mode=0;
            int newMode=0;
            ai=0;
            DrawGameModeMove(0,0);

            while(key=getch()){
                if(key==84){
                    ai=1;
                    gotoxy(0,0); cout << "Test on";
                }
                if(key==' '){
                    if(Mode!=4){
                        mode=Mode;
                        break;
                    }else{
                        MapNo=(MapNo+1)%10;
                    }
                }
                if(key==DOWN){
                    newMode=Mode+1;
                    newMode=newMode%5;
                }else if(key==UP){
                    newMode=Mode-1;
                    if(newMode<0) newMode=4;
                }
                DrawGameModeMove(Mode,newMode);
                Mode=newMode;

            }
            stage=(mode==3);
            DrawWhiteSpace(0,0,80,25);
            if(mode!=3)
                SetAbilityMode();
        }
        void SetAbilityMode(){
            DrawAbilityMode();
            char key; int level=0, Mode=0, name=0;
            DrawAbilityModeMove(Mode, level, name);
            while(key=getch()){
                if(key==' '){
                    ability_mode=(level+1)*10+name+1;
                    break;
                }
                if(key==DOWN or key==UP) Mode^=1;
                if(!Mode){
                    if(key==RIGHT){
                        level++; level%=4;
                        name=0;
                    }else if(key==LEFT){
                        level--; if(level<0) level=3;
                        name=0;
                    }
                }else if(Mode){
                    if(key==RIGHT){
                        name++; name%=Ability_Name[level].size();
                    }else if(key==LEFT){
                        name--; if(name<0) name=Ability_Name[level].size()-1;
                    }
                }
                DrawAbilityModeMove(Mode, level, name);
            }
            DrawWhiteSpace(0,0,80,25);
        }

};
class Game_Data{
    public:
        int score;
        int Bomb_count;
        int Enemy_count;
        int Stage;
        int life;

        void Output(){
            DrawWhiteSpace(18,1,20,2);
            DrawWhiteSpace(27,1,29,2);
            DrawWhiteSpace(36,1,38,2);

            if(Stage>0){
                gotoxy(8,1);
                cout << Stage;
            }else{
                gotoxy(7,1);
                cout << Bomb_count;
            }
            gotoxy(18,1); cout << Enemy_count;
            gotoxy(27,1); cout << score;
            gotoxy(36,1); cout << life;
        }
        void Get_Score(){
            score++;
        }
        void LifeDecrease(){life--;}
        void Get_Bomb(){
            if(Bomb_count>=2){
                DrawWhiteSpace(46,3,80,5);
                gotoxy(46,3);
                cout << "Too many bomb.";
            }
            else Bomb_count++;
        }
        void Use_Bomb(){
            Bomb_count--;
        }
        void Explode(int x){
            DrawWhiteSpace(46,3,80,5);
            gotoxy(46,3);
            cout << x << " enemies are eliminated.";
            Enemy_count-=x;
            Output();
        }
};
class Character{
private:
    int x;
    int y;
    int life;
    bool Set_bombs;
    bool imDead;
public:
    int X(){return x;}
    int Y(){return y;}
    int Life(){return life;}
    void ResetPos(int nx, int ny){
        x=nx; y=ny;
    }

    Character(int _x, int _y){
        x=_x;
        y=_y;
        Set_bombs=0;
        imDead=0;
        life=3;
    }

    bool UnDeath=0;
    int range=0;

    bool Move(char key){
        int nx=x, ny=y;
        switch(key){
            case UP: ny-=(range+1); break;
            case DOWN: ny+=(range+1); break;
            case LEFT: nx-=(range+1); break;
            case RIGHT: nx+=(range+1); break;
        }
        if(!OutofRange(nx,ny) and (nx!=x or ny!=y)){
            if(World_Map[x][y]=='C') World_Map[x][y]='.';
            x=nx; y=ny;
            Set_bombs=0;
            return 1;
        }
        return 0;
    }
    void AI_Move(int tx, int ty){
        int nx=x, ny=y, D;
        int Dx=tx-nx;
        int Dy=ty-ny;
        Dx=(Dx<0)? -1: 1;
        Dy=(Dy<0)? -1: 1;
        D=(abs(tx-nx)<abs(ty-ny))? 1: 0;
        if(abs(tx-nx)==1 and abs(ty-ny)==1) D=rand()%2;
        if(!abs(ty-ny)) D=1;
        else if(!abs(tx-nx)) D=0;
        int T=0, R=1;
        while(T<4){
            T++; if(T>2) R=-1;
            nx=x+D*Dx*R;
            ny=y+(D^1)*Dy*R;
            if(OutofRange(nx,ny) or Check_Map[nx][ny]){
                    D^=1;
                    continue;
            }
            if(World_Map[x][y]=='C') World_Map[x][y]='.';
            x=nx;
            y=ny;
            break;
        }
    }

    bool isDead(){
        return imDead;
    }
    void toDead(){
        if(!UnDeath) life--;
        DrawWhiteSpace(46,3,80,5);
        gotoxy(46,3); cout << "It hurts.";
        if(UnDeath) cout << " But Nothing.";
        if(!life) imDead=1;
    }
    bool toSet(){
        if(Set_bombs){
            DrawWhiteSpace(46,3,80,5);
            gotoxy(46,3);
            cout << "Had done.";
            return 0;
        }
        Set_bombs=1;
        return 1;
    }

};

class Enemy{
private:
    int x;
    int y;
    int type;
    bool imDead;
    int rest;
    int web[4];

public:
    deque<pair<int,int>> Step;
    int prx, pry;
    int deathcount;
    bool fiction=0;
    bool UnDeath=0;
    int paralyze=0;
    int relifecount=-1;

    int X(){return x;}
    int Y(){return y;}
    void Reset_Pos(int nx, int ny, int px, int py){
        x=nx; y=ny; prx=px; pry=py;
    }
    void toDead(){if(!UnDeath) imDead=1;}
    bool isDead(){
        if(imDead==1){
            if(World_Map[x][y]=='E') World_Map[x][y]='.';
        }
        return imDead;
    }
    Enemy(int _x, int _y, int _type){
        x=_x;
        y=_y;
        type=_type;
        imDead=0;
        rest=0;
        if(type==4){
            web[0]=x-4; web[2]=x+4;
            web[1]=y-4; web[3]=y+4;
        }
        deathcount=-1;
        prx=x; pry=y;
    }
    bool InWeb(int tx, int ty){
        return tx>=web[0] and tx<=web[2] and ty>=web[1] and ty<=web[3];
    }

    bool Move(int tx, int ty, bool toRest){
        if(paralyze){
            paralyze--; return 0;
        }
        Step.push_back({prx, pry});
        if(Step.size()>11) Step.pop_front();

        if(fiction){
            Obstacle_Map[MapNo][prx][pry]=0;
        }
        prx=x; pry=y;
        int nx=x, ny=y,D;
        int nowtype=type;
        if((type==2 or type==3) and (abs(tx-nx)<=3 and abs(ty-ny)<=3)) nowtype=1;
        if(type==4 and InWeb(tx,ty)) nowtype=1;
        if(nowtype==2){
            tx+=(tx>nx)? -2: 2;
            ty+=(ty>ny)? -2: 2;
        }
        int Dx=tx-nx;
        int Dy=ty-ny;
        Dx=(Dx<0)? -1: 1;
        Dy=(Dy<0)? -1: 1;
        if(toRest and !rest) rest=3;
        int T=0;
        switch(nowtype){
            case 1:
            case 2:
                D=(abs(tx-nx)<abs(ty-ny))? 1: 0;
                if(abs(tx-nx)==1 and abs(ty-ny)==1) D=rand()%2;
                if(!abs(ty-ny)) D=1;
                else if(!abs(tx-nx)) D=0;
                while(T<2){
                    T++;
                    if(rest){
                        Dx*=-1; Dy*=-1;
                    }
                    nx=x+D*Dx;
                    ny=y+(D^1)*Dy;
                    if(OutofRange(nx,ny) or Check_Map[nx][ny] or World_Map[nx][ny]=='S' or Item_Map[nx][ny]!=-1){
                            D^=1;
                            continue;
                    }
                    World_Map[x][y]='.';
                    x=nx;
                    y=ny;
                    if(rest) rest--;
                    if(Bomb_Map[nx][ny]) return 1;

                    World_Map[nx][ny]='E';
                    //if(rest) World_Map[nx][ny]='R';
                    //if(type==2) World_Map[nx][ny]='F';

                    Check_Map[nx][ny]=1;
                    break;
                }
                break;
            case 0:
                D=rand()%2;
                while(T<2){
                    T++;
                    if(rest){
                        Dx*=-1; Dy*=-1;
                    }
                    nx=x+D*Dx;
                    ny=y+(D^1)*Dy;
                    if(OutofRange(nx,ny) or Check_Map[nx][ny]or World_Map[nx][ny]=='S' or Item_Map[nx][ny]!=-1){
                            D^=1;
                            continue;
                    }
                    World_Map[x][y]='.';
                    x=nx;
                    y=ny;
                    if(rest) rest--;
                    if(Bomb_Map[nx][ny]){
                            return 1;
                    }

                    World_Map[nx][ny]='E';
                    //if(rest) World_Map[nx][ny]='R';

                    Check_Map[nx][ny]=1;
                    break;
                }
                break;
            case 3:
            case 4:
                int dx[4]={0,0,1,-1};
                int dy[4]={1,-1,0,0};
                int rnd[4]={0,1,2,3};
                random_shuffle(rnd,rnd+4);
                for(int i=0; i<4; i++){
                    nx=x+dx[rnd[i]]; ny=y+dy[rnd[i]];
                    if(OutofRange(nx,ny) or Check_Map[nx][ny]or World_Map[nx][ny]=='S' or Item_Map[nx][ny]!=-1){
                        continue;
                    }
                    if(fiction and World_Map[nx][ny]=='C') continue;
                    if(type==4 and !InWeb(nx,ny)) continue;
                    World_Map[x][y]='.';
                    x=nx;
                    y=ny;
                    if(Bomb_Map[nx][ny]){
                            return 1;
                    }

                    World_Map[nx][ny]='E';
                    //if(type==4) World_Map[nx][ny]='W';
                    //if(rest) World_Map[nx][ny]='R';
                    Check_Map[nx][ny]=1;
                    break;

                }
                break;
        }
        return 0;
    }

};
class Reward{
    private:
        int X;
        int Y;
        deque<pair<int,int>> reward;
    public:
        int PosX(){return reward[0].first;}
        int PosY(){return reward[0].second;}
        void Set_Reward(){
            int T=0;
            while(T<1e5){
                int x=rand()%21+1;
                int y=rand()%21+1;
                if(World_Map[x][y]!='.') continue;
                World_Map[x][y]='S';
                reward.push_back({x,y});
                break;
            }

        }

        void Get_Reward(int tx, int ty){
            int P;
            for(; P<reward.size(); P++){
                if(reward[P].first==tx and reward[P].second==ty) break;
            }
            if(P>=reward.size()) return;
            reward.erase(reward.begin()+P);
            Set_Reward();
        }
        void Erase_Reward(){
            for(int P=0; P<reward.size(); P++){
                int nx=reward[P].first, ny=reward[P].second;
                if(World_Map[nx][ny]=='S') World_Map[nx][ny]='.';
            }
            reward.clear();
        }

};
class Item{
    private:
        int x;
        int y;
        bool paralyze;
        deque<pair<int,int>> itemlist;
    public:
        int X(){return x;}
        int Y(){return y;}
        void StageMode(){paralyze=1;}
        int Checkitemsize(){return itemlist.size();}

        void Set_Item(int ax=-1, int ay=-1){
            if(ax==-1 and ay==-1){
                int T=0;
                while(T<1e5){
                    int nx=rand()%21+1;
                    int ny=rand()%21+1;
                    if(World_Map[nx][ny]!='.') continue;
                    World_Map[nx][ny]='I';
                    if(paralyze) World_Map[nx][ny]='P';
                    Item_Map[nx][ny]=1;
                    itemlist.push_back({nx,ny});
                    break;
                }
            }else{
                if(World_Map[ax][ay]!='.') return;
                World_Map[ax][ay]='I';
                Item_Map[ax][ay]=1;
                itemlist.push_back({ax,ay});
            }

        }

        void Get_Item(int tx, int ty){
            int P;
            for(; P<itemlist.size(); P++){
                if(itemlist[P].first==tx and itemlist[P].second==ty) break;
            }
            if(P>=itemlist.size()) return;
            itemlist.erase(itemlist.begin()+P);
            Item_Map[tx][ty]=-1;
        }
        void Erase_Item(bool reset=0){
            for(int P=0; P<itemlist.size(); P++){
                int nx=itemlist[P].first, ny=itemlist[P].second;
                Item_Map[nx][ny]=-1;
                if(World_Map[nx][ny]=='I') World_Map[nx][ny]='.';
                if(reset) DrawWorldMap(nx-1, ny-1, (nx-1)*2+1, ny+3);
            }
            itemlist.clear();
        }

};
struct Object{
    int X,Y; //Position
    char Type;
    int EType;
};

Game_Mode gamemode;
Game_Data data;
deque<Enemy> E;
Item Itemlist;
deque<pair<int,int>> Q;
Reward SS;
Character Ch= Character(11,11);
vector<vector<Object>> StageModeData;
int NowStage=0;
void Rand_Position(int tx, int ty);

class Ability{
    // No item
    private:
        int abilitymode;
        int skill=2;
        int Effect_13=-1;
        int Effect_14= 0;
        int Effect_15=-1;
        int Effect_16= 0;
        int Effect_21=-1;
        int Effect_22=-1;
        int Effect_24=-1;
        int NowLife_24=0;
        int Web_24[2];
        int Effect_25=-1;
        int Effect_26=0;
        int Effect_31=-1;
        int grid_33[22][22];
        int Effect_34=-1;
        int grid_35[22][22];
        bool getitem=0;
        bool getreward=0;
    public:

        int ABM(){return abilitymode;}
        bool hadDone=0;
        void GetData(){abilitymode=gamemode.AB();}

        void GetItem(){skill++; if(skill>5) skill=5; getitem=1;}
        void GetReward(){getreward=1;}
        void DrawSkillLimit(){
            if(abilitymode==11) return;
            DrawWhiteSpace(0,1,12,2);
            gotoxy(0,1); cout << "Skill:";
            SetColor(119);
            for(int i=0; i<skill; i++){
                gotoxy(6+i,1); cout << ' ';
            }
            SetColor();
        }
        void DrawWorldMap(int x, int y, int color, char ch){
            x--; y--;
            //char ch=World_Map[x+1][y+1];
            gotoxy(1+x*2, 4+y);
            SetColor(color);
            cout << ch;
            SetColor();
        }
        void DrawEffect(){
            DrawSkillLimit();
            switch(abilitymode){
                case 13:
                    if(Effect_13!=-1){
                        DrawWorldMap(Ch.X(), Ch.Y(), 3, 'C');
                    }
                    break;
                case 15:
                    if(Effect_15!=-1){
                        for(int i=0; i<E.size(); i++){
                            DrawWorldMap(E[i].X(), E[i].Y(), 13, 'E');
                            if(E[i].X()==Ch.X() and E[i].Y()==Ch.Y())
                                DrawWorldMap(Ch.X(), Ch.Y(), 75, 'C');
                        }
                    }
                    break;
                case 16:
                    for(int i=0; i<E.size(); i++){
                        if(E[i].paralyze) DrawWorldMap(E[i].X(), E[i].Y(), 13, 'E');
                    }
                    break;
                case 21:
                    for(int i=0; i<E.size(); i++){
                        if(E[i].deathcount!=-1){
                            DrawWorldMap(E[i].prx, E[i].pry, 13, 'E');
                        }
                        if(E[i].deathcount==1) DrawWorldMap(E[i].prx, E[i].pry, 173, 'E');
                    }
                    break;
                case 22:
                    if(Effect_22!=-1){
                        for(int i=0; i<E.size(); i++){
                            if(World_Map[E[i].prx][E[i].pry]=='.')
                                DrawWorldMap(E[i].prx, E[i].pry, 7, '.');
                            if(World_Map[Ch.X()][Ch.Y()]=='E')
                                DrawWorldMap(Ch.X(), Ch.Y(), 75, 'C');
                            else if(World_Map[E[i].X()][E[i].Y()]=='E')
                                DrawWorldMap(E[i].X(), E[i].Y(), 13, 'E');
                        }
                    }
                    break;
                case 23:
                    for(int i=0; i<E.size(); i++){
                        if(E[i].fiction and E[i].deathcount!=0){
                            DrawWorldMap(E[i].prx, E[i].pry, 9, 'E');
                        }
                    }
                    break;
                case 24:
                    for(int i=1; i<=21; i++){
                        for(int s=1; s<=21; s++){
                            if(Obstacle_Map[MapNo][i][s]>=2)
                                DrawWorldMap(i,s,255,' ');
                        }
                    }
                    break;
                case 25:
                    if(Effect_25!=-1)
                        DrawWorldMap(Ch.X(), Ch.Y(), 3, 'C');
                    break;
                case 26:
                    if(Effect_26){
                        for(int i=0; i<E.size(); i++){
                            DrawWorldMap(E[i].prx, E[i].pry, 7, '.');
                        }
                    }
                    break;
                case 31:
                    for(int i=0; i<E.size(); i++){
                        if(E[i].UnDeath) DrawWorldMap(E[i].prx, E[i].pry, 5, 'E');
                    }
                    if(Effect_31!=-1){
                        for(int i=0; i<E.size(); i++){
                            if(!E[i].UnDeath)
                                DrawWorldMap(E[i].prx, E[i].pry, 252, 'E');
                        }
                    }
                    break;
                case 33:
                    for(int i=0; i<E.size(); i++){
                        DrawWorldMap(E[i].prx, E[i].pry, 7, '.');
                    }
                    for(int i=1; i<=21; i++){
                        for(int s=1; s<=21; s++){
                            if(grid_33[i][s]){
                                if(World_Map[i][s]=='C')
                                    DrawWorldMap(i,s,123,'C');
                                else if(World_Map[i][s]=='S')
                                    DrawWorldMap(i,s,126,'S');
                                else
                                    DrawWorldMap(i,s,119,' ');
                            }
                        }
                    }
                    break;
                case 34:
                    if(Effect_34!=-1){
                        for(int i=0; i<E.size(); i++)
                            DrawWorldMap(E[i].prx, E[i].pry, 15, 'E');
                    }
                    break;
            }
        }
        void Effecton(){
            DrawWhiteSpace(46,3,80,5);
            gotoxy(46,3); cout << "Ability on.";
        }
        void Effectoff(){
            DrawWhiteSpace(46,3,80,5);
            gotoxy(46,3); cout << "Ability off.";
        }
        void Active(){
            switch(abilitymode){
                case 13:
                    if(Effect_13!=-1) return;
                    if(hadDone) return;
                    if(skill<4) return; skill-=4;
                    Effecton(); hadDone=1;
                    Effect_13=1;
                    Ch.UnDeath=1;
                    break;
                case 14:
                    if(skill<1) return; skill--;
                    Ch.range++; Effect_14++;
                    Effecton(); cout << " level:" << Effect_14;
                    DrawEffect();
                    break;
                case 15:
                    if(Effect_15!=-1) return;
                    if(hadDone) return;
                    if(skill<2) return; skill-=2;
                    Effecton();
                    Effect_15=1; hadDone=1;
                    DrawEffect();
                    Effect_15=-1;
                    break;
                case 16:
                    if(skill<2) return;
                    if(hadDone){
                        if(skill==3){Effect_16=1; skill=0;}
                        else return;
                    }else
                        skill-=2;
                    hadDone=1; Effecton();
                    if(Effect_16) cout << " Level Max.";
                    for(int i=0; i<E.size(); i++){
                        int nx=E[i].X(), ny=E[i].Y();
                        int px=E[i].prx, py=E[i].pry;
                        if(px>=Ch.X()-2 and px<=Ch.X()+2 and py>=Ch.Y()-2 and py<=Ch.Y()+2){
                            E[i].paralyze=5;
                        }
                        if(Effect_16) E[i].paralyze=5;
                    }
                    Effect_16=0;
                    break;
                case 21:
                    if(Effect_21!=-1) return;
                    if(hadDone) return;
                    if(skill<3) return; skill-=3;
                    Effecton(); hadDone=1;
                    Effect_21=20;
                    Itemlist.Erase_Item();
                    for(int i=0; i<E.size(); i++){
                        E[i].deathcount=20;
                    }
                    break;
                case 23:
                    if(skill<3) return; skill-=3;
                    Effecton(); hadDone=1;
                    for(int i=0; i<10; i++){
                        int T=0;
                        while(T<1e5){
                            int x=rand()%21+1;
                            int y=rand()%21+1;
                            if(World_Map[x][y]!='.') continue;
                            World_Map[x][y]='E';
                            E.push_back(Enemy(x,y,4));
                            E.back().deathcount=20;
                            E.back().fiction=1;
                            break;
                        }
                    }
                    break;
                case 24:
                    if(Effect_24!=-1) return;
                    if(hadDone) return;
                    if(skill<3) return; skill-=3;
                    hadDone=1; Effecton(); Effect_24=10;
                    Web_24[0]=Ch.X(); Web_24[1]=Ch.Y();
                    for(int i=Ch.X()-5; i<=Ch.X()+5; i++){
                        for(int s=Ch.Y()-5; s<=Ch.Y()+5; s++){
                            if(i>=1 and i<=21 and s>=1 and s<=21)
                                Obstacle_Map[MapNo][i][s]+=2;
                        }
                    }
                    for(int i=Ch.X()-4; i<=Ch.X()+4; i++){
                        for(int s=Ch.Y()-4; s<=Ch.Y()+4; s++){
                            if(i>=1 and i<=21 and s>=1 and s<=21)
                                Obstacle_Map[MapNo][i][s]-=2;
                        }
                    }
                    for(int i=0; i<E.size(); i++){
                        if(E[i].X()>=Ch.X()-4 and E[i].X()<=Ch.X()+4 and E[i].Y()>=Ch.Y()-4 and E[i].Y()<=Ch.Y()+4)
                            continue;
                        E[i].paralyze=10;
                    }
                    NowLife_24=Ch.Life();
                    break;
                case 25:
                    if(Effect_25==-1 and hadDone) return;
                    if(Effect_25==-1){
                        Effecton(); hadDone=1;
                        Ch.UnDeath=1; Effect_25=30;
                    }else if(Effect_25!=-1 and skill==5){
                        Effectoff();
                        skill=0;
                        Ch.UnDeath=0;
                        Effect_25=-1;
                    }
                    hadDone=1;
                    break;
                case 31:
                    if(hadDone) return;
                    if(skill<=0) return; skill--;
                    hadDone=1; Effect_31=1;
                    DrawEffect();
                    for(int i=0; i<E.size(); i++){
                        E[i].toDead();
                        if(!E[i].UnDeath) data.Get_Score();
                    }
                    Effect_31=-1;
                    Rand_Position(Ch.X(), Ch.Y());
                    E.back().UnDeath=1;
                    break;
                case 34:
                    if(Effect_34!=-1) return;
                    if(hadDone) return;
                    if(skill<5) return; skill=0;
                    Effect_34=1; hadDone=1; Effecton();
                    SS.Erase_Reward();
                    Ch.UnDeath=1;
                    break;
            }
        }

        void Passive(){
            hadDone=0;
            DrawEffect();
            switch(abilitymode){
                case 13:
                    if(Effect_13!=-1){
                        Effect_13=-1;
                        Ch.UnDeath=0;
                    }
                    break;
                case 14:
                    if(Effect_14){
                        Effect_14=0;
                        Ch.range=0;
                    }
                    break;
                case 21:
                    if(Effect_21!=-1) DrawWorldMap(Ch.X(), Ch.Y(), 43, 'C');
                    for(int i=0; i<E.size(); i++){
                        if(E[i].deathcount!=-1){
                            E[i].deathcount--;
                            if(!E[i].deathcount){
                                E[i].toDead();
                                data.Get_Score();
                            }
                        }
                    }
                    if(Effect_21==-1) break;
                    if(getitem){Effect_21=-1; break;}
                    Effect_21--;
                    if(Effect_21==0){
                        Ch.toDead();
                        data.LifeDecrease();
                        Effect_21=-1;
                    }
                    break;
                case 22:
                    if(skill==5){skill=0; Effect_22=1; Itemlist.Erase_Item();}
                    if(Effect_22!=-1){
                        if(skill==3){skill=0; Effect_22=-1; break;}
                    }
                    break;
                case 23:
                    for(int i=0; i<E.size(); i++){
                        if(E[i].deathcount) E[i].deathcount--;
                        if(!E[i].deathcount){
                            E[i].toDead();
                            if(Obstacle_Map[MapNo][E[i].prx][E[i].pry])
                                Obstacle_Map[MapNo][E[i].prx][E[i].pry]=0;
                        }
                        else if(E[i].fiction){
                            Check_Map[E[i].prx][E[i].pry]=0;
                            Check_Map[E[i].X()][E[i].Y()]=0;
                            Obstacle_Map[MapNo][E[i].prx][E[i].pry]=1;
                        }
                    }
                    break;
                case 24:
                    if(Effect_24==-1) break;
                    Effect_24--;
                    if(!Effect_24 or NowLife_24!=Ch.Life()){
                        Effect_24=-1;
                        for(int i=Web_24[0]-5; i<=Web_24[0]+5; i++){
                            for(int s=Web_24[1]-5; s<=Web_24[1]+5; s++){
                                if(i>=1 and i<=21 and s>=1 and s<=21)
                                    Obstacle_Map[MapNo][i][s]-=2;
                            }
                        }
                        for(int i=Web_24[0]-4; i<=Web_24[0]+4; i++){
                            for(int s=Web_24[1]-4; s<=Web_24[1]+4; s++){
                                if(i>=1 and i<=21 and s>=1 and s<=21)
                                    Obstacle_Map[MapNo][i][s]+=2;
                            }
                        }
                        DrawWindowFrame(0,0,43,22,0);
                        if(NowLife_24==Ch.Life()){
                            for(int i=0; i<E.size(); i++){
                                if(E[i].X()>=Web_24[0]-4 and E[i].X()<=Web_24[0]+4 and E[i].Y()>=Web_24[1]-4 and E[i].Y()<=Web_24[1]+4){
                                    E[i].toDead();
                                    DrawWorldMap(E[i].X(), E[i].Y(), 124, 'E');
                                    data.Get_Score();
                                }
                            }
                        }
                        for(int i=0; i<E.size(); i++) E[i].paralyze=0;
                    }
                    break;
                case 25:
                    if(Effect_25==-1) break;
                    Effect_25--;
                    if(!Effect_25){
                        Ch.UnDeath=0;
                        Effect_25=-1;
                        Ch.toDead();
                    }
                    break;
                case 26:
                    Effect_26++; Effect_26%=5;
                    if(skill==5){
                        skill=0;
                        for(int i=0; i<2; i++) data.Get_Score();
                    }
                    break;
                case 32:
                    skill=0;
                    Itemlist.Erase_Item(1);
                    for(int i=0; i<E.size(); i++){
                        if(E[i].relifecount==-1) E[i].relifecount=0;
                        if(E[i].relifecount>0){
                                E[i].relifecount--;
                                Check_Map[E[i].X()][E[i].Y()]=0;
                                if(E[i].relifecount>0)
                                    DrawWorldMap(E[i].X(), E[i].Y(), 7, '.');
                                if(E[i].relifecount==0)
                                    DrawWorldMap(E[i].prx, E[i].pry, 12, 'E');
                        }

                        if(E[i].Step.size()<10 or E[i].relifecount>0) continue;
                        int nx=E[i].Step[1].first;
                        int ny=E[i].Step[1].second;
                        if(getitem and Ch.X()==E[i].Step.front().first and Ch.Y()==E[i].Step.front().second){
                                E[i].relifecount=15;
                                E[i].Step.clear();
                                E[i].paralyze=14;
                                World_Map[E[i].X()][E[i].Y()]='.';
                                DrawWorldMap(E[i].prx, E[i].pry, 124, 'E');
                                data.Get_Score();
                        }
                        else
                            Itemlist.Set_Item(nx, ny);
                    }
                    for(int i=1; i<=21; i++){
                        for(int s=1; s<=21; s++){
                            if(Check_Map[i][s]) continue;
                            if(Item_Map[i][s]!=-1 and World_Map[i][s]=='I'){
                                DrawWorldMap(i,s,8,'E');
                            }
                        }
                    }
                    for(int i=0; i<E.size(); i++){
                        if(E[i].relifecount==1){
                            if(World_Map[E[i].X()][E[i].Y()]=='C')
                                DrawWorldMap(E[i].X(), E[i].Y(), 43, 'C');
                            else if(World_Map[E[i].X()][E[i].Y()]=='S'){
                                DrawWorldMap(E[i].X(), E[i].Y(), 34, ' ');
                                SS.Get_Reward(E[i].X(), E[i].Y());
                            }else if(World_Map[E[i].X()][E[i].Y()]=='I')
                                DrawWorldMap(E[i].X(), E[i].Y(), 40, 'E');
                            else if(World_Map[E[i].X()][E[i].Y()]=='E')
                                DrawWorldMap(E[i].X(), E[i].Y(), 44, 'E');
                            else
                                DrawWorldMap(E[i].X(), E[i].Y(), 34, ' ');
                        }
                    }
                    break;
                case 33:
                    for(int i=1; i<=21; i++){
                        for(int s=1; s<=21; s++)
                            grid_33[i][s]=0;
                    }
                    for(int i=0; i<E.size(); i++){
                        int nx=E[i].X(), ny=E[i].Y();
                        int px=E[i].prx, py=E[i].pry;
                        grid_33[E[i].prx][E[i].pry]=1;
                        grid_33[nx][ny]=1;
                        for(int s=0; s<2; s++){
                            E[i].Move(Ch.X(), Ch.Y(), 0);
                            if(World_Map[E[i].X()][E[i].Y()]=='E')
                                World_Map[E[i].X()][E[i].Y()]='.';
                            Check_Map[E[i].X()][E[i].Y()]=0;
                            grid_33[E[i].X()][E[i].Y()]=1;
                        }
                        E[i].Reset_Pos(nx,ny,px,py);
                        Check_Map[nx][ny]=1;
                        Check_Map[px][py]=1;
                    }
                    if(skill==5){
                        for(int i=0; i<E.size(); i++)
                            DrawWorldMap(E[i].prx,E[i].pry,12,'E');
                        skill=0;
                    }
                    break;
                case 34:
                    if(Effect_34==-1) break;
                    if(getitem){
                        Effect_34=-1;
                        Ch.UnDeath=0;
                        break;
                    }
                    Effect_34++; Effect_34%=5;
                    if(Effect_34==2){
                        Itemlist.Erase_Item();
                        Itemlist.Set_Item();
                    }
                    DrawWhiteSpace(46,3,80,5);
                    gotoxy(46,3); cout << "So, why are you standing here?";
                    break;
                case 35:
                    skill=0;
                    Itemlist.Erase_Item();
                    if(getreward){
                        memset(grid_35, 0, sizeof(grid_35));
                        memset(Check_Map, 0, sizeof(Check_Map));
                        memset(World_Map, '.', sizeof(World_Map));
                        for(int i=1; i<=21; i++){
                            for(int s=1; s<=21; s++){
                                if(Obstacle_Map[MapNo][i][s]){
                                    World_Map[i][s]='W';
                                    grid_35[i][s]=1;
                                }
                            }
                        }
                        SS.Erase_Reward();
                        int rx=rand()%21+1;
                        int ry=rand()%21+1;
                        World_Map[rx][ry]='C';
                        Ch.ResetPos(rx,ry);
                        for(int i=0; i<E.size(); i++){
                            while(1){
                                rx=rand()%21+1; ry=rand()%21+1;
                                if(grid_35[rx][ry]) continue;
                                grid_35[rx][ry]=1;
                                World_Map[rx][ry]='E';
                                Check_Map[rx][ry]=1;
                                E[i].Reset_Pos(rx, ry, rx, ry);
                                break;
                            }
                        }
                        SS.Set_Reward();
                        DrawWindowFrame(0,0,43,22,0);
                    }
                    break;

            }

            getitem=0; getreward=0;
        }

};

Ability GameAbility;

void WriteIn(){
    std::ifstream ifs("StageModeData.txt", std::ios::in);
    vector<Object> tmp;
    for(int i=0; i<=100; i++) StageModeData.push_back(tmp);
    int No, ObjectAmount;
    while(ifs >> No >> ObjectAmount){
        Object Ob;
        tmp.clear();
        if(ObjectAmount==-1){
            char ch;
            for(int y=1; y<22; y++){
                for(int x=1; x<22; x++){
                    ifs >> ch;
                    if(ch=='1') Obstacle_Map[No][x][y]=1;
                }
            }
            continue;
        }
        while(ObjectAmount--){
            ifs >> Ob.X >> Ob.Y >> Ob.Type;
            if(Ob.Type=='E') ifs >> Ob.EType;
            tmp.push_back(Ob);
        }
        StageModeData[No]=tmp;
    }
    ifs.close();
    std::ifstream ifs2("AbilityName.txt", std::ios::in);
    int T; string SS;
    for(int i=0; ifs2>>T; i++){
        while(T--){
            ifs2>>SS;
            Ability_Name[i].push_back(SS);
        }
    }
    ifs2.close();
    //gotoxy(25,60); cout << "Done";
}
void SetPosition(int x, int y, char ch){
    World_Map[x][y]=ch;
}
void Reset_Map(){
    //Initial
    if(gamemode.Stage()) NowStage++;
    data.Stage=NowStage;
    for(int r=1; r<=21; r++){
        for(int c=1; c<=21; c++){
            World_Map[r][c]='.';
            if(Obstacle_Map[MapNo][r][c]) World_Map[r][c]='W';
            Item_Map[r][c]=-1;
            Bomb_Map[r][c]=0;
        }
    }
    E.clear();

    for(int i=0; i<StageModeData[NowStage].size(); i++){
        int x=StageModeData[NowStage][i].X;
        int y=StageModeData[NowStage][i].Y;
        char type=StageModeData[NowStage][i].Type;
        char Etype;
        SetPosition(x,y,type);
        if(type=='C') Ch=Character(x,y);
        if(type=='E'){
            Etype=StageModeData[NowStage][i].EType;
            E.push_back(Enemy(x,y,Etype));
        }
    }

    SS.Erase_Reward();
    Itemlist.Erase_Item();
    for(int i=0; i<2; i++) SS.Set_Reward();
    data.score=0;
    data.Enemy_count=E.size();
    data.life=3;
    DrawGameLimits(gamemode.Stage());
    if(!gamemode.Stage()) data.Bomb_count=1;

}



void Rand_Position(int tx, int ty){
    bool grid[22][22]={};
    int ETypeRand[10]={0,1,2,2,3,3,3,4,4,4};
    for(int i=tx-4; i<=tx+4; i++){
        for(int s=ty-4; s<=ty+4; s++){
            if(OutofRange(i,s)) continue;
            grid[i][s]=1;
        }
    }
    int T=0;
    while(T<1e5){
        int x=rand()%21+1;
        int y=rand()%21+1;
        if(World_Map[x][y]!='.' or grid[x][y]) continue;
        World_Map[x][y]='E';
        int tmp=(E.size()>=2)? rand()%10: rand()%3;
        E.push_back(Enemy(x,y,ETypeRand[tmp]));
        DrawWhiteSpace(46,3,80,5);
        gotoxy(46,3);
        cout << "New enemy appeared.";
        break;
    }
}

void Set_Bomb(int x, int y){
    World_Map[x][y]='O';
    Bomb_Map[x][y]=1;
    DrawWhiteSpace(46,3,80,5);
    gotoxy(46,3);
    cout << "Bomb has been set.";
}
void Erase_Bomb(int x, int y){
    World_Map[x][y]='.';
    Bomb_Map[x][y]=0;
}

int main(){
    //Start
    srand(time(NULL));
    HideCursor();
    WriteIn();
    WelcomeMessage();
    TipsMessage();
    gamemode.SetGameMode();
    GameAbility.GetData();
    Reset_Map();

    int explode=0;
    int AIturn=0, a=50, b=5;
    int Paralyze=0;
    bool suicide=0;

    while(!Ch.isDead()){
        //random part
        if(!gamemode.Stage()){
            int R=rand()%gamemode.ER();
            if(!R) Rand_Position(Ch.X(), Ch.Y());
            if(E.size()<gamemode.EL()) Rand_Position(Ch.X(), Ch.Y());
            R=rand()%gamemode.BR();
            if(!R and Itemlist.Checkitemsize()<2){
                Itemlist.Set_Item();
            }else if(Itemlist.Checkitemsize()<gamemode.BL()){
                Itemlist.Set_Item();
            }
        }else{
            int R=rand()%gamemode.BR();

            if(Itemlist.Checkitemsize()<gamemode.BL() and !R){
                Itemlist.StageMode();
                Itemlist.Set_Item();
            }
        }

        data.Enemy_count=E.size();
        data.life=Ch.Life();
        data.Output();

        //Initial
        for(int i=0; i<22; i++){
            for(int s=0 ;s<22; s++){
                Check_Map[i][s]=0;
            }
        }

        for(int i=0; i<E.size(); i++){
            Check_Map[E[i].X()][E[i].Y()]=1;
        }
        if(Paralyze<=0){
            int Ex, Ey;
            for(int i=0; i<E.size(); i++){
                if(E[i].Move(Ch.X(), Ch.Y(), rand()%gamemode.RR()==0)){
                    Ex=E[i].X(); Ey=E[i].Y();
                    Q.push_back({Ex,Ey});
                    E.erase(E.begin()+i);
                    i--;
                    explode++;
                }
            }
        }else{
            Paralyze--;
            for(int i=0; i<E.size(); i++){
                World_Map[E[i].X()][E[i].Y()]='E';
                E[i].prx=E[i].X(); E[i].pry=E[i].Y();
            }
        }

        GameAbility.Passive();

        /*
        int ax=50, ay=5;
        for(int i=1; i<=21; i++){
            for(int s=1; s<21; s++){
                gotoxy(ax+i, ay+s);
                cout << Check_Map[i][s];
            }
        }
        */


        //Character's move
        char key;
        while(!gamemode.AI()){
            key=getch();
            if(key==' ' and !gamemode.Stage()){
                //Bomb
                if(GameAbility.ABM()==11){
                    if(data.Bomb_count<=0){
                        DrawWhiteSpace(46,3,80,5);
                        gotoxy(46,3);
                        cout << "No bomb.";
                    }else if(Ch.toSet()){
                        Set_Bomb(Ch.X(), Ch.Y());
                        data.Use_Bomb();
                    }
                    continue;
                }

                GameAbility.Active();
                continue;
            }
            if(Ch.Move(key)){
                    DrawWhiteSpace(46,3,80,5);
                    break;
            }
            else{
                DrawWhiteSpace(46,3,80,5);
                gotoxy(46,3);

                cout << "Can't move.";
            }
        }
        if(gamemode.AI()){
            Ch.AI_Move(SS.PosX(), SS.PosY());
            Sleep(50);
            DrawWhiteSpace(46,3,80,5);
        }
        if(Check_Map[Ch.X()][Ch.Y()] or Bomb_Map[Ch.X()][Ch.Y()]){
            if(gamemode.AI() and AIturn<30){
                gotoxy(a,b++); cout << data.score << "   " << data.Enemy_count;
                if((AIturn+1)%10==0){
                    a+=9; b=5;
                }
                Reset_Map();
                AIturn++;
                continue;
            }else Ch.toDead();
            World_Map[Ch.X()][Ch.Y()]='C';
            data.LifeDecrease();
            Paralyze=1;
            if(Bomb_Map[Ch.X()][Ch.Y()]){
                Q.push_back({Ch.X(), Ch.Y()});
                suicide=1;
            }
        }else{
            if(World_Map[Ch.X()][Ch.Y()]=='S'){
                SS.Get_Reward(Ch.X(), Ch.Y());
                data.Get_Score();
                GameAbility.GetReward();
            }
            if(World_Map[Ch.X()][Ch.Y()]=='I'){
                Itemlist.Get_Item(Ch.X(), Ch.Y());
                if(GameAbility.ABM()==11) data.Get_Bomb();
                else GameAbility.GetItem();
            }
            if(World_Map[Ch.X()][Ch.Y()]=='P'){
                Paralyze=5;
                Itemlist.Get_Item(Ch.X(), Ch.Y());
                gotoxy(46,3);
                cout << "Enemies got paralyzed.";
            }

            World_Map[Ch.X()][Ch.Y()]='C';
        }
        if(gamemode.Stage() and data.score>=5){

            DrawWindowFrame(0,0,43,22,0);
            data.Output();
            gotoxy(46,3); cout << "Congratulations.";
            Sleep(3000);
            Reset_Map();
        }


        for(int i=0; i<E.size(); i++){
            if(Bomb_Map[E[i].X()][E[i].Y()]){
                Q.push_back({E[i].X(),E[i].Y()});
                E[i].toDead();
                explode++;
            }
        }
        if(!Q.empty()){
                if(explode) data.Explode(explode);
                for(int i=0; i<explode; i++) data.Get_Score();
                while(!Q.empty()){
                    Erase_Bomb(Q.front().first, Q.front().second);
                    Q.pop_front();
                }
        }
        for(int i=0; i<E.size(); i++){
            if(E[i].relifecount!=-1) continue;
            if(E[i].isDead()) E.erase(E.begin()+i);
        }
        explode=0;
        if(suicide) World_Map[Ch.X()][Ch.Y()]='C';
        if(Ch.isDead()) World_Map[Ch.X()][Ch.Y()]='X';
        suicide=0;

        DrawWindowFrame(0,0,43,22,0);
    }

    DrawWhiteSpace(46,3,80,5);
    gotoxy(46,3); cout << "You died.";
    Sleep(5000); if(gamemode.AI()) getch();
    DrawWhiteSpace(0,0,80,26);
    gotoxy(20,10); cout << "Game Over";
    Sleep(3000);
    return 0;
}
