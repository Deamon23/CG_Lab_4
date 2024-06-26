#include "hammockwidget.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>

HammockWidget::HammockWidget(QWidget *parent)
    : QOpenGLWidget(parent), slices(10), rotationAngle(0.0f), step(0.5f)
{
    connect(&timer, &QTimer::timeout, this, &HammockWidget::updateView); // Подключение таймера к слоту обновления вида
    timer.start(1000 / 60); // Запуск таймера с частотой 60 FPS
}

HammockWidget::~HammockWidget()
{
}

void HammockWidget::setSlices(int slices)
{
    this->slices = slices; // Установка количества срезов
    generateSurface(); // Генерация новой поверхности
    update(); // Обновление вида
}

void HammockWidget::setRotation(float angle)
{
    rotationAngle = angle; // Установка угла вращения
    update(); // Обновление вида
}

void HammockWidget::setStep(float step)
{
    this->step = step; // Установка шага сетки
    generateSurface(); // Генерация новой поверхности
    update(); // Обновление вида
}

void HammockWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Инициализация функций OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Установка цвета очистки экрана
    glEnable(GL_DEPTH_TEST); // Включение теста глубины
    generateSurface(); // Генерация поверхности
}

void HammockWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // Установка области просмотра
}

void HammockWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экрана и буфера глубины

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f); // Установка перспективной проекции
    projection.translate(0, 0, -15); // Перемещение камеры назад
    projection.rotate(rotationAngle, 0, 1, 0); // Вращение только по оси Y

    QOpenGLShaderProgram program;
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        attribute vec3 vertexPosition;
        uniform mat4 mvpMatrix;
        void main() {
            gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
        }
    )"); // Вершинный шейдер
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, R"(
        void main() {
            gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    )"); // Фрагментный шейдер

    program.link(); // Линковка шейдерной программы
    program.bind(); // Привязка шейдерной программы

    program.setUniformValue("mvpMatrix", projection); // Установка матрицы проекции

    program.enableAttributeArray("vertexPosition"); // Включение атрибута вершин
    program.setAttributeArray("vertexPosition", vertices.constData(), 3); // Установка данных вершин

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, indices.constData()); // Отрисовка линий

    program.disableAttributeArray("vertexPosition"); // Отключение атрибута вершин
    program.release(); // Освобождение шейдерной программы
}

void HammockWidget::updateView()
{
    rotationAngle += 1.0f; // Увеличение угла вращения
    update(); // Обновление вида
}

void HammockWidget::generateSurface()
{
    vertices.clear(); // Очистка вершин
    indices.clear(); // Очистка индексов

    for (float x = -2.0f; x <= 2.0f; x += step) {
        for (float z = -2.0f; z <= 2.0f; z += step) {
            float y = x * x + z * z; // Вычисление координаты y
            vertices << x << y << z; // Добавление вершины
        }
    }

    int numVerticesPerRow = (4.0f / step) + 1; // Количество вершин в строке

    for (int i = 0; i < numVerticesPerRow - 1; ++i) {
        for (int j = 0; j < numVerticesPerRow - 1; ++j) {
            int start = i * numVerticesPerRow + j;
            indices << start << start + 1; // Индексы для линий по оси X
            indices << start << start + numVerticesPerRow; // Индексы для линий по оси Z
        }
    }
}
