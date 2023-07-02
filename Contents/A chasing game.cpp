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
bool Obstacle_Map[10][22][22]={};
bool Check_Map[22][22]={};
int Item_Map[22][22]={};
bool Bomb_Map[22][22]={};
int MapNo=0;
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
        case 'B': SetColor(10); break;
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
    gotoxy(x-20, y+3); cout << "'C'=You, 'B'=Bomb, 'E'=Enemy, 'S'=Score, 'O'=Set Bomb";
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
    public:
        GameMode(int _mode){mode=_mode; ai=0; stage=0;}

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
        }

};
class Game_Data{
    public:
        int score;
        int Bomb_count;
        int Enemy_count;
        int Stage;

        void Output(){
            DrawWhiteSpace(18,1,20,2);
            DrawWhiteSpace(27,1,29,2);

            if(Stage>0){
                gotoxy(8,1);
                cout << Stage;
            }else{
                gotoxy(7,1);
                cout << Bomb_count;
            }
            gotoxy(18,1); cout << Enemy_count;
            gotoxy(27,1); cout << score;
        }
        void Get_Score(){
            score++;
        }
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
    bool Set_bombs;
    bool imDead;
public:
    int X(){return x;}
    int Y(){return y;}

    Character(int _x, int _y){
        x=_x;
        y=_y;
        Set_bombs=0;
        imDead=0;
    }

    bool Move(char key){
        int nx=x, ny=y;
        switch(key){
            case UP: ny--; break;
            case DOWN: ny++; break;
            case LEFT: nx--; break;
            case RIGHT: nx++; break;
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
        imDead=1;
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
    int X(){return x;}
    int Y(){return y;}

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
    }
    bool InWeb(int tx, int ty){
        return tx>=web[0] and tx<=web[2] and ty>=web[1] and ty<=web[3];
    }

    bool Move(int tx, int ty, bool toRest){
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
    public:
        int PosX(){return X;}
        int PosY(){return Y;}
        void Set_Reward(){
            int T=0;
            while(T<1e5){
                int x=rand()%21+1;
                int y=rand()%21+1;
                if(World_Map[x][y]!='.') continue;
                World_Map[x][y]='S';
                X=x; Y=y;
                break;
            }

        }

        void Get_Reward(){
            Set_Reward();
        }

};
class Bomb{
    private:
        int x;
        int y;
        int num;
        bool paralyze;
    public:
        int X(){return x;}
        int Y(){return y;}
        void StageMode(){paralyze=1;}

        Bomb(int _x, int _y){
            x=_x;
            y=_y;
        }
        void Map_Bomb(){
            int T=0;
            while(T<1e5){
                int nx=rand()%21+1;
                int ny=rand()%21+1;
                if(World_Map[nx][ny]!='.') continue;
                World_Map[nx][ny]='B';
                if(paralyze) World_Map[nx][ny]='P';
                Item_Map[nx][ny]=num;
                x=nx; y=ny;
                break;
            }
        }
        void Get_Bomb(){
            Item_Map[x][y]=-1;
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
deque<Bomb> B;
deque<pair<int,int>> Q;
Reward SS;
Character Ch= Character(11,11);
vector<vector<Object>> StageModeData;
int NowStage=0;

void WriteIn(){
    std::ifstream ifs("StageModeData.txt", std::ios::in);
    if(!ifs.is_open()){
        gotoxy(0,0);
        cout << "Failed to open";
        return;
    }
    vector<Object> tmp;
    for(int i=0; i<=100; i++) StageModeData.push_back(tmp);
    int No, ObjectAmount;
    while(ifs >> No >> ObjectAmount){
        Object Ob;
        tmp.clear();
        if(ObjectAmount==-1){
            char ch;
            for(int x=1; x<22; x++){
                for(int y=1; y<22; y++){
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
    B.clear();

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

    SS.Set_Reward();
    data.score=0;
    data.Enemy_count=E.size();
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
    WelcomeMessage();
    TipsMessage();
    gamemode.SetGameMode();
    WriteIn();
    Reset_Map();

    int explode=0;
    int AIturn=0, a=50, b=5;
    int Paralyze=0;

    while(!Ch.isDead()){
        //random part
        if(!gamemode.Stage()){
            int R=rand()%gamemode.ER();
            if(!R) Rand_Position(Ch.X(), Ch.Y());
            if(E.size()<gamemode.EL()) Rand_Position(Ch.X(), Ch.Y());
            R=rand()%gamemode.BR();
            if(!R and B.size()<2){
                B.push_back(Bomb(0,0));
                B.back().Map_Bomb();
            }else if(B.size()<gamemode.BL()){
                B.push_back(Bomb(0,0));
                B.back().Map_Bomb();
            }
        }else{
            int R=rand()%gamemode.BR();
            if(B.size()<gamemode.BL() and !R){
                B.push_back(Bomb(0,0));
                B.back().StageMode();
                B.back().Map_Bomb();
            }
        }



        data.Enemy_count=E.size();
        data.Output();

        //Initial
        for(int i=0; i<22; i++){
            for(int s=0 ;s<22; s++)
                Check_Map[i][s]=0;
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
        }

        //Character's move
        char key;
        while(!gamemode.AI()){
            key=getch();
            if(key==' ' and !gamemode.Stage()){
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
            World_Map[Ch.X()][Ch.Y()]='X';
        }else{
            if(World_Map[Ch.X()][Ch.Y()]=='S'){
                SS.Get_Reward();
                data.Get_Score();
            }
            if(World_Map[Ch.X()][Ch.Y()]=='B'){
                int P=0;
                for(; P<2; P++){
                    if(B[P].X()==Ch.X() and B[P].Y()==Ch.Y())
                        break;
                }
                B[P].Get_Bomb();
                data.Get_Bomb();
                B.erase(B.begin()+P);
            }
            if(World_Map[Ch.X()][Ch.Y()]=='P'){
                B[0].Get_Bomb();
                Paralyze=5;
                B.erase(B.begin());
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
                E.erase(E.begin()+i);
                i--;
                explode++;
            }
        }
        if(explode){
                data.Explode(explode);
                for(int i=0; i<explode; i++) data.Get_Score();
                while(!Q.empty()){
                    Erase_Bomb(Q.front().first, Q.front().second);
                    Q.pop_front();
                }
        }
        explode=0;

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
