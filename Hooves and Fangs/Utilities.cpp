#pragma once
#include "lib.h"
#include "strutture.h"
#include <random>
#include <fstream>
#include "Utilities.h"
#include "struttureModel.hpp"
#include "inizializzazioni.h"
#include "gestione_curve.h"
extern int width, height;
extern GLuint IdProj, IdModel;

extern unsigned int selected_bk;
extern unsigned int programId, programId_BK, programId_BK2;
extern Screen currentScreen;

extern vector<GameScore> scoreboard;

void goToGameScreen() {
    selected_bk = programId_BK2;
    currentScreen = GAME;
}

void leaveGameScreen(Screen newScreen) {
    selected_bk = programId_BK;
    currentScreen = newScreen;
}

void changeScreen(Screen newScreen) {
    currentScreen = newScreen;
}

vec2 randomPosition(int width, int height) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(0.0f, static_cast<float>(width));
    std::uniform_real_distribution<> disY(0.0f, static_cast<float>(height));
    return glm::vec2(disX(gen), disY(gen));
}

// Funzione per attivare uno shader e impostare le uniformi necessarie per la trasformazione e altri parametri
 // Parametri:
 // - shader_programId: ID del programma shader OpenGL da attivare
 // - projection: matrice di proiezione (glm::mat4) da passare allo shader
 // - model: matrice di trasformazione modello (mat4) da passare allo shader
 // - currentTime: valore float che rappresenta il tempo corrente (ad esempio per animazioni nello shader)
 // - resolution: vettore 2D (vec2) che indica la risoluzione del viewport o finestra

void usaShaderConUniforms(unsigned int shader_programId, glm::mat4& projection, mat4& model, float currentTime, vec2 resolution) {
    // Attiva il programma shader specificato
    glUseProgram(shader_programId);

    // Passa la matrice di proiezione alla uniform "IdProj"
    // 1 indica il numero di matrici, GL_FALSE significa che la matrice non deve essere trasposta
    // glm::value_ptr ritorna un puntatore ai dati della matrice in formato compatibile con OpenGL
    glUniformMatrix4fv(IdProj, 1, GL_FALSE, glm::value_ptr(projection));

    // Passa la matrice modello alla uniform "IdModel"
    glUniformMatrix4fv(IdModel, 1, GL_FALSE, glm::value_ptr(model));

    // Se la risoluzione è valida (componenti positive) passa la uniform "iResolution"
    // che comunemente rappresenta la dimensione della finestra in pixel nello shader
    if (resolution.x > 0.0f && resolution.y > 0.0f) {
        glUniform2f(glGetUniformLocation(shader_programId, "iResolution"), resolution.x, resolution.y);
    }

    // Se currentTime è positivo, passa la uniform "iTime"
    // Utilizzata spesso per animazioni legate al tempo nello shader
    if (currentTime > 0.0f) {
        glUniform1f(glGetUniformLocation(shader_programId, "iTime"), currentTime);
    }
}

void findBB(Figura* fig) {
    int n;
    n = fig->vertices.size();
    float minx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float miny = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    float maxx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il massimo iniziale
    float maxy = fig->vertices[0].y; // Assumiamo che il primo elemento sia il massimo iniziale

    for (int i = 1; i < n; i++) {
        if (fig->vertices[i].x < minx) {
            minx = fig->vertices[i].x;
        }
        if (fig->vertices[i].x > maxx) {
            maxx = fig->vertices[i].x;
        }

        if (fig->vertices[i].y < miny) {
            miny = fig->vertices[i].y;
        }

        if (fig->vertices[i].y > maxy) {
            maxy = fig->vertices[i].y;
        }
    }
     
    fig->min_BB_obj = vec4(minx, miny, 0.0, 1.0);
    fig->max_BB_obj = vec4(maxx, maxy, 0.0, 1.0);
}


/**
* Calcola il BB di una figura fatta di curve di Hermite consideranto solo le curve fino a quella di indice curvaBBindex.
* @param fig Puntatore a tipo FiguraHermite.
* @param curvaBBindex l'indice della curva (essa esclusa) fino alla quale si vuole considerare il BB.
*/
void findHermiteBB(FiguraHermite* fig, int curvaBBindex) {
    // Prepara i max e min per il BB
    float minx = 2.0f, maxx = 2.0f, miny = 2.0f, maxy = 2.0f;
    for (int i = 0; i < fig->curve.size(); i++) {
        if (i < curvaBBindex)
            for (auto& cp : fig->curve[i].CP) {

                //Inizializza i min e max BB la prima volta, poi aggiorna i min e max
                if (minx > 1.5f) {
                    minx = cp.x;
                    miny = cp.y;
                    maxx = cp.x;
                    maxy = cp.y;
                }
                if (cp.x < minx) {
                    minx = cp.x;
                }
                if (cp.x > maxx) {
                    maxx = cp.x;
                }

                if (cp.y < miny) {
                    miny = cp.y;
                }

                if (cp.y > maxy) {
                    maxy = cp.y;
                }
            }
    }

    fig->hitbox.min_BB_obj = vec4(minx, miny, 0.0, 1.0);
    fig->hitbox.max_BB_obj = vec4(maxx, maxy, 0.0, 1.0);
}

/**
* Calcola il Bounding Box laterale della figura pony in base alla direzione specificata.
* @param fig Puntatore alla figura di tipo Figura.
* @param ponyDirection Direzione del pony (LEFT o RIGHT).
* @return Un vettore vec4 che rappresenta il Bounding Box laterale calcolato. (minX, minY, maxX, maxY)
*/
vec4 findSideBB(Figura* fig, Direction ponyDirection) {

    float sizex = fig->max_BB_w.x - fig->min_BB_w.x;
    if (ponyDirection == RIGHT) {
        float minx = fig->min_BB_w.x - sizex * 0.8f;
        return vec4(minx, fig->min_BB_w.y, fig->min_BB_w.x, fig->max_BB_w.y);
    }
    else {
		float maxx = fig->max_BB_w.x + sizex * 0.8f;
		return vec4(fig->max_BB_w.x, fig->min_BB_w.y, maxx, fig->max_BB_w.y);
    }
}

// Aggiorna le coordinate del Bounding Box di una figura nel sistema di riferimento del mondo,
// applicando la matrice di modellazione alle coordinate locali.
// Parametri:
//   - fig: Puntatore alla struttura Figura il cui Bounding Box deve essere aggiornato.
void updateBB(Figura* fig) {
    // Trasforma direttamente le coordinate del Bounding Box locale nel sistema del mondo.
	fig->min_BB_w = fig->Model * fig->min_BB_obj;
	fig->max_BB_w = fig->Model * fig->max_BB_obj;
}

bool checkCollision(vec4 obj1Min, vec4 obj1Max, vec4 obj2Min, vec4 obj2Max) {
    // guardo collisioni su asse x
    bool collisionX = false;
    if (obj1Min.x <= obj2Max.x && obj1Max.x >= obj2Min.x) {
        collisionX = true;
    }

    // guardo collisioni su asse y
    bool collisionY = false;
    if (obj1Min.y <= obj2Max.y && obj1Max.y >= obj2Min.y) {
        collisionY = true;
    }

    //Si ha collisione se c'è collisione sia nella direzione x che nella direzione y
    return collisionX && collisionY;
}

// Verifica se due oggetti di tipo Figura collidono utilizzando i loro Bounding Box nel sistema di riferimento del mondo.
// Parametri:
//   - obj1: Prima figura da controllare per la collisione.
//   - obj2: Seconda figura da controllare per la collisione.
// Restituisce:
//   - true se i Bounding Box delle due figure si intersecano, false altrimenti.
bool checkCollision(Figura obj1, Figura obj2) {

	return checkCollision(obj1.min_BB_w, obj1.max_BB_w, obj2.min_BB_w, obj2.max_BB_w);


 //   // guardo collisioni su asse x
 //   bool collisionX=false;

	//if (obj1.min_BB_w.x <= obj2.max_BB_w.x && obj1.max_BB_w.x >= obj2.min_BB_w.x) {
	//	collisionX = true;
	//}

 //   // guardo collisioni su asse y

 //   bool collisionY=false;
	//if (obj1.min_BB_w.y <= obj2.max_BB_w.y && obj1.max_BB_w.y >= obj2.min_BB_w.y) {
	//	collisionY = true;
	//}

 //   //Si ha collisione se c'è collisione sia nella direzione x che nella direzione y

 //   return collisionX && collisionY;
}

// Trasforma e mappa le coordinate del model in coordinate della finestra, da 0.8 a -0.8.
vec3 positionModelToView(glm::vec2 position, glm::vec2 fieldDimensions)
{
    return vec3(position.x * 1.6 / fieldDimensions.x - 0.8, position.y * 1.6 / fieldDimensions.y - 0.8, 0.0);
}

extern FiguraHermite figuraCorrente;
//Carica da file una figura completa formata da curve di hermite.
void loadHermite(FiguraHermite* fig, const char* fileName, bool withColor)
{
    int i;
    struct Dati {
        int index_curva;
        float x;
        float y;
        float z;
        float Tens;
        float Bias;
        float Cont;
    };

    // Apertura input file
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Impossibile aprire il file");
    }


    // Resetta le curve esistenti (opzionale: elimina punti precedenti)
    for (auto& c : fig->curve) {
        c.CP.clear();
        c.Tens.clear();
        c.Bias.clear();
        c.Cont.clear();
        c.Derivata.clear();
    }

    Dati dati;
    int total_line = 0;
    while (fscanf(file, "%d %f %f %f %f %f %f", &dati.index_curva, &dati.x, &dati.y, &dati.z, &dati.Tens, &dati.Bias, &dati.Cont) == 7) {
        // Alloca curve vuote se non esistono abbastanza elementi
        //cout << "CURVA " << dati.index_curva << endl;
        while (fig->curve.size() <= dati.index_curva) {
            fig->curve.emplace_back(Curva());
            //cout << "Nuova curva " << dati.index_curva << endl;
        }
        // Aggiunge i dati solo alla curva giusta
        fig->curve[dati.index_curva].CP.push_back(vec3(dati.x, dati.y, dati.z));
        fig->curve[dati.index_curva].Tens.push_back(dati.Tens);
        fig->curve[dati.index_curva].Bias.push_back(dati.Bias);
        fig->curve[dati.index_curva].Cont.push_back(dati.Cont);
        fig->curve[dati.index_curva].Derivata.push_back(vec3(0.0, 0.0, 0.0));
        total_line++;
    }
    fclose(file);

    if (withColor)
    {
        struct DatiColore {
            int index_curva;
            float c1r;
            float c1g;
            float c1b;
            float c2r;
            float c2g;
            float c2b;
        };
        DatiColore dati_colore;
        string colorFileName = string(fileName);
        colorFileName.insert(colorFileName.size() - 4, "_color");
        FILE* color_file = fopen(colorFileName.c_str(), "r");
        if (color_file == NULL) {
            perror("Impossibile aprire il file Color");
        }
        for (auto& curva : fig->curve) {
            if (fscanf(color_file, "%d %f %f %f %f %f %f", &dati_colore.index_curva, &dati_colore.c1r, &dati_colore.c1g, &dati_colore.c1b, &dati_colore.c2r, &dati_colore.c2g, &dati_colore.c2b) == 7) {
                curva.colorA = vec4(dati_colore.c1r, dati_colore.c1g, dati_colore.c1b, 1.0);
                curva.colorB = vec4(dati_colore.c2r, dati_colore.c2g, dati_colore.c2b, 1.0);
            }
        }

        fclose(color_file);
    }


    for (int i = 0; i < fig->curve.size(); i++)
    {
        fig->curve[i].ncp = fig->curve[i].CP.size();
        fig->curve[i].nv = 200;
        fig->curve[i].ntg = 100;
        INIT_VAO_DYNAMIC(&fig->curve[i], true);
        //cout << "Curva " << i << "Numero di punti " << fig->curve[i].ncp << endl;

        figuraCorrente = *fig;
        t_corde(i);
        // Calcola la curva di Hermite
        CostruisciHermite(&fig->curve[i], i);

        // Aggiorna il VAO della curva
        UPDATE_VAO_DYNAMIC(&fig->curve[i], true);
    }
}

//Carica da file i punteggi.
void loadScoreboard() {
    FILE* file = fopen("scoreboard.txt", "r");
    if (file == NULL) {
        return;
    }
    char name[4];
    int score;
    int i = 0;
    while (fscanf(file, "%s %d", name, &score) == 2) {
        scoreboard.emplace_back(GameScore());
        scoreboard[i].name = string(name);
        scoreboard[i].score = score;
        i++;
    }
    fclose(file);
}

//Scrive su file i punteggi.
void saveScoreboard() {
    FILE* outputFile = fopen("scoreboard.txt", "w");
    if (outputFile == NULL) {
        return;
    }
    for (auto& score : scoreboard) {
        fprintf(outputFile, "%s %d\n", score.name.c_str(), score.score);
    }
    fclose(outputFile);
    //std::cout << "Dati scritti con successo." << std::endl;
}

void resetScoreboard() {
    scoreboard.clear();
    FILE* outputFile = fopen("scoreboard.txt", "w");
    if (outputFile == NULL) {
        return;
    }
    fprintf(outputFile, "\n");
    fclose(outputFile);
}