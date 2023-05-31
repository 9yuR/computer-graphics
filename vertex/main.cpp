#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "toys.h"
#include <vector>
#include <cmath> //M_PI를 사용하기 위한 cmath 헤더

void render( GLFWwindow* window );
void init();
GLfloat TriangleNum = 0; // 삼각형 개수 정보
GLfloat r = 0.8; // 반지름 정보

Program program;
GLuint vertBuf=0;
GLuint vertArray=0;
GLuint triBuf=0;

int main(void){
    if ( !glfwInit() ) exit(EXIT_FAILURE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow( 640, 640, "Red Circle", NULL, NULL );
    glfwMakeContextCurrent( window );
//    glfwSwapInterval(1);
    init();
    while ( !glfwWindowShouldClose( window ) ) {
        render( window );
        glfwPollEvents();
    }
    glfwDestroyWindow( window );
    glfwTerminate();
}

void init() {
    program.loadShaders("/Users/kamgyul/Desktop/xcode/HelloOpenGL/HelloOpenGL/shader.vert", "/Users/kamgyul/Desktop/xcode/HelloOpenGL/HelloOpenGL/shader.frag");
    std::vector<glm::vec3> vert; // vertex 좌표를 저장하는 벡터
    vert.push_back( glm::vec3(0, 0, 0));
    for(GLfloat theta = 0; theta < 2 * (GLfloat)M_PI;){
        vert.push_back( glm::vec3(r*cos(theta), r*sin(theta), 0));
        theta += (GLfloat)0.001 * 2 * (GLfloat)M_PI;
    }
    
    std::vector<glm::u32vec3> triangles; // 삼각형 정보
    for(int i = 1; i < vert.size() - 1; i++)
        triangles.push_back( glm::u32vec3(0, i, i + 1));
    triangles.push_back(glm::u32vec3(0, 1, vert.size()-1));
    TriangleNum = vert.size();
    
    glGenBuffers( 1, &vertBuf );
    glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
    glBufferData(GL_ARRAY_BUFFER, vert.size()*sizeof(glm::vec3), vert.data(), GL_STATIC_DRAW);
    
    glGenVertexArrays( 1, &vertArray );
    glBindVertexArray( vertArray );
    glEnableVertexAttribArray( 0 );
//    glGenBuffers( 1, &vertBuf ); // 기존의 만든 buffer의 인덱스를 덮어 씌우므로 적절하지 않음
    glVertexAttribPointer( 0, 3, GL_FLOAT, 0, 0, 0 );
    
    glGenBuffers( 1, &triBuf );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size()*sizeof(glm::u32vec3), triangles.data(), GL_STATIC_DRAW);
    
}
void render( GLFWwindow* window ) {
    int width, height;
    glfwGetFramebufferSize( window, &width, &height );
    glViewport( 0, 0, width, height );
    glClearColor( 0, 0, 0.2, 0 );
    glClear( GL_COLOR_BUFFER_BIT );
    glUseProgram( program.programID );
    glBindVertexArray(vertArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triBuf);
    glDrawElements(GL_TRIANGLES, TriangleNum * 3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers( window );
}

