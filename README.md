# directory-emptier
A software enabling the user to empty directories directly from context menu.

This software is available for Windows in a French-only version right now. It does not include administrator rights so you can't harm your system with it (with this software you could delete any directory from your disk).

## Usage

```
directory-emptier.exe {OPTIONS} [DIRECTORIES]
```
Moves the contents of DIRECTORIES into their parent directories and deletes them.

### OPTIONS:
* -r --recursive : empty recursively (aka removes any directory tree to keep only files)
* -c --conservative : let the original directory unchanged
* -b --behaviour BEHAVIOUR : specify behaviour
* -h --help : show a quick help and quit after 1min
### BEHAVIOURS:
Three char string defining behaviour in case of conflict :
* first char is about default behaviour (letters available : i c n r)
* second char is about directory-directory conflicts (letters available : i m c n r)
* last char is about conflicts with origin directory (letters available : i n r)
### letter code:
**i** : ask user  
**c** : crush  
**n** : do nothing  
**r** : rename  
**m** : merge  

# context-menu-tuner

A software which edits Windows registry to create custom context menu entries related with directory-emptier.

It enables the user to select names and options of the context menu entries that will run directory-emptier.exe. The created keys live in **"\HKEY_CLASSES_ROOT\Directory\shell"**. It won't directly read the registry but save its custom entries in a file named "regentries". This software needs administrators privileges to work.

## fix

If the file "regentries" is accidently deleted or any issue with context menu is encountered, user needs to:
* Go to the registry editor (Windows+R, type "regedit" and then Enter) and delete all subkeys of "\HKEY_CLASSES_ROOT\Directory\shell" which begin with "directory_emptier"
* Create an empty file named "regentries" (with no extension) in the same directory as "context-menu-tuner.exe" (replace it if it already exists)
* Now user can use context-menu-tuner to create the entries they need
