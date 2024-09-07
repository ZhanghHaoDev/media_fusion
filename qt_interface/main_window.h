#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMenuBar>
#include <QTreeView>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>

class main_window : public QMainWindow
{
    Q_OBJECT
public:
    main_window(); 
    ~main_window();

    void init_menu();

private:
    QMenuBar *menuBar;       // 菜单栏
    QTreeView *fileTree;     // 左侧文件树
    QTextEdit *fileContent;  // 中间文件内容
    QTextEdit *fileInfo;     // 右侧文件信息
    QVBoxLayout *mainLayout; // 主窗口布局器
    QHBoxLayout *centralLayout; // 中央布局
    QWidget *centralWidget;  // 中央窗口部件
    QSplitter *splitter;     // 分隔器
};

#endif // MAIN_WINDOW_H