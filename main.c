#include "raylib.h" // I've changed my distro to use this library. It was worth it.
#include <math.h>
#include <stdlib.h>
#include <time.h> //for CLOCKS_PER_SECOND parameter

typedef struct{
    //constants
    int length; //the distance between the circles
    int radii; //radius of the circle, "radii" sounds cooler
    float frequence; //the frequence of the revolution relative to the spoke below it
    Color color;
    bool isHub;

    //variables
    Vector2 pos; //circle's absolute position
    double angle; //angle relative to the spoke below it
    Vector2 disp;
} circle;

//linkedList related types and functions
typedef struct mem{
    struct mem* nextmem;
    circle spec;
} mem;

typedef struct {
    mem* hub;
    int size;
} list;
void listAdd(list* circs, circle circ);
void listRemove(list* circs);
circle* getSpoke(list* circs, int index);

//random attribute pickers
void circleMaker(list* circs);
Color colorPick(void);
int radPick(void);
int lenPick(void);
double freqPick(void);

void posUp(list* circs); //updater

void DrawTreeExt(list* circs);//tree drawer

//toggle parameters
bool circDebug=false;
bool optimize=false;
bool zoom=true;

double scrollX = 10.0;

int main(void){

    SetTargetFPS(240);

    SetRandomSeed(CLOCKS_PER_SEC/3); //initial values won't change from run to run, but they will change from CPU to CPU :P

    list circleList = {.hub = NULL, .size = 0};
    circleMaker(&circleList); // add a hub

    Image icon = LoadImage("icon.png");

    InitWindow(1024,768,"Riley Tranquillow - When Spokes Rotate");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(icon);

    while (!WindowShouldClose()){

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTreeExt(&circleList); 

        DrawText(TextFormat("Amount of Circles: %i",circleList.size), 10, 10, 20, BLACK);
        DrawText("[X] adds a circle, [S] adds a circle continuously, [C] removes a circle, [D] removes a circle continuously, [Z] toggles the parametres of circles, [V] toggles the outlines on/off, [F] switches to an alternate view", scrollX, GetScreenHeight()-30, 20, GRAY);
        scrollX = scrollX-2;
        if (scrollX<-2200){
            scrollX=GetScreenWidth()+10;
        }
        DrawFPS(GetScreenWidth()-87,10); //why not
        posUp(&circleList);

        if (IsKeyPressed(KEY_X) || IsKeyDown(KEY_S)){
            if (GetFPS()<20){for(int i=0;i<=20;i++){circleMaker(&circleList);}}// if the frame rate is less than 20 it adds 20 circles at once
            if (GetFPS()<10){for(int i=0;i<=50;i++){circleMaker(&circleList);}} // if the frame rate is less than 10 it adds 50 circles at once
            if (GetFPS()<5){for(int i=0;i<=100;i++){circleMaker(&circleList);}} // if the frame rate is less than 5 it adds 100 circles at once
            circleMaker(&circleList);
        }
        else if (IsKeyPressed(KEY_C)||IsKeyDown(KEY_D)){listRemove(&circleList);}
        else if (IsKeyPressed(KEY_Z)){
            if(circDebug){circDebug=false;}
            else{circDebug=true;}
        }
        else if (IsKeyPressed(KEY_V)){
            if (optimize){optimize=false;}
            else{optimize=true;}
        }
        else if (IsKeyPressed(KEY_F)){
            if (zoom){zoom=false;}
            else{zoom=true;}
        }

        EndDrawing();
    }
}

Color colorPick(void) {
    int choice = GetRandomValue(0, 23); 
    switch (choice){
        case 0: return LIGHTGRAY;
        case 1: return MAGENTA;
        case 2: return DARKGRAY;
        case 3: return YELLOW;
        case 4: return GOLD;
        case 5: return ORANGE;
        case 6: return PINK;
        case 7: return RED;
        case 8: return MAROON;
        case 9: return GREEN;
        case 10: return LIME;
        case 11: return DARKGREEN;
        case 12: return SKYBLUE;
        case 13: return BLUE;
        case 14: return DARKBLUE;
        case 15: return PURPLE;
        case 16: return VIOLET;
        case 17: return BEIGE;
        case 18: return GRAY;
        case 19: return BROWN;
        case 20: return DARKBROWN;
        case 21: return WHITE;
        case 22: return BLACK;
        default: return DARKPURPLE;
    }

}
int radPick(void){return GetRandomValue(5, 20);}
int lenPick(void){return GetRandomValue(30, 70);}
double freqPick(void){
    double a =  GetRandomValue(-32,32)/13.37; //in order to get a double, I had to divide int by double.
    if (a==0){return 1.31;} //stationary circles are caused by the frequency being ==0
    return a;
}

void posUp(list* circs){
    for(int i=1;i<circs->size;i++){
        circle* circ = getSpoke(circs,i);
        circle* parent = getSpoke(circs,i-1);

        double oldAng = circ->angle;
        double freq = circ->frequence;

        double angDiff = (PI*freq)*GetFrameTime();
        double newAng=oldAng+angDiff;

        if (newAng>=2*PI){newAng=newAng-2*PI;}
        circ->angle = newAng;

        float posDiff = 2*circ->length*sin(angDiff/2); // scalar value of the difference in position
        Vector2 displacement = {posDiff*cos(PI/2+angDiff/2+oldAng),posDiff*sin(PI/2+angDiff/2+oldAng)};
        Vector2 oldPos = {circ->pos.x+parent->disp.x,circ->pos.y+parent->disp.y};
        Vector2 newPos = {oldPos.x+displacement.x,oldPos.y+displacement.y};// some vector calculations, no biggie
        Vector2 newDisp = {parent->disp.x+displacement.x,parent->disp.y+displacement.y}; //that funky behavior where the spokes have a mind of its own was because I wasn't adding their displacement values up

        circ->disp = newDisp;
        circ->pos = newPos;
    }
}

void DrawTreeExt(list* circs){
    for(int i=1;i<circs->size;i++){ // the loop for the lines that are drawn first
        circle* circ = getSpoke(circs, i);
        circle* parent = getSpoke(circs, i-1);
        if(zoom){DrawLine(parent->pos.x+GetScreenWidth()/2,parent->pos.y+GetScreenHeight()/2, circ->pos.x+GetScreenWidth()/2,circ->pos.y+GetScreenHeight()/2, DARKGRAY);}
        else{DrawLine(parent->pos.x/5+GetScreenWidth()/2,parent->pos.y/5+GetScreenHeight()/2, circ->pos.x/5+GetScreenWidth()/2,circ->pos.y/5+GetScreenHeight()/2, BLACK);}//zoom being false zooms out the view and disables the circles themselves
    }
    for(int i=0;i<circs->size;i++){ //the loop for the circles and their info (optional) that are drawn later
        circle* circ = getSpoke(circs, i);
        
        if(zoom){
            if(!optimize){DrawCircle(circ->pos.x+GetScreenWidth()/2,circ->pos.y+GetScreenHeight()/2, circ->radii+3, BLACK);} //optimize parameter decides whether we should draw the outlines
            DrawCircle(circ->pos.x+GetScreenWidth()/2,circ->pos.y+GetScreenHeight()/2, circ->radii, circ->color);
        }
        if(circDebug && 15*i+25 < GetScreenWidth()){DrawText(TextFormat("Circle %i: Pos[%.2f, %.2f]  Disp[%.3f,%.3f], Freq[%.2f], Ang[%2.f]",i, circ->pos.x, circ->pos.y,circ->disp.x,circ->disp.y,circ->frequence,circ->angle), 10, (15*i+30), 10, DARKGRAY);}
    }
}
void circleMaker(list* circs){
    double freq = freqPick(); //picking random attributes
    int len = lenPick();
    int radius = radPick();
    Color color = colorPick();
    
    bool isCenter =true; //initially True, but will be changed to False
    Vector2 initialPos= {0,0}; //initially the origin but it will be translated to the center
    
    if(circs->size!=0){
        isCenter=false;
        circle* parent=getSpoke(circs,(circs->size-1));
        initialPos.x = parent->pos.x+len;
        initialPos.y = parent->pos.y;
    }
    circle circ = { //we construct our circle struct
        .color= color,
        .radii= radius,
        .length= len,
        .isHub=isCenter,
        .disp=0.0,
        .pos=initialPos,
        .angle=0.0,
        .frequence=freq,
    };
    listAdd(circs,circ);
}

void listAdd(list* circs, circle circ){
    mem* a = malloc(sizeof(circle));
    a->nextmem=NULL;
    a->spec = circ;

    if (circs->hub==NULL){circs->hub = a;}
    else{
        mem* readHead = circs->hub;
        while((*readHead).nextmem != NULL){readHead = readHead->nextmem;} //traversing the linked list
        readHead->nextmem = a; //a was a pointer afterall
    }
    circs->size++;
}
void listRemove(list* circs){
    if(circs->size==1){
        return;
    }
    else{
        mem* behind = NULL;
        mem* readHead = circs->hub;
        while((*readHead).nextmem != NULL){
            behind = readHead;
            readHead = readHead->nextmem;
        }
        if(behind != NULL){
            behind->nextmem=NULL;
        }
        free(readHead);
        circs->size = (circs->size)-1;
    }
}
circle* getSpoke(list* circs, int index){
    if (index > circs->size -1){return NULL;}

    mem* readHead = circs->hub;
    for(int i = 0;i<index;i++){readHead = readHead->nextmem;} //traversing the list until the desired circle
    
    return &readHead->spec;
    }
