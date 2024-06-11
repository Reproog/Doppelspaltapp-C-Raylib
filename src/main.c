// DoppelspaltApp.c
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

typedef struct {
    int lambda;
    int gitterD;
    int winkel;
    float dLambda;
    int amplitude;
} AppState;

AppState state = {50, 75, 90, 1.5, 40};

void DrawKugelwelle(int x, int y, int breite, int hoehe, AppState* state, Color color) {
    const int apertur = 90;
    int s0;
    double phi = (state->winkel - 90) * PI / 180.0;

    if (y > hoehe / 2) {
        s0 = (int)((double) state->gitterD / 2.0 * sin(phi)) % state->lambda;
    } else {
        s0 = -(int)((double) state->gitterD / 2.0 * sin(phi)) % state->lambda;
    }

    for (int i = 0; i * state->lambda + s0 < breite; i++) {
        int radius = i * state->lambda + s0;
        int xOffset = x - radius;
        int yOffset = y - radius;

        if (radius > s0) {
            DrawCircleLines(x, y, radius, color);
        }
    }
}

void DrawWavesFromSlits(int screenWidth, int screenHeight, AppState* state) {
    int xSpalt = screenWidth / 3;
    int ySpalt1 = (screenHeight - state->gitterD) / 2;
    int ySpalt2 = (screenHeight + state->gitterD) / 2;

    // Calculate the field size similarly to the Java code
    int sizeFeld = sqrt((screenWidth - xSpalt) * (screenWidth - xSpalt) + screenHeight * screenHeight);

    // Draw waves for the first slit
    DrawKugelwelle(xSpalt, ySpalt1, sizeFeld, screenHeight, state, BLUE);

    // Draw waves for the second slit
    DrawKugelwelle(xSpalt, ySpalt2, sizeFeld, screenHeight, state, GREEN);
}
 


void DrawInterferencePoints(int x, int breite, int hoehe, Color color) {
    double phi = (state.winkel - 90) * M_PI / 180.0;
    double s0 = fmod((double) state.gitterD / 2.0 * sin(phi), state.lambda);
    int nWellen = breite / state.lambda;
    int tempR3 = breite / 80;
    if (tempR3 < 5) tempR3 = 5;

    for (int i1 = 0; i1 < nWellen; i1++) {
        for (int i2 = 0; i2 < nWellen; i2++) {
            double tempR1 = i1 * state.lambda - s0;
            double tempR2 = i2 * state.lambda + s0;

            if (tempR1 + tempR2 > state.gitterD && tempR1 <= tempR2 + state.gitterD && tempR2 <= tempR1 + state.gitterD) {
                if (state.gitterD != 0) {
                    double tempY = (tempR1 * tempR1 - tempR2 * tempR2 + state.gitterD * state.gitterD) / (2.0 * state.gitterD);
                    double tempX = sqrt(tempR1 * tempR1 - tempY * tempY);

                    int circleX = x + (int)tempX - tempR3 / 2 + 480;
                    int circleY = (hoehe - state.gitterD) / 2 + ((int)tempY - tempR3 / 2) + 10;
                    //int circleY = centerYBar + (int)tempY - state.lambda / 2;

                    DrawCircle(circleX, circleY, tempR3 / 2, color);
                }
            }
        }
    }
}




void DrawSphericalWave(int x, int y, int width, int height) {
    const int apertur = 90;
    double phi = (state.winkel - 90) * PI / 180.0;
    int s0;

    if (y > height / 2) {
        s0 = (int)((double)state.gitterD / 2.0 * sin(phi)) % state.lambda;
    } else {
        s0 = -(int)((double)state.gitterD / 2.0 * sin(phi)) % state.lambda;
    }

    for (int i = 0; i * state.lambda + s0 < width; i++) {
        int circleX = x - i * state.lambda - s0;
        int circleY = y - i * state.lambda - s0;
        int diameter = 2 * (i * state.lambda + s0);

        DrawEllipseLines(circleX, circleY, diameter, diameter, BLACK);
    }
}


void DrawPlaneWave(int width, int height) {
    double dWinkel = state.winkel * PI / 180.0;
    double nX = sin(dWinkel);
    double nY = cos(dWinkel);

    double d0, d1, dMin, dMax;
    if (state.winkel < 90) {
        dMin = 0.0;
        d1 = (double)width * nX + (double)height / 2.0 * nY;
        dMax = (double)width * nX + (double)height * nY;
    } else {
        dMin = (double)height * nY;
        d1 = (double)width * nX + (double)height / 2.0 * nY;
        dMax = (double)width * nX;
    }

    d0 = dMin + fmod(d1 - dMin, (double)state.lambda);
    for (double d = d0; d < dMax; d += (double)state.lambda) {
        int x0, y0;

        double schnittX_Achse = d / nY;
        double schnittY_Achse = d / nX;
        double schnittX_breite = (d - (double)width * nX) / nY;
        double schnittY_hoehe = (d - (double)height * nY) / nX;

        if (schnittY_Achse < 0.0) {
            x0 = 0;
            y0 = (int)schnittX_Achse;
        } else if (schnittY_Achse < (double)width) {
            x0 = (int)schnittY_Achse;
            y0 = 0;
        } else {
            x0 = width;
            y0 = (int)schnittX_breite;
        }
    }
}

void DrawVerticalLines(int breite, int hoehe, int lambda, int winkel) {
    double dWinkel = winkel * PI / 180.0;

    if (winkel == 90) {
        for (int i = breite; i > 0; i -= lambda)
            DrawLine(i, 0, i, hoehe, BLACK);
    } else if (winkel == 0 || winkel == 180) {
        for (int i = (hoehe / 2) % lambda; i <= hoehe; i += lambda)
            DrawLine(0, i, breite, i, BLACK);
    } else {
        double nX = sin(dWinkel);
        double nY = cos(dWinkel);

        double d0, d1, dMin, dMax;
        if (winkel < 90) {
            dMin = 0.0;
            d1 = (double)breite * nX + (double)hoehe / 2.0 * nY;
            dMax = (double)breite * nX + (double)hoehe * nY;
        } else {
            dMin = (double)hoehe * nY;
            d1 = (double)breite * nX + (double)hoehe / 2.0 * nY;
            dMax = (double)breite * nX;
        }

        d0 = dMin + fmod(d1 - dMin, (double)lambda);
        for (double d = d0; d < dMax; d += (double)lambda) {
            int x0, y0, x1, y1;

            double schnittX_Achse = d / nY;
            double schnittY_Achse = d / nX;
            double schnittX_breite = (d - (double)breite * nX) / nY;
            double schnittY_hoehe = (d - (double)hoehe * nY) / nX;

            if (schnittY_Achse < 0.0) {
                x0 = 0;
                y0 = (int)schnittX_Achse;
            } else if (schnittY_Achse < (double)breite) {
                x0 = (int)schnittY_Achse;
                y0 = 0;
            } else {
                x0 = breite;
                y0 = (int)schnittX_breite;
            }

            if (schnittY_hoehe < 0.0) {
                x1 = 0;
                y1 = (int)schnittX_Achse;
            } else if (schnittY_hoehe < (double)breite) {
                x1 = (int)schnittY_hoehe;
                y1 = hoehe;
            } else {
                x1 = breite;
                y1 = (int)schnittX_breite;
            }

            DrawLine(x0, y0, x1, y1, BLACK);
        }
    }
}

void DrawSinAndWall(int width, int height, AppState* state) {
    int xSpalt = width / 3; 
    int ySpalt1 = (height - state->gitterD) / 2;
    int ySpalt2 = (height + state->gitterD) / 2;

    double phi = (state->winkel - 90) * PI / 180.0;
    int lSinus;

    if (fabs(phi) < atan((double)(0.5 * height) / (double)xSpalt)) {
        lSinus = (int)((double)xSpalt / cos(phi));
    } else {
        lSinus = (int)(0.5 * height / fabs(sin(phi)));
    }

    Vector2* points = (Vector2*)malloc(lSinus * sizeof(Vector2));

    for (int i = 0; i < lSinus; i++) {
        double x = (double)(i); 
        double y = (state->amplitude * sin(1.5 * PI + 2 * PI * (double)(i % state->lambda) / (double)state->lambda));

        points[i].x = xSpalt - (x * cos(phi) - y * sin(phi));
        points[i].y = x * sin(phi) + y * cos(phi) + height / 2;
    }

    for (int i = 0; i < lSinus - 1; i++) {
        DrawLineV(points[i], points[i + 1], RED);
    }



    int RectWidth = 5;
    int loch = 5;

    DrawRectangle(xSpalt - 2, 0, RectWidth, ySpalt1 - loch, BLACK);
    DrawRectangle(xSpalt - 2, ySpalt1 + loch, RectWidth, ySpalt2 - ySpalt1 - (loch * 2), BLACK);
    DrawRectangle(xSpalt - 2, ySpalt2 + loch, RectWidth, height - ySpalt2 - loch, BLACK);


    DrawVerticalLines(xSpalt, height, state->lambda, state->winkel);

    /*
    DrawText(TextFormat("Wavelength (lambda): %d", state->lambda), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Slit Distance (d): %d", state->gitterD), 10, 40, 20, DARKGRAY);
    DrawText(TextFormat("Angle of Incidence: %d", state->winkel), 10, 70, 20, DARKGRAY);

    */

    free(points);
}



int main(void) {
    // Initialization
    int screenWidth = 9 * 1920 / 10;
    int screenHeight = 9 * 1080 / 10;


    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);    // Window configuration flags
    InitWindow(screenWidth, screenHeight, "Doppelspaltenapp in C - Raylib / Raygui");

    // Define initial values and bounds for sliders
    float valueLambda = 50;
    float valueD = 75;
    float valueWinkel = 90;

    // Define the height of the window box and sliders
    const int windowBoxHeight = screenHeight / 8 ;
    const int sliderHeight = screenHeight / 50;
    const int sliderSpacing = screenHeight / 100; // Space between sliders
    const int sliderBeginY = screenHeight / 20;
    const int displayText = screenHeight / 25;

    // Define bounds for the window box (bottom of the screen)
    Rectangle windowBoxBounds = { 0, screenHeight - windowBoxHeight, screenWidth, windowBoxHeight };

    // Define bounds for the sliders inside the window box
    Rectangle boundsLambda = { sliderBeginY, windowBoxBounds.y + 10, windowBoxBounds.width - displayText - sliderBeginY, sliderHeight };
    Rectangle boundsD = { sliderBeginY, boundsLambda.y + sliderHeight + sliderSpacing, windowBoxBounds.width - displayText - sliderBeginY, sliderHeight };
    Rectangle boundsWinkel = { sliderBeginY, boundsD.y + sliderHeight + sliderSpacing, windowBoxBounds.width - displayText - sliderBeginY, sliderHeight };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

 
        // Werte in AppState aktualisieren
        state.lambda = (int)valueLambda;
        state.gitterD = (int)valueD;
        state.winkel = (int)valueWinkel;


        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();



        Rectangle whiteRect = {
            .x = 0,
            .y = 0,
            .height = GetScreenHeight(),
            .width = GetScreenWidth() / 3,
        };

        BeginDrawing();



 
        ClearBackground(RAYWHITE);


        DrawWavesFromSlits(screenWidth, screenHeight, &state);
        DrawInterferencePoints(100, screenWidth - 200, screenHeight, RED);
        DrawRectangleRec(whiteRect, RAYWHITE);
        DrawSinAndWall(GetScreenWidth(), GetScreenHeight(), &state);


        GuiPanel(windowBoxBounds, "");

        // Adjust sliders position relative to the window box
        boundsLambda.y = windowBoxBounds.y + displayText;
        boundsD.y = boundsLambda.y + sliderHeight + sliderSpacing;
        boundsWinkel.y = boundsD.y + sliderHeight + sliderSpacing;
 
        //GuiSliderBar(Rectangle bounds, const char *textLeft, const char *textRight, float *value, float minValue, float maxValue); // Slider Bar control
        GuiSlider(boundsLambda, "Lambda", TextFormat("%2.2f", valueLambda), &valueLambda, 10, 85);
        GuiSlider(boundsD, "D", TextFormat("%2.2f", valueD), &valueD, 0, 150);
        GuiSlider(boundsWinkel, "Winkel", TextFormat("%2.2f", valueWinkel), &valueWinkel, 0, 180);


 

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
