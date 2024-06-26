#include "hammockwidget.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>

HammockWidget::HammockWidget(QWidget *parent)
    : QOpenGLWidget(parent), slices(10), rotationAngle(0.0f), step(0.5f)
{
    connect(&timer, &QTimer::timeout, this, &HammockWidget::updateView);
    timer.start(1000 / 60); // 60 FPS
}

HammockWidget::~HammockWidget()
{
}

void HammockWidget::setSlices(int slices)
{
    this->slices = slices;
    generateSurface();
    update();
}

void HammockWidget::setRotation(float angle)
{
    rotationAngle = angle;
    update();
}

void HammockWidget::setStep(float step)
{
    this->step = step;
    generateSurface();
    update();
}

void HammockWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    generateSurface();
}

void HammockWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HammockWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f);
    projection.translate(0, 0, -15);
    projection.rotate(rotationAngle, 1, 1, 0); // Поворот вокруг осей x и y

    QOpenGLShaderProgram program;
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        attribute vec3 vertexPosition;
        uniform mat4 mvpMatrix;
        void main() {
            gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
        }
    )");
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        void main() {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )");
    program.link();
    program.bind();

    program.setUniformValue("mvpMatrix", projection);

    program.enableAttributeArray("vertexPosition");
    program.setAttributeArray("vertexPosition", vertices.constData(), 3);

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.constData());

    program.disableAttributeArray("vertexPosition");
    program.release();
}

void HammockWidget::updateView()
{
    rotationAngle += 1.0f;
    update();
}

void HammockWidget::generateSurface()
{
    vertices.clear();
    indices.clear();

    for (float x = -2.0f; x <= 2.0f; x += step) {
        for (float z = -2.0f; z <= 2.0f; z += step) {
            float y = x * x + z * z;
            vertices << x << y << z;
        }
    }

    int numVerticesPerRow = (4.0f / step) + 1;

    for (int i = 0; i < numVerticesPerRow - 1; ++i) {
        for (int j = 0; j < numVerticesPerRow - 1; ++j) {
            int start = i * numVerticesPerRow + j;
            indices << start << start + 1;
            indices << start << start + numVerticesPerRow;
        }
    }
}
