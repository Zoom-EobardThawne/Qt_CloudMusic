#ifndef ABOUTINTERFACE_H
#define ABOUTINTERFACE_H

#include <QWidget>

namespace Ui {
class AboutInterface;
}

class AboutInterface : public QWidget
{
    Q_OBJECT

public:
    explicit AboutInterface(QWidget *parent = 0);
    ~AboutInterface();

private:
    Ui::AboutInterface *ui;
};

#endif // ABOUTINTERFACE_H
