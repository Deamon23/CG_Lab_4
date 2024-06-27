#include "hammockwidget.h"
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QDebug>
// Конструктор класса HammockWidget
HammockWidget::HammockWidget(QWidget *parent)
    : QOpenGLWidget(parent), rotationAngle(0.0f), step(0.5f)
{
    // Подключение таймера к слоту обновления вида
    connect(&timer, &QTimer::timeout, this, &HammockWidget::updateView);
    // Запуск таймера с частотой 60 FPS
    timer.start(1000 / 60);
}

// Деструктор класса HammockWidget
HammockWidget::~HammockWidget()
{
}

// Инициализация OpenGL
void HammockWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Инициализация функций OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Установка цвета очистки экрана
    glEnable(GL_DEPTH_TEST); // Включение теста глубины
    generateSurface(); // Генерация поверхности
}

// Обработка изменения размера окна
void HammockWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h); // Установка области просмотра
}

// Отрисовка сцены
void HammockWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экрана и буфера глубины

    // Создание матрицы проекции
    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / height(), 0.1f, 100.0f); // Установка перспективной проекции
    projection.translate(0, 0, -20); // Перемещение камеры назад
    projection.rotate(rotationAngle, 0, 1, 0); // Вращение только по оси Y

    // Создание и компиляция шейдерной программы
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

    // Используем режим отрисовки линий
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.constData()); // Отрисовка треугольников
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Возвращение режима заполнения

    program.disableAttributeArray("vertexPosition"); // Отключение атрибута вершин
    program.release(); // Освобождение шейдерной программы
}

// Обновление вида
void HammockWidget::updateView()
{
    rotationAngle += 1.0f; // Увеличение угла вращения
    update(); // Обновление вида
}

// Генерация поверхности
void HammockWidget::generateSurface()
{
    vertices.clear(); // Очистка вершин
    indices.clear(); // Очистка индексов

    // Диапазон от -2 до 2
    float range = 4.0f;

    // Генерация вершин поверхности
    for (float x = -2.0f; x <= 2.0f; x += step) {
        for (float z = -2.0f; z <= 2.0f; z += step) {
            float y = x * x + z * z;
            vertices << x << y << z; // Добавление вершин
        }
    }

    // Количество вершин в строке
    int numVerticesPerRow = (range / step) + 1;
    //  qDebug() << numVerticesPerRow;

    // Генерация индексов для треугольников
    for (int i = 0; i < numVerticesPerRow - 1; ++i) {
        for (int j = 0; j < numVerticesPerRow - 1; ++j) {
            int start = i * numVerticesPerRow + j;
            indices << start << start + 1 << start + numVerticesPerRow;
            indices << start + 1 << start + numVerticesPerRow << start + numVerticesPerRow + 1;
        }
    }
}
