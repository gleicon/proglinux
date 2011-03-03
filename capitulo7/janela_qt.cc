#include <qapplication.h>
#include <qlabel.h>

int main(int argc, char **argv) {
 QApplication app(argc, argv);
 QLabel etiqueta("Etiqueta", 0);
 etiqueta.resize(100, 100);
 app.setMainWidget(&etiqueta);
 etiqueta.show();
 return app.exec();
}

