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
#include <QFileSystemModel>
#include <QStandardItemModel>

#include "aac_file.h"

class main_window : public QMainWindow
{
    Q_OBJECT
public:
    main_window(); 
    ~main_window();

    // 初始化菜单
    void init_menu();

    // 文件菜单
    void file_menu();
    
    // 关于菜单
    void about_menu();

    // 打开文件
    void open_file();

    // 退出程序
    void exit_app();

    // 左侧文件树
    void init_left_tree(const QString &fileName);

    // 右侧文件信息
    void init_right_info(const QString &fileName);

private:
    QMenuBar *menuBar;       // 菜单栏
    QTreeView *fileTree;     // 左侧文件树
    QTextEdit *fileContent;  // 中间文件内容
    QTextEdit *fileInfo;     // 右侧文件信息
    QVBoxLayout *mainLayout; // 主窗口布局器
    QHBoxLayout *centralLayout; // 中央布局
    QWidget *centralWidget;  // 中央窗口部件
    QSplitter *splitter;     // 分隔器
    QStandardItemModel *fileModel; // 文件系统模型
    QStringList all_file_names;   // 所有文件名

    aac_file aac;

private slots:
    void file_tree_menu(const QPoint &pos);
    void refresh_file_tree();
    void close_file_tree();
    void attribute_file(const QString &fileName);
};

#endif // MAIN_WINDOW_H