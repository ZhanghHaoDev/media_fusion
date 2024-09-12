#include <QApplication>
#include "main_window.h"

#include "glog/logging.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]); 
    google::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = true; 

    QApplication app(argc, argv);

    main_window mainWindow;
    mainWindow.show();

    return app.exec();
}