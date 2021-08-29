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
int WIDTH = 700;
int HEIGHT = 700;
int size = (WIDTH * HEIGHT * 3);
unsigned char buffer[700 * 700 * 3 * 2];
ImVec4 clear_color = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
ImVec4 fill_color = ImVec4(1.00f, 1.00f, 0.00f, 1.00f);

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


CircleDrawing circle;
vector<CircleDrawing> circle_buffer;

int pattern = 15;
double a, b;
vector<ImVec4> myColor(4);
void boundaryFill(double, double, ImVec4&);
bool isFill = false;

void new_circle() {
    CircleDrawing c;
    circle.gap = gap;
    circle.pattern = pattern;
    circle_buffer.push_back(circle);
    circle = c;
}

struct pixel {
    double x, y;
    double angle;
    ImVec4 color;
    struct pixel()
    {
        x = 0;
        y = 0;
        angle = 0;
    }
};

void draw_axis() {
    glColor4f(1,1,1,1);
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
        status = true;
        glfwGetCursorPos(window, &x, &y);
        y = -y;
        x -= WIDTH / 2;
        y += HEIGHT / 2;
        if (y < HEIGHT/2 - 100) {
            circle.createCircle(x, y);
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        status = true;
        glfwGetCursorPos(window, &a, &b);
        b = -b;
        a -= WIDTH / 2;
        b += HEIGHT / 2;
        if(y < HEIGHT / 2 - 100)
            circle.translate(a, b);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwGetCursorPos(window, &x, &y);
        y = -y;
        x -= WIDTH / 2;
        y += HEIGHT / 2;
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            cout << x << " " << y << endl;
            circle.seedX = x;
            circle.seedY = y;
            isFill = true;
            pattern = 15;
        }
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


void boundaryFillRec(double X, double Y, ImVec4& color, ImVec4& my_color) {
    int i = ((X + WIDTH / 2) + WIDTH * (Y + HEIGHT / 2)) * 3;
    if (check(i, color) && check(i, my_color)) {
        buffer[i] = 255 * my_color.x;
        buffer[i + 1] = 255 * my_color.y;
        buffer[i + 2] = 255 * my_color.z;
        circle.fillColor.push_back({ a, b });
        glColor4f(my_color.x, my_color.y, my_color.z, my_color.w);
        glBegin(GL_POINTS);
        glVertex2f(2.0 * X / WIDTH, 2.0 * Y / HEIGHT);
        glEnd();
        boundaryFillRec(X + 1, Y    , color, my_color);
        boundaryFillRec(X - 1, Y    , color, my_color);
        boundaryFillRec(X    , Y + 1, color, my_color);
        boundaryFillRec(X    , Y - 1, color, my_color);
    }
}

void boundaryFill(double X, double Y, ImVec4& color, ImVec4& my_color) {
    queue<vector<double> > Q;
    Q.push({ X, Y });
    while (!Q.empty()) {
        int size = Q.size();
        //if (Q.size() > 100 * 5) break;
        for (int j = 0; j < size; j++) {
            double a = Q.front()[0], b = Q.front()[1];
            int i = ((a + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3;
            Q.pop();
            if (a<-WIDTH / 2 || a>WIDTH / 2) continue;
            if (b<-HEIGHT / 2 || b>HEIGHT / 2) continue;
            if (check(i, color) && check(i, my_color)) {
                if (check(((a + 1 + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3, color))
                    Q.push({ a + 1, b });
                if (check(((a - 1 + WIDTH / 2) + WIDTH * (b + HEIGHT / 2)) * 3, color))
                    Q.push({ a - 1, b });
                if (check(((a + WIDTH / 2) + WIDTH * (b + 1 + HEIGHT / 2)) * 3, color))
                    Q.push({ a, b + 1 });
                if (check(((a + WIDTH / 2) + WIDTH * (b - 1 + HEIGHT / 2)) * 3, color))
                    Q.push({ a, b - 1 });
            }
            else continue;
            buffer[i] = 255 * my_color.x;
            buffer[i + 1] = 255 * my_color.y;
            buffer[i + 2] = 255 * my_color.z;
            circle.fillColor.push_back({ a, b });
            glColor4f(my_color.x, my_color.y, my_color.z, my_color.w);
            glBegin(GL_POINTS);
            glVertex2f(2.0 * a / WIDTH, 2.0 * b / HEIGHT);
            glEnd();
        }
    }
}


void draw_circle(CircleDrawing circle, bool last = false) {
    if (last) {
        circle.gap = gap;
        circle.pattern = pattern;
    }
    if (circle.circle.size() >= 2) {
        glColor4f(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glBegin(GL_POINTS);
        for (int i = 0; i < circle.circle.size(); i++) {
            int check = (circle.pattern >> ((i / circle.gap) % 4)) & 1;
            if (check) glVertex2f(2.0 * (circle.circle[i][0]) / WIDTH, 2.0 * (circle.circle[i][1]) / HEIGHT);
        }
        glEnd();
    }
}

void draw() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_axis();
    for (auto c : circle_buffer) {
        draw_circle(c);
    }
    draw_circle(circle, true);
}

void imgui_menu() {
    ImGui::Begin("Attribute Menu");
    ImGui::SetWindowPos(
        ImVec2(0, 0),
        true);
    ImGui::SetWindowSize(ImVec2(500, 120), 0);
    if (ImGui::Button("New Circle")) {
        new_circle();
    }
    const char* Patterns[] = { "No Fill","1","2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
    ImGui::Combo("Pattern", &pattern, Patterns, IM_ARRAYSIZE(Patterns));
    if (isFill) pattern = 15;
    const char* GAP[] = { "","1","2", "3", "4", "5"};
    ImGui::Combo("GAP", &gap, GAP, IM_ARRAYSIZE(GAP));
    ImGui::ColorEdit3("Choose Boundary Color", (float*)&clear_color);
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

