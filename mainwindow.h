



// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include <QVector>
// #include <QString>
// #include <QJsonArray>
// #include <QJsonObject>
// #include <QJsonDocument>
// #include <QSet>

// // forward declarations for widgets we create programmatically
// class QStackedWidget;
// class QWidget;
// class QPushButton;
// class QLabel;

// QT_BEGIN_NAMESPACE
// namespace Ui { class MainWindow; }
// QT_END_NAMESPACE

// // Camera structure
// struct Camera {
//     int id;
//     QString name;
//     QString ipAddress;
//     QString status;

//     QJsonObject toJson() const {
//         QJsonObject obj;
//         obj["id"] = id;
//         obj["name"] = name;
//         obj["ipAddress"] = ipAddress;
//         obj["status"] = status;
//         return obj;
//     }

//     static Camera fromJson(const QJsonObject &obj) {
//         Camera cam;
//         cam.id = obj["id"].toInt();
//         cam.name = obj["name"].toString();
//         cam.ipAddress = obj["ipAddress"].toString();
//         cam.status = obj["status"].toString();
//         return cam;
//     }
// };

// class MainWindow : public QMainWindow {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private slots:
//     void onAddCamera();
//     void onEditCamera();
//     void onDeleteCamera();
//     void onRetrieveCamera();
//     void onUndoAction();
//     void onRedoAction();
//     void onSaveData();
//     void onSaveAsData();
//     void onOpenData();

// private:
//     Ui::MainWindow *ui;
//     QVector<Camera> cameras;
//     QSet<int> m_idSet; // for fast ID uniqueness check

//     enum class ActionType { Add, Edit, Delete };

//     struct Action {
//         ActionType type;
//         Camera cam;
//         int index;
//         Camera oldCam;
//     };

//     QVector<Action> m_actionStack;
//     QVector<Action> m_redoStack;

//     void pushAction(const Action &a);
//     void loadData();
//     void saveData();
//     void refreshCameraList();

//     bool validateName(const QString &name);
//     bool validateIPv4(const QString &ip);

//     QString m_currentFile;
//     void saveToJson(const QString &fileName);
//     void saveToCsv(const QString &fileName);
//     void saveToXml(const QString &fileName);

//     void loadFromJson(const QString &fileName);
//     void loadFromCsv(const QString &fileName);
//     void loadFromXml(const QString &fileName);

//     // --- Welcome screen components ---
//     QStackedWidget *m_stackedWidget = nullptr;
//     QWidget *m_welcomePage = nullptr;
//     QPushButton *m_btnAddWelcome = nullptr;
//     QLabel *m_welcomeLabel = nullptr;

//     void updateStackedPage(); // <-- NEW helper
// };

// #endif // MAINWINDOW_H








#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSet>
#include <QProgressBar>

// forward declarations for widgets we create programmatically
class QStackedWidget;
class QWidget;
class QPushButton;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Camera structure
struct Camera {
    int id;
    QString name;
    QString ipAddress;
    QString status;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["ipAddress"] = ipAddress;
        obj["status"] = status;
        return obj;
    }

    static Camera fromJson(const QJsonObject &obj) {
        Camera cam;
        cam.id = obj["id"].toInt();
        cam.name = obj["name"].toString();
        cam.ipAddress = obj["ipAddress"].toString();
        cam.status = obj["status"].toString();
        return cam;
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddCamera();
    void onEditCamera();
    void onDeleteCamera();
    void onRetrieveCamera();
    void onUndoAction();
    void onRedoAction();
    void onSaveData();
    void onSaveAsData();
    void onOpenData();

private:
    Ui::MainWindow *ui;
    QVector<Camera> cameras;
    QSet<int> m_idSet; // for fast ID uniqueness check

    enum class ActionType { Add, Edit, Delete };

    struct Action {
        ActionType type;
        Camera cam;
        int index;
        Camera oldCam;
    };

    QVector<Action> m_actionStack;
    QVector<Action> m_redoStack;

    void pushAction(const Action &a);
    void loadData();
    void saveData();
    void refreshCameraList();

    bool validateName(const QString &name);
    bool validateIPv4(const QString &ip);

    QString m_currentFile;
    void saveToJson(const QString &fileName);
    void saveToCsv(const QString &fileName);
    void saveToXml(const QString &fileName);

    void loadFromJson(const QString &fileName);
    void loadFromCsv(const QString &fileName);
    void loadFromXml(const QString &fileName);

    // --- Welcome screen components ---
    QStackedWidget *m_stackedWidget = nullptr;
    QWidget *m_welcomePage = nullptr;
    QPushButton *m_btnAddWelcome = nullptr;
    QLabel *m_welcomeLabel = nullptr;

    void updateStackedPage(); // <-- NEW helper
    void runWelcomeLoader(QProgressBar *progressBar);
};

#endif // MAINWINDOW_H

