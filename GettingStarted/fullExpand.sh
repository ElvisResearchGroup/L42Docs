rm index.xhtml
rm note1.xhtml
rm S.xhtml
rm Decorators.xhtml
m4 -P index.c > index.xhtml
m4 -P tutorial.c > tutorial.xhtml
#gcc -E -x c -P -C -ansi c99 index.c -o fileAux1.xhtml
#sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
#mv fileAux2.xhtml index.xhtml
#gcc -ansi c99 -E -CC note1.c -o fileAux1.xhtml
#sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
#mv fileAux2.xhtml note1.xhtml
#gcc -ansi c99 -E -CC S.c -o fileAux1.xhtml
#sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
#mv fileAux2.xhtml S.xhtml
#gcc -ansi c99 -E -CC Decorators.c -o fileAux1.xhtml
#sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
#mv fileAux2.xhtml Decorators.xhtml


