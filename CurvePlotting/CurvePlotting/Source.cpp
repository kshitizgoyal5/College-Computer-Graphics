#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<map>
#include<unordered_map>
#include<vector>
#include<string>
#include<chrono>
#include<stdio.h>
#include<stdlib.h>
#include <queue>
#include<math.h>
#include<algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "lineDrawingalgos.h"

using namespace std;

int WIDTH = 700;
int HEIGHT = 700;
bool leftClick = false, rightClick = false;
double x = 0, y = 0;
int X = 0, Y = 1;
bool change = false;
bool option = false;
// GLOBAL VARIABLES
int tempI = -1;

struct key {
    bool cur, prev;
};
GLFWwindow* window;
GLFWmonitor* monitor;
bool running = 1, fullscrean;
std::map<int, key> keyMap;


int points[500][2], s = 0, tempsize = 0;
double P[500];

int factorial(int n) {
    if (n == 1) return 1;
    return factorial(n - 1) * n;
}

void plotPt(int xpos, int ypos) {
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex2f(2.0 * xpos / WIDTH, 2.0 * ypos / HEIGHT);
    glEnd();
}

bool checkCache(int xpos, int ypos) {
    for (int i = 0; i < tempsize; i++) {
        if (points[i][X] > xpos - 10 && points[i][X] < xpos + 10) {
            if (points[i][Y] > ypos - 10 && points[i][Y] < ypos + 10) {
                tempI = i;
                return true;
            }
        }
    }
    return false;
}

void input() {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
        glfwSetWindowShouldClose(window, 1);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwGetCursorPos(window, &x, &y);
        x = x - WIDTH / 2;
        y = HEIGHT / 2 - y;
        if (x < -WIDTH / 2 + 400 && y > HEIGHT / 2 - 120) {}
        else if (checkCache(x, y) || leftClick) {
            leftClick = true;
            points[tempI][X] = x;
            points[tempI][Y] = y;
        }
        else if (!leftClick) {
            leftClick = !leftClick;
            if (x < -WIDTH/2 + 400 && y > HEIGHT/2 - 120) {}
            else {
                points[tempsize][0] = x;
                points[tempsize][1] = y;
                tempsize++;
                plotPt(x, y);
                cout << s << endl;
            }
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        if (leftClick) {
            leftClick = !leftClick;
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!rightClick) {
            rightClick = !rightClick;
            glfwGetCursorPos(window, &x, &y);
            x = x - WIDTH / 2;
            y = HEIGHT / 2 - y;
            if (x < -WIDTH / 2 + 400 && y > HEIGHT / 2 - 120) {}
            else {
                change = true;
                points[tempsize][0] = x;
                points[tempsize][1] = y;
                tempsize++;
                s = tempsize;
                double a = 1, b = 1;
                for (int i = 0; i < s; i++) {
                    P[i] = a / b;
                    a *= (s - 1 - i);
                    b *= (i + 1);
                    P[s - 1] = 1;
                    cout << P[i] << " ";
                }
                plotPt(x, y);
                cout << s << endl;
            }
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        if (rightClick) {
            rightClick = !rightClick;
            glfwGetCursorPos(window, &x, &y);
        }
    }
    keyMap[GLFW_KEY_F].prev = glfwGetKey(window, GLFW_KEY_F);
}

void func() {
    if (s < 2) return;
    double X_pos = points[0][0], Y_pos = points[0][1];
    for (int i = 0; i <= (s)*20; i++) {
        double t = (1.0 * i) / (s * 20.0);
        double q = 1 - t;
        double x_pos = 0, y_pos = 0;
        for (int j = 0; j < s; j++) {
            if (s - j - 1 == 0) {
                x_pos += P[j] * pow(t, j) * (1.0 * points[j][X]);
                y_pos += P[j] * pow(t, j) * (1.0 * points[j][Y]);
            }
            else {
                x_pos += P[j] * pow(q, s - j - 1) * pow(t, j) * (1.0 * points[j][X]);
                y_pos += P[j] * pow(q, s - j - 1) * pow(t, j) * (1.0 * points[j][Y]);
            }
        }
        glPointSize(1);
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(2.0 * X_pos / WIDTH, 2.0 * Y_pos / HEIGHT);
        glVertex2f(2.0 * x_pos / WIDTH, 2.0 * y_pos / HEIGHT);
        glEnd();
        X_pos = x_pos;
        Y_pos = y_pos;
    }
}

int order = 3;
int I = 0;
int J = s - 1 + order;

double N_func(double i, double k, double t) {
    if (k == 1) {
        if (i <= t && t < i + 1) return 1;
        return 0;
    }
    double x1 = (t - i) / (k - 1);
    double x2 = (i+k - t) / (k - 1);
    return x1 * N_func(i, k - 1, t) + x2 * N_func(i + 1, k - 1, t);
}

void splineCurve() {
    if (s < 2) return;
    J = s - 1 + 2*order - 1;
    double X_pos = points[0][X], Y_pos = points[0][Y];
    for (int i = 20*order; i <= (J) * 20; i++) {
        double t = (1.0 * i) / (20.0) ;
        double x_pos = 0, y_pos = 0;
        //cout << "For t = " << t << " : ";
        for (int j = 0; j < s; j++) {
            if (j == 0) {
                for (int l = 0; l < order; l++) {
                    double z = N_func(1.0 * l, order, t);
                    x_pos += points[0][X] * z;
                    y_pos += points[0][Y] * z;
                }
            }
            else if (j == s - 1) {
                for (int l = 0; l < order; l++) {
                    double z = N_func(1.0 * (j + l + order - 1), order, t);
                    x_pos += points[s-1][X] * z;
                    y_pos += points[s-1][Y] * z;
                }
            }
            else {
                double z = N_func(1.0 * (j + order - 1), order, t);
                x_pos += points[j][X] * z;
                y_pos += points[j][Y] * z;
                //cout << z << " ";
            }
            
        }
        //cout << endl;
        //cout << X_pos << " " << Y_pos << endl;
        glPointSize(1);
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(2.0 * X_pos / WIDTH, 2.0 * Y_pos / HEIGHT);
        glVertex2f(2.0 * x_pos / WIDTH, 2.0 * y_pos / HEIGHT);
        glEnd();
        X_pos = x_pos;
        Y_pos = y_pos;
    }
    //cout << endl << endl;
}

void draw() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < tempsize; i++) {
        plotPt(points[i][X], points[i][Y]);
    }
    if (change) {
        change = false;
        if (option) func();
        else splineCurve();
    }
    if (option) func();
    else splineCurve();
}

void imgui_menu() {
    ImGui::Begin("Attribute Menu");
    ImGui::SetWindowSize({ 400, 120 }, 0);
    ImGui::Text("Reset Clip Window      : ");
    ImGui::SameLine();
    if (ImGui::Button("reset")) {
        tempsize = 0;
        s = 0;
        tempI = -1;
    }
    if (ImGui::Button("Bezier Curve")) option = true;
    ImGui::SameLine();
    if (ImGui::Button("Spline Curve")) option = false;
    string s = (option) ? "Bezier Curve" : "Spline Curve";
    ImGui::Text("Algorithm used : %s", s.c_str());
    if(!option)
        ImGui::InputInt("Order", &order);
}

int main() {
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

        if (deltaTime >= 1 / 120) {
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

