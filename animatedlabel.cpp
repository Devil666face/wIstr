#include "animatedlabel.h"

AnimatedLabel::AnimatedLabel(QWidget *parent, QString path_to_gif, QString whats_this, int minw, int minh) : QLabel(parent)
{
    this->setMinimumSize(minw,minh);
    this->setMaximumSize(minw,minh);
    movie = new QMovie(path_to_gif);
    this->setMovie(movie);
    //this->setStyleSheet("background-color: qlineargradient(spread:repeat, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(74, 74, 74, 255),stop:1 rgba(49, 49, 49, 255));"
    //                    "border: 0px solid #fff;");
    this->setStyleSheet("background-color: qlineargradient(spread:repeat, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(184, 184, 184, 255),stop:1 rgba(159, 159, 159, 255));"
                        "border-style: solid;"
                        "border-width: 1px;"
                        "border-color: #051a39;"
                        "padding: 5px;");
    this->setScaledContents(true);
    this->setCursor(Qt::PointingHandCursor);
    this->setMouseTracking(true);
    Filter* filter_start_stop = new Filter(this);
    this->installEventFilter(filter_start_stop);
    this->setStatusTip(whats_this);
    //this->setStatusTip("Привет");
    connect(filter_start_stop,SIGNAL(start_movie_signal()),this,SLOT(start_movie()));
    connect(filter_start_stop,SIGNAL(stop_movie_signal()),this,SLOT(stop_movie()));
    movie->start();
    movie->stop();
}

AnimatedLabel::start_movie()
{
    movie->start();
    this->setStyleSheet("background-color: rgba(70,162,218,50%);"
                        "border: 1px solid #46a2da;");
}

AnimatedLabel::stop_movie()
{
    movie->stop();
    movie->start();
    movie->stop();
    this->setStyleSheet("background-color: qlineargradient(spread:repeat, x1:1, y1:0, x2:1, y2:1, stop:0 rgba(184, 184, 184, 255),stop:1 rgba(159, 159, 159, 255));"
                        "border-style: solid;"
                        "border-width: 1px;"
                        "border-color: #051a39;"
                        "padding: 5px;");
    this->setFrameShape(WinPanel);
}

void AnimatedLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}

void AnimatedLabel::mouseMoveEvent(QMouseEvent *event)
{

}

bool Filter::eventFilter(QObject *obj, QEvent *e)
{
      if (!obj->isWidgetType()) return true;
      QWidget* w = (QWidget*) obj;
      if (e->type() == QEvent::Enter) {
            emit start_movie_signal();
      }
      else if (e->type() == QEvent::Leave) {
            emit stop_movie_signal();
      }
      return false;
}
