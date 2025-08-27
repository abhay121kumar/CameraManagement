📘 Camera Management System

A Qt6 desktop application for managing camera records. This project demonstrates CRUD operations, data persistence, undo/redo actions, and a custom splash/welcome loader screen with progress indicator.

🚀 Features

Splash + Welcome Loader

On launch, a welcome screen with a progress bar appears for 5 seconds.

If camera records already exist → After loading, the app switches automatically to the main camera list page.

If no records exist → The welcome screen stays active, prompting the user to add their first camera.

Camera CRUD Operations

Add Camera

Unique Camera ID required.

Name must be alphanumeric, up to 20 characters.

IP Address must be a valid IPv4 (checked via strict regex).

Status can be set to Active / Inactive.

Edit Camera → Modify existing camera details.

Delete Camera → Remove a camera permanently.

Retrieve Camera → Show camera details in a dedicated dialog.

Data Persistence

Records are stored in cameras.json automatically.

Supports Save As to JSON, CSV, or XML.

Supports Open to import cameras from JSON, CSV, or XML.

Undo / Redo Support

Undo and Redo shortcuts available:

Ctrl+Z → Undo last action.

Ctrl+Y → Redo undone action.

Up to 20 actions are kept in history.

UI Behavior

If no cameras exist → Always shows Welcome Page until user adds at least one camera.

If cameras exist → Main Page with camera list appears after splash loader.

Status messages appear on the status bar after each operation.

📂 Project Structure
CameraManagement/
│
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── retrievedialog.h
├── retrievedialog.cpp
└── resources/ (icons, images if any)

⚙️ How to Run

Install Qt 6.x (with MinGW or MSVC kit).

Clone or copy the project into your workspace.

Open in Qt Creator.

Build & Run:

qmake && make
./CameraManagement


On first launch, you will see the Welcome Loader Page with a progress bar.

📖 Example Flow

Launch App

Welcome Page loads with "Loading cameras…" progress bar.

Case A: No Cameras Exist

Loader completes.

Welcome Page remains active → Prompt to Add Camera.

User clicks Add Camera button → Enter details → Switch to Main Camera List page.

Case B: Cameras Already Exist

Loader completes.

Application automatically switches to the Main Camera List page.

User sees all previously saved cameras.

Perform CRUD

Add → Validate ID, Name, IP, Status.

Edit → Change details, validations applied again.

Delete → Confirmation + undo support.

Retrieve → Opens small dialog showing selected camera.

Save / Load Data

Automatically saved to cameras.json.

Export or Import from JSON, CSV, XML using toolbar buttons.

🛡️ Rules & Validations

Camera ID must be unique (integer).

Name must be 1–20 alphanumeric characters (no spaces/symbols).

IPv4 Address must be valid, e.g., 192.168.0.1 (no leading zeroes, no out-of-range octets).

Status must be either Active or Inactive.

🎯 Design Choices

QStackedWidget used to switch between Welcome Page and Main Page cleanly.

QTimer + QProgressBar simulate a loader before showing main UI.

Undo/Redo implemented via a custom Action stack (max depth 20).

Qt JSON / CSV / XML APIs used for flexible save/load.

Status bar messages ensure user always knows what happened.

✅ Summary

Start → Welcome Splash (5s).

If no data → Stay on Welcome Page until user adds camera.

If data exists → Auto-switch to Main CRUD Page.

Full CRUD with strict rules.

Undo/Redo + Save/Load supported.
