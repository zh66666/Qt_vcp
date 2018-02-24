/**
 ** -------------------------------
 ** Create By:   Ryan LEUNG
 ** Date:        2009-09-23
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-11
 ** Content:     Add comments
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-13
 ** Content:     Add line number and syntax-highlighting
 ** -------------------------------
 ** -------------------------------
 ** Modified by: Ryan LEUNG
 ** Date:        2010-01-21
 ** Content:     Add config dialog
 ** -------------------------------
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtGui/QMainWindow>
#include <QDir>
#include "mdichild.h"
#include "ui_config.h"
#include "configdialog.h"

class MdiChild; // ������ mdichild.cpp��
QT_BEGIN_NAMESPACE
class QAction;
class QMdiArea;
class QListWidget;
class QMenu;
class QLineEdit;
class QLabel;
class QPushButton;
class QListWidgetItem;
class QSignalMapper;
class QMdiSubWindow;
class QDir;
class QUiLoader;
class QComboBox;
class QSpinBox;
class QDialogButtonBox;
QT_END_NAMESPACE

const int kTIME_OUT = 3000; // ״̬����Ϣ��ʾʱ��

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void newFile(); // �½��ļ�
    void openFile(); // ���ļ�
    void save(); // �����ļ�
    void saveAs(); // �ļ����Ϊ
    void saveAll(); // ���������ļ�
    void print(); // ��ӡ
    void about(); // ��ʾ�����ڡ���Ϣ
    void unDo(); // ����
    void reDo(); // ����
    void cut(); // ����
    void copy(); // ����
    void paste(); // ճ��
    void del(); // ɾ��ѡ������
    void selAll(); // ȫѡ
    void preProcess(); // ��Ԥ��������
    void vitalCodedPrcs(); // ����ȫ���롰����
    void showSetting(); // ��ʾ����
    void updateFileAndEditMenus(); // �������в˵�
    void updateWindowMenu(); // ���¡����ڡ��˵�
    void updateProjMenu(); // ���¡���Ŀ���˵�
    void updateDockBtns(); // ���¡�Ԥ����ǰ�ļ����е��ĸ���ť״̬
    void openItemOfLst(QListWidgetItem* item); // �������ļ��б��ĳһ��
    void setActiveSubWindow(QWidget* window); // ���ö��ļ��༭����������Ӵ���
    void addFls(); // ��Ԥ����ǰ�ļ����С�����ļ�����ť����
    void delFls(); // ��Ԥ����ǰ�ļ����С�ɾ���ļ�����ť����
    void moveUp(); // ��Ԥ����ǰ�ļ����С����ơ���ť����
    void moveDown(); // ��Ԥ����ǰ�ļ����С����ơ���ť����
    void refreshAfterPreList(); // ˢ�¡�Ԥ������ļ��б�
    void refreshFinalList();  // ˢ�¡���ȫ������ļ��б�
    void getChangedConfig(QString mode, QString afterPreDir,
                          QString afterVitalDir, QString entryFuncName,
                          QFont editorFont); // ����������Ϣ

private:
    void createActions(); // ����������ɲ˵��͹�������QAction����
    void createMenus(); // �����˵���
    void createToolBars(); // ����������
    void createStatusBar(); // ����״̬��
    void createMdiArea(); // �������ĵ��༭��
    void createDockWindows(); // ���������ļ��б�͡������Ϣ��ʾ���б�ĸ�����
    QPushButton* createButton(const QString& text, const char* member);//������ť
    MdiChild* createMdiChild(); // ����MidChild����ָ�룬һ��MidChild�����Ӧ�ڡ��ļ�
                                // �༭������һ���Ӵ���
    void loadFile(const QString& fileName); // �����ļ��������ļ�
    void showFiles(const QStringList& files,
                   QListWidget* listWidget); // ��ָ���б�����ʾ�ļ�files
    MdiChild* getActiveMdiChild(); // ��ȡ��ǰ���ļ��༭�����б�������Ӵ���
    QMdiSubWindow* findMdiChild(const QString& fileName); // ��ȡ����fileNameΪ
                                                          // �ļ������ļ����Ӵ���
    void setRsltLst(const QString& strItem); // ��ʾ��ϢstrItem
    void updateStatusBar(const QString& statusMsg); // ����״̬��
    bool createDir(const QString& dirStr); // ������dirStrΪ���·������Ŀ¼
    QString cmbneFlNms(
            const QListWidget* listWidget); // ��listWidget�е��ļ���
                                            // ƴ�ӵ�һ��QString�У��ÿո����
    void getFilesFromDir(QStringList& filesList,
                         const QString& dirName); // ��ָ����Ŀ¼��ȡ�������ļ�
    void getConfig();  // �������ļ��ж�ȡ������Ϣ
    bool isFileCreated(QString fileName);  // �ж��ļ�filaName��Ԥ�������Ƿ񴴽��ɹ�
    bool isFileExists(QString dir, QString fileName);  // �ж�dir�µ��ļ�fileName
                                                       // �Ƿ����
    void saveBeforeProcess(QString fileName); // �ж��ļ�fileName�Ƿ���Ҫ��Ԥ����
                                              // ��ȫ����ǰ���б���

    bool needPreCmpl; // �Ƿ���Ҫ����Ԥ��������ı�־
    QString runMode;  // ���õ�����ģʽ
    QString afterPreDir;  // ���õ�Ԥ�������ļ�Ŀ¼
    QString afterVitalDir;  // ���õİ�ȫ������ļ�Ŀ¼
    QString entryFuncName;  // ���õ���ں�����
    QFont editorFont;  // ���õı༭��������

    QDir currentDir; // ��Ŵ��ļ�ʱ���ڷ��ʵ�·����
    QSignalMapper* windowMapper; // ������Ϣӳ��
    QString lastOpenDir; // ��¼���򿪵��ļ�·��

    QMdiArea* mdiArea; // ���ļ��༭����
    QListWidget* fileList; // Ԥ����ǰ��ԭʼ�ļ��б�
    QListWidget* afterPreList; // Ԥ�������ļ��б�
    QListWidget* finalFlsList; // ��ȫ�������ļ��б�
    QListWidget* rsltList; // �����Ϣ�б�

    QMenu* fileMenu; // ���ļ����˵���
    QMenu* editMenu; // ���༭���˵���
    QMenu* projMenu; // ����Ŀ���˵���
    QMenu* viewMenu; // ����ͼ���˵���
    QMenu* propertyMenu; // �����á��˵���
    QMenu* windowMenu; // �����ڡ��˵���
    QMenu* helpMenu; // ���������˵���

    QToolBar* fileTlBar; // ���ļ���������
    QToolBar* editTlBar; // ���༭��������
    QToolBar* projTlBar; // ����Ŀ��������

    QAction* newFileAct; // ���½��ļ�����ť
    QAction* openAct; // �����ļ�����ť
    QAction* saveAct; // �������ļ�����ť
    QAction* saveAsAct; // �����Ϊ�ļ�����ť
    QAction* saveAllAct; // ������ȫ���ļ�����ť
    QAction* printAct; // ����ӡ�ļ�����ť
    QAction* quitAct; // ���˳�ϵͳ����ť
    QAction* unDoAct; // ����������ť
    QAction* reDoAct; // ����������ť
    QAction* cutAct; // �����С���ť
    QAction* copyAct; // �����ơ���ť
    QAction* pasteAct; // ��ճ������ť
    QAction* delAct; // ��ɾ������ť
    QAction* selAllAct; // ��ѡ��ȫ������ť
    QAction* prePrcsAct; // ��Ԥ������ť
    QAction* vtlCdPrcsAct; // ����ȫ���롱��ť
    QAction* settingAct; // �����á���ť
    QAction* closeAct; // ���رյ�ǰ���ڡ���ť
    QAction* closeAllAct; // ���ر����д��ڡ���ť
    QAction* tileAct; // ��ƽ�����д��ڡ���ť
    QAction* cascadeAct; // ��������д��ڡ���ť
    QAction* nextAct; // ����һ���ڡ���ť
    QAction* previousAct; // ��ǰһ���ڡ���ť
    QAction* separatorAct; // �ָ���
    QAction* aboutAct; // �����ڡ���ť

    QPushButton* addFlBtn; // ������ļ�����ť
    QPushButton* delFlBtn; // ��ɾ���ļ�����ť
    QPushButton* moveUpBtn; // �����ơ���ť
    QPushButton* moveDownBtn; // �����ơ���ť
    QPushButton* refreshAfterPreBtn;  // ˢ�¡�Ԥ������ļ��б���ť
    QPushButton* refreshFinalBtn;  // ˢ�¡���ȫ������ļ��б���ť

    ConfigDialog* configDialog;  // ������Ϣ�Ի���
};

#endif // MAINWINDOW_H
