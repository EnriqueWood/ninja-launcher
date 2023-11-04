# NinjaLauncher

**NinjaLauncher** is a sleek, user-friendly tool designed to enhance your gaming experience by minimizing or sending to the background all those gaming launchers that pop up when you start your computer.

## Key Features

- **Automatic Minimization:** Effortlessly minimizes or sends to the background game launchers like Steam, Epic Games, and others.
- **Quick Start:** Optimize your gaming session start time, keeping your desktop clean and ready for action.
- **Customizable:** Tailor the settings to fit your specific needs and preferences.

# Configuration File Guide

## First-Time Setup
Upon the first execution of the application, an example configuration file will be automatically created in your user's documents folder:

```plaintext
%HOMEPATH%\Documents\NinjaLauncher\NinjaLauncherSettings.ini
```
## How to Edit the Configuration File

The configuration file controls the application's behavior in launching and managing certain processes and windows. Adhere to the following format for the application to function correctly.

### Section: `Executables to Launch`
List the executables to be started with the application, along with their optional working directories.

```plaintext
executable_path|working_directory
```

- **Executable Path:** Full path to the executable file.
- **Working Directory:** The directory from which the executable starts (optional).
- **Example:**
```plaintext
  C:\\Program Files\\Steam\\Steam.exe|C:\\Program Files\\Steam
```

### Section: `Executables to Hide`
Include filenames of executables that should run in the background (hidden from view).

- **Example:**
  ```plaintext
  update.exe
  ```

### Section: `Window Classes Not to Minimize`
Specify class names of windows that should stay on screen and not be minimized.

- **Tip:** Use WinSpy or similar tools to obtain the exact class name of a window.
- **Example:**
  ```plaintext
  Notepad++
  ```

Entries must be on separate lines and properly formatted. Misconfiguration may result in runtime issues or unintended behaviors.

## Example Configuration File
Below is an example of a fully configured file:
```plaintext
[ExecutablesToLaunch]
C:\Program Files\Steam\Steam.exe|C:\Program Files\Steam
C:\Apps\MyLauncher\thelauncher.exe

[ExecutablesToHide]
steam.exe

[DoNotMinimizeTheseClassNames]
Chrome_WidgetWin_1
```
## Contact

For questions, comments, or assistance, feel free to ask directly in github.