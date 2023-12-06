#pragma once

#include <iostream>
#include <map>
#include <cstdlib>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <initializer_list>

//fontisms
#include "ft2build.h"
#include FT_FREETYPE_H

//glisms
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "gtc/matrix_transform.hpp"

const glm::vec4 white     = {255/ 255.f, 255/ 255.f, 255/ 255.f, 255/ 255.f};  //    white
const glm::vec4 grey      = {121/ 255.f, 124/ 255.f, 126/ 255.f, 255/ 255.f};  //    grey
const glm::vec4 yellow    = {200/ 255.f, 181/ 255.f, 87/ 255.f, 255/ 255.f};   //    yellow
const glm::vec4 green     = {106/ 255.f, 171/ 255.f, 100/ 255.f, 255/ 255.f};  //    green
const glm::vec4 red       = {175/ 255.f, 75/ 255.f, 75/ 255.f, 255/ 255.f};  //    red

enum class Align {
    Left = 0, 
    Center = 1, 
    Right = 2, 
    Bottom = Left, 
    Top = Right,
};

enum class ShapeForm {
    Rectangle = 0,
    Circle = 1,
    OutlinedCircle = 2,
    Glyph = 3
};

/* Map with char(glyphname) and a struct for conveniently storing glyph data */
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class Shader
{
public:
    // the program ID
    unsigned int ID;
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
};

class Shape {
public:
    ShapeForm form;
    glm::vec4 color;
    Shape(ShapeForm form, glm::vec4 color);
};

class Glyph : public Shape {
public:
    std::map<char, Character> fCharacters;
    Glyph(std::map<char, Character> fCharacters, glm::vec4 color);
};

void RenderShape(Shape shape,
                 Shader& s,
                 const std::array<Align, 2>& align,
                 float x, float y, float w, float h);

void RenderText(Glyph glyph,
                Shader& s,
                const std::string& text,
                const std::array<Align, 2>& align,
                float x, float y, float scale);

std::map<char, Character> fontLoad(const char* fontPath);

/* glyph shader paths */
inline const char* glyph_vertexShaderPath = "shaders/glyph.vs.glsl";
inline const char* glyph_fragmentShaderPath = "shaders/glyph.fs.glsl";

inline std::map<char, Character> Characters;
inline unsigned int VAO, VBO;
inline int fbw, fbh;