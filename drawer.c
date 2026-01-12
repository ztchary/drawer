#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define PI 3.14159
#define MAXLEN 100000

int width, height;
int pressing = 0;
unsigned int pen_color = 0xffffffff;
int pen_radius = 10;
float mousex, mousey;

struct {
	int len;
	int cap;
	struct {
		float x;
		float y;
		unsigned int color;
		int radius;
	} *data;
} points;

struct {
	int len;
	int cap;
	struct {
		int i;
		float r;
		float g;
		float b;
		float a;
	} *data;
} changes;

void draw_circle(float cx, float cy, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); 

    for (float i = 0; i <= 2.0; i += 0.125) {
        float theta = M_PI * i;
        float x = radius / width * cosf(theta);
        float y = radius / height * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) pen_color = 0xffffffff;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) pen_color = 0x000000ff;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) pen_color = 0xff0000ff;
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) pen_color = 0x00ff00ff;
	if (key == GLFW_KEY_5 && action == GLFW_PRESS) pen_color = 0x0000ffff;
	if (key == GLFW_KEY_6 && action == GLFW_PRESS) pen_color = 0xffff00ff;
	if (key == GLFW_KEY_7 && action == GLFW_PRESS) pen_color = 0x00ffffff;
	if (key == GLFW_KEY_8 && action == GLFW_PRESS) pen_color = 0xff00ffff;
	if (key == GLFW_KEY_9 && action == GLFW_PRESS) pen_color = 0xff8000ff;
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) pen_color = 0x00000000;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) points.len = 0;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void scroll_callback(GLFWwindow *window, double xoff, double yoff) {
	pen_radius *= yoff > 0 ? 1.1 : 0.9;
	if (pen_radius <= 10) pen_radius = 10;
}

void add_point() {
	points.data[points.len].x = mousex;
	points.data[points.len].y = mousey;
	points.data[points.len].color = pen_color;
	points.data[points.len].radius = pen_radius;
	points.len++;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	mousex = xpos / (float)width * 2.0 - 1.0;
	mousey = (ypos / (float)height * 2.0 - 1.0) * -1;
	if (!pressing) return;
	if (points.len >= points.cap) return;
	add_point();
}

void click_callback(GLFWwindow *window, int button, int action, int mods) {
	if (button != GLFW_MOUSE_BUTTON_LEFT) return;
	pressing = action == GLFW_PRESS;
	add_point();
}

int main() {
	points.len = 0;
	points.cap = MAXLEN;
	points.data = malloc(MAXLEN * sizeof(*points.data));
	changes.len = 0;
	changes.cap = MAXLEN;
	changes.data = malloc(MAXLEN * sizeof(*changes.data));

	GLFWwindow* window;

	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwWindowHintString(GLFW_WAYLAND_APP_ID, "drawer");

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	
	window = glfwCreateWindow(mode->width, mode->height, "drawer", monitor, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, click_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	while (!glfwWindowShouldClose(window)) {
		glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < points.len; i++) {
			glColor4f(
				(float)(points.data[i].color >> 24 & 0xff) / 255.0,
				(float)(points.data[i].color >> 16 & 0xff) / 255.0,
				(float)(points.data[i].color >>  8 & 0xff) / 255.0,
				(float)(points.data[i].color >>  0 & 0xff) / 255.0
			);				
			draw_circle(points.data[i].x, points.data[i].y, points.data[i].radius);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

