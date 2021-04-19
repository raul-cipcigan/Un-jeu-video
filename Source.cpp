#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Header.h"
#include "shader.h"

int main() {
	
	std::vector<unsigned int> gvbo(81);
	std::vector<unsigned int> gebo(81);
	std::vector<unsigned int> gvao(81);
	std::vector<unsigned int>& rgvbo = gvbo;
	std::vector<unsigned int>& rgebo = gebo;
	std::vector<unsigned int>& rgvao = gvao;

	std::vector<unsigned int> evbo(5);
	std::vector<unsigned int> eebo(5);
	std::vector<unsigned int> evao(5);
	std::vector<unsigned int>& revbo = evbo;
	std::vector<unsigned int>& reebo = eebo;
	std::vector<unsigned int>& revao = evao;

	std::vector<float> enemyX(5);
	std::vector<float> enemyY(5);
	std::vector<float>& ex = enemyX;
	std::vector<float>& ey = enemyY;

	std::vector<bool> alreadyMoved(5);
	std::vector<bool>& doneMoving = alreadyMoved;

	std::vector<int> respawnCooldown(5);

	glm::mat4 transE[5];

	int seed = time(NULL);
	std::vector<bool> enemyIsAlive = { false, false, false, false, false };
	std::vector<bool>& life = enemyIsAlive;

	float transX = 0.0f;
	float transY = 0.0f;
	bool playerIsAlive = true;
	bool& pLife = playerIsAlive;
	int wait = 0;
	bool recentKill = false;
	bool& RrecentKill = recentKill;

	unsigned int pvbo, pebo, pvao;

	int difficulty;

	std::cout << "Choisissez une difficulte entre 1 et 3: ";
	std::cin >> difficulty;
	while (difficulty > 3 || difficulty < 1) {
		std::cout << "Le nombre est trop petit ou trop grand.\n\nChoissisez un autre: ";
		std::cin >> difficulty;
	}

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Un jeu video", NULL, NULL);
	if (window == NULL) {
		std::cout << "GLFW ne s'est pas intialise :(\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD ne s'est pas initialise :(\n";
		return -1;
	}

	srand(seed);

	shader boardShader;
	boardShader.startShader("vertexShader.glsl", "fragmentShader.glsl");
	shader playerShader;
	playerShader.startShader("movingVertexShader.glsl", "fragmentShader.glsl");
	shader enemy1;
	enemy1.startShader("movingVertexShader.glsl", "fragmentShader.glsl");
	shader enemy2;
	enemy2.startShader("movingVertexShader.glsl", "fragmentShader.glsl");
	shader enemy3;
	enemy3.startShader("movingVertexShader.glsl", "fragmentShader.glsl");
	shader enemy4;
	enemy4.startShader("movingVertexShader.glsl", "fragmentShader.glsl");
	shader enemy5;
	enemy5.startShader("movingVertexShader.glsl", "fragmentShader.glsl");

	unsigned int playerTransformLoc = glGetUniformLocation(playerShader.ID, "transform");
	unsigned int enemyLoc[5];
	enemyLoc[0] = glGetUniformLocation(enemy1.ID, "transform");
	enemyLoc[1] = glGetUniformLocation(enemy2.ID, "transform");
	enemyLoc[2] = glGetUniformLocation(enemy3.ID, "transform");
	enemyLoc[3] = glGetUniformLocation(enemy4.ID, "transform");
	enemyLoc[4] = glGetUniformLocation(enemy5.ID, "transform");

	prepGrid(rgvbo, rgebo, rgvao);
	prepEnemy(revbo, reebo, revao);

	float playerVertices[] = {
		-0.05f, -0.05f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.05f, -0.05f, 0.0f,   0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f, 0.0f,   0.0f, 1.0f, 0.0f,
		-0.05f,  0.05f, 0.0f,   0.0f, 1.0f, 0.0f
	};

	int playerIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenBuffers(1, &pvbo);
	glGenBuffers(1, &pebo);
	glGenVertexArrays(1, &pvao);
	glBindVertexArray(pvao);

	glBindBuffer(GL_ARRAY_BUFFER, pvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndices), playerIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		boardShader.use();
		for (int i = 0; i <= 80; i++) {
			glBindVertexArray(gvao[i]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		for (int i = 0; i <= difficulty + 1; i++) {
			if (!enemyIsAlive[i] && respawnCooldown[i] == 0) {
				spawnEnemy(transX, transY, ex, ey, i, life);
			}
			else if (!enemyIsAlive[i] && respawnCooldown[i] != 0 && wait == 7) {
				respawnCooldown[i]--;
			}
		}

		for (int i = 0; i <= difficulty + 1; i++) {
			transE[i] = glm::mat4(1.0f);
			transE[i] = glm::translate(transE[i], glm::vec3(enemyX[i], enemyY[i], 0.0f));
		}

		if (enemyIsAlive[0]) {
			enemy1.use();
			glUniformMatrix4fv(enemyLoc[0], 1, GL_FALSE, glm::value_ptr(transE[0]));
			glBindVertexArray(evao[0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (enemyIsAlive[1]) {
			enemy2.use();
			glUniformMatrix4fv(enemyLoc[1], 1, GL_FALSE, glm::value_ptr(transE[1]));
			glBindVertexArray(evao[1]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (enemyIsAlive[2]) {
			enemy3.use();
			glUniformMatrix4fv(enemyLoc[2], 1, GL_FALSE, glm::value_ptr(transE[2]));
			glBindVertexArray(evao[2]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (enemyIsAlive[3]) {
			enemy4.use();
			glUniformMatrix4fv(enemyLoc[3], 1, GL_FALSE, glm::value_ptr(transE[3]));
			glBindVertexArray(evao[3]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (enemyIsAlive[4]) {
			enemy5.use();
			glUniformMatrix4fv(enemyLoc[4], 1, GL_FALSE, glm::value_ptr(transE[4]));
			glBindVertexArray(evao[4]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		if (playerIsAlive) {
			playerShader.use();
			if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS && transY < 0.8f && wait == 0) {
				transY += 0.2f;
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS && transY > -0.8f && wait == 0) {
				transY -= 0.2f;
					wait = 15;
					for (int i = 0; i <= difficulty + 1; i++) {
						alreadyMoved[i] = false;
							std::cout << "Reinitialise ennemi " << i << "\n";
					}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS && transX > -0.8f && wait == 0) {
				transX -= 0.2f;
					wait = 15;
					for (int i = 0; i <= difficulty + 1; i++) {
						alreadyMoved[i] = false;
						std::cout << "Reinitialise ennemi " << i << "\n";
					}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS && transX < 0.8f && wait == 0) {
				transX += 0.2f;
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && wait == 0) {
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS && transX < 0.8f && transY < 0.8f && wait == 0) {
				transX += 0.2f;
				transY += 0.2f;
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS && transX > -0.8f && transY < 0.8f && wait == 0) {
				transX -= 0.2f;
				transY += 0.2f;
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS && transX < 0.8f && transY > -0.8f && wait == 0) {
				transX += 0.2f;
				transY -= 0.2f;
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS && transX > -0.8f && transY > -0.8f && wait == 0) {
				transX -= 0.2f;
				transY -= 0.2f;
				wait = 15;
				for (int i = 0; i <= difficulty + 1; i++) {
					alreadyMoved[i] = false;
					std::cout << "Reinitialise ennemi " << i << "\n";
				}
			}
			else if (wait > 0) {
				wait--;
			}

			if (transX > 0.8f) {
				transX = 0.8f;
			}
			else if (transX < -0.8) {
				transX = -0.8f;
			}
			else if (transY > 0.8f) {
				transY = 0.8f;
			}
			else if (transY < -0.8f) {
				transY = -0.8f;
			}

			if (wait == 1 && recentKill == true) {
				recentKill = false;
			}

			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(transX, transY, 0.0f));
			glUniformMatrix4fv(playerTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glBindVertexArray(pvao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			wait = 0;
			transX = 0.0f;
			transY = 0.0f;
			for (int i = 0; i <= difficulty + 1; i++) {
				enemyIsAlive[i] = false;
			}
			playerIsAlive = true;
			for (int i = 0; i <= difficulty + 1; i++) {
				alreadyMoved[i] = false;
			}
		}

		for (int i = 0; i <= difficulty + 1; i++) {
			if ((enemyX[i] <= transX + 0.1 && enemyX[i] >= transX - 0.1) && (enemyY[i] <= transY + 0.1 && enemyY[i] >= transY - 0.1) && enemyIsAlive[i]) {
				respawnCooldown[i] = 10;
				enemyIsAlive[i] = false;
				recentKill = true;
				std::cout << "Vaincu ennemi " << i << "!\n";
			}
		}

		moveEnemy(ex, ey, pLife, transX, transY, wait, life, doneMoving, RrecentKill, difficulty + 2);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	for (int i = 0; i <= 80; i++) {
		glDeleteVertexArrays(1, &gvao[i]);
		glDeleteBuffers(1, &gvbo[i]);
		glDeleteBuffers(1, &gebo[i]);
	}

	glfwTerminate();
	return 0;

}