#ifndef LASERAPPLICATION_H
#define LASERAPPLICATION_H

#include <QApplication>

class LaserApplication Q_DECL_FINAL : public QApplication {
Q_OBJECT
public:
    LaserApplication(int& argc, char** argv);
    bool notify(QObject* receiver, QEvent* event);

private:
    int* m_buffer;
};

#endif // LASERAPPLICATION_H
