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

#include "lineDrawingalgos.h"

using namespace std;

int WIDTH = 700;
int HEIGHT = 700;
bool leftClick = false, rightClick = false;
double x = 0, y = 0;
int X = 0, Y = 1;
bool option = false;
string s1 = "Point Clipping";
string s2 = "Line  Clipping";
ImVec4 clip_color = ImVec4(1.00f, 0.00f, 1.00f, 1.00f);
ImVec4 line_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
// GLOBAL VARIABLES
struct key {
    bool cur, prev;
};
GLFWwindow* window;
GLFWmonitor* monitor;
bool running = 1, fullscrean;
std::map<int, key> keyMap;

int clipWindow[2][2], clipWindowIndex = 0;
bool clipWindowSet = false, changeClipWindow = true;

int linesBuffer[500][2], lineIndex = -1;


bool LineBuffercheck = false;

void resetClipWindow() {
    clipWindow[0][X] = 0;
    clipWindow[1][X] = WIDTH;

    clipWindow[0][Y] = 0;
    clipWindow[1][Y] = HEIGHT;
}

void resetLineBuffer() {
    lineIndex = -1;
}

void translateClipWindow(int xpos, int ypos) {
    int midx = (clipWindow[1][X] + clipWindow[0][X]) / 2, midy = (clipWindow[0][Y] + clipWindow[1][Y]) / 2;
    clipWindow[0][X] += xpos - midx;
    clipWindow[1][X] += xpos - midx;
    clipWindow[0][Y] += ypos - midy;
    clipWindow[1][Y] += ypos - midy;
}

void draw_axis() {
    glColor4f(1, 1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(0, 1);
    glVertex2f(0, -1);

    glVertex2f(-1, 0);
    glVertex2f(1, 0);
    glEnd();
}

void input() {
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        running = false;
        glfwSetWindowShouldClose(window, 1);
        
    }
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!leftClick) {
            leftClick = !leftClick;
            glfwGetCursorPos(window, &x, &y);
            y = HEIGHT - y;
            if (x < 400 && y > HEIGHT - 120) {}
            else{
                if (changeClipWindow) {
                    clipWindow[clipWindowIndex][X] = x;
                    clipWindow[clipWindowIndex][Y] = y;
                    clipWindowIndex++;
                    if (clipWindowIndex == 2) {
                        changeClipWindow = false;
                        if (clipWindow[0][X] > clipWindow[1][X]) swap(clipWindow[0][X], clipWindow[1][X]);
                        if (clipWindow[0][Y] > clipWindow[1][Y]) swap(clipWindow[0][Y], clipWindow[1][Y]);
                    }
                }
                else {
                    linesBuffer[lineIndex + 1][X] = x;
                    linesBuffer[lineIndex + 1][Y] = y;
                    lineIndex++;
                }
            }
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        if (leftClick) {
            leftClick = !leftClick;
            glfwGetCursorPos(window, &x, &y);
        }
    }


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        /*if (!rightClick) {
            rightClick = !rightClick;
            
        }*/
        glfwGetCursorPos(window, &x, &y);
        translateClipWindow(x, HEIGHT - y);
    }

    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        if (rightClick) {
            rightClick = !rightClick;
            glfwGetCursorPos(window, &x, &y);
        }
    }
    keyMap[GLFW_KEY_F].prev = glfwGetKey(window, GLFW_KEY_F);
}

bool pointClipping(int x_pos, int y_pos) {

    if (clipWindow[0][X] < x_pos && clipWindow[1][X] > x_pos) {
        if (clipWindow[0][Y] < y_pos && clipWindow[1][Y] > y_pos) {
            return true;
        }
    }
    return false;
}

void drawClipWindow() {
    glColor4f(clip_color.x, clip_color.y, clip_color.z, clip_color.w);
    glBegin(GL_LINES);
    glVertex2f(2.0 * (clipWindow[0][X]) / WIDTH - 1, 2.0 * (clipWindow[0][Y]) / HEIGHT - 1);
    glVertex2f(2.0 * (clipWindow[1][X]) / WIDTH - 1, 2.0 * (clipWindow[0][Y]) / HEIGHT - 1);
                                                                                   
    glVertex2f(2.0 * (clipWindow[1][X]) / WIDTH - 1, 2.0 * (clipWindow[0][Y]) / HEIGHT - 1);
    glVertex2f(2.0 * (clipWindow[1][X]) / WIDTH - 1, 2.0 * (clipWindow[1][Y]) / HEIGHT - 1);
                                                                                   
    glVertex2f(2.0 * (clipWindow[1][X]) / WIDTH - 1, 2.0 * (clipWindow[1][Y]) / HEIGHT - 1);
    glVertex2f(2.0 * (clipWindow[0][X]) / WIDTH - 1, 2.0 * (clipWindow[1][Y]) / HEIGHT - 1);
                                                                                  
    glVertex2f(2.0 * (clipWindow[0][X]) / WIDTH - 1, 2.0 * (clipWindow[1][Y]) / HEIGHT - 1);
    glVertex2f(2.0 * (clipWindow[0][X]) / WIDTH - 1, 2.0 * (clipWindow[0][Y]) / HEIGHT - 1);
    glEnd();
}

void pointClippingAlgo() {
    for (int i = 0; i <= lineIndex; i += 2) {
        if (i == lineIndex) break;
        vector<vector<int>> line = bresenhamLDA(linesBuffer[i][X], linesBuffer[i][Y], linesBuffer[i + 1][X], linesBuffer[i + 1][Y]);
        glColor4f(line_color.x, line_color.y, line_color.z, line_color.w);
        glBegin(GL_POINTS);
        for (int j = 0; j < line.size(); j++) {
            if (pointClipping(line[j][X], line[j][Y])) {
                glVertex2f(2.0 * (line[j][X]) / WIDTH - 1, 2.0 * (line[j][Y]) / HEIGHT - 1);
            }
        }
        glEnd();
    }
}


void liangBarasky(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1, dy = y2 - y1;
    if (dx == 0 && (x1 - clipWindow[0][X] < 0 || clipWindow[1][X] - x1 < 0)) return;
    if (dy == 0 && (y1 - clipWindow[0][Y] < 0 || clipWindow[1][Y] - y1 < 0)) return;
    int data[4][2] = { {-dx, x1 - clipWindow[0][X]}, {dx, clipWindow[1][X] - x1}, {-dy, y1 - clipWindow[0][Y]}, {dy, clipWindow[1][Y] - y1} };
    double u1 = 0, u2 = 1;
    for (int i = 0; i < 4; i++) {
        if (data[i][0] < 0) {
            u1 = max(u1, (1.0 * data[i][1]) / data[i][0]);
        }
        if (data[i][0] > 0) {
            u2 = min(u2, (1.0 * data[i][1]) / data[i][0]);
        }
    }
    if (u1 > 1) {
        return;
    }
    if (u2 < 0) {
        return;
    }
    if (u1 > u2) return;

    int X1 = x1 + round(u1 * dx), Y1 = y1 + round(u1 * dy);
    int X2 = x1 + round(u2 * dx), Y2 = y1 + round(u2 * dy);
    glColor4f(line_color.x, line_color.y, line_color.z, line_color.w);
    glBegin(GL_LINES);
    glVertex2f(2.0 * (X1) / WIDTH - 1, 2.0 * (Y1) / HEIGHT - 1);
    glVertex2f(2.0 * (X2) / WIDTH - 1, 2.0 * (Y2) / HEIGHT - 1);
    glEnd();
}


void lineClipping() {
    for (int i = 0; i <= lineIndex; i += 2) {
        if (i == lineIndex) break;
        liangBarasky(linesBuffer[i][X], linesBuffer[i][Y], linesBuffer[i + 1][X], linesBuffer[i + 1][Y]);
    }
}


void draw() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_axis();
    if (clipWindowSet == true) {
        resetClipWindow();
        clipWindowSet = false;
        changeClipWindow = true;
        clipWindowIndex = 0;
    }

    if (LineBuffercheck == true) {
        resetLineBuffer();
        LineBuffercheck = false;
    }
    if (!changeClipWindow) drawClipWindow();

    if(option) pointClippingAlgo();
    else lineClipping();
}

void imgui_menu() {
    ImGui::Begin("Attribute Menu");
    ImGui::SetWindowSize({ 400, 120 }, 0);
    ImGui::Text("Reset Clip Window      : ");
    ImGui::SameLine();
    clipWindowSet = ImGui::Button("Reset", { 50,20 });
    ImGui::Text("Clear Screen      : ");
    ImGui::SameLine();
    LineBuffercheck = ImGui::Button("Clear", { 50,20 });
    ImGui::Text("Algorithm : ");
    ImGui::SameLine();
    if(ImGui::Button("Point Clipping", { 140,20 })) option = true;
    ImGui::SameLine();
    if(ImGui::Button("Line Clipping", { 140,20 })) option = false;
    ImGui::Text("Current Algorithm = %s", (option ? s1 : s2).c_str());
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
    resetLineBuffer();
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

