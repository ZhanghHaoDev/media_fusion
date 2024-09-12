#include "main_window.h"

#include <QFileDialog>
#include <QApplication>
#include <QStandardItemModel>

#include "glog/logging.h"

#include "file_management.h"

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

    fileModel = new QStandardItemModel(this);
    fileModel->setHorizontalHeaderLabels(QStringList() << "文件名");
    fileTree->setModel(fileModel);

    connect(fileTree, &QTreeView::customContextMenuRequested, this, &main_window::file_tree_menu);
    fileTree->setContextMenuPolicy(Qt::CustomContextMenu);

    this->init_menu();
    this->init_left_tree("");
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

void main_window::init_menu() {
    this->file_menu();
    this->about_menu();
}

void main_window::file_menu() {
    QMenu *fileMenu = menuBar->addMenu("文件");

    QAction *openAction = new QAction("打开文件", this);
    QAction *exitAction = new QAction("退出", this);

    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);

    connect(openAction, &QAction::triggered, this, &main_window::open_file);
    connect(exitAction, &QAction::triggered, this, &main_window::exit_app);
}

void main_window::exit_app() {
    QApplication::quit();
}

void main_window::about_menu() {
    QMenu *aboutMenu = menuBar->addMenu("关于");
    QAction *aboutAction = new QAction("软件信息", this);
    aboutMenu->addAction(aboutAction);

    connect(aboutAction, &QAction::triggered, this, []() {
        QMessageBox::information(nullptr, "软件信息", "音视频分析处理工具\n版本 1.0");
    });
}

void main_window::open_file() {
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件 (*.*)");
    if (!fileName.isEmpty()) {
        file_management file;
        int fileType = file.file_type(fileName.toStdString());
        if (fileType == FILE_TYPE_UNKNOWN) {
            QMessageBox::information(this, "文件类型", "选择的文件类型未知");
        } else if(fileType == FILE_TYPE_AAC) {
            aac.input_aac_file(fileName.toStdString());
            if (!aac.is_aac_file()) {
                QMessageBox::information(this, "文件类型", "选择的文件不是AAC文件");
                return;
            }
            aac_file::aac_file_info aac_info = aac.get_aac_file_info();
        }
        init_left_tree(fileName);
    }
}

void main_window::init_left_tree(const QString &fileName) {
    if (fileName.isEmpty()) {
        LOG(ERROR) << "文件名为空" << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        return;
    }
    QStandardItem *item = new QStandardItem(fileName);
    item->setData(fileName, Qt::UserRole); 
    fileModel->appendRow(item);
    this->all_file_names.append(fileName);
}

void main_window::file_tree_menu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QModelIndex currentIndex = fileTree->indexAt(pos);
    if (!currentIndex.isValid()) {
        return;
    }
    QString selectedFilePath = fileModel->itemFromIndex(currentIndex)->data(Qt::UserRole).toString();

    QAction action0("属性", this);
    connect(&action0, &QAction::triggered, [this, selectedFilePath]() { this->attribute_file(selectedFilePath); });
    contextMenu.addAction(&action0);

    QAction action1("刷新", this);
    connect(&action1, &QAction::triggered, this, &main_window::refresh_file_tree);
    contextMenu.addAction(&action1);

    QAction action2("关闭", this);
    connect(&action2, &QAction::triggered, this, &main_window::close_file_tree);
    contextMenu.addAction(&action2);

    QAction action3("重命名", this);
    connect(&action3, &QAction::triggered, this, &main_window::close_file_tree);
    contextMenu.addAction(&action3);

    contextMenu.exec(fileTree->mapToGlobal(pos));
}

void main_window::refresh_file_tree() {
    fileModel->clear();
    fileModel->setHorizontalHeaderLabels(QStringList() << "文件名");
    for (const QString &fileName : all_file_names) {
        QStandardItem *item = new QStandardItem(fileName);
        fileModel->appendRow(item);
    }
}

void main_window::close_file_tree() {
    QModelIndex currentIndex = fileTree->currentIndex();
    if (!currentIndex.isValid()) {
        return; 
    }
    fileModel->removeRow(currentIndex.row(), currentIndex.parent());
}

void main_window::init_right_info(const QString &fileName){

}

void main_window::attribute_file(const QString &fileName) {
    fileInfo->clear();
    fileInfo->append(QString("文件名: %1").arg(fileName));

    // 获取 AAC 文件信息
    aac_file aac(fileName.toStdString());
    aac_file::aac_file_info info = aac.get_aac_file_info();

    // 显示文件信息
    fileInfo->append(QString("文件路径: %1").arg(QString::fromStdString(info.file_path)));
    fileInfo->append(QString("是否是AAC文件: %1").arg(info.is_aac ? "是" : "否"));
    fileInfo->append(QString("采样率: %1").arg(info.sample_rate));
    fileInfo->append(QString("声道数: %1").arg(info.channels));
    fileInfo->append(QString("比特率: %1").arg(info.bit_rate));
    fileInfo->append(QString("封装格式: %1").arg(info.format == 0 ? "ADTS" : "ADIF"));
    fileInfo->append(QString("帧长度: %1").arg(info.frame_length));
    fileInfo->append(QString("音频时长: %1秒").arg(info.duration));
}