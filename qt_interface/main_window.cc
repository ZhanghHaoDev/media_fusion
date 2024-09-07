#include "main_window.h"

main_window::main_window() {
    setWindowTitle("音视频分析处理工具");
    resize(800, 600);
    
    menuBar = new QMenuBar(this);   // 菜单栏
    fileTree = new QTreeView(this); // 左侧文件树
    fileContent = new QTextEdit(this);  // 中间文件内容
    fileInfo = new QTextEdit(this);    // 右侧文件信息
    centralWidget = new QWidget(this);  // 中央窗口部件
    mainLayout = new QVBoxLayout(centralWidget);    // 主窗口布局器
    splitter = new QSplitter(Qt::Horizontal, centralWidget);    // 分隔器

    splitter->addWidget(fileTree);
    splitter->addWidget(fileContent);
    splitter->addWidget(fileInfo);
    mainLayout->addWidget(splitter);

    setMenuBar(menuBar);
    setCentralWidget(centralWidget);

    this->init_menu();

}

main_window::~main_window() {
    delete menuBar;
    delete fileTree;
    delete fileContent;
    delete mainLayout;
    delete centralLayout;
    delete centralWidget;
    delete splitter;
}

void main_window::init_menu(){
    QMenu *fileMenu = menuBar->addMenu("文件");
    QAction *importAction = new QAction("导入文件", this);
    QAction *exitAction = new QAction("退出", this);
    fileMenu->addAction(importAction);
    fileMenu->addAction(exitAction);

    // 创建关于菜单
    QMenu *aboutMenu = menuBar->addMenu("关于");
    QAction *aboutAction = new QAction("软件信息", this);
    aboutMenu->addAction(aboutAction);

    // 连接动作信号到槽函数
    connect(importAction, &QAction::triggered, this, []() {
        // 处理导入文件逻辑
        QMessageBox::information(nullptr, "导入文件", "导入文件功能尚未实现");
    });

    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    connect(aboutAction, &QAction::triggered, this, []() {
        QMessageBox::information(nullptr, "软件信息", "音视频分析处理工具\n版本 1.0");
    });
}