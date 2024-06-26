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

    void setSlices(int slices);
    void setRotation(float angle);
    void setStep(float step);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private slots:
    void updateView();

private:
    QTimer timer;
    int slices;
    float rotationAngle;
    float step;
    QVector<float> vertices;
    QVector<unsigned int> indices;
    void generateSurface();
};

#endif // HAMMOCKWIDGET_H
