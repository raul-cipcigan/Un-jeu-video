#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void prepGrid(std::vector<unsigned int>& vbo, std::vector<unsigned int>& ebo, std::vector<unsigned int>& vao);
void spawnEnemy(float transX, float transY, std::vector<float>& x, std::vector<float>& y, int num, std::vector<bool>& life);
void prepEnemy(std::vector<unsigned int>& vbo, std::vector<unsigned int>& ebo, std::vector<unsigned int>& vao);
void moveEnemy(std::vector<float>& x, std::vector<float>& y, bool& life, float transX, float transY, int wait, std::vector<bool>& life1, std::vector<bool>& done, bool& recentKill, int enemynum);
void checkValues(std::vector<bool>& done, std::vector<bool>& life, int wait);