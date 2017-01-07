/*
Programmers: Thuy Nguyen, Stephen Reese, Andrew Tsai, Lacey Taylor, and Tambra Smith
Last updated: 11-7-16
Description: Holds the main image displayed on the canvas. Enables different clicks and mouse movements.
*/

#ifndef SLIDEVIEW_H
#define SLIDEVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QPixmap>
#include <iostream>
#include <stack>
#include <string>

class SlideView : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY (QColor previewColor READ previewColor NOTIFY updatePalettePreviewSignal)
public:
    explicit SlideView(QGraphicsView *parent, int size = 10);
    QImage getImage();
    void setTool(std::string tool);
    void setFill(bool fill);
    void setShapeWidth(int w);
    void setPaintWidth(int w);
    void updateScene();
    void setImage(QImage image);
    void updatePalettePreview(QColor previewColor);

    QColor previewColor() const
    {
        return m_previewColor;
    }

signals:
    void updatePreview();
    void updatePalettePreviewSignal(QColor previewColor);

private:
    QImage theImage;
    QRgb color;

    QPoint startPos;
    QPoint lastPoint;

    QGraphicsScene *theScene;
    QGraphicsPixmapItem *pixMap;
    QPixmap pixImage;
    QPixmap pixImageZoomed;
    QPointF origPoint;
    QGraphicsLineItem* itemToDraw;
    QGraphicsEllipseItem* circleToDraw;
    QGraphicsRectItem* SquareToDraw;

    int shapeWidth;
    int paintWidth;
    int pixelHeight;
    int pixelWidth;
    int drawingY;
    int drawingX;

    double scaledPixelWidth;

    bool fillShape;
    bool drawing;
    bool drawingLine = false;
    bool drawingCircle = false;
    bool drawingRect = false;


    //the types of tool we use to edit pixels
    enum tools {test, pen, paintBrush, erase, eyedropper, shapeLine, shapeCircle, shapeSquare, paintBucket};
    tools theTool;

    //Undo-redo fields
    std::stack<QImage> undoStack;
    std::stack<QImage> redoStack;

    QPixmap getPixMap();

    //Paint bucket fields
    bool hasPaintBucket = false;
    void fillInArea(int x, int y);
    bool isValidPoint(int x, int y);
    bool hasAreaColor(QColor areaColor, QColor neighborColor);
    bool isFillableNeighbor(int x, int y, QColor areaColor);

    int NUMBER_OF_PIXEL; // an matrix of pixels should be a square matrix.

    void drawLine(int x1, int y1, int x2, int y2);
    void drawCirle(int x1, int y1, int w, int h);
    void drawSquare(int x1, int y1, int w, int h);
    void brush(int x, int y);

    void drawCheckerBoard();

    QColor m_previewColor;

protected:
    virtual void mouseMoveEvent( QMouseEvent* event);
    virtual void mousePressEvent( QMouseEvent* event);
    virtual void mouseReleaseEvent( QMouseEvent* event);

    //virtual void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void undoSlot();
    void redoSlot();
    void rotateLeftSlot();
    void rotateRightSlot();
    void flipHorizontalSlot();
    void flipVerticalSlot();
    void colorPickerSlot(QColor _color);

};

#endif // SLIDEVIEW_H
