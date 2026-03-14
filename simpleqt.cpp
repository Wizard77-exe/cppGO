#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Welcome App");

    // Widgets
    QLineEdit *nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter your name");

    QPushButton *button = new QPushButton("Welcome");

    QLabel *label = new QLabel("Your welcome message will appear here");

    // Layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(nameInput);
    layout->addWidget(button);
    layout->addWidget(label);

    window.setLayout(layout);

    // Signal + Slot using lambda
    QObject::connect(button, &QPushButton::clicked, [=]() {

        QString name = nameInput->text();
        label->setText("Welcome " + name);

    });

    window.resize(300,150);
    window.show();

    return app.exec();
}