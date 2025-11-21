#pragma once
#include "lib.h"
#include "struttureModel.hpp"
#include <ImGui\imgui.h>           // Libreria core ImGui
#include <ImGui\imgui_impl_glfw.h> // Integrazione ImGui con GLFW (gestione finestra/input)
#include <ImGui\imgui_impl_opengl3.h> // Integrazione ImGui con OpenGL3 (rendering)
#include "Controller.hpp"
#include "strutture.h"
#include "Utilities.h"
extern GLFWwindow* window;
double mousex,mousey;
extern int height, width;
extern unsigned int selected_bk;
extern unsigned int programId, programId_BK2;
extern Action currentAction;
extern Screen currentScreen;
extern float w_up, h_up;
 
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    float mousex_m, mousey_m;
    mousex = xpos;
    mousey = height - ypos;
    //Mappa le coordinate del mouse dallo spazio finestra ([0, width], [0, height]) allo spazio Normalized Device Coordinates (NDC) in cui X e Y variano tra -1 e 1.
    mousex_m = 2 * (mousex / width) - 1;
    mousey_m = 2 * (mousey / height) - 1;
}

 void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Variabili locali per posizione mouse, coordinate mappate, distanze e tolleranza per selezione punti
    double xpos, ypos, mousex_m, mousey_m, dist, dist1, TOLL = 0.1;

   // // Controllo se il bottone sinistro del mouse è premuto e l'input non è sopra un widget ImGui
   // if (button == GLFW_MOUSE_BUTTON_LEFT && !ImGui::IsAnyItemHovered())
   // {
   //     // Si attiva solo quando il bottone sinistro viene premuto (non rilasciato)
   //     if (action == GLFW_PRESS)
   //     {
			//selected_bk = selected_bk == programId ? programId_BK2 : programId;
   //     }
   // }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // Se il tasto ESCAPE è premuto, chiude la finestra
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) {
            if (currentScreen == GAME)
                leaveGameScreen(PAUSE);
            else if (currentScreen == PAUSE)
            {
                goToGameScreen();
            }
        }
        break;
    case GLFW_KEY_A:
        if (action == GLFW_PRESS)
            currentAction = MOVE_LEFT;
		else if (currentAction == MOVE_LEFT && action == GLFW_RELEASE)
			currentAction = NONE;
        break;
	case GLFW_KEY_D:
		if (action == GLFW_PRESS)
			currentAction = MOVE_RIGHT;
		else if (currentAction == MOVE_RIGHT && action == GLFW_RELEASE)
			currentAction = NONE;
		break;
	case GLFW_KEY_W:
		if (action == GLFW_PRESS)
			currentAction = MOVE_UP;
		else if (currentAction == MOVE_UP && action == GLFW_RELEASE)
			currentAction = NONE;
		break;
	case GLFW_KEY_S:
		if (action == GLFW_PRESS)
			currentAction = MOVE_DOWN;
		else if (currentAction == MOVE_DOWN && action == GLFW_RELEASE)
			currentAction = NONE;
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
            if (currentScreen == GAME)
			    ponyAttack();
		break;
    default:

        break;
    }

}

extern int h_offset;
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    h_offset = h - h_up;
    //Imposto la matrice di proiezione per la scena da diegnare
    float AspectRatio_mondo = (float)(width) / (float)(height); //Rapporto larghezza altezza di tutto ciò che è nel mondo
    //Se l'aspect ratio del mondo è diversa da quella della finestra devo mappare in modo diverso 
    //per evitare distorsioni del disegno
    if (AspectRatio_mondo > (float)w / (float)h)   //Se ridimensioniamo la larghezza della Viewport
    {
        w_up = (float)w;
        h_up = (float)w / AspectRatio_mondo;
    }
    else {  //Se ridimensioniamo la larghezza della viewport oppure se l'aspect ratio tra la finestra del mondo 
        //e la finestra sullo schermo sono uguali
        w_up = (float)h * AspectRatio_mondo;
        h_up = (float)h;
    }
    h_offset = h - h_up;
    glViewport(0, 0, w_up, h_up);
    //glViewport(0, h - h_up, w_up, h_up);
}
