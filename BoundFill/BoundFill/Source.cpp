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
#include<unordered_map>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "CircleDrawing.h"
#include "lineDrawingalgos.h"

using namespace std;
extern int temp;
int WIDTH = 700;
int HEIGHT = 700;
int size = (WIDTH * HEIGHT * 3);
unsigned char buffer[700 * 700 * 3 * 2];
ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
ImVec4 fill_color = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);
ImVec4 bound_color = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);

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


struct Rectangle {
    double x1, y1, x2, y2;
    ImVec4 color;
    struct Rectangle(double x, double y, double X, double Y, ImVec4 c) {
        x1 = x;
        x2 = X;
        y1 = y;
        y2 = Y;
        color = c;
    }
    void draw() {
        glColor4f(color.x, color.y, color.z, color.w);
        glBegin(GL_LINES);
        glVertex2f(2.0 * x1 / WIDTH, 2.0 * y1 / HEIGHT);
        glVertex2f(2.0 * x1 / WIDTH, 2.0 * y2 / HEIGHT);

        glVertex2f(2.0 * x1 / WIDTH, 2.0 * y2 / HEIGHT);
        glVertex2f(2.0 * x2 / WIDTH, 2.0 * y2 / HEIGHT);

        glVertex2f(2.0 * x2 / WIDTH, 2.0 * y2 / HEIGHT);
        glVertex2f(2.0 * x2 / WIDTH, 2.0 * y1 / HEIGHT);

        glVertex2f(2.0 * x2 / WIDTH, 2.0 * y1 / HEIGHT);
        glVertex2f(2.0 * x1 / WIDTH, 2.0 * y1 / HEIGHT);
        glEnd();
    }
};


vector<Rectangle> rect_buff;

bool rBut = false, lBut = false;

struct fill_point {
    double first, second;
    ImVec4 color;
    struct fill_point(double a, double b, ImVec4 c) {
        first = a;
        second = b;
        color = c;
    }
};

vector<fill_point> fill_color_buffer;

int options = 0;
double X1=0, X2=0, Y1=0, Y2=0;

unordered_map<int, ImVec4> hashColor;

void color_pixel(double x, double y, ImVec4 color) {
    glColor4f(color.x, color.y, color.z, 1);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();

    //if point wise display
    glfwSwapBuffers(window);
}

void color_pixelit(double x, double y, ImVec4 color) {
    glColor4f(color.x, color.y, color.z, 1);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

bool isEqual(float a, float b) {
    if (a > b - 5 && a < b + 5) return true;
    return false;
}

bool check(int i, ImVec4 color) {
    if ((  isEqual(buffer[i    ], color.x * 255) && 
         ( isEqual(buffer[i + 1], color.y * 255) && 
         ( isEqual(buffer[i + 2], color.z * 255)) )
        )
       ){
        return false;
    }
    return true;
}

queue<pair<int, ImVec4>> col_buff;
// Recursive approach
void boundaryFill(double X, double Y, ImVec4 color, ImVec4 my_color) {
    int i = ((X + WIDTH / 2) + WIDTH * (Y + HEIGHT / 2)) * 3;
    if (check(i, color) && check(i, my_color)) {
        buffer[i] = 255 * my_color.x;
        buffer[i + 1] = 255 * my_color.y;
        buffer[i + 2] = 255 * my_color.z;
        //color_pixel(2.0 * X / WIDTH, 2.0 * Y / HEIGHT, my_color);
        col_buff.push(make_pair((X + WIDTH / 2) * WIDTH + Y + HEIGHT / 2, my_color));
        boundaryFill(X + 1, Y    , color, my_color);
        boundaryFill(X - 1, Y    , color, my_color);
        boundaryFill(X    , Y + 1, color, my_color);
        boundaryFill(X    , Y - 1, color, my_color);
    }
}

//Iterative approach
void boundaryFillIt(double X, double Y, ImVec4 color, ImVec4 my_color) {
    queue<vector<double> > Q;
    Q.push({ X, Y });
    while (!Q.empty()) {
        int size = Q.size();
        //if (Q.size() > 200 * 5) break;
        for (int j = 0; j < size; j++) {
            double a = Q.front()[0], b = Q.front()[1];
            int i = ((a + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3;
            Q.pop();
            if (a<-WIDTH / 2 || a>WIDTH / 2) continue;
            if (b<-HEIGHT / 2 || b>HEIGHT / 2) continue;
            if (check(i, color) && check(i, my_color)) {
                    Q.push({ a + 1, b });
                    Q.push({ a - 1, b });
                    Q.push({ a, b + 1 });
                    Q.push({ a, b - 1 });
            }
            else continue;
            buffer[i] = 255 * my_color.x;
            buffer[i + 1] = 255 * my_color.y;
            buffer[i + 2] = 255 * my_color.z;
            hashColor[(a+WIDTH/2) * WIDTH + b+HEIGHT/2] = my_color;
        }
    }
}


// Recursive approach
void floodFill(double X, double Y, ImVec4 color, ImVec4 my_color) {
    int i = ((X + WIDTH / 2) + WIDTH * (Y + HEIGHT / 2)) * 3;
    if (!check(i, color)) {
        buffer[i] = 255 * my_color.x;
        buffer[i + 1] = 255 * my_color.y;
        buffer[i + 2] = 255 * my_color.z;
        //color_pixel(2.0 * X / WIDTH, 2.0 * Y / HEIGHT, my_color);
        col_buff.push(make_pair((X + WIDTH / 2) * WIDTH + Y + HEIGHT / 2, my_color));
        floodFill(X + 1, Y, color, my_color);
        floodFill(X - 1, Y, color, my_color);
        floodFill(X, Y + 1, color, my_color);
        floodFill(X, Y - 1, color, my_color);
    }
}

// Iterative Approach
void floodFillIt(double X, double Y, ImVec4 color, ImVec4 my_color) {
    queue<vector<double> > Q;
    Q.push({ X, Y });
    while (!Q.empty()) {
        int size = Q.size();
        //if (Q.size() > 200 * 5) break;
        for (int j = 0; j < size; j++) {
            double a = Q.front()[0], b = Q.front()[1];
            int i = ((a + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3;
            Q.pop();
            if (a<-WIDTH / 2 || a>WIDTH / 2) continue;
            if (b<-HEIGHT / 2 || b>HEIGHT / 2) continue;
            if (!check(i, color)) {
                Q.push({ a + 1, b });
                Q.push({ a - 1, b });
                Q.push({ a, b + 1 });
                Q.push({ a, b - 1 });
            }
            else continue;
            buffer[i] = 255 * my_color.x;
            buffer[i + 1] = 255 * my_color.y;
            buffer[i + 2] = 255 * my_color.z;
            hashColor[(a + WIDTH / 2) * WIDTH + b + HEIGHT / 2] = my_color;
        }
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
        if (y < HEIGHT/2 - 150) {
            if (rBut == false) {
                rBut = true;
                if (options == 0) {
                    fill_color_buffer.clear();
                    X1 = x;
                    Y1 = y;
                    options = 1;
                }
                else if (options == 1) {
                    X2 = x;
                    Y2 = y;
                    Rectangle rect(X1, Y1, X2, Y2, fill_color);
                    rect_buff.push_back(rect);
                    options = 0;
                }
                else if (options == 3) {
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    fill_color_buffer.push_back(fill_point(x, y, clear_color));
                    for (auto c : rect_buff) {
                        c.draw();
                    }
                    glReadPixels(0, 0, HEIGHT, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                    boundaryFill(x, y, bound_color, clear_color);
                    while (!col_buff.empty()) {
                        auto c = col_buff.front();
                        col_buff.pop();
                        color_pixel(2.0 * (c.first / WIDTH - WIDTH / 2) / WIDTH, 2.0 * (c.first % WIDTH - HEIGHT / 2) / HEIGHT, c.second);
                    }
                }
                else if (options == 4) {
                    fill_color_buffer.push_back(fill_point(x, y, clear_color));
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    for (auto c : rect_buff) {
                        c.draw();
                    }
                    glReadPixels(0, 0, HEIGHT, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                    ImVec4 curColor;
                    int i = ((x + WIDTH / 2) + WIDTH * (y + HEIGHT / 2)) * 3;
                    curColor.x = 1.0*buffer[i] / 255.0;
                    curColor.y = 1.0 * buffer[i + 1] / 255.0;
                    curColor.z = 1.0 * buffer[i + 2] / 255.0;
                    curColor.w = 1;
                    floodFill(x, y, curColor, clear_color);
                   

                    while (!col_buff.empty()) {
                        auto c = col_buff.front();
                        col_buff.pop();
                        color_pixel(2.0 * (c.first / WIDTH - WIDTH / 2) / WIDTH, 2.0 * (c.first % WIDTH - HEIGHT / 2) / HEIGHT, c.second);
                    }
                }
                else if (options == 5) {
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT);
                    fill_color_buffer.push_back(fill_point(x, y, clear_color));
                    for (auto c : rect_buff) {
                        c.draw();
                    }
                    glReadPixels(0, 0, HEIGHT, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                    for (auto c : fill_color_buffer) {
                        boundaryFillIt(c.first, c.second, bound_color, c.color);
                    }
                }
                else if (options == 6) {
                    glReadPixels(0, 0, HEIGHT, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, buffer);
                    ImVec4 curColor;
                    int i = ((x + WIDTH / 2) + WIDTH * (y + HEIGHT / 2)) * 3;
                    curColor.x = 1.0 * buffer[i] / 255.0;
                    curColor.y = 1.0 * buffer[i + 1] / 255.0;
                    curColor.z = 1.0 * buffer[i + 2] / 255.0;
                    curColor.w = 1;
                    floodFillIt(x, y, curColor, clear_color);
                    cout << 1;
                }
            }
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        if (rBut == true) rBut = false;
    }

    keyMap[GLFW_KEY_F].prev = glfwGetKey(window, GLFW_KEY_F);
}


void draw() {
    if (options == 0 || options == 1 || options == 5 || options == 6) {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto c : rect_buff) {
            c.draw();
        }

        for (auto it = hashColor.begin(); it != hashColor.end(); ++it) {
            ImVec4 c = it->second;
            //cout << it->first / WIDTH << " " << it->first % WIDTH << endl;
            color_pixelit(2.0 * (it->first / WIDTH - WIDTH / 2) / WIDTH, 2.0 * (it->first % WIDTH - HEIGHT/2) / HEIGHT, c);
        }
    }
}

void imgui_menu() {
    ImGui::Begin("Attribute Menu");
    ImGui::SetWindowPos(
        ImVec2(0, 0),
        true);
    ImGui::SetWindowSize(ImVec2(550, 120), 0);
    if (ImGui::Button("Rectangle")) {
        options = 0;
        fill_color_buffer.clear();
        hashColor.clear();
    }
    ImGui::SameLine();
    if (ImGui::Button("Boundary Fill")) options = 3;
    ImGui::SameLine();
    if (ImGui::Button("Flood Fill")) options = 4;
    ImGui::SameLine();
    if (ImGui::Button("Boundary Fill It.")) options = 5;
    ImGui::SameLine();
    if (ImGui::Button("Flood Fill It.")) options = 6;
    ImGui::ColorEdit3("Choose Rectangle Color", (float*)&fill_color);
    ImGui::ColorEdit3("Choose Boundary Color", (float*)&bound_color);
    ImGui::ColorEdit3("Choose Fill Color", (float*)&clear_color);
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
    glfwSwapInterval(2);
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

