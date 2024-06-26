#ifndef HAMMOCKWIDGET_H
#define HAMMOCKWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QVector>

class HammockWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit HammockWidget(QWidget *parent = nullptr);
    ~HammockWidget();

    void setSlices(int slices); // Установить количество срезов
    void setRotation(float angle); // Установить угол вращения
    void setStep(float step); // Установить шаг сетки

protected:
    void initializeGL() override; // Инициализация OpenGL
    void resizeGL(int w, int h) override; // Обработка изменения размера окна
    void paintGL() override; // Отрисовка сцены

private slots:
    void updateView(); // Обновление вида

private:
    QTimer timer; // Таймер для обновления вида
    int slices; // Количество срезов
    float rotationAngle; // Угол вращения
    float step; // Шаг сетки
    QVector<float> vertices; // Вершины поверхности
    QVector<unsigned int> indices; // Индексы для отрисовки
    void generateSurface(); // Генерация поверхности
};

#endif // HAMMOCKWIDGET_H
