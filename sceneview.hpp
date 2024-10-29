#ifndef SCENEVIEW_HPP
#define SCENEVIEW_HPP

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>

class SceneView : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    SceneView(QWidget *parent=nullptr);
    ~SceneView();
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void teardownGL();
private:
    void printContextInformation();
    unsigned int shaderProgram{0};
    std::vector<float> generateCircleVertices(float centerX, float centerY, float radius, int numberOfSegments);
    unsigned int VBO{0};
    unsigned int VAO{0};
};


#endif // SCENEVIEW_HPP
