#include "config.h"
#include "triangle_mesh.h"
#include "game.h"
#include <filesystem>

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);

const int WIN_X = 800, 
          WIN_Y = 600;

int main(int argc, char* argv[]){

    // GLFW owns the operating-system window and the OpenGL context. The
    // context is the stateful environment in which all later OpenGL calls run.
    GLFWwindow* window;

    if(!glfwInit()){
        std::cout << "GLFW couldn't start." << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, 0);
    glfwWindowHint(GLFW_FOCUSED, 1);

    // Creating the window also creates the OpenGL context that will be made
    // current immediately afterward. OpenGL calls require a current context.
    window = glfwCreateWindow(WIN_X, WIN_Y, "Rogue Asteroid", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    // OpenGL functions are obtained from the graphics driver at runtime.
    // GLAD stores those function addresses behind the normal gl* function names.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    // Set the color used whenever the color buffer is cleared at the start of
    // a frame. The four values are red, green, blue, and alpha.
    glClearColor(0.00f, 0.00f, 0.00f, 0.0f);
    int w,h;
    glfwGetFramebufferSize(window, &w, &h);

    // Tell OpenGL which rectangle of the framebuffer receives clip-space
    // output. The viewport converts shader coordinates from -1..1 into pixels.
    glViewport(0,0,w,h);
    
    const std::filesystem::path executablePath =
        std::filesystem::absolute(argv[0]).parent_path();
    const std::filesystem::path shaderDirectory = executablePath / "shaders";
    unsigned int shader = make_shader(
        (shaderDirectory / "vertex.txt").string(),
        (shaderDirectory / "fragment.txt").string()
    );

    
    InputBuffer* inputBuffer = new InputBuffer();
    Game* game = new Game(window, shader, inputBuffer);

    inputBuffer->setGame(game);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        /*
        double _x,_y;
        glfwGetCursorPos(window, &_x, &_y);
        triangle->setPosition((2 *(float)_x/WIN_X) - 1, (2 * -(float)_y/WIN_Y) + 1.0f);
        */

        // Set the frame counter and delta time
        game->calculateFrames();

        // Clear the previous frame's color pixels before drawing the new frame.
        glClear(GL_COLOR_BUFFER_BIT);

        // Select the linked shader program for all subsequent draw operations.
        glUseProgram(shader);
        
        game->logic(); 
        game->draw();
        

        // Present the completed back buffer and make a fresh back buffer
        // available for the next frame. This prevents partially drawn frames
        // from being shown to the usaer.

        inputBuffer->actionBuffer();

        glfwSwapBuffers(window);
    }

    std::cout << glGetString(GL_VERSION) << '\n';

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}

unsigned int make_module(const std::string& filepath, unsigned int module_type){

    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    // Read shader files and store each line into buffer
    file.open(filepath);
    while(std::getline(file, line)){
        bufferedLines << line << std::endl;
    }

    // Compile all code from buffer into one string and close the buffer/filestream
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str(); // C Compatible
    bufferedLines.str("");
    file.close();

    // Create a GPU shader object, provide its source text, and compile it.
    // A vertex shader transforms vertices; a fragment shader produces pixels.
    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    // Ask the driver whether compilation succeeded and print its diagnostic log
    // if the GLSL source was invalid for the active OpenGL implementation.
    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if(!success){
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
    }

    return shaderModule;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath){

    // Compile the two stages separately. They are linked together below into
    // one executable GPU program that can be selected with glUseProgram().
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    // Attach both stages to the program and link them. Linking checks that the
    // outputs of one stage match the inputs expected by the next stage.
    unsigned int shader = glCreateProgram();
    for(unsigned int shaderModule : modules){
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    // Query the link result and print the driver's explanation if linking fails.
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if(!success){
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "Shader Linking error:\n" << errorLog << std::endl;
    }

    // Once linking succeeds, the standalone shader objects are no longer
    // needed. The linked program keeps the executable stages internally.
    for(unsigned int shaderModule : modules){
        glDeleteShader(shaderModule);
    }

    return shader;
}

