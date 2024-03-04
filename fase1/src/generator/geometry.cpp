#include "geometry.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <filesystem>
#include <cmath>
#include <corecrt_math_defines.h>

#include "../utils/ponto.hpp"
#include <fstream>

namespace fs = std::filesystem;

void generateSphere(float radius, int slices, int stacks, const std::string &filename)
{
    fs::path outputPath = fs::current_path().parent_path() / "output";
    if (!fs::exists(outputPath))
    {
        fs::create_directories(outputPath);
    }

    fs::path filePath = outputPath / filename;
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Não foi possível abrir o arquivo para escrita: " << filePath << std::endl;
        return;
    }

    int totalVertices = 2 * 3 * slices * stacks; // Cada quadrado na esfera é dividido em 2 triângulos, 3 vértices por triângulo
    file << totalVertices << std::endl;

    float deltaPhi = M_PI / stacks;       // Intervalo de variação para phi (de 0 a PI)
    float deltaTheta = 2 * M_PI / slices; // Intervalo de variação para theta (de 0 a 2*PI)

    for (int i = 0; i < stacks; ++i)
    {
        float phi = i * deltaPhi;
        float nextPhi = (i + 1) * deltaPhi;

        for (int j = 0; j < slices; ++j)
        {
            float theta = j * deltaTheta;
            float nextTheta = (j + 1) * deltaTheta;

            // Coordenadas dos vértices do quadrado atual
            float x1 = radius * sinf(phi) * cosf(theta);
            float y1 = radius * cosf(phi);
            float z1 = radius * sinf(phi) * sinf(theta);

            float x2 = radius * sinf(nextPhi) * cosf(theta);
            float y2 = radius * cosf(nextPhi);
            float z2 = radius * sinf(nextPhi) * sinf(theta);

            float x3 = radius * sinf(phi) * cosf(nextTheta);
            float y3 = radius * cosf(phi);
            float z3 = radius * sinf(phi) * sinf(nextTheta);

            float x4 = radius * sinf(nextPhi) * cosf(nextTheta);
            float y4 = radius * cosf(nextPhi);
            float z4 = radius * sinf(nextPhi) * sinf(nextTheta);

            // Primeiro triângulo
            file << x1 << "," << y1 << "," << z1 << "\n";
            file << x2 << "," << y2 << "," << z2 << "\n";
            file << x4 << "," << y4 << "," << z4 << "\n";

            // Segundo triângulo
            file << x4 << "," << y4 << "," << z4 << "\n";
            file << x3 << "," << y3 << "," << z3 << "\n";
            file << x1 << "," << y1 << "," << z1 << "\n";
        }
    }

    file.close();
    std::cout << "Arquivo '" << filePath << "' criado com sucesso." << std::endl;
}

void generateBox(float size, int divisions, const std::string &filename)
{
    fs::path outputPath = fs::current_path().parent_path() / "output";
    if (!fs::exists(outputPath))
    {
        fs::create_directories(outputPath);
    }

    fs::path filePath = outputPath / filename;
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Não foi possível abrir o arquivo para escrita." << std::endl;
        return;
    }

    float step = size / divisions;
    float half = size / 2;
    int totalVertices = 6 * 2 * divisions * divisions * 3;
    file << totalVertices << std::endl;

    // Função de vértices (para ser mais legivel)
    auto writeVertex = [&](float x, float y, float z)
    {
        file << x << "," << y << "," << z << std::endl;
    };

    // Faces do cubo
    for (int i = 0; i < divisions; ++i)
    {
        float y = -half + i * step;
        float yNext = y + step;
        for (int j = 0; j < divisions; ++j)
        {
            float x = -half + j * step;
            float xNext = x + step;

            // Topo (y = half)
            writeVertex(x, half, y);
            writeVertex(x, half, yNext);
            writeVertex(xNext, half, y);
            writeVertex(xNext, half, y);
            writeVertex(x, half, yNext);
            writeVertex(xNext, half, yNext);

            // Base (y = -half)
            writeVertex(x, -half, y);
            writeVertex(xNext, -half, y);
            writeVertex(x, -half, yNext);
            writeVertex(xNext, -half, y);
            writeVertex(xNext, -half, yNext);
            writeVertex(x, -half, yNext);

            // Frente (z = half)
            writeVertex(x, y, half);
            writeVertex(xNext, y, half);
            writeVertex(x, yNext, half);
            writeVertex(xNext, y, half);
            writeVertex(xNext, yNext, half);
            writeVertex(x, yNext, half);

            // Trás (z = -half)
            writeVertex(x, y, -half);
            writeVertex(x, yNext, -half);
            writeVertex(xNext, y, -half);
            writeVertex(xNext, y, -half);
            writeVertex(x, yNext, -half);
            writeVertex(xNext, yNext, -half);

            // Direita (x = half)
            writeVertex(half, y, x);
            writeVertex(half, yNext, x);
            writeVertex(half, y, xNext);
            writeVertex(half, y, xNext);
            writeVertex(half, yNext, x);
            writeVertex(half, yNext, xNext);

            // Esquerda (x = -half)
            writeVertex(-half, y, x);
            writeVertex(-half, y, xNext);
            writeVertex(-half, yNext, x);
            writeVertex(-half, y, xNext);
            writeVertex(-half, yNext, xNext);
            writeVertex(-half, yNext, x);
        }
    }
    file.close();
    std::cout << "Arquivo '" << filename << "' criado com sucesso." << std::endl;
}

void generatePlane(float size, int divisions, const std::string &filename)
{
    fs::path outputPath = fs::current_path().parent_path() / "output";
    if (!fs::exists(outputPath))
    {
        fs::create_directories(outputPath);
    }

    fs::path filePath = outputPath / filename;

    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Não foi possível abrir o arquivo para escrita." << std::endl;
        return;
    }

    float step = size / divisions;
    float half = size / 2.0f;
    int totalVertices = divisions * divisions * 6; // Cada quadrado é dividido em 2 triângulos, 3 vértices por triângulo

    // Escrevendo o número total de vértices no arquivo
    file << totalVertices << std::endl;

    // Geração de vértices para cada divisão do plano e escrita direta no arquivo
    for (int i = 0; i < divisions; ++i)
    {
        for (int j = 0; j < divisions; ++j)
        {
            float x = (i * step) - half;
            float z = (j * step) - half;

            // Ajustando a ordem dos vértices para que as normais apontem para cima
            // Primeiro triângulo do quadrado
            file << x << ", 0, " << z + step << std::endl; // Superior esquerdo
            file << x + step << ", 0, " << z << std::endl; // Inferior direito
            file << x << ", 0, " << z << std::endl;        // Inferior esquerdo

            // Segundo triângulo do quadrado
            file << x << ", 0, " << z + step << std::endl;        // Superior esquerdo
            file << x + step << ", 0, " << z + step << std::endl; // Superior direito
            file << x + step << ", 0, " << z << std::endl;        // Inferior direito
        }
    }

    file.close();
    std::cout << "Arquivo '" << filename << "' criado com sucesso." << std::endl;
}

void generateCone(float radius, float height, int slices, int stacks, const std::string &filename)
{
    fs::path outputPath = fs::current_path().parent_path() / "output";
    if (!fs::exists(outputPath))
    {
        fs::create_directories(outputPath);
    }

    fs::path filePath = outputPath / filename;

    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Não foi possível abrir o arquivo para escrita." << std::endl;
        return;
    }

    // Calculando o número total de vértices
    // Cada slice da base gera 3 vértices e cada stack de cada slice gera 6 vértices
    int totalVertices = slices * 3 + slices * stacks * 6;
    file << totalVertices << std::endl;

    // Ângulo entre cada slice
    float deltaAngle = 2 * M_PI / slices;
    // Diferença de altura entre cada stack
    float deltaHeight = height / stacks;

    // Gerar a base do cone
    for (int i = 0; i < slices; ++i)
    {
        float angle = i * deltaAngle;
        float nextAngle = (i + 1) * deltaAngle;

        // Vértices da base (0, 0, 0) e dois pontos na borda
        file << "0, 0, 0\n";                                                           // Centro da base
        file << radius * cos(nextAngle) << ", 0, " << radius * sin(nextAngle) << "\n"; // Próximo ponto na borda
        file << radius * cos(angle) << ", 0, " << radius * sin(angle) << "\n";         // Ponto atual na borda
    }

    // Gerar os lados do cone
    for (int i = 0; i < slices; ++i)
    {
        float angle = i * deltaAngle;
        float nextAngle = (i + 1) * deltaAngle;

        // Topo do cone
        float tipX = 0, tipY = height, tipZ = 0; // Vértice do topo do cone

        // Base do cone
        float baseX1 = radius * cos(angle), baseZ1 = radius * sin(angle);         // Ponto atual na borda da base
        float baseX2 = radius * cos(nextAngle), baseZ2 = radius * sin(nextAngle); // Próximo ponto na borda da base

        // Triângulo que liga a base ao topo
        file << tipX << ", " << tipY << ", " << tipZ << "\n"; // Topo
        file << baseX2 << ", 0, " << baseZ2 << "\n";          // Próximo ponto na borda da base
        file << baseX1 << ", 0, " << baseZ1 << "\n";          // Ponto atual na borda da base
    }

    file.close();
    std::cout << "Arquivo '" << filename << "' criado com sucesso." << std::endl;
}