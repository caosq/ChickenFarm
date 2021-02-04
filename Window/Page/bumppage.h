#ifndef BUMPPAGE_H
#define BUMPPAGE_H

#include <QWidget>

namespace Ui {
class BumpPage;
}

class BumpPage : public QWidget
{
    Q_OBJECT

public:
    explicit BumpPage(QWidget *parent = nullptr);
    ~BumpPage();

private:
    Ui::BumpPage *ui;
};

#endif // BUMPPAGE_H
