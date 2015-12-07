rm index.xhtml
rm note1.xhtml
rm S.xhtml
rm Decorators.xhtml
gcc -E -CC index.c -o fileAux1.xhtml
sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
mv fileAux2.xhtml index.xhtml
gcc -E -CC note1.c -o fileAux1.xhtml
sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
mv fileAux2.xhtml note1.xhtml
gcc -E -CC S.c -o fileAux1.xhtml
sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
mv fileAux2.xhtml S.xhtml
gcc -E -CC Decorators.c -o fileAux1.xhtml
sed '/^#/ d' < fileAux1.xhtml > fileAux2.xhtml
mv fileAux2.xhtml Decorators.xhtml


