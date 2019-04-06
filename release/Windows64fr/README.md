# Installation

1. Extract directory-emptier.zip in any directory. For exemple in **"C:\Program Files\Directory Emptier\"**.
2. Run context-menu-tuner.exe. To understand how it works, you can click on "reinitialize" button and see what happens when you right-click a directory.

If you are afraid to run context-menu-tuner because it requires root privileges, you can read the note below to understand what keys you must edit to add the required context menu entries. You can type ".\directory-emptier.exe --help" to understand some more about the possible options.

You can also look at the source code at "https://github.com/Bauthe/directory-emptier", or even build it yourself with Qt.

# context-menu-tuner

A software which edits Windows registry to create custom context menu entries related with directory-emptier.

It enables the user to select names and options of the context menu entries that will run directory-emptier.exe. This will only work if context-menu-tuner.exe is in the same directory as directory-emptier.exe. The created keys live in **"\HKEY_CLASSES_ROOT\Directory\shell"**.

They all have this structure :
* random_key_name (KEY) :
  * (default) (REG_SZ) : name of the context menu entry
  * icon (REG_SZ) : "path\to\directory-emptier.exe",0
  * command (KEY) :
    * (default) (REG_SZ) : "path\to\directory-emptier.exe" "option1" "option2" ... "%V"
    
To remember the entries it created, context-menu-tuner will save it in a file named "regentries". This software needs root privileges to work.

## Fix

If the file "regentries" is accidently deleted or any issue with context menu is encountered, user needs to:
1. Go to the registry editor (Windows+R, type "regedit" and then Enter) and delete all subkeys of "\HKEY_CLASSES_ROOT\Directory\shell" which begin with "directory_emptier"
2. Create an empty file named "regentries" (with no extension) in the same directory as "context-menu-tuner.exe" (replace it if it already exists)
3. Now user can use context-menu-tuner to create the entries they need

# Uninstallation

1. Open context-menu-tuner.exe, delete any key and save the changes (or follow step 1 of previous section "Fix").
2. Delete any file you don't want (basically the directory you extracted at install).
