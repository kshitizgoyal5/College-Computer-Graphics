#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<map>
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
#include "CircleDrawing.h"
#include "lineDrawingalgos.h"

using namespace std;
extern int temp;
#define PI 3.141592653589
int WIDTH = 700;
int HEIGHT = 700;
int size = (WIDTH * HEIGHT * 3);
unsigned char buffer[700 * 700 * 3 * 2];
ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
// GLOBAL VARIABLES
struct key {
    bool cur, prev;
};
GLFWwindow* window;
GLFWmonitor* monitor;
bool running = 1, fullscrean;
std::map<int, key> keyMap;
bool status = false;
double x = 0, y = 0;
int gap = 2;
CircleDrawing circle(0, 0);
vector<vector<double> > buffer2;
int pattern = 15;
double a, b;
int maxSize = 8, Index = 0, tempIndex = -1, inc = 0;
double Angle = 45;
bool filled = false;
vector<ImVec4> myColor(4);
void boundaryFill(double, double, ImVec4&);
int speed = 10, slowSpeed = 0;
struct pixel {
    double x, y;
    double xO, yO;
    double angle;
    ImVec4 color;
    vector<vector<double>> buffer;
    pixel()
    {
        x = 0;
        y = 0;
        angle = 0;
    }
};
vector<pixel> Q(maxSize);
void input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        slowSpeed++;
        if (slowSpeed > 10) slowSpeed = 10;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        slowSpeed--;
        if (slowSpeed <= 0) slowSpeed = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Angle = abs(Angle);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        Angle = -abs(Angle);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        status = true;
        glfwGetCursorPos(window, &x, &y);
        y = -y;
        x -= WIDTH / 2;
        y += HEIGHT / 2;
        if (y < 90) {
            myColor[0] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            myColor[1] = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
            myColor[2] = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);
            myColor[3] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
            if (Index != maxSize) {
                Q[Index].x = x;
                Q[Index].y = y;
                Q[Index].xO = 0;
                Q[Index].yO = 0;
                Q[Index].angle = 0;
                Q[Index].color = myColor[Index % 4];
                tempIndex = (Index) % (maxSize + 1);
            }
        }
        
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        status = true;
        glfwGetCursorPos(window, &a, &b);
        b = -b;
        a -= WIDTH / 2;
        b += HEIGHT / 2;
        circle.translate(a, b);
        for (int i = 0; i < Index; i++) {
            double dx = a - Q[i].xO, dy = b - Q[i].yO;
            Q[i].x += dx;
            Q[i].y += dy;
            for (int j = 0; j < Q[i].buffer.size(); j++) {
                Q[i].buffer[j][0] += dx;
                Q[i].buffer[j][1] += dy;
            }
            Q[i].xO = a;
            Q[i].yO = b;
        }

    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        if (Index != maxSize)
            Index = (tempIndex + 1) % (maxSize + 1);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    }
    keyMap[GLFW_KEY_F].prev = glfwGetKey(window, GLFW_KEY_F);
}

bool isEqual(float a, float b) {
    int q = a * 255, r = b * 255;
    if (q > r - 5 && q < r + 5) return true;
    return false;
}

bool check(int i, ImVec4 color) {
    if (!(isEqual(buffer[i] / 255.0, color.x) && (isEqual(buffer[i + 1] / 255.0, color.y)) && (isEqual(buffer[i + 2] / 255.0, color.z)))) {
        return true;
    }
    return false;
}

void rotatePoint(double angle, double& X, double& Y, double a = 0, double b = 0) {
    double pi = 2 * acos(0.0), tx = X;

    X = a + round((X - a) * cos(angle * pi / 180) - (Y - b) * sin(angle * pi / 180));
    Y = b + round((tx - a) * sin(angle * pi / 180) + (Y - b) * cos(angle * pi / 180));
}


void boundaryFill(double X, double Y, ImVec4& color, ImVec4& my_color, int idx) {
    queue<vector<double> > Que;
    Que.push({ X, Y });
    while (!Que.empty()) {
        int size = Que.size();
        if (Que.size() > 100 * 5) break;
        for (int j = 0; j < size; j++) {
            double a = Que.front()[0], b = Que.front()[1];
            int i = ((a + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3;
            Que.pop();
            if (a<-WIDTH / 2 || a>WIDTH / 2) continue;
            if (b<-HEIGHT / 2 || b>HEIGHT / 2) continue;
            if (check(i, color) && check(i, my_color)) {
                if (check(((a + 1 + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3, color))
                    Que.push({ a + 1, b });
                if (check(((a - 1 + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3, color))
                    Que.push({ a - 1, b });
                if (check(((a + WIDTH / 2) + WIDTH * (b + 1 + HEIGHT / 2)) * 3, color))
                    Que.push({ a, b + 1 });
                if (check(((a + WIDTH / 2) + WIDTH * (b - 1 + HEIGHT / 2)) * 3, color))
                    Que.push({ a, b - 1 });
            }
            else continue;
            buffer[i] = 255 * my_color.x;
            buffer[i + 1] = 255 * my_color.y;
            buffer[i + 2] = 255 * my_color.z;

            Q[idx].buffer.push_back({ a, b });
            glColor4f(my_color.x, my_color.y, my_color.z, my_color.w);
            glBegin(GL_POINTS);
            glVertex2f(2.0 * a / WIDTH, 2.0 * b / HEIGHT);
            glEnd();
        }
    }
}

void draw() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //cout << Index << endl;
    if (circle.circle.size() >= 2) {
        glColor4f(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glBegin(GL_POINTS);
        for (int i = 0; i < circle.circle.size(); i++) {
            int check = (pattern >> ((i / gap) % 4)) & 1;
            if (check) glVertex2f(2.0 * (circle.circle[i][0]) / WIDTH, 2.0 * (circle.circle[i][1]) / HEIGHT);
        }
        glEnd();

        glColor4f(1, 1, 1, 1);
        glBegin(GL_POINTS);
        for (int i = 0; i < circle.spokes.size(); i++) {
            glVertex2f(2.0 * (circle.spokes[i][0]) / WIDTH, 2.0 * (circle.spokes[i][1]) / HEIGHT);
        }
        glEnd();

        if ((Index > 0 && Index <= maxSize) && filled == false) {
            glReadPixels(0, 0, HEIGHT, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, buffer);
            for (int i = 0; i < Index; i++) {
                boundaryFill(Q[i].x, Q[i].y, clear_color, Q[i].color, i);
            }
            if (Index == maxSize) filled = true;

        }
        else {
            for (int i = 0; i < maxSize; i++) {
                glColor4f(Q[i].color.x, Q[i].color.y, Q[i].color.z, Q[i].color.w);
                glBegin(GL_POINTS);
                for (int j = 0; j < Q[i].buffer.size(); j++) {
                    glVertex2f(2.0 * Q[i].buffer[j][0] / WIDTH, 2.0 * Q[i].buffer[j][1] / HEIGHT);
                }
                glEnd();
            }
        }
        glFlush();

        if (Angle > 0) {
            if (Index == maxSize && !speed) {
                speed = slowSpeed;
                vector<ImVec4> temp;
                for (int i = 0; i < maxSize; i++) {
                    ImVec4 t;
                    double dx = Q[i].x, dy = Q[i].y;
                    rotatePoint(-45, dx, dy, circle.X, circle.Y);
                    float tempColor[3];
                    glReadPixels(dx + WIDTH / 2, dy + HEIGHT / 2, 1, 1, GL_RGB, GL_FLOAT, tempColor);
                    t.x = tempColor[0];
                    t.y = tempColor[1];
                    t.z = tempColor[2];
                    t.w = 1;
                    temp.push_back(t);
                }
                for (int i = 0; i < maxSize; i++) {
                    Q[i].color = temp[i];
                }

            }
            speed--;
            if (speed < 0) speed = 0;
        }
        else {
            if (Index == maxSize && !speed) {
                speed = slowSpeed;
                vector<ImVec4> temp;
                for (int i = 0; i < maxSize; i++) {
                    ImVec4 t;
                    double dx = Q[i].x, dy = Q[i].y;
                    rotatePoint(45, dx, dy, circle.X, circle.Y);
                    float tempColor[3];
                    glReadPixels(dx + WIDTH / 2, dy + HEIGHT / 2, 1, 1, GL_RGB, GL_FLOAT, tempColor);
                    t.x = tempColor[0];
                    t.y = tempColor[1];
                    t.z = tempColor[2];
                    t.w = 1;
                    temp.push_back(t);
                }
                for (int i = 0; i < maxSize; i++) {
                    Q[i].color = temp[i];
                }

            }
            speed--;
            if (speed < 0) speed = 0;
        }
    }
}

void imgui_menu() {
    ImGui::Begin("Attribute Menu");
    ImGui::SetWindowSize({400, 90}, 0);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Speed = %d", 10-slowSpeed);
    ImGui::Text(Angle > 0 ? "Anti-Clockwise" : "Clockwise");
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

