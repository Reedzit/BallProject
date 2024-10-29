#include "sceneview.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QFile>
#include <QDebug>
#include <math.h>

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";

SceneView::SceneView(QWidget *parent): QOpenGLWidget(parent)
{
    setMinimumSize(600,400);
}

SceneView::~SceneView()
{
    teardownGL();
}

void SceneView::initializeGL()
{
    initializeOpenGLFunctions();
    printContextInformation();
    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }

    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }

    //link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //setup vertex data and buffers and configurre vertex attributes

    std::vector<float> vertices = generateCircleVertices(0.0f,1.0f,0.5f, 100);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the vertex array object first, then bind and set vertex buffers, and then configure vertex attributes
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex bufferso afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //you can unbind the VAO afterwards so other VAO calls wont accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs nor VBOs when it is not directly necessary.
    glBindVertexArray(0);
}
}

void SceneView::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //draw the triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there is no need to bind it every time but we will do so to keep things organized.
    glDrawArrays(GL_TRIANGLE_FAN, 0, 102);
}
void SceneView::resizeGL(int width, int height)
{

}
void SceneView::teardownGL()
{
    makeCurrent();
}
void SceneView::printContextInformation()
{
    QString glType{(context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL"};
    QString glVersion{reinterpret_cast<const char*>(glGetString(GL_VERSION))};
    QString glProfile;

    //get profile info
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break

    switch (format().profile())
    {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }

#undef CASE
    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
std::vector<float> SceneView::generateCircleVertices(float centerX, float centerY, float radius, int numberOfSegments)
{
    std::vector<float> vertices;
    vertices.push_back(centerX);
    vertices.push_back(centerY);
    vertices.push_back(0.0f);

    for (int i=0; i <= numberOfSegments; i++)
    {
        float theta{2.0f * Pi * float(i) / float(numberOfSegments)};
        float x{radius * cos(theta)};
        float y{radius * sin(theta)};
        vertices.push_back(x + centerX);
        vertices.push_back(y + centerY);
        vertices.push_back(0.0f);
    }
    return vertices;
}
