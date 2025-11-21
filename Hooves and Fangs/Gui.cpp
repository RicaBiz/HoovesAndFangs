#pragma once
// Garantisce che il file venga incluso una sola volta in fase di compilazione
#include <fstream>
#include <string>
#include "strutture.h"
#include <ImGui\imgui.h>           // Libreria core ImGui
#include <ImGui\imgui_impl_glfw.h> // Integrazione ImGui con GLFW (gestione finestra/input)
#include <ImGui\imgui_impl_opengl3.h> // Integrazione ImGui con OpenGL3 (rendering)
#include "lib.h"
#include "gestione_curve.h"
#include "inizializzazioni.h"
#include <math.h> // Libreria matematica standard C
#include "Controller.hpp"
#include "Utilities.h"
#pragma warning(disable:4996)

extern ImVec4 clear_color;  // Colore di sfondo (RGBA), dichiarato esternamente
extern double mousex, mousey; // Coordinate del mouse, dichiarate esternamente
extern bool flag_wireframe;
extern int width, height;
int h_offset = 0;
extern float w_up, h_up;
extern int param;
extern vector<GameScore> scoreboard;
string punteggio = "SCORE ";
string title = "HOOVES AND FANGS";
string pauseTitle = "PAUSE";
string gameOverTitle = "GAME OVER";
int buttonWidth = 200;
int buttonHeight = 50;
extern Screen currentScreen;
extern GLFWwindow* window;
char buffer[10] = "";
 
// Funzione che inizializza ImGui con GLFW e OpenGL
void Initialize_IMGUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();               // Controlla compatibilità versione ImGui
    ImGui::CreateContext();             // Crea il contesto ImGui (necessario)
    ImGuiIO& io = ImGui::GetIO();       // Ottiene l’oggetto IO di ImGui (config e input)
    io.FontGlobalScale = 2.0f;          // Scala globale del font (doppio delle dimensioni)
    ImGui::StyleColorsDark();            // Imposta tema scuro predefinito
    
    // Inizializza l’integrazione con GLFW per OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core"); // Inizializza renderer OpenGL3 con versione 330 core
}


// Funzione di rendering dell’interfaccia grafica per ogni frame
void my_interface()
{
    ImGui_ImplGlfw_NewFrame();          // Prepara frame nuovo per input GLFW
    ImGui_ImplOpenGL3_NewFrame();       // Prepara frame nuovo per rendering OpenGL3
    ImGui::NewFrame();                  // Inizia registrazione nuova UI frame

    if (currentScreen == GAME)
    {
        //Game UI
        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - ImGui::CalcTextSize(punteggio.c_str()).x / 2, h_offset + 20));
        ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize(punteggio.c_str()).x * 2, ImGui::CalcTextSize(punteggio.c_str()).y * 3));

        ImGui::Begin("Punteggio", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs);
        /*
        I flag sono valori interi con un solo bit attivato; combinati col bitwise OR
        abilitano più opzioni contemporaneamente. ImGui controlla quali flag sono attivi
        con operazioni bit a bit per definire il comportamento finestra.
        */
        ImGui::Text((punteggio + string(to_string(getFieldScore()))).c_str());

        ImGui::End();
    }
    else if (currentScreen == MENU) {
        // Menu UI
        // Title
        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - ImGui::CalcTextSize(title.c_str()).x / 2, h_offset + h_up / 6));
        ImGui::Begin("Title", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs);
        ImGui::Text(title.c_str());
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - buttonWidth / 2, h_offset + h_up / 8 * 3));
        ImGui::Begin("Selection", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar);
        if (ImGui::Button("PLAY", ImVec2(buttonWidth, buttonHeight))) {
            goToGameScreen();
        }
        if (ImGui::Button("SCOREBOARD", ImVec2(buttonWidth, buttonHeight))) {
            changeScreen(SCOREBOARD);
        }
        if (ImGui::Button("EXIT", ImVec2(buttonWidth, buttonHeight))) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        ImGui::End();
    }
    else if (currentScreen == PAUSE) {
        // Pause UI
        // Title
        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - ImGui::CalcTextSize(pauseTitle.c_str()).x / 2, h_offset + h_up / 6));
        ImGui::Begin("Title", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs);
        ImGui::Text(pauseTitle.c_str());
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - buttonWidth / 2, h_offset + h_up / 8 * 3));
        ImGui::Begin("Selection", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar);
        if (ImGui::Button("RESUME", ImVec2(buttonWidth, buttonHeight))) {
            goToGameScreen();
        }
        if (ImGui::Button("RETURN TO MENU", ImVec2(buttonWidth, buttonHeight))) {
            initializeFieldModel();
            changeScreen(MENU);
        }
        if (ImGui::Button("EXIT", ImVec2(buttonWidth, buttonHeight))) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        ImGui::End();
    }
    else if (currentScreen == GAMEOVER) {
        // Game Over UI
        // Title
        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - ImGui::CalcTextSize(gameOverTitle.c_str()).x / 2, h_offset + h_up / 6));
        ImGui::Begin("Title", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs);
        ImGui::Text(gameOverTitle.c_str());
        ImGui::End();
        string puntiFatti = string("Hai fatto ") + string(to_string(getFieldScore())) + string(" punti!");
        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - ImGui::CalcTextSize(puntiFatti.c_str()).x / 2, h_offset + h_up / 4));
        ImGui::Begin("punti", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoInputs);
        ImGui::Text(puntiFatti.c_str());
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - buttonWidth / 2, h_offset + h_up / 8 * 3));
        ImGui::Begin("Selection", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar);
        ImGui::InputText("Name", buffer, 4);
        if (ImGui::Button("Save Score!")) {
            if (buffer != NULL && buffer != "") {
                GameScore score;
                score.name = buffer;
                score.score = getFieldScore();
                scoreboard.emplace_back(score);
                saveScoreboard();
            }
        }
        if (ImGui::Button("RETURN TO MENU", ImVec2(buttonWidth, buttonHeight))) {
            initializeFieldModel();
            changeScreen(MENU);
        }
        if (ImGui::Button("EXIT", ImVec2(buttonWidth, buttonHeight))) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        ImGui::End();
    }
    else if (currentScreen == SCOREBOARD) {
        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - ImGui::CalcTextSize(gameOverTitle.c_str()).x / 2, h_offset + h_up / 6));
        ImGui::Begin("Scores", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove);
        for (auto& score : scoreboard) {
            ImGui::Text((score.name + " " + to_string(score.score)).c_str());
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(w_up / 2 - buttonWidth / 2, h_offset + h_up / 4 * 3));
        ImGui::Begin("Selection", NULL,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar);
        if (ImGui::Button("RESET SCOREBOARD", ImVec2(buttonWidth, buttonHeight))) {
            resetScoreboard();
        }
        if (ImGui::Button("RETURN TO MENU", ImVec2(buttonWidth, buttonHeight))) {
            changeScreen(MENU);
        }
        ImGui::End();
    }

    // Render ImGui
    ImGui::Render(); // Renderizza gli elementi dell'interfaccia ImGui

}
    

// Funzione per chiudere e pulire ImGui correttamente
void close_GUI() {
    ImGui_ImplOpenGL3_Shutdown();   // Rilascia risorse renderer OpenGL3
    ImGui_ImplGlfw_Shutdown();      // Rilascia risorse integrazione GLFW
    ImGui::DestroyContext();        // Distrugge contesto ImGui liberando memoria
}