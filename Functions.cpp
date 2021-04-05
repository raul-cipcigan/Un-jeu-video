#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void prepGrid(std::vector<unsigned int>& vbo, std::vector<unsigned int>& ebo, std::vector<unsigned int>& vao) {

	int columns = 1;

	float vertices[] = {
		-0.9f, -0.9f, 0.0f,   0.0f, 0.0f, 0.0f,
		-0.7f, -0.9f, 0.0f,   0.0f, 0.0f, 0.0f,
		-0.7f, -0.7f, 0.0f,   0.0f, 0.0f, 0.0f,
		-0.9f, -0.7f, 0.0f,   0.0f, 0.0f, 0.0f
	};

	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	for (int i = 0; i <= 80; i++) {
		glGenBuffers(1, &vbo[i]);
		glGenBuffers(1, &ebo[i]);
		glGenVertexArrays(1, &vao[i]);
		glBindVertexArray(vao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		int32_t bsize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);

		if (bsize == 0) {
			std::cout << "Le memoire tampon n'a pas fonctionne :(";
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (columns < 9) {
			columns++;
			vertices[0] += 0.2f;
			vertices[6] += 0.2f;
			vertices[12] += 0.2f;
			vertices[18] += 0.2f;
		}
		else {
			columns = 1;
			vertices[0] = -0.9f;
			vertices[6] = -0.7f;
			vertices[12] = -0.7f;
			vertices[18] = -0.9f;
			vertices[1] += 0.2f;
			vertices[7] += 0.2f;
			vertices[13] += 0.2f;
			vertices[19] += 0.2f;
		}

		if (!(i % 2)) {
			vertices[3] = 1.0f;
			vertices[4] = 1.0f;
			vertices[5] = 1.0f;
			vertices[9] = 1.0f;
			vertices[10] = 1.0f;
			vertices[11] = 1.0f;
			vertices[15] = 1.0f;
			vertices[16] = 1.0f;
			vertices[17] = 1.0f;
			vertices[21] = 1.0f;
			vertices[22] = 1.0f;
			vertices[23] = 1.0f;
		}
		else {
			vertices[3] = 0.0f;
			vertices[4] = 0.0f;
			vertices[5] = 0.0f;
			vertices[9] = 0.0f;
			vertices[10] = 0.0f;
			vertices[11] = 0.0f;
			vertices[15] = 0.0f;
			vertices[16] = 0.0f;
			vertices[17] = 0.0f;
			vertices[21] = 0.0f;
			vertices[22] = 0.0f;
			vertices[23] = 0.0f;
		}
	}

}

void spawnEnemy(float transX, float transY, std::vector<float>& x, std::vector<float>& y, int num, std::vector<bool>& life) {
	
	x[num] = rand() % 9;
	y[num] = rand() % 9;
	x[num] = x[num] / 5 - 0.8f;
	y[num] = y[num] / 5 - 0.8f;

	life[num] = true;

	int a[4];

	switch (num) {
	case 0:
		for (int i = 0; i <= 3; i++) {
			a[i] = i + 1;
		}
		break;
	case 1:
		a[0] = 0;
		for (int i = 1; i <= 3; i++) {
			a[i] = i + 1;
		}
		break;
	case 2:
		a[0] = 0;
		a[1] = 1;
		a[2] = 3;
		a[3] = 4;
		break;
	case 3:
		for (int i = 0; i <= 2; i++) {
			a[i] = i;
		}
		a[3] = 4;
		break;
	case 4:
		for (int i = 0; i <= 3; i++) {
			a[i] = i;
		}
		break;
	default:
		std::cout << "Le nombre d'ennemis est brise :(";
		break;
	}

	while ((x[num] < transX + 0.5 && x[num] > transX - 0.6) && (y[num] < transY + 0.5 && y[num] > transY - 0.6) || (x[num] == x[a[0]]) || (x[num] == x[a[1]]) || (x[num] == x[a[2]]) || (x[num] == x[a[3]]) || (y[num] == y[a[0]]) || (y[num] == y[a[1]]) || (y[num] == y[a[2]]) || (y[num] == y[a[3]])) {

		while (x[num] < transX + 0.5 && x[num] > transX - 0.6 && y[num] < transY + 0.5 && y[num] > transY - 0.6) {
			x[num] = rand() % 9;
			x[num] = x[num] / 5 - 0.8;
			y[num] = rand() % 9;
			y[num] = y[num] / 5 - 0.8;
		}

		while ((x[num] == x[a[0]]) || (x[num] == x[a[1]]) || (x[num] == x[a[2]]) || (x[num] == x[a[3]])) {
			x[num] = rand() % 9;
			x[num] = x[num] / 5 - 0.8;
		}

		while ((y[num] == y[a[0]]) || (y[num] == y[a[1]]) || (y[num] == y[a[2]]) || (y[num] == y[a[3]])) {
			y[num] = rand() % 9;
			y[num] = y[num] / 5 - 0.8;
		}
	}
}

void prepEnemy(std::vector<unsigned int>& vbo, std::vector<unsigned int>& ebo, std::vector<unsigned int>& vao) {

	float vertices[] = {
			-0.05f, -0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.05f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.05f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f
	};

	int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	for (int i = 0; i <= 4; i++) {

		glGenBuffers(1, &vbo[i]);
		glGenBuffers(1, &ebo[i]);
		glGenVertexArrays(1, &vao[i]);
		glBindVertexArray(vao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		int32_t bsize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bsize);

		if (bsize == 0) {
			std::cout << "Le memoire tampon n'a pas fonctionne :(";
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void moveEnemy(std::vector<float>& x, std::vector<float>& y, bool& life, float transX, float transY, int wait, std::vector<bool>& life1, std::vector<bool>& done, bool& recentKill, int enemynum) {
	if (wait == 7 && life == true) {
		for (int i = 0; i < enemynum; i++) {
			if (abs(x[i] - transX) < abs(y[i] - transY) && !(x[i] <= transX + 0.1 && x[i] >= transX - 0.1) && !(y[i] <= transY + 0.1 && y[i] >= transY - 0.1) && !done[i] && life1[i]) {
				if (x[i] > transX) {
					x[i] -= 0.2f;
					done[i] = true;
					std::cout << "Ennemi " << i << " s'est deplace vers la gauche\n";
				}
				else if (x[i] < transX) {
					x[i] += 0.2f;
					done[i] = true;
					std::cout << "Ennemi " << i << " s'est deplace vers la droite\n";
				}
			}
			else if (abs(y[i] - transY) < abs(x[i] - transX) && !(y[i] <= transY + 0.1 && y[i] >= transY - 0.1) && !(x[i] <= transX + 0.1 && x[i] >= transX - 0.1) && !done[i] && life1[i]) {
				if (y[i] > transY) {
					y[i] -= 0.2f;
					done[i] = true;
					std::cout << "Ennemi " << i << " s'est deplace vers le bas\n";
				}
				else if (y[i] < transY) {
					y[i] += 0.2f;
					done[i] = true;
					std::cout << "Ennemi " << i << " s'est deplace vers le haut\n";
				}
			}
			else if (((x[i] <= transX + 0.1 && x[i] >= transX - 0.1) || (y[i] <= transY + 0.1 && y[i] >= transY - 0.1)) && !done[i] && life1[i] && !recentKill) {
				life = false;
				for (int j = 0; j <= 4; j++) {
					done[j] = true;
				}
				std::cout << "Vaincu par ennemi " << i << "\n\n";
			}
			else if (life1[i] && recentKill) {
				std::cout << i << " a saute leur tour\n";
			}
		}
		std::cout << "\n";
	}
}

void checkValues(std::vector<bool>& done, std::vector<bool>& life, int wait) {
	if (wait == 7) {
		for (int i = 0; i <= 4; i++) {
			std::cout << done[i] << " ";
			std::cout << life[i] << "\n";
		}
	}
}