#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QDebug>
#include <QMovie>


class AnimatedLabel : public QLabel
{
    Q_OBJECT
public:
    AnimatedLabel(QWidget *parent, QString path_to_gif, QString whats_this, int minw, int minh);
    QMovie *movie;

signals:
    clicked();

public slots:
    start_movie();
    stop_movie();



protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

};

class Filter : public QObject
{
    Q_OBJECT
public:
    Filter( QObject* parent = 0 ) : QObject(parent) {}
    bool eventFilter( QObject* obj, QEvent* e );
signals:
    start_movie_signal();
    stop_movie_signal();

};

#endif // ANIMATEDLABEL_H
