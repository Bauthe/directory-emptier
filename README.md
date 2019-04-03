# directory-emptier
A software enabling the user to empty directories directly from context menu

This software is available for windows right now

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
#### BEHAVIOURS:
Three char string defining behaviour in case of conflict :
* first char is about default behaviour (letters available : i c n r)
* second char is about directory-directory conflicts (letters available : i m c n r)
* last char is about conflicts with origin directory (letters available : i n r)
##### letter code:
**i** : ask user  
**c** : crush  
**n** : do nothing  
**r** : rename  
**m** : merge  

# context-menu-tuner

A software which edits windows registry to create custom context menu entries related with directory-emptier.

It enables the user to select names and options of the context menu entries that will run directory-emptier.exe.
