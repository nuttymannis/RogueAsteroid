#include "config.h"
#include <unordered_map>

#include "ft2build.h"
#include FT_FREETYPE_H

struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class TextRenderer {
    public:
    TextRenderer(GLuint _shader);

    FT_Error init(){
        return FT_Init_FreeType(&ft);
    }

    FT_Error load_face(char* font_dir){
        return FT_New_Face(ft, font_dir, 0, &face);
    }

    FT_Error set_font_size(FT_UInt _w, FT_UInt _h){
        return FT_Set_Pixel_Sizes(face, 0, 48); 
    }

    void generateCharmap();

    void initVertexBuffer(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);   
    }

    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

    private:
    GLuint shader;
    std::unordered_map<char, Character> charMap;
    FT_Library ft;
    FT_Face face;
    unsigned int VAO, VBO;
};