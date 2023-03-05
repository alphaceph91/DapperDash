#include "raylib.h"

//declare custom data type called AnimData
struct AnimData
{   
    //giving AnimData its own variables
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

//animation data function
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{   
    //initializing an array
    int windowDimensions [2];
    windowDimensions [0] = 800;
    windowDimensions [1] = 600;

    //Initializing the window
    InitWindow(windowDimensions [0], windowDimensions [1], "Dapper Dasher!");
    
    //acceleration due to gravity (pixel/s)/s
    const int gravity{1000};

    //nebula parameters
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};     //array for nebulas

    for (int i=0; i<sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.width/8;
        nebulae[i].pos.y =  windowDimensions [1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i*300;
    }

    float finishLine{ nebulae[sizeOfNebulae - 1].pos.x };

    //nebula x velocity(pixels/second)
    int nebulaVelocity{-200};

    //scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    //creating a custom data type scarfyData and accessing it using previously defined AnimData members using .
    AnimData scarfyData; 
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions [0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions [1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0.0;

    //is rectangle in air?
    bool isInAir{};

    //jump velocity (pixels/s)
    const int jumpVelocity{-600};

    int velocity{0};

    //background textures
    Texture2D background = LoadTexture("textures/far-buildings.png");   //background image
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");  //midground image
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");    //foreground image
    float fgX{};

    //setting the frame rate
    SetTargetFPS(60);

    bool collision{};

    while(!WindowShouldClose())
    {   
        //delta time (time since last frame)
        const float dT{GetFrameTime()};

        //Start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //scroll background
        bgX -= 20 * dT;
        if (bgX <= -background.width*2)
        {
            bgX = 0.0;
        }

        //scroll midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width*2)
        {
            mgX = 0.0;
        }

        //scroll foreground
        fgX -= 80 * dT;
        if (fgX <= - foreground.width*2)
        {
            fgX = 0.0;
        }
        
        //draw the background image
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 3.5, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 3.5, WHITE);

        //draw the midground image
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 3.5, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 3.5, WHITE);

        //draw the foreground image
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 3.5, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 3.5, WHITE);

        //perform ground check
        if(isOnGround(scarfyData, windowDimensions[1])) 
        {
            //rectangle on the ground
            velocity = 0;
            isInAir = false;
        }
        else 
        {
            //rectangle in the air
            velocity += gravity * dT;
            isInAir = true;
        }

        //space key to move the rectangle
        if(IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVelocity;
        }

        for(int i=0; i<sizeOfNebulae; i++)
        {
            //update the position of each nebula
            nebulae[i].pos.x += nebulaVelocity * dT;
        }

        //update finish line
        finishLine += nebulaVelocity * dT;

        //update scarfy position
        scarfyData.pos.y += velocity * dT;

        //update scarfy's animation frame
        if(!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for(AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebulaRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if (CheckCollisionRecs(nebulaRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            //lose the game
            DrawText("GAME OVER!", windowDimensions[0]/4, windowDimensions[1]/2, 80, RED);
        }
        else if (scarfyData.pos.x >=finishLine)
        {
            //win the game
            DrawText("YOU WON!", windowDimensions[0]/4, windowDimensions[1]/2, 80, GREEN);
        }
        
        
        else
        {
            for(int i=0; i<sizeOfNebulae; i++)
            {
                //draw nebula
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            //draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        //Stop drawing
        EndDrawing();
    }

    //unloading the textures
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    //raylib close window function
    CloseWindow(); 
}