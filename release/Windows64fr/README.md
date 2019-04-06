# context-menu-tuner

A software which edits Windows registry to create custom context menu entries related with directory-emptier.

It enables the user to select names and options of the context menu entries that will run directory-emptier.exe. This will only work if context-menu-tuner.exe is in the same directory as directory-emptier.exe. The created keys live in **"\HKEY_CLASSES_ROOT\Directory\shell"**. It won't directly read the registry but save its custom entries in a file named "regentries". This software needs administrators privileges to work.

## fix

If the file "regentries" is accidently deleted or any issue with context menu is encountered, user needs to:
1. Go to the registry editor (Windows+R, type "regedit" and then Enter) and delete all subkeys of "\HKEY_CLASSES_ROOT\Directory\shell" which begin with "directory_emptier"
2. Create an empty file named "regentries" (with no extension) in the same directory as "context-menu-tuner.exe" (replace it if it already exists)
3. Now user can use context-menu-tuner to create the entries they need
