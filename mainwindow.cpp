



// #include "mainwindow.h"
// #include "ui_mainwindow.h"
// #include "retrievedialog.h"

// #include <QFile>
// #include <QMessageBox>
// #include <QInputDialog>
// #include <QShortcut>
// #include <QKeySequence>
// #include <QStatusBar>
// #include <QRegularExpression>
// #include <QFileDialog>
// #include <QTextStream>
// #include <QJsonArray>
// #include <QJsonDocument>
// #include <QVBoxLayout>
// #include <QLabel>
// #include <QPushButton>
// #include <QStackedWidget>
// #include <QXmlStreamWriter>
// #include <QXmlStreamReader>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
// {
//     ui->setupUi(this);

//     // ---------------------------
//     // Setup stacked widget
//     // ---------------------------
//     m_stackedWidget = new QStackedWidget(this);

//     // Add Welcome page
//     m_welcomePage = new QWidget(this);
//     QVBoxLayout *vbox = new QVBoxLayout(m_welcomePage);
//     m_welcomeLabel = new QLabel("Welcome to Camera Management System", m_welcomePage);
//     m_welcomeLabel->setAlignment(Qt::AlignCenter);
//     m_welcomeLabel->setWordWrap(true);
//     m_btnAddWelcome = new QPushButton("Add Camera", m_welcomePage);
//     vbox->addStretch();
//     vbox->addWidget(m_welcomeLabel);
//     vbox->addWidget(m_btnAddWelcome, 0, Qt::AlignCenter);
//     vbox->addStretch();

//     // Add both pages to stacked widget
//     m_stackedWidget->addWidget(m_welcomePage);     // index 0
//     m_stackedWidget->addWidget(ui->centralwidget); // index 1
//     setCentralWidget(m_stackedWidget);

//     // Connect welcome add button
//     connect(m_btnAddWelcome, &QPushButton::clicked, this, &MainWindow::onAddCamera);

//     // Connect main UI buttons
//     connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddCamera);
//     connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::onEditCamera);
//     connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteCamera);
//     connect(ui->btnRetrieve, &QPushButton::clicked, this, &MainWindow::onRetrieveCamera);
//     connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::onSaveData);
//     connect(ui->btnSaveAs, &QPushButton::clicked, this, &MainWindow::onSaveAsData);
//     connect(ui->btnOpen, &QPushButton::clicked, this, &MainWindow::onOpenData);

//     // Undo/Redo shortcuts
//     QShortcut *undoShortcut = new QShortcut(QKeySequence::Undo, this);
//     connect(undoShortcut, &QShortcut::activated, this, &MainWindow::onUndoAction);
//     QShortcut *redoShortcut = new QShortcut(QKeySequence::Redo, this);
//     connect(redoShortcut, &QShortcut::activated, this, &MainWindow::onRedoAction);

//     // Load autosave and refresh list
//     loadData();
//     refreshCameraList();
// }

// MainWindow::~MainWindow() {
//     saveData(); // autosave
//     delete ui;
// }

// // ----------------- PUSH ACTION -----------------
// void MainWindow::pushAction(const Action &a) {
//     if (m_actionStack.size() >= 20) m_actionStack.pop_front();
//     m_actionStack.push_back(a);
//     m_redoStack.clear();
// }

// // ----------------- VALIDATIONS -----------------
// bool MainWindow::validateName(const QString &name) {
//     QRegularExpression re("^[A-Za-z0-9]{1,20}$");
//     return re.match(name).hasMatch();
// }

// bool MainWindow::validateIPv4(const QString &ip) {
//     QStringList parts = ip.trimmed().split('.');
//     if (parts.size() != 4) return false;
//     for (const QString &part : parts) {
//         if (part.isEmpty() || part.size() > 3) return false;
//         for (QChar ch : part) if (!ch.isDigit()) return false;
//         if (part.size() > 1 && part.startsWith('0')) return false;
//         bool ok; int val = part.toInt(&ok);
//         if (!ok || val < 0 || val > 255) return false;
//     }
//     return true;
// }

// // ----------------- LOAD / SAVE -----------------
// void MainWindow::loadData() {
//     cameras.clear();
//     m_idSet.clear();
//     QFile file("cameras.json");
//     if (!file.open(QIODevice::ReadOnly)) return;

//     QByteArray data = file.readAll();
//     file.close();
//     QJsonDocument doc = QJsonDocument::fromJson(data);
//     if (!doc.isArray()) return;

//     QJsonArray arr = doc.array();
//     for (auto camVal : arr) {
//         if (camVal.isObject()) {
//             Camera cam = Camera::fromJson(camVal.toObject());
//             cameras.append(cam);
//             m_idSet.insert(cam.id);
//         }
//     }
// }

// void MainWindow::saveData() {
//     QJsonArray arr;
//     for (const Camera &cam : cameras) arr.append(cam.toJson());
//     QJsonDocument doc(arr);
//     QFile file("cameras.json");
//     if (!file.open(QIODevice::WriteOnly)) return;
//     file.write(doc.toJson());
//     file.close();
// }

// // ----------------- REFRESH LIST -----------------
// void MainWindow::refreshCameraList() {
//     ui->listWidget->clear();

//     if (cameras.isEmpty()) {
//         m_stackedWidget->setCurrentWidget(m_welcomePage);
//         return;
//     }

//     m_stackedWidget->setCurrentWidget(ui->centralwidget);

//     for (const Camera &cam : cameras)
//         ui->listWidget->addItem(QString("%1 | %2 | %3 | %4")
//                                     .arg(cam.id).arg(cam.name).arg(cam.ipAddress).arg(cam.status));
// }

// // ----------------- ADD CAMERA -----------------
// void MainWindow::onAddCamera() {
//     bool ok;
//     int id = QInputDialog::getInt(this, "Add Camera", "Enter Camera ID:", 1, 1, 10000, 1, &ok);
//     if (!ok) return;
//     if (m_idSet.contains(id)) { QMessageBox::warning(this, "Error", "Camera ID already exists!"); return; }

//     QString name = QInputDialog::getText(this, "Add Camera", "Enter Camera Name:", QLineEdit::Normal, "", &ok);
//     if (!ok || !validateName(name)) { QMessageBox::warning(this, "Error", "Invalid name!"); return; }

//     QString ip = QInputDialog::getText(this, "Add Camera", "Enter IP Address:", QLineEdit::Normal, "", &ok);
//     if (!ok || !validateIPv4(ip)) { QMessageBox::warning(this, "Error", "Invalid IPv4 address!"); return; }

//     QStringList statusOptions = {"Active", "Inactive"};
//     QString status = QInputDialog::getItem(this, "Add Camera", "Select Status:", statusOptions, 0, false, &ok);
//     if (!ok) return;

//     Camera newCam = {id, name, ip, status};
//     cameras.append(newCam);
//     m_idSet.insert(id);
//     pushAction({ActionType::Add, newCam, cameras.size() - 1, {}});

//     // Switch to main page after adding first camera
//     m_stackedWidget->setCurrentWidget(ui->centralwidget);

//     refreshCameraList();
//     saveData();
//     statusBar()->showMessage("Camera added.", 3000);
// }

// // ----------------- EDIT CAMERA -----------------
// void MainWindow::onEditCamera() {
//     QListWidgetItem *item = ui->listWidget->currentItem();
//     if (!item) { QMessageBox::warning(this, "Error", "Select a camera to edit."); return; }
//     int index = ui->listWidget->row(item);
//     Camera oldCam = cameras[index];
//     Camera &cam = cameras[index];

//     bool ok;
//     QString newName = QInputDialog::getText(this, "Edit Camera", "Enter Camera Name:", QLineEdit::Normal, cam.name, &ok);
//     if (!ok || !validateName(newName)) { QMessageBox::warning(this, "Error", "Invalid name!"); return; }
//     cam.name = newName;

//     QString newIp = QInputDialog::getText(this, "Edit Camera", "Enter IP Address:", QLineEdit::Normal, cam.ipAddress, &ok);
//     if (!ok || !validateIPv4(newIp)) { QMessageBox::warning(this, "Error", "Invalid IP!"); return; }
//     cam.ipAddress = newIp;

//     QStringList statusOptions = {"Active", "Inactive"};
//     cam.status = QInputDialog::getItem(this, "Edit Camera", "Select Status:", statusOptions,
//                                        statusOptions.indexOf(cam.status), false, &ok);
//     if (!ok) return;

//     pushAction({ActionType::Edit, cam, index, oldCam});
//     refreshCameraList();
//     saveData();
//     statusBar()->showMessage("Camera updated.", 3000);
// }

// // ----------------- DELETE CAMERA -----------------
// void MainWindow::onDeleteCamera() {
//     QListWidgetItem *item = ui->listWidget->currentItem();
//     if (!item) { QMessageBox::warning(this, "Error", "Select a camera to delete."); return; }

//     int index = ui->listWidget->row(item);
//     Camera cam = cameras[index];

//     pushAction({ActionType::Delete, cam, index, {}});
//     cameras.removeAt(index);
//     m_idSet.remove(cam.id);

//     refreshCameraList();
//     saveData();
//     statusBar()->showMessage("Camera deleted — press Ctrl+Z to undo.", 5000);
// }

// // ----------------- UNDO / REDO -----------------
// void MainWindow::onUndoAction() {
//     if (m_actionStack.isEmpty()) { QMessageBox::information(this, "Undo", "Nothing to undo."); return; }
//     Action last = m_actionStack.back(); m_actionStack.pop_back();

//     switch (last.type) {
//     case ActionType::Add:
//         cameras.removeAt(last.index); m_idSet.remove(last.cam.id); m_redoStack.push_back(last); break;
//     case ActionType::Delete:
//         cameras.insert(last.index, last.cam); m_idSet.insert(last.cam.id); m_redoStack.push_back(last); break;
//     case ActionType::Edit:
//         cameras[last.index] = last.oldCam; m_redoStack.push_back(last); break;
//     }

//     refreshCameraList();
//     saveData();
// }

// void MainWindow::onRedoAction() {
//     if (m_redoStack.isEmpty()) { QMessageBox::information(this, "Redo", "Nothing to redo."); return; }
//     Action act = m_redoStack.back(); m_redoStack.pop_back();

//     switch (act.type) {
//     case ActionType::Add:
//         cameras.insert(act.index, act.cam); m_idSet.insert(act.cam.id); m_actionStack.push_back(act); break;
//     case ActionType::Delete:
//         cameras.removeAt(act.index); m_idSet.remove(act.cam.id); m_actionStack.push_back(act); break;
//     case ActionType::Edit:
//         cameras[act.index] = act.cam; m_actionStack.push_back(act); break;
//     }

//     refreshCameraList();
//     saveData();
// }

// // ----------------- RETRIEVE -----------------
// void MainWindow::onRetrieveCamera() {
//     QListWidgetItem *item = ui->listWidget->currentItem();
//     if (!item) { QMessageBox::warning(this, "Error", "Select a camera to retrieve."); return; }
//     int index = ui->listWidget->row(item);
//     RetrieveDialog dlg(cameras[index], this);
//     dlg.exec();
// }

// // ----------------- FILE MANAGEMENT -----------------

// // ----------------- Save / Save As / Open (user file management) -----------------

// void MainWindow::onSaveData() {
//     // If user hasn't chosen a file yet, redirect to Save As
//     if (m_currentFile.isEmpty()) {
//         onSaveAsData();
//         return;
//     }

//     // Basic validation before saving
//     if (cameras.isEmpty()) {
//         QMessageBox::warning(this, "Save", "No cameras to save.");
//         return;
//     }

//     if (m_currentFile.endsWith(".json", Qt::CaseInsensitive)) saveToJson(m_currentFile);
//     else if (m_currentFile.endsWith(".csv", Qt::CaseInsensitive)) saveToCsv(m_currentFile);
//     else if (m_currentFile.endsWith(".xml", Qt::CaseInsensitive)) saveToXml(m_currentFile);
//     else {
//         // If extension unknown, default to JSON
//         saveToJson(m_currentFile);
//     }

//     QMessageBox::information(this, "Saved", "Camera data saved to:\n" + m_currentFile);
// }

// void MainWindow::onSaveAsData() {
//     if (cameras.isEmpty()) {
//         QMessageBox::warning(this, "Save As", "No cameras to save.");
//         return;
//     }

//     QString selectedFilter;
//     QString fileName = QFileDialog::getSaveFileName(this, "Save As", "",
//                                                     "JSON Files (*.json);;CSV Files (*.csv);;XML Files (*.xml)",
//                                                     &selectedFilter);
//     if (fileName.isEmpty()) return;

//     // Determine extension from selectedFilter if user didn't provide one
//     if (!fileName.contains('.')) {
//         if (selectedFilter.contains("json", Qt::CaseInsensitive)) fileName += ".json";
//         else if (selectedFilter.contains("csv", Qt::CaseInsensitive)) fileName += ".csv";
//         else if (selectedFilter.contains("xml", Qt::CaseInsensitive)) fileName += ".xml";
//     }

//     // If file exists, ask overwrite
//     QFile fCheck(fileName);
//     if (fCheck.exists()) {
//         auto res = QMessageBox::question(this, "Overwrite", "File exists. Overwrite?");
//         if (res != QMessageBox::Yes) return;
//     }

//     m_currentFile = fileName;
//     onSaveData(); // will save using m_currentFile
// }

// void MainWindow::onOpenData() {
//     QString fileName = QFileDialog::getOpenFileName(this, "Open File", "",
//                                                     "JSON Files (*.json);;CSV Files (*.csv);;XML Files (*.xml)");
//     if (fileName.isEmpty()) return;

//     QFile check(fileName);
//     if (!check.exists()) {
//         QMessageBox::warning(this, "Open", "File not found.");
//         return;
//     }

//     // Clear existing in-memory data (we will validate and re-populate)
//     cameras.clear();
//     m_idSet.clear();

//     bool loaded = false;
//     if (fileName.endsWith(".json", Qt::CaseInsensitive)) {
//         loadFromJson(fileName);
//         loaded = true;
//     } else if (fileName.endsWith(".csv", Qt::CaseInsensitive)) {
//         loadFromCsv(fileName);
//         loaded = true;
//     } else if (fileName.endsWith(".xml", Qt::CaseInsensitive)) {
//         loadFromXml(fileName);
//         loaded = true;
//     } else {
//         QMessageBox::warning(this, "Open", "Unsupported file format.");
//         return;
//     }

//     if (loaded) {
//         m_currentFile = fileName;
//         refreshCameraList();
//         saveData(); // update autosave from newly loaded data
//         QMessageBox::information(this, "Open", "File loaded: " + fileName);
//     }
// }

// // ---------- Save implementations ----------
// void MainWindow::saveToJson(const QString &fileName) {
//     QJsonArray arr;
//     for (const Camera &cam : cameras)
//         arr.append(cam.toJson());

//     QJsonDocument doc(arr);
//     QFile file(fileName);
//     if (!file.open(QIODevice::WriteOnly)) {
//         QMessageBox::warning(this, "Save", "Cannot open file for writing.");
//         return;
//     }
//     file.write(doc.toJson());
//     file.close();
// }

// void MainWindow::saveToCsv(const QString &fileName) {
//     QFile file(fileName);
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         QMessageBox::warning(this, "Save", "Cannot open file for writing.");
//         return;
//     }
//     QTextStream out(&file);
//     // header
//     out << "id,name,ipAddress,status\n";
//     for (const Camera &cam : cameras) {
//         // escape commas in names if needed (simple approach)
//         QString nameEsc = cam.name;
//         nameEsc.replace("\"", "\"\"");
//         if (nameEsc.contains(',') || nameEsc.contains('"')) nameEsc = "\"" + nameEsc + "\"";
//         out << cam.id << "," << nameEsc << "," << cam.ipAddress << "," << cam.status << "\n";
//     }
//     file.close();
// }

// void MainWindow::saveToXml(const QString &fileName) {
//     QFile file(fileName);
//     if (!file.open(QIODevice::WriteOnly)) {
//         QMessageBox::warning(this, "Save", "Cannot open file for writing.");
//         return;
//     }
//     QXmlStreamWriter xml(&file);
//     xml.setAutoFormatting(true);
//     xml.writeStartDocument();
//     xml.writeStartElement("Cameras");
//     for (const Camera &cam : cameras) {
//         xml.writeStartElement("Camera");
//         xml.writeTextElement("ID", QString::number(cam.id));
//         xml.writeTextElement("Name", cam.name);
//         xml.writeTextElement("IPAddress", cam.ipAddress);
//         xml.writeTextElement("Status", cam.status);
//         xml.writeEndElement(); // Camera
//     }
//     xml.writeEndElement(); // Cameras
//     xml.writeEndDocument();
//     file.close();
// }

// // ---------- Load implementations ----------
// void MainWindow::loadFromJson(const QString &fileName) {
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly)) {
//         QMessageBox::warning(this, "Open", "Cannot open file for reading.");
//         return;
//     }
//     QByteArray bytes = file.readAll();
//     file.close();

//     QJsonParseError parseError;
//     QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
//     if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
//         QMessageBox::warning(this, "Open", "Invalid JSON format.");
//         return;
//     }

//     QJsonArray arr = doc.array();
//     if (arr.isEmpty()) {
//         QMessageBox::information(this, "Open", "File contains no records.");
//         return;
//     }

//     for (auto v : arr) {
//         if (!v.isObject()) {
//             QMessageBox::warning(this, "Open", "Invalid record skipped.");
//             continue;
//         }
//         Camera cam = Camera::fromJson(v.toObject());
//         // validations: id integer already parsed, unique, name & ip valid
//         if (m_idSet.contains(cam.id)) {
//             QMessageBox::warning(this, "Open", QString("Duplicate ID %1 skipped.").arg(cam.id));
//             continue;
//         }
//         if (!validateName(cam.name)) {
//             QMessageBox::warning(this, "Open", QString("Invalid name for ID %1 skipped.").arg(cam.id));
//             continue;
//         }
//         if (!validateIPv4(cam.ipAddress)) {
//             QMessageBox::warning(this, "Open", QString("Invalid IP for ID %1 skipped.").arg(cam.id));
//             continue;
//         }
//         cameras.append(cam);
//         m_idSet.insert(cam.id);
//     }
// }

// void MainWindow::loadFromCsv(const QString &fileName) {
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QMessageBox::warning(this, "Open", "Cannot open file for reading.");
//         return;
//     }
//     QTextStream in(&file);
//     bool firstLine = true;
//     bool anyRecord = false;
//     while (!in.atEnd()) {
//         QString line = in.readLine();
//         if (firstLine) {
//             // accept header line if present (id,name,ipAddress,status) - skip it
//             if (line.toLower().contains("id") && line.toLower().contains("name")) {
//                 firstLine = false;
//                 continue;
//             }
//             firstLine = false;
//         }
//         if (line.trimmed().isEmpty()) continue;

//         // naive CSV splitting: handle quoted names
//         QStringList parts;
//         QString cur;
//         bool inQuotes = false;
//         for (int i = 0; i < line.size(); ++i) {
//             QChar ch = line[i];
//             if (ch == '"' ) {
//                 inQuotes = !inQuotes;
//                 continue;
//             }
//             if (ch == ',' && !inQuotes) {
//                 parts << cur;
//                 cur.clear();
//             } else {
//                 cur.append(ch);
//             }
//         }
//         parts << cur;

//         if (parts.size() < 4) {
//             QMessageBox::warning(this, "Open", "Invalid CSV row skipped.");
//             continue;
//         }

//         bool ok = false;
//         int id = parts[0].trimmed().toInt(&ok);
//         QString name = parts[1].trimmed();
//         QString ip = parts[2].trimmed();
//         QString status = parts[3].trimmed();

//         if (!ok) {
//             QMessageBox::warning(this, "Open", "Non-integer ID skipped.");
//             continue;
//         }
//         if (m_idSet.contains(id)) {
//             QMessageBox::warning(this, "Open", QString("Duplicate ID %1 skipped.").arg(id));
//             continue;
//         }
//         if (!validateName(name)) {
//             QMessageBox::warning(this, "Open", QString("Invalid name for ID %1 skipped.").arg(id));
//             continue;
//         }
//         if (!validateIPv4(ip)) {
//             QMessageBox::warning(this, "Open", QString("Invalid IP for ID %1 skipped.").arg(id));
//             continue;
//         }

//         Camera cam = { id, name, ip, status };
//         cameras.append(cam);
//         m_idSet.insert(id);
//         anyRecord = true;
//     }
//     file.close();
//     if (!anyRecord && cameras.isEmpty()) {
//         QMessageBox::information(this, "Open", "No valid records found in CSV.");
//     }
// }

// void MainWindow::loadFromXml(const QString &fileName) {
//     QFile file(fileName);
//     if (!file.open(QIODevice::ReadOnly)) {
//         QMessageBox::warning(this, "Open", "Cannot open file for reading.");
//         return;
//     }
//     QXmlStreamReader xml(&file);
//     bool anyRecord = false;

//     while (!xml.atEnd()) {
//         xml.readNext();
//         if (xml.isStartElement() && xml.name() == "Camera") {
//             QString idStr, name, ip, status;
//             while (!(xml.isEndElement() && xml.name() == "Camera")) {
//                 xml.readNext();
//                 if (xml.isStartElement()) {
//                     if (xml.name() == "ID") idStr = xml.readElementText();
//                     else if (xml.name() == "Name") name = xml.readElementText();
//                     else if (xml.name() == "IPAddress") ip = xml.readElementText();
//                     else if (xml.name() == "Status") status = xml.readElementText();
//                 }
//             }

//             bool ok = false;
//             int id = idStr.trimmed().toInt(&ok);
//             if (!ok) {
//                 QMessageBox::warning(this, "Open", "Non-integer ID skipped in XML.");
//                 continue;
//             }
//             if (m_idSet.contains(id)) {
//                 QMessageBox::warning(this, "Open", QString("Duplicate ID %1 skipped.").arg(id));
//                 continue;
//             }
//             if (!validateName(name)) {
//                 QMessageBox::warning(this, "Open", QString("Invalid name for ID %1 skipped.").arg(id));
//                 continue;
//             }
//             if (!validateIPv4(ip)) {
//                 QMessageBox::warning(this, "Open", QString("Invalid IP for ID %1 skipped.").arg(id));
//                 continue;
//             }

//             Camera cam = { id, name.trimmed(), ip.trimmed(), status.trimmed() };
//             cameras.append(cam);
//             m_idSet.insert(id);
//             anyRecord = true;
//         }
//     }

//     if (xml.hasError()) {
//         QMessageBox::warning(this, "Open", "XML parse error.");
//     }
//     file.close();

//     if (!anyRecord && cameras.isEmpty()) {
//         QMessageBox::information(this, "Open", "No valid records found in XML.");
//     }
// }






// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "retrievedialog.h"

#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QShortcut>
#include <QKeySequence>
#include <QStatusBar>
#include <QRegularExpression>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QProgressBar>
#include <QTimer>
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ---------------------------------------------
    // NEW: Setup stacked widget (Welcome page + Main page)
    // - This preserves your original ui->centralwidget as the main page.
    // - Use defensive switching logic via updateStackedPage() to avoid crashes
    //   when a widget isn't present in the stack.
    // ---------------------------------------------
    m_stackedWidget = new QStackedWidget(this);

    // Create welcome page (with progress bar)
    m_welcomePage = new QWidget(this);
    QVBoxLayout *vbox = new QVBoxLayout(m_welcomePage);
    m_welcomeLabel = new QLabel("Welcome to Camera Management System", m_welcomePage);
    m_welcomeLabel->setAlignment(Qt::AlignCenter);
    m_welcomeLabel->setWordWrap(true);
    m_btnAddWelcome = new QPushButton("Add Camera", m_welcomePage);

    // Progress bar shown during brief loader / splash (about 3 seconds)
    QProgressBar *progressBar = new QProgressBar(m_welcomePage);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    vbox->addStretch();
    vbox->addWidget(m_welcomeLabel);
    vbox->addWidget(progressBar);
    vbox->addWidget(m_btnAddWelcome, 0, Qt::AlignCenter);
    vbox->addStretch();

    // Add both pages into stacked widget
    // index 0 = welcome page, index 1 = original main central widget
    m_stackedWidget->addWidget(m_welcomePage);
    m_stackedWidget->addWidget(ui->centralwidget);

    // Make stacked widget the main central widget
    setCentralWidget(m_stackedWidget);

    // Connect the Welcome add button to existing add logic
    connect(m_btnAddWelcome, &QPushButton::clicked, this, &MainWindow::onAddCamera);

    // ---------------------------------------------
    // Connect main UI buttons (existing behavior)
    // ---------------------------------------------
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onAddCamera);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::onEditCamera);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteCamera);
    connect(ui->btnRetrieve, &QPushButton::clicked, this, &MainWindow::onRetrieveCamera);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::onSaveData);
    connect(ui->btnSaveAs, &QPushButton::clicked, this, &MainWindow::onSaveAsData);
    connect(ui->btnOpen, &QPushButton::clicked, this, &MainWindow::onOpenData);

    // Undo/Redo shortcuts
    QShortcut *undoShortcut = new QShortcut(QKeySequence::Undo, this);
    connect(undoShortcut, &QShortcut::activated, this, &MainWindow::onUndoAction);
    QShortcut *redoShortcut = new QShortcut(QKeySequence::Redo, this);
    connect(redoShortcut, &QShortcut::activated, this, &MainWindow::onRedoAction);

    // ---------------------------------------------
    // Load autosaved data and refresh list
    // Note: loadData() populates cameras and m_idSet.
    // ---------------------------------------------
    // loadData();// for testing purpose

    // Always show welcome page first
    m_stackedWidget->setCurrentWidget(m_welcomePage);

    // Ensure UI is updated before starting the timer
    QTimer::singleShot(100, this, [=]() {
        runWelcomeLoader(progressBar);
    });





    // ---------------------------------------------
    // POPULATE LIST BASED ON LOADED DATA
    // ---------------------------------------------
    refreshCameraList();
}

MainWindow::~MainWindow() {
    // autosave on exit (same behaviour as before)
    saveData();
    delete ui;
}

// ----------------- HELPER: safe stack switching -----------------
// This was declared in your header as a new helper; implemented here.
// It ensures we do not call setCurrentWidget() with a widget not in the stack
// (this is the defensive fix for your crash).

// ----------------- HELPER: safe stack switching -----------------
// REPLACE your old updateStackedPage() with this version
// This ensures:
// 1. If no cameras exist, welcome page remains visible until user adds one
// 2. If cameras exist, main page shows automatically
void MainWindow::updateStackedPage()
{
    if (!m_stackedWidget) return;

    if (cameras.isEmpty()) {
        // NO RECORDS: Always stay on welcome page
        int idx = m_stackedWidget->indexOf(m_welcomePage);
        if (idx != -1) {
            m_stackedWidget->setCurrentIndex(idx);
        } else if (m_stackedWidget->count() > 0) {
            m_stackedWidget->setCurrentIndex(0); // fallback
        }
        return; // do NOT switch to main page
    }

    // RECORDS EXIST: show main page (ui->centralwidget)
    int mainIdx = m_stackedWidget->indexOf(ui->centralwidget);
    if (mainIdx != -1) {
        m_stackedWidget->setCurrentIndex(mainIdx);
    } else if (m_stackedWidget->count() > 1) {
        // fallback to second page if available
        m_stackedWidget->setCurrentIndex(1);
    }
}


// ----------------- PUSH ACTION -----------------
void MainWindow::pushAction(const Action &a) {
    if (m_actionStack.size() >= 20) m_actionStack.pop_front();
    m_actionStack.push_back(a);
    m_redoStack.clear();
}

// ----------------- VALIDATIONS -----------------
bool MainWindow::validateName(const QString &name) {
    QRegularExpression re("^[A-Za-z0-9]{1,20}$");
    return re.match(name).hasMatch();
}

bool MainWindow::validateIPv4(const QString &ip) {
    QStringList parts = ip.trimmed().split('.');
    if (parts.size() != 4) return false;
    for (const QString &part : parts) {
        if (part.isEmpty() || part.size() > 3) return false;
        for (QChar ch : part) if (!ch.isDigit()) return false;
        // reject leading zeros like "01", except exactly "0"
        if (part.size() > 1 && part.startsWith('0')) return false;
        bool ok; int val = part.toInt(&ok);
        if (!ok || val < 0 || val > 255) return false;
    }
    return true;
}

// ----------------- LOAD / SAVE (autosave cameras.json) -----------------
void MainWindow::loadData() {
    cameras.clear();
    m_idSet.clear();
    QFile file("cameras.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray data = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray arr = doc.array();
    for (auto camVal : arr) {
        if (camVal.isObject()) {
            Camera cam = Camera::fromJson(camVal.toObject());
            cameras.append(cam);
            m_idSet.insert(cam.id);
        }
    }
}

void MainWindow::saveData() {
    QJsonArray arr;
    for (const Camera &cam : cameras) arr.append(cam.toJson());
    QJsonDocument doc(arr);
    QFile file("cameras.json");
    if (!file.open(QIODevice::WriteOnly)) return;
    file.write(doc.toJson());
    file.close();
}

// ----------------- REFRESH LIST -----------------
void MainWindow::refreshCameraList() {
    ui->listWidget->clear();

    // Switch stacked page safely depending on data presence
    updateStackedPage();

    // If showing main page, populate list
    int mainIdx = m_stackedWidget->indexOf(ui->centralwidget);
    if (m_stackedWidget->currentIndex() == mainIdx) {
        for (const Camera &cam : cameras)
            ui->listWidget->addItem(QString("%1 | %2 | %3 | %4")
                                        .arg(cam.id).arg(cam.name).arg(cam.ipAddress).arg(cam.status));
    }
}

// ----------------- ADD CAMERA -----------------
void MainWindow::onAddCamera() {
    bool ok;
    int id = QInputDialog::getInt(this, "Add Camera", "Enter Camera ID:", 1, 1, 10000, 1, &ok);
    if (!ok) return;

    if (m_idSet.contains(id)) {
        QMessageBox::warning(this, "Error", "Camera ID already exists!");
        return;
    }

    QString name = QInputDialog::getText(this, "Add Camera", "Enter Camera Name:", QLineEdit::Normal, "", &ok);
    if (!ok || !validateName(name)) {
        QMessageBox::warning(this, "Error", "Invalid name! Only letters/numbers up to 20 chars.");
        return;
    }

    QString ip = QInputDialog::getText(this, "Add Camera", "Enter IP Address:", QLineEdit::Normal, "", &ok);
    if (!ok || !validateIPv4(ip)) {
        QMessageBox::warning(this, "Error", "Invalid IPv4 address!");
        return;
    }

    QStringList statusOptions = {"Active", "Inactive"};
    QString status = QInputDialog::getItem(this, "Add Camera", "Select Status:", statusOptions, 0, false, &ok);
    if (!ok) return;

    Camera newCam = {id, name, ip, status};
    cameras.append(newCam);
    m_idSet.insert(id);

    pushAction({ActionType::Add, newCam, cameras.size() - 1, {}});

    // Show main page (safe switch)
    updateStackedPage();

    refreshCameraList();
    saveData(); // autosave
    statusBar()->showMessage("Camera added.", 3000);
}

// ----------------- EDIT CAMERA -----------------
void MainWindow::onEditCamera() {
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Error", "Select a camera to edit.");
        return;
    }

    int index = ui->listWidget->row(item);
    Camera oldCam = cameras[index];
    Camera &cam = cameras[index];

    bool ok;
    QString newName = QInputDialog::getText(this, "Edit Camera", "Enter Camera Name:", QLineEdit::Normal, cam.name, &ok);
    if (!ok || !validateName(newName)) {
        QMessageBox::warning(this, "Error", "Invalid name! Only letters/numbers up to 20 chars.");
        return;
    }
    cam.name = newName;

    QString newIp = QInputDialog::getText(this, "Edit Camera", "Enter IP Address:", QLineEdit::Normal, cam.ipAddress, &ok);
    if (!ok || !validateIPv4(newIp)) {
        QMessageBox::warning(this, "Error", "Invalid IPv4 address!");
        return;
    }
    cam.ipAddress = newIp;

    QStringList statusOptions = {"Active", "Inactive"};
    cam.status = QInputDialog::getItem(this, "Edit Camera", "Select Status:", statusOptions,
                                       statusOptions.indexOf(cam.status), false, &ok);
    if (!ok) return;

    pushAction({ActionType::Edit, cam, index, oldCam});
    refreshCameraList();
    saveData(); // autosave
    statusBar()->showMessage("Camera updated.", 3000);
}

// ----------------- DELETE CAMERA -----------------
void MainWindow::onDeleteCamera() {
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Error", "Select a camera to delete.");
        return;
    }

    int index = ui->listWidget->row(item);
    const Camera cam = cameras.at(index);

    pushAction({ActionType::Delete, cam, index, {}});

    cameras.removeAt(index);
    m_idSet.remove(cam.id);

    refreshCameraList();
    saveData(); // autosave

    statusBar()->showMessage("Camera deleted — press Ctrl+Z to undo.", 5000);
}

// ----------------- UNDO / REDO -----------------
void MainWindow::onUndoAction() {
    if (m_actionStack.isEmpty()) {
        QMessageBox::information(this, "Undo", "Nothing to undo.");
        return;
    }

    Action last = m_actionStack.back();
    m_actionStack.pop_back();

    switch (last.type) {
    case ActionType::Add:
        // safety: ensure last.index within range
        if (last.index >= 0 && last.index < cameras.size()) {
            cameras.removeAt(last.index);
            m_idSet.remove(last.cam.id);
        }
        m_redoStack.push_back(last);
        statusBar()->showMessage("Undo Add successful.", 3000);
        break;
    case ActionType::Delete:
        // insert at last.index or clamp
        {
            int insertIndex = last.index;
            if (insertIndex < 0) insertIndex = 0;
            if (insertIndex > cameras.size()) insertIndex = cameras.size();
            cameras.insert(insertIndex, last.cam);
            m_idSet.insert(last.cam.id);
        }
        m_redoStack.push_back(last);
        statusBar()->showMessage("Undo Delete successful.", 3000);
        break;
    case ActionType::Edit:
        if (last.index >= 0 && last.index < cameras.size()) {
            cameras[last.index] = last.oldCam;
        }
        m_redoStack.push_back(last);
        statusBar()->showMessage("Undo Edit successful.", 3000);
        break;
    }

    refreshCameraList();
    saveData(); // autosave
}

void MainWindow::onRedoAction() {
    if (m_redoStack.isEmpty()) {
        QMessageBox::information(this, "Redo", "Nothing to redo.");
        return;
    }

    Action act = m_redoStack.back();
    m_redoStack.pop_back();

    switch (act.type) {
    case ActionType::Add:
    {
        int insertIndex = act.index;
        if (insertIndex < 0) insertIndex = 0;
        if (insertIndex > cameras.size()) insertIndex = cameras.size();
        cameras.insert(insertIndex, act.cam);
        m_idSet.insert(act.cam.id);
        m_actionStack.push_back(act);
    }
        statusBar()->showMessage("Redo Add successful.", 3000);
        break;
    case ActionType::Delete:
        if (act.index >= 0 && act.index < cameras.size()) {
            cameras.removeAt(act.index);
            m_idSet.remove(act.cam.id);
        }
        m_actionStack.push_back(act);
        statusBar()->showMessage("Redo Delete successful.", 3000);
        break;
    case ActionType::Edit:
        if (act.index >= 0 && act.index < cameras.size()) {
            cameras[act.index] = act.cam;
            m_actionStack.push_back(act);
        }
        statusBar()->showMessage("Redo Edit successful.", 3000);
        break;
    }

    refreshCameraList();
    saveData(); // autosave
}

// ----------------- RETRIEVE -----------------
void MainWindow::onRetrieveCamera() {
    QListWidgetItem *item = ui->listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Error", "Select a camera to retrieve.");
        return;
    }

    int index = ui->listWidget->row(item);
    Camera cam = cameras[index];

    RetrieveDialog dlg(cam, this);
    dlg.exec();
}

// ----------------- FILE MANAGEMENT -----------------

void MainWindow::onSaveData() {
    // If user hasn't chosen a file yet, redirect to Save As
    if (m_currentFile.isEmpty()) {
        onSaveAsData();
        return;
    }

    // Basic validation before saving
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "Save", "No cameras to save.");
        return;
    }

    if (m_currentFile.endsWith(".json", Qt::CaseInsensitive)) saveToJson(m_currentFile);
    else if (m_currentFile.endsWith(".csv", Qt::CaseInsensitive)) saveToCsv(m_currentFile);
    else if (m_currentFile.endsWith(".xml", Qt::CaseInsensitive)) saveToXml(m_currentFile);
    else {
        // default to JSON
        saveToJson(m_currentFile);
    }

    QMessageBox::information(this, "Saved", "Camera data saved to:\n" + m_currentFile);
}

void MainWindow::onSaveAsData() {
    if (cameras.isEmpty()) {
        QMessageBox::warning(this, "Save As", "No cameras to save.");
        return;
    }

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "",
                                                    "JSON Files (*.json);;CSV Files (*.csv);;XML Files (*.xml)",
                                                    &selectedFilter);
    if (fileName.isEmpty()) return;

    // add extension if user didn't provide one
    if (!fileName.contains('.')) {
        if (selectedFilter.contains("json", Qt::CaseInsensitive)) fileName += ".json";
        else if (selectedFilter.contains("csv", Qt::CaseInsensitive)) fileName += ".csv";
        else if (selectedFilter.contains("xml", Qt::CaseInsensitive)) fileName += ".xml";
    }

    // if exists ask to overwrite
    QFile fCheck(fileName);
    if (fCheck.exists()) {
        auto res = QMessageBox::question(this, "Overwrite", "File exists. Overwrite?");
        if (res != QMessageBox::Yes) return;
    }

    m_currentFile = fileName;
    onSaveData();
}

void MainWindow::onOpenData() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "",
                                                    "JSON Files (*.json);;CSV Files (*.csv);;XML Files (*.xml)");
    if (fileName.isEmpty()) return;

    QFile check(fileName);
    if (!check.exists()) {
        QMessageBox::warning(this, "Open", "File not found.");
        return;
    }

    // Clear existing in-memory data (we will validate and re-populate)
    cameras.clear();
    m_idSet.clear();

    bool loaded = false;
    if (fileName.endsWith(".json", Qt::CaseInsensitive)) {
        loadFromJson(fileName);
        loaded = true;
    } else if (fileName.endsWith(".csv", Qt::CaseInsensitive)) {
        loadFromCsv(fileName);
        loaded = true;
    } else if (fileName.endsWith(".xml", Qt::CaseInsensitive)) {
        loadFromXml(fileName);
        loaded = true;
    } else {
        QMessageBox::warning(this, "Open", "Unsupported file format.");
        return;
    }

    if (loaded) {
        m_currentFile = fileName;
        refreshCameraList();
        saveData(); // update autosave from newly loaded data
        QMessageBox::information(this, "Open", "File loaded: " + fileName);
    }
}

// ---------- Save implementations ----------
void MainWindow::saveToJson(const QString &fileName) {
    QJsonArray arr;
    for (const Camera &cam : cameras)
        arr.append(cam.toJson());

    QJsonDocument doc(arr);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save", "Cannot open file for writing.");
        return;
    }
    file.write(doc.toJson());
    file.close();
}

void MainWindow::saveToCsv(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save", "Cannot open file for writing.");
        return;
    }
    QTextStream out(&file);
    out << "id,name,ipAddress,status\n";
    for (const Camera &cam : cameras) {
        QString nameEsc = cam.name;
        nameEsc.replace("\"", "\"\"");
        if (nameEsc.contains(',') || nameEsc.contains('"')) nameEsc = "\"" + nameEsc + "\"";
        out << cam.id << "," << nameEsc << "," << cam.ipAddress << "," << cam.status << "\n";
    }
    file.close();
}

void MainWindow::saveToXml(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save", "Cannot open file for writing.");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Cameras");
    for (const Camera &cam : cameras) {
        xml.writeStartElement("Camera");
        xml.writeTextElement("ID", QString::number(cam.id));
        xml.writeTextElement("Name", cam.name);
        xml.writeTextElement("IPAddress", cam.ipAddress);
        xml.writeTextElement("Status", cam.status);
        xml.writeEndElement(); // Camera
    }
    xml.writeEndElement(); // Cameras
    xml.writeEndDocument();
    file.close();
}

// ---------- Load implementations ----------
void MainWindow::loadFromJson(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Open", "Cannot open file for reading.");
        return;
    }
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
        QMessageBox::warning(this, "Open", "Invalid JSON format.");
        return;
    }

    QJsonArray arr = doc.array();
    if (arr.isEmpty()) {
        QMessageBox::information(this, "Open", "File contains no records.");
        return;
    }

    for (auto v : arr) {
        if (!v.isObject()) {
            QMessageBox::warning(this, "Open", "Invalid record skipped.");
            continue;
        }
        Camera cam = Camera::fromJson(v.toObject());
        if (m_idSet.contains(cam.id)) {
            QMessageBox::warning(this, "Open", QString("Duplicate ID %1 skipped.").arg(cam.id));
            continue;
        }
        if (!validateName(cam.name)) {
            QMessageBox::warning(this, "Open", QString("Invalid name for ID %1 skipped.").arg(cam.id));
            continue;
        }
        if (!validateIPv4(cam.ipAddress)) {
            QMessageBox::warning(this, "Open", QString("Invalid IP for ID %1 skipped.").arg(cam.id));
            continue;
        }
        cameras.append(cam);
        m_idSet.insert(cam.id);
    }
}

void MainWindow::loadFromCsv(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Open", "Cannot open file for reading.");
        return;
    }
    QTextStream in(&file);
    bool firstLine = true;
    bool anyRecord = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (firstLine) {
            if (line.toLower().contains("id") && line.toLower().contains("name")) {
                firstLine = false;
                continue;
            }
            firstLine = false;
        }
        if (line.trimmed().isEmpty()) continue;

        QStringList parts;
        QString cur;
        bool inQuotes = false;
        for (int i = 0; i < line.size(); ++i) {
            QChar ch = line[i];
            if (ch == '"' ) {
                inQuotes = !inQuotes;
                continue;
            }
            if (ch == ',' && !inQuotes) {
                parts << cur;
                cur.clear();
            } else {
                cur.append(ch);
            }
        }
        parts << cur;

        if (parts.size() < 4) {
            QMessageBox::warning(this, "Open", "Invalid CSV row skipped.");
            continue;
        }

        bool ok = false;
        int id = parts[0].trimmed().toInt(&ok);
        QString name = parts[1].trimmed();
        QString ip = parts[2].trimmed();
        QString status = parts[3].trimmed();

        if (!ok) {
            QMessageBox::warning(this, "Open", "Non-integer ID skipped.");
            continue;
        }
        if (m_idSet.contains(id)) {
            QMessageBox::warning(this, "Open", QString("Duplicate ID %1 skipped.").arg(id));
            continue;
        }
        if (!validateName(name)) {
            QMessageBox::warning(this, "Open", QString("Invalid name for ID %1 skipped.").arg(id));
            continue;
        }
        if (!validateIPv4(ip)) {
            QMessageBox::warning(this, "Open", QString("Invalid IP for ID %1 skipped.").arg(id));
            continue;
        }

        Camera cam = { id, name, ip, status };
        cameras.append(cam);
        m_idSet.insert(id);
        anyRecord = true;
    }
    file.close();
    if (!anyRecord && cameras.isEmpty()) {
        QMessageBox::information(this, "Open", "No valid records found in CSV.");
    }
}

void MainWindow::loadFromXml(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Open", "Cannot open file for reading.");
        return;
    }
    QXmlStreamReader xml(&file);
    bool anyRecord = false;

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "Camera") {
            QString idStr, name, ip, status;
            while (!(xml.isEndElement() && xml.name() == "Camera")) {
                xml.readNext();
                if (xml.isStartElement()) {
                    if (xml.name() == "ID") idStr = xml.readElementText();
                    else if (xml.name() == "Name") name = xml.readElementText();
                    else if (xml.name() == "IPAddress") ip = xml.readElementText();
                    else if (xml.name() == "Status") status = xml.readElementText();
                }
            }

            bool ok = false;
            int id = idStr.trimmed().toInt(&ok);
            if (!ok) {
                QMessageBox::warning(this, "Open", "Non-integer ID skipped in XML.");
                continue;
            }
            if (m_idSet.contains(id)) {
                QMessageBox::warning(this, "Open", QString("Duplicate ID %1 skipped.").arg(id));
                continue;
            }
            if (!validateName(name)) {
                QMessageBox::warning(this, "Open", QString("Invalid name for ID %1 skipped.").arg(id));
                continue;
            }
            if (!validateIPv4(ip)) {
                QMessageBox::warning(this, "Open", QString("Invalid IP for ID %1 skipped.").arg(id));
                continue;
            }

            Camera cam = { id, name.trimmed(), ip.trimmed(), status.trimmed() };
            cameras.append(cam);
            m_idSet.insert(id);
            anyRecord = true;
        }
    }

    if (xml.hasError()) {
        QMessageBox::warning(this, "Open", "XML parse error.");
    }
    file.close();

    if (!anyRecord && cameras.isEmpty()) {
        QMessageBox::information(this, "Open", "No valid records found in XML.");
    }
}

void MainWindow::runWelcomeLoader(QProgressBar *progressBar) {
    progressBar->setValue(0);

    int durationMs = 5000;
    int intervalMs = 50;
    int steps = durationMs / intervalMs;
    int *progress = new int(0);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        (*progress)++;
        progressBar->setValue((*progress * 100) / steps);

        if (*progress >= steps) {
            timer->stop();
            timer->deleteLater();
            delete progress;
            // Always switch page AFTER animation ends
            qDebug() << "Welcome loader complete. Switching to main page.";
            // Load data *after* welcome animation
            loadData();

            // Switch to appropriate page depending on data
            updateStackedPage();
        }
    });

    timer->start(intervalMs);
}
