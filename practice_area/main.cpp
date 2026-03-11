#include <QApplication>
#include <QPushButton>
#include <QString>
#include <vector>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Qt GUI part
    QPushButton button("Click Me!");
    button.resize(300, 100); // Set button size
    button.show();

    // STL part
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int n : numbers) sum += n;
    std::cout << "Sum of numbers: " << sum << std::endl;

    return app.exec();
}