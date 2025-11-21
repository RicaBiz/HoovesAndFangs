
#pragma once
#include <glad/glad.h>
#include "earcut.hpp"
#include "ShaderMaker.h"
#include "strutture.h"
#include "inizializzazioni.h"
#include "Utilities.h"
#include "Scena.h"
#include "Controller.hpp"
#include <array>

#include <ImGui\imgui.h>           // Libreria core ImGui
#include <ImGui\imgui_impl_glfw.h> // Integrazione ImGui con GLFW (gestione finestra/input)
#include <ImGui\imgui_impl_opengl3.h> // Integrazione ImGui con OpenGL3 (rendering)
#include "gestione_curve.h"
#include "Gui.h"

extern Figura bkg;
extern FiguraHermite pony;
extern FiguraHermite attacking_pony;
extern vector<FiguraHermite> carrots;
extern Figura fermenter;
extern vector<Figura> hearts;
extern vector<FiguraHermite> fangs;
extern FiguraHermite attacking_fang;
//extern vector<Figura> carrots;
extern vector<Figura> Scena;
extern int width, height;
extern ImVec4 clear_color;
extern mat4 Projection;
extern unsigned int selected_bk, programId_Triangul;
extern unsigned int IdProj_Triangul, IdModel_Triangul;
extern GLuint IdColorA, IdColorB;
extern Screen currentScreen;
float angoloPony = 0.0f;
float cambioAngolo = 4.0f;
extern float w_up, h_up;
extern GLFWwindow* window;

// Inizializzazioni per gestione_curve. figuraCorrente va assegnata prima di ogni costruzione di hermite.
float* t;
FiguraHermite figuraCorrente;

using Coord = float;
using Point2D = std::array<Coord, 2>;

void PosizionaOrientaScalaOrientatoY(Figura* pony, float angoloY = 0.0f, float angoloZ = 0.0f)
{
    // Inizializza la matrice di modellazione nel mondo orientata
    pony->ModelAngle = glm::mat4(1.0f);
    pony->ModelAngle = glm::translate(pony->ModelAngle, glm::vec3(pony->position.x, pony->position.y, 0.0f));
    pony->ModelAngle = glm::rotate(pony->ModelAngle, glm::radians(angoloY), glm::vec3(0, 1, 0));
    pony->ModelAngle = glm::rotate(pony->ModelAngle, glm::radians(angoloZ), glm::vec3(0, 0, 1));
    pony->ModelAngle = glm::scale(pony->ModelAngle, pony->scale);
}

void PosizionaOrientaScala(Figura* forma, float angolo = 0.0f)
{
    forma->Model = glm::mat4(1.0f);
    forma->Model = glm::translate(forma->Model, glm::vec3(forma->position.x, forma->position.y, 0.0f));
    forma->Model = glm::rotate(forma->Model, glm::radians(angolo), glm::vec3(0, 0, 1));
    forma->Model = glm::scale(forma->Model, forma->scale);
}

void costruisciAngoloPony() {
    Direction ponyDirection = getPony().direction;
    switch (ponyDirection) {
    case LEFT:
        if (angoloPony < 180.0f)
            angoloPony += cambioAngolo;
        if (angoloPony > 180.0f)
            angoloPony = 180.0f;
        break;
    case RIGHT:
        if (angoloPony > 0.0f)
            angoloPony -= cambioAngolo;
        if (angoloPony < 0.0f)
            angoloPony = 0.0f;
        break;
    default:
        break;
    }
}

void disegnaForma(Figura& forma) {
    // Se wireframe == true, imposta la modalita' di disegno in modo che tutte le facce siano solo contorni
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Associa il Vertex Array Object (VAO) della forma, che contiene i dati dei vertici da disegnare
    glBindVertexArray(forma.VAO);

    // Disegna i vertici della forma come specificato da forma.render,
    // a partire dal primo vertice (0), per (forma.nv - 4) vertici in totale
    // La sottrazione di 4, per escludere i vertici della bounding box
    glDrawArrays(forma.render, 0, forma.nv);
}

void disegnaFormaConBB(Figura& forma, bool wireframe = false, bool showBB = false) {
    // Se wireframe � true, imposta la modalit� di disegno in modo che tutte le facce siano solo contorni
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        // Altrimenti, riempi le facce in modalit� solida
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Associa il Vertex Array Object (VAO) della forma, che contiene i dati dei vertici da disegnare
    glBindVertexArray(forma.VAO);

    // Disegna i vertici della forma come specificato da forma.render,
    // a partire dal primo vertice (0), per (forma.nv - 4) vertici in totale
    // La sottrazione di 4 � per escludere i vertici della bounding box
    glDrawArrays(forma.render, 0, forma.nv - 4);
    if (showBB)
        // Se showBB � true, disegna la bounding box con una line loop collegando gli ultimi 4 vertici
        glDrawArrays(GL_LINE_LOOP, forma.nv - 4, 4);
}

void triangolarizzazzione_e_rendering(Curva* curva, vec4 colorA, vec4 colorB, mat4 model)
{
    vector<Point2D> polygon2D;
    polygon2D.reserve(curva->vertices.size());

    for (const auto& v : curva->vertices)
        polygon2D.push_back(Point2D{ v.x, v.y });


    vector<vector<Point2D>> polygon = { polygon2D };
    // Triangolazione facebndo uso della funzione eartcut
    vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    // Output degli indici dei triangoli
    curva->indices = indices;
    UPDATE_VAO_DYNAMIC(curva);

    glUseProgram(programId_Triangul);
    glUniform4f(IdColorA, colorA.r, colorA.g, colorA.b, colorA.a);
    glUniform4f(IdColorB, colorB.r, colorB.g, colorB.b, colorB.a);
    glUniformMatrix4fv(IdProj_Triangul, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(IdModel_Triangul, 1, GL_FALSE, value_ptr(model));
    glBindVertexArray(curva->VAO);
    glDrawElements(GL_TRIANGLES, curva->indices.size(), GL_UNSIGNED_INT, 0);  //Modalità di rendering GL_LINE_STRIP: tutti i vertici vengono uniti da una spezzata
    glBindVertexArray(0);
}

void disegnaPonyHermite(vector<Curva>* fig, vec3 position, float currentTime, float angoloY = 0.0f) {

    for (int i = 0; i < fig->size(); i++) {

        Curva curva = (*fig)[i];
        curva.position = position;
        if (getPonyStatus() == MOVING) {
            if (i == LEGFDX || i == LEGBSX)
                PosizionaOrientaScalaOrientatoY(&curva, angoloY, sin(currentTime * 5) * 5.0f);
            else if (i == LEGFSX || i == LEGBDX)
                PosizionaOrientaScalaOrientatoY(&curva, angoloY, -sin(currentTime * 5) * 5.0f);
            else if (i >= HAIR)
                PosizionaOrientaScalaOrientatoY(&curva, angoloY, sin(currentTime * 5) * 3.0f);
            else
                PosizionaOrientaScalaOrientatoY(&curva, angoloY);
        }
        else if (getPonyStatus() == ATTACKING) {
            if (i == LEGBDX) {
                curva = attacking_pony.curve[i];
                curva.position = position;
                curva.scale = vec3(curva.scale.x * (1 - 0.1 * sin(currentTime * 20)), curva.scale.y, 0.0);
                PosizionaOrientaScalaOrientatoY(&curva, angoloY);
            }
            else if (i == LEGBSX) {
                curva = attacking_pony.curve[i];
                curva.position = position;
                curva.scale = vec3(curva.scale.x * (1 - 0.1 * -sin(currentTime * 20)), curva.scale.y, 0.0);
                PosizionaOrientaScalaOrientatoY(&curva, angoloY);
            }
            else if (i == TAIL) {
                curva = attacking_pony.curve[i];
                curva.position = position;
                PosizionaOrientaScalaOrientatoY(&curva, angoloY);
            }
            else if (i < BODY) {
                curva.scale = vec3(curva.scale.x, curva.scale.y * 0.8, 0.0);
                PosizionaOrientaScalaOrientatoY(&curva, angoloY);
            }
            else if (i >= HAIR) {
                PosizionaOrientaScalaOrientatoY(&curva, angoloY, -10.0);
            }
            else
                PosizionaOrientaScalaOrientatoY(&curva, angoloY);
        }
        else
            PosizionaOrientaScalaOrientatoY(&curva, angoloY);

        triangolarizzazzione_e_rendering(&curva, (*fig)[i].colorA, (*fig)[i].colorB, curva.ModelAngle);

    }
}

void disegnaFangHermite(vector<Curva> fig, FangInfo fangInfo, float currentTime) {
    int i = 0;
    for (int i = 0; i < fig.size(); i++) {
        Curva curva = fig[i];
        curva.position = fangInfo.position;
        bool isMoving = fangInfo.status != ATTACKING && fangInfo.status != SPAWNING;
        if (fangInfo.status == DEAD)
            PosizionaOrientaScala(&curva, 180.0);
        else if (isMoving) {
            if (i == BACKLEG1)
                PosizionaOrientaScala(&curva, -sin(currentTime * 5) * 5.0f);
            else if (i == BACKLEG2)
                PosizionaOrientaScala(&curva, sin(currentTime * 5) * 5.0f);
            else if (i == MIDDLELEG1)
                PosizionaOrientaScala(&curva, -sin(currentTime * 5 + 1) * 5.0f);
            else if (i == MIDDLELEG2)
                PosizionaOrientaScala(&curva, sin(currentTime * 5 + 1) * 5.0f);
            else if (i == FRONTLEG1)
                PosizionaOrientaScala(&curva, -sin(currentTime * 5 + 2) * 5.0f);
            else if (i == FRONTLEG2)
                PosizionaOrientaScala(&curva, sin(currentTime * 5 + 2) * 5.0f);
            else
                PosizionaOrientaScala(&curva);
        }
        else if (fangInfo.status == ATTACKING) {
            if (i == RIGHTFANG) {
                curva = attacking_fang.curve[i];
                curva.position = fangInfo.position;
                PosizionaOrientaScala(&curva, -40.0 * abs(sin(currentTime * 10)));
            }
            else if (i == LEFTFANG) {
                curva = attacking_fang.curve[i];
                curva.position = fangInfo.position;
                PosizionaOrientaScala(&curva, 40.0 * abs(sin(currentTime * 10)));
            }
            else
                PosizionaOrientaScala(&curva);
        }
        else
            PosizionaOrientaScala(&curva);
        triangolarizzazzione_e_rendering(&curva, fig[i].colorA, fig[i].colorB, curva.Model);
    }
}

void disegnaCarotaHermite(vector<Curva> fig, float currentTime) {
    int i = 0;
    for (auto& curva : fig) {
        if (i == 0)
            PosizionaOrientaScala(&curva, sin(currentTime * 5) * 3.0f);
        else
            PosizionaOrientaScala(&curva);

        triangolarizzazzione_e_rendering(&curva, curva.colorA, curva.colorB, curva.Model);
        i++;
    }
}

void renderizza(float currentTime) {

    glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Render background
    bkg.shader_programId = selected_bk;
    usaShaderConUniforms(bkg.shader_programId, Projection, bkg.Model, currentTime, glm::vec2(w_up, h_up));
    disegnaFormaConBB(bkg);

    if (currentScreen == GAME)
    {
        // Render carrots
        for (auto& carrot : carrots) {
            disegnaCarotaHermite(carrot.curve, currentTime);
            PosizionaOrientaScala(&carrot.hitbox);
        }

        // Render fangs
        int i = 0;
        vector<FangInfo> fieldFangs = getFangsInfo();
        for (auto& fang : fangs) {
            fang.hitbox.position = fieldFangs[i].position;
            disegnaFangHermite(fang.curve, fieldFangs[i], currentTime);
            PosizionaOrientaScala(&fang.hitbox);
            i++;
        }

        // Render pony
        costruisciAngoloPony();
        pony.hitbox.position = getPonyPosition();
        disegnaPonyHermite(&pony.curve, getPonyPosition(), currentTime, angoloPony);
        PosizionaOrientaScala(&pony.hitbox);

        // Render hearts
        for (auto& heart : hearts) {
            PosizionaOrientaScala(&heart);
            heart.Model = glm::rotate(heart.Model, glm::radians(abs(sin(currentTime)) * 360.0f), glm::vec3(0, 1, 0));
            usaShaderConUniforms(heart.shader_programId, Projection, heart.Model, currentTime, glm::vec2((float)width, height));
            disegnaFormaConBB(heart);
        }
    }

    //ImGui
    my_interface(); // Disegna l�interfaccia ImGui
    /* Renderizza la GUI ImGui */
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}