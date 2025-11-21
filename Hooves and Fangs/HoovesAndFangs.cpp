#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "update_collisions.h"
#include "gestione_interazioni.h"
#include "Init_Finestra_Eventi.h"
#include "init_GLFW_OpenGL.h"
#include "inizializzazioni.h"
#include "strutture.h"
#include "Scena.h"
#include "Controller.hpp"
#include "rendering.hpp"
#include <ImGui\imgui.h>           // Libreria core ImGui
#include <ImGui\imgui_impl_glfw.h> // Integrazione ImGui con GLFW (gestione finestra/input)
#include <ImGui\imgui_impl_opengl3.h> // Integrazione ImGui con OpenGL3 (rendering)
#include "Gui.h"

GLFWwindow* window;
int height, width;
Figura bkg;
FiguraHermite pony;
FiguraHermite attacking_pony;
vector<Figura> hearts;
vector<FiguraHermite> fangs;
FiguraHermite attacking_fang;
vector<FiguraHermite> carrots;
vector<Figura> Scena;
vector<GameScore> scoreboard;
unsigned int programId, programId_BK, programId_BK2, programId_Triangul;
unsigned int selected_bk = 0;
mat4 Projection;
unsigned int IdProj_Triangul, IdModel_Triangul;
GLuint IdProj, IdModel, IdColorA, IdColorB;
Action currentAction = NONE;
Screen currentScreen = MENU;
float w_up, h_up;

ImVec4 clear_color = ImVec4(0.45f, 0.0f, 0.0f, 1.00f);

int main() {

    //Inizializza GLFW, imposta le propriet� del contesto e del profile
    int flag_gl_GLFW = init_gl_GLFW();

    /* Inizializza la finestra opengl ed associa contesto e la gestione degli eventi */
    int flag = inizializza_window_context_events();

    w_up = width;
    h_up = height;
    //Inizializza l'intefaccia ImGUI
    Initialize_IMGUI(window);

    //Inizializza gli shader 
    INIT_SHADER();

    loadScoreboard();

	selected_bk = programId_BK;
	costruisci_Scena(&bkg, &pony, &hearts);
    //Gestione della trasparenza:
    /*
    In OpenGL, la funzione glBlend serve a definire come combinare i colori di un pixel sorgente (che stiamo disegnabdo) e di un pixel destinazione (gi� presente nel framebuffer)
     quando si disegnano oggetti trasparenti.
    In questo caso specifico, glBlend(GL_SRC_ALPHA, GL_ONES_MINUS_SRC_ALPHA); indica che :
     GL_SRC_ALPHA: Il valore alfa del pixel sorgente(quello che stiamo disegnando) verr� utilizzato come fattore di miscelazione per il colore sorgente.
     GL_ONES_MINUS_SRC_ALPHA : Il valore alfa del pixel destinazione(quello gi� presente nel buffer) verr� sottratto da 1 e poi utilizzato come fattore di miscelazione
     per il colore destinazione. (gi� presente nel framebuffer
*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setto le coordinate dle mondo e la matrice di proiezione

    //In questo caso le coordinate del mondo variano sono in [-1,-1+x[1,1]
    Projection = ortho(-1.0, 1.0, -1.0, 1.0);

    //Location delle variabili uniform per i tre diversi shader

    //Shader per la visualizzazione dei vertici della curva o della figura
    IdProj = glGetUniformLocation(programId, "Projection");
    IdModel = glGetUniformLocation(programId, "Model");
    //Shader per la visualizzazione dei vertici della curva Hermite
    IdColorA = glGetUniformLocation(programId_Triangul, "colorA");
    IdColorB = glGetUniformLocation(programId_Triangul, "colorB");
    IdModel_Triangul = glGetUniformLocation(programId_Triangul, "Model");
    IdProj_Triangul = glGetUniformLocation(programId_Triangul, "Projection");

	initializeFieldModel();

    float currentTime = glfwGetTime();
    float deltaTime;
    while (!glfwWindowShouldClose(window))
    {
		deltaTime = (glfwGetTime() - currentTime);
        currentTime = glfwGetTime(); //rappresenta i secondi trascorsi dall'avvio di GLFW e cresce continuamente.
        //glfwGetWindowSize(window, &width, &height);

        if (currentScreen == GAME) {
            updateFieldModel(deltaTime, currentAction);
            aggiornaHearts(&hearts);
            aggiornaFangs(&fangs);
            aggiornaCarrots(&carrots);
            checkCollisions();
            if (getHealth() == 0) {
                leaveGameScreen(GAMEOVER);
            }
        }

        renderizza(currentTime);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    /* Pulizia finale */
    close_GUI();
    glfwDestroyWindow(window); // Distrugge la finestra
    glDeleteProgram(programId);

    glfwTerminate();
    return 0;
}