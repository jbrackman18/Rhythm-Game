#include <iostream>
#include <raylib.h>
#include <raymath.h>

using namespace std;


int cellSize = 30, cellCount = 25, totalCell = cellSize*cellCount;

class MouseControls{
    public:
    Vector2 mousePosition;
    Vector2 mouseSize;


    void Draw(){
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        mousePosition = GetMousePosition();
        mouseSize = {mousePosition.x,mousePosition.y};
        DrawCircleV(mouseSize,10,SKYBLUE);
        }
    }

    void Update(){
        //cout << "X Mouse Position: " << mousePosition.x << " Y Mouse Position: " << mousePosition.y << endl;
    }

};

class Notes{
    public:
    Vector2 playerPosition;
    Vector2 ringPosition;
    Vector2 ringGoodPosition;

    int tooEarlyNoteValues[6] = {30,29,28,27,26,25};
    int perfectNoteValues[6] = {25,24,23,23,23,23};
    int goodNoteValues[6] = {22,21,20,17,17,17};
    int badNoteValues[6] = {16,15,14,13,13,13};
    int tooLateNoteValues[6] = {12,11,10,9,8,7};


    float songBPM,secPerBeat,songPosition,lastBeat = 0,crotchet,songPositioninBeats,noteRadius = 50;


    Notes(){
        playerPosition = GenerateRandomPos();
        ringPosition = playerPosition;
        ringGoodPosition = playerPosition;
    }
    
    void Draw(){
        DrawCircleV(playerPosition,25,WHITE);
        DrawCircleLinesV(ringPosition,noteRadius,WHITE);
        DrawCircleLinesV(ringGoodPosition,25,GREEN);
    }

    Vector2 GenerateRandomPos(){
        float x = GetRandomValue(0, totalCell - 150);
        float y = GetRandomValue(0, totalCell - 150);
        return Vector2{x,y};
    }

    void Update(){
        noteRadius -=  30 * (secPerBeat * GetFrameTime());
    }

};

class Text{
    public:
    int playerScore = 0;

    void Draw(){
        DrawText(TextFormat("Score: %08i", playerScore),cellSize - 5,cellCount,20,WHITE);
    }

};

class Game{
    public:
    MouseControls mcontrols;
    Notes notes;
    Text screenText;

    void Draw(){
        notes.Draw();
        mcontrols.Draw();
        screenText.Draw();
    }

    void Update(){
        notes.songBPM = 124;
        notes.secPerBeat = notes.songBPM / 60;
        notes.crotchet = 60/notes.songBPM;
        mcontrols.Update();
        
        notes.Update();
        CheckCollisionWithMouse();
    }

    void CheckCollisionWithMouse(){

            int perfectNotes = notes.noteRadius;


            //cout << perfectNotes << endl;

            if(CheckCollisionCircles(mcontrols.mouseSize,10,notes.playerPosition,25)){
                //cout << "Touching" << endl;
                notes.playerPosition = notes.GenerateRandomPos();
                notes.ringPosition = notes.playerPosition;
                notes.ringGoodPosition = notes.playerPosition;
                notes.noteRadius = 50;
            }

            for (int i = 0; i < 6; i++){
                if ((perfectNotes >= notes.tooEarlyNoteValues[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    cout << "Too Early" << endl;
                    screenText.playerScore += 100;
                }
                else if (( perfectNotes == notes.perfectNoteValues[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    cout << "Perfect" << endl;
                    screenText.playerScore += 100;    
                }
                else if((perfectNotes == notes.goodNoteValues[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    cout << "Good" << endl;
                    screenText.playerScore += 75;
                }
                else if((perfectNotes == notes.badNoteValues[i]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    cout << "Bad" << endl;
                    screenText.playerScore += 25;
                }
                else if (perfectNotes == notes.tooLateNoteValues[i] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    cout << "Late" << endl;
                    screenText.playerScore += 5;
                }

            if (perfectNotes <= 6){
                notes.playerPosition = notes.GenerateRandomPos();
                notes.ringPosition = notes.playerPosition;
                notes.ringGoodPosition = notes.playerPosition;
                notes.noteRadius = 50;
                screenText.playerScore -= 30;
            }
            
        }
    }
};

int main(void)
{
    InitWindow(totalCell,totalCell , "Example Game");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    InitAudioDevice();
    //Music music = LoadMusicStream("D:\\OLD STUFF\\Street Fighter 3 2nd Impact Giant Attack OST Leave Alone (Theme of Dudley) [ ezmp3.cc ].mp3");
    //PlayMusicStream(music);
    Game game;


    // Main game loop
    while (!WindowShouldClose())
    {
        //UpdateMusicStream(music);
        game.Update();
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw();
        EndDrawing();
    }

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
