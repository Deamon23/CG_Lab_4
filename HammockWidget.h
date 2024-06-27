#ifndef HAMMOCKWIDGET_H
#define HAMMOCKWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QVector>
#include <QMatrix4x4>

class HammockWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit HammockWidget(QWidget *parent = nullptr);
    ~HammockWidget();

    void setRotation(float angle);
    void setStep(float step);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private slots:
    void updateView();

private:
    void generateSurface();

    float rotationAngle; // Угол вращения
    float step; // Шаг сетки
    QTimer timer; // Таймер для обновления вида
    QVector<float> vertices; // Вершины поверхности
    QVector<unsigned int> indices; // Индексы для треугольников
};

#endif // HAMMOCKWIDGET_H
