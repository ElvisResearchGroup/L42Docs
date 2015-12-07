cmd /c C:\MinGW\set_distro_paths.bat 
del index.xhtml
del note1.xhtml
del S.xhtml
del Decorators.xhtml
C:\MinGW\bin\gcc -E -CC index.c -o fileAux1.xhtml
bash _expand.sh
ren fileAux2.xhtml index.xhtml
C:\MinGW\bin\gcc -E -CC note1.c -o fileAux1.xhtml
bash _expand.sh
ren fileAux2.xhtml note1.xhtml
C:\MinGW\bin\gcc -E -CC S.c -o fileAux1.xhtml
bash _expand.sh
ren fileAux2.xhtml S.xhtml
C:\MinGW\bin\gcc -E -CC Decorators.c -o fileAux1.xhtml
bash _expand.sh
ren fileAux2.xhtml Decorators.xhtml
pause