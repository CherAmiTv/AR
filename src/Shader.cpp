//
// Created by julien on 14/09/17.
//

#include "Shader.h"
#include <fstream>
#include <sstream>
#include <glcore.h>
#include <program.h>
#include <uniforms.h>
#include <window.h>

std::string Shader::read(const char *filename ){
    std::stringbuf source;
    std::ifstream in(filename);
    // verifie que le fichier existe
    if(in.good() == false)
        // affiche une erreur, si le fichier n'existe pas ou n'est pas accessible
        printf("[error] loading program '%s'...\n", filename);
    else
        printf("loading program '%s'...\n", filename);
    // lire le fichier, jusqu'au premier separateur,
    // le caractere '\0' ne peut pas se trouver dans un fichier texte, donc lit tout le fichier d'un seul coup
    in.get(source, 0);
    // renvoyer la chaine de caracteres
    return source.str();
}

Shader::Shader(char *filename, int nbv) {
    program = read_program(filename);
    program_print_errors(program);
    //prepass = read_program("src/Shaders/prePass.glsl");
    glGenVertexArrays(1, &vertexArray);

    glUseProgram(0);
    glBindVertexArray(0);
    nbVertex = nbv;
}

void Shader::setVertexArray(GLuint _vao) {
    glDeleteVertexArrays(1, &vertexArray);
    vertexArray = _vao;
}

void Shader::setVertexArray(const std::vector<Vector>& vec) {

    GLuint size;
    size = vec.size() * sizeof(Vector);
    glDeleteVertexArrays(1, &vertexArray);
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

    size_t offset= 0;

    glBufferSubData(GL_ARRAY_BUFFER, offset, size, &vec[0].x);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shader::setVertexArray(const float *vec, int nb) {
    GLuint size;
    size = nb * sizeof(float);
    glDeleteVertexArrays(1, &vertexArray);
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

    size_t offset= 0;

    glBufferSubData(GL_ARRAY_BUFFER, offset, size, vec);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shader::draw(const Transform& view, const Transform& proj, GLuint texture) {
    glBindVertexArray(vertexArray);
    glUseProgram(program);

    Transform MVP = proj*view*Identity();

    program_uniform(program, "mvpMatrix", MVP);
    program_use_texture(program, "diffuse_color", 0, texture);
    glDrawArrays(GL_TRIANGLES, 0, nbVertex);
}
