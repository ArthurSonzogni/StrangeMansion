#ifndef LEVELITEMSELECTOR_77O5KG2E
#define LEVELITEMSELECTOR_77O5KG2E

class LevelEditor;
class ShaderProgram;

class LevelItemSelector
{
    public:
        LevelItemSelector(LevelEditor& levelEditor);
        void draw();
    private:
        LevelEditor& levelEditor;
        ShaderProgram& shader;
        void build();
        GLuint vbo;
        GLuint vao;
};

#endif /* end of include guard: LEVELITEMSELECTOR_77O5KG2E */
