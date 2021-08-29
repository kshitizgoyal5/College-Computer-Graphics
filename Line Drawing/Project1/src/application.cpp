#include<gl/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<map>
#include<vector>
#include<chrono>
#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include "../Header Files/Header.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
using namespace std;

int WIDTH = 1920;
int HEIGHT = 1030;
float r = 1, g = 1, b = 1, cr = 0.001, cg = 0.005, cb = 0.01;

struct key {
    bool cur, prev;
};

// GLOBAL VARIABLES
//int dots = 1, blanks = 0;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
GLFWwindow* window;
GLFWmonitor* monitor;
bool running = 1, fullscrean;
std::map<int, key> keyMap;
bool status = false;
vector<vector<double> > Q;
double x = 0, y = 0;
int times = 0;
int lda = 0;
int pattern = 15;
void update(double x, double y, double X, double Y, int times);

void menu() {
    system("cls");
    string curr, welcome = "\t\t\t........LINE DRAWING ALGORITHMS........\n"
        " Press S : SIMPLE DDA\n"
        " Press Y : SYMMETRICAL DDA\n"
        " PRESS H : BRESENHAM ALGORITHM\n"
        " PRESS M : MID-POINT ALGORITHM\n\n"
        " TO CHANGE PATTERN PRESS : 1-9/A-F\n"
        " SCROLL TO INCREASE / DECREASE THICKNESS\n\n\n"
        " THICKNESS : ";
    if (lda == 0) curr = " \n CURRENT ALGORITHM : "" SIMPLE DDA";
    if (lda == 1) curr = " \n CURRENT ALGORITHM : "" SYMMETRICAL DDA";
    if (lda == 2) curr = " \n CURRENT ALGORITHM : "" BRESENHAM ALGORITHM";
    if (lda == 3) curr = " \n CURRENT ALGORITHM : "" MID-POINT ALGORITHM";
    char temp = (pattern / 10 == 1) ? char('A' + pattern % 10) : char(pattern + '0');
    cout << welcome << times << curr << "\n Pattern : " << temp << flush;
    
}

void increaseThickness(double x, double y, double X, double Y, int times) {
    if (times == 0) return;
    double slope =  Y - y == 0? INT_MAX : -((X - x) / (Y - y));
    if (abs(slope) > 1) {
        double b = y - slope * x, B = Y - slope * X;
        for (int i = 1; i <= times; i++) {
            update(round((y + i - b) / slope), y + i, round((Y + i - B) / slope), Y + i, 0);
            update(round((y - i - b) / slope), y - i, round((Y - i - B) / slope), Y - i, 0);
        }
    }
    else {
        double b = y - slope * x, B = Y - slope * X;
        for (int i = 1; i <= times; i++) {
            update(x + i, round(slope * (x + i) + b), X + i, round(slope * (X + i) + B ), 0);
            update(x - i, round(slope * (x - i) + b), X - i, round(slope * (X - i) + B), 0);
        }
    }
}

void changeShade(float& color, float& change) {
    color += change;
    if (color >= 1) {
        color = 1;
        change = -change;
    }
    else if (color <= 0) {
        color = 0;
        change = -change;
    }
}

void showLineEndPoints() {
    if (Q.size() < 1) return;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POINTS);
    glVertex2f(2 * Q[0][0] / WIDTH, 2 * Q[0][1] / HEIGHT);
    if(Q.size() == 2) glVertex2f(2 * Q[1][0] / WIDTH, 2 * Q[1][1] / HEIGHT);
    glEnd();
}

void update(double x, double y, double X, double Y, int times) {
    if (Q.size() < 2) return;
    vector<vector<double> > line;
    switch (lda){
        case 0  :  line = simpleDDA(x, y, X, Y);
        case 1  :  line = symmerticalDDA(x, y, X, Y);
        case 2  :  line = bresenhamLDA(x, y, X, Y);
        case 3  :  line = midPointLDA(x, y, X, Y);
        default :  break;
    }
    if (line.size() < 2) return;
    //glColor3f(r, g, b);
    /*changeShade(r, cr);
    changeShade(g, cg);
    changeShade(b, cb);*/
    glColor4f(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glBegin(GL_POINTS);

    for (int i = 0; i <= line.size() / 2; i++){
        int x = 1 & pattern >> (i % 4);
        if (!x) continue;
        glVertex2f(2 * (line[i][0]) / WIDTH, 2 * (line[i][1]) / HEIGHT);
    }

    for (int i = line.size()-1; i > line.size() / 2; i--){
        int x = 1 & pattern >> (i % 4);
        if (!x) continue;
        glVertex2f(2 * (line[i][0]) / WIDTH, 2 * (line[i][1]) / HEIGHT);
    }
    glEnd();
    increaseThickness(x, y, X, Y, times);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    menu();
    times += yoffset;
    if (times < 0) times = 0;
    else times = times < 10 ? times : 10;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    unsigned int key_1 = GLFW_KEY_1;
    for (int KEY = key_1; KEY <= key_1 + 9; KEY++) {
        if (key == KEY && action == GLFW_PRESS) {
            pattern = 1 + KEY - key_1;
            menu();
        }    
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        pattern = 10;
        menu();
    }
    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        pattern = 11;
        menu();
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        pattern = 12;
        menu();
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        pattern = 13;
        menu();
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        pattern = 14;
        menu();
    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        pattern = 15;
        menu();
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        lda = 0;
        menu();
    }
    if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
        lda = 1;
        menu();
    }
    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        lda = 2;
        menu();
    }
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        lda = 3;
        menu();
    }
}

void input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
        glfwSetWindowShouldClose(window, 1);
    }
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        status = true;
        glfwGetCursorPos(window, &x, &y);
        y = -y;
        x -= WIDTH / 2;
        y += HEIGHT / 2;
        while (Q.size() > 1) {
            Q.pop_back();
        }
        Q.push_back({ x, y });
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        status = false;
        while (Q.size() > 1) {
            Q.erase(Q.begin());
        }
    }
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    keyMap[GLFW_KEY_F].prev = glfwGetKey(window, GLFW_KEY_F);
}

void draw() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    if (status && Q.size() >= 2) update(Q[0][0], Q[0][1], Q[1][0], Q[1][1], times);
    showLineEndPoints();  
}

void imgui_menu() {
    ImGui::Begin("Attribute Menu");
    ImGui::SetWindowPos(
        ImVec2(0, 0),
        true);
    ImGui::SetWindowSize(ImVec2(0, 0), true);
    const char* items[] = { "Simple DDA", "Symmetrical DDA", "Bresenham LDA", "Mid-Point LDA" };
    ImGui::Combo("Algorithm", &lda, items, IM_ARRAYSIZE(items));
    const char* Patterns[] = {"No Fill","1","2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
    ImGui::Combo("Pattern", &pattern, Patterns, IM_ARRAYSIZE(Patterns));
    ImGui::SliderInt("Thickness", &times, 0, 15);
    ImGui::ColorEdit3("Choose Color", (float*)&clear_color);
}

int main(){
    if (!glewInit()) return -1;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (!glfwInit()) fprintf(stderr, "Failed to init GLFW\n");
    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to open window!" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    bool err = glewInit() != 0;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    monitor = glfwGetPrimaryMonitor();
    running = true;
    fullscrean = false;
    double lastTime = 0.0;

    //menu();
    
    while (running) {
        double time = glfwGetTime();
        double deltaTime = time - lastTime;
        if (deltaTime >= 1 / 30) {
            glfwPollEvents();
            lastTime = time;
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            imgui_menu();
            input();
            draw();
            

            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }
     
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
} 

