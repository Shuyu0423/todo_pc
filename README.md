# TodoFocus

Qt 6 desktop todo and focus timer app for Windows.

## Build

Open the repository in VS Code and run the `Run Todo App` task, or build with CMake and Ninja.

## Create Installer

Install NSIS, then run:

```powershell
powershell -ExecutionPolicy Bypass -File .\todo_app\package.ps1 -Version 1.0.0
```

The installer is created at:

```text
todo_app\dist\TodoFocus-Setup-1.0.0.exe
```

The installer creates desktop and Start Menu shortcuts. User task data remains in the Windows application data directory when the app is upgraded.
