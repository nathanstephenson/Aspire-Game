#include <GL/glew.h>//needs to be included before other openGL stuff
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "EventHandler.h"
#include "Application.h"

//Tests
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h""
#include "tests/TestBatchingColor.h""
#include "tests/TestBatchingTexture.h""
#include "tests/TestDynamicGeometry.h""


EventHandler e;//manages user inputs
static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {//doesn't wait for polling
    e.KeyCallback(window, key, scancode, action, mods);
}
static void CursorCallback(GLFWwindow* window, double xpos, double ypos) {
    e.CursorCallback(window, xpos, ypos);
}
static void MouseCallback(GLFWwindow* window, int button, int action, int mods) {
    e.MouseCallback(window, button, action, mods);
}
static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    e.WindowSizeCallback(window, width, height);
}

int main(void){//using default types so that it is nicer to deal with non-opengl apis

    int wWidth = 960;
    int wHeight = 540;
    unsigned int swapInterval = 1;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()){
        return -1;
    }
    
    //setting opengl versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(wWidth, wHeight, "Aspire", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(swapInterval);//value of 1 binds it to the monitor's refresh rate, so VSync

    if (glewInit() != GLEW_OK) {//this needs to come after setting the window as openGL context
        std::cout << "Error with GLEW" << std::endl;
    } else {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    }

    /* Init ImGUI */
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    //Tell OpenGL how to handle inputs
    glfwSetKeyCallback(glfwGetCurrentContext(), KeyCallback);
    glfwSetCursorPosCallback(glfwGetCurrentContext(), CursorCallback);
    glfwSetMouseButtonCallback(glfwGetCurrentContext(), MouseCallback);
    glfwSetWindowSizeCallback(glfwGetCurrentContext(), WindowSizeCallback);

    {//provides a scope so that the application terminates correctly
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//tells OpenGL how to blend alpha pixels (which also allows transparency)
    
    //Create renderer & application layer and attach it to event handler
    Renderer renderer;
    Application app(wWidth, wHeight);
    e.SetApplication(&app);

    //Testing interface
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");//test for changing background colour
    testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
    testMenu->RegisterTest<test::TestBatchingColor>("RGBA Batching");
    testMenu->RegisterTest<test::TestBatchingTexture>("Textured Batching");
    testMenu->RegisterTest<test::TestDynamicGeometry>("Dynamic Geometry");
    
    while (!glfwWindowShouldClose(window)){//Loop until the user closes the window
        GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
        renderer.Clear();//clear frame to background colour


        ImGui_ImplGlfwGL3_NewFrame();//new imgui frame
        //if (currentTest) {//enables tests registered above
        //    currentTest->OnUpdate(0.0f);
        //    currentTest->OnRender();
        //    ImGui::Begin("Test");
        //    if (currentTest != testMenu && ImGui::Button("<-")) {//if there is a current test, gives the option to go back
        //        delete currentTest;
        //        currentTest = testMenu;
        //    } 
        //    currentTest->OnImGuiRender();
        //    ImGui::End();
        //}
        app.OnUpdate();//takes delta time
        app.OnRender();
        app.OnImGuiRender();
        
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        e.Poll();
        glfwPollEvents();
    }//Window Closed
    
    //Delete testing environment
    if (currentTest != testMenu) {
        delete testMenu;
    }
    delete currentTest;
    //buffers and shader deleted by destructor as the scope ends
    }//end application scope before killing glfw & imgui. event handler is killed when the application is fully closed (after "return 0" below).
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}


//Inspired by The Cherno's OpenGL tutorial:
//https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2