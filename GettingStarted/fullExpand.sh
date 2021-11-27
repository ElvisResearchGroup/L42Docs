m4 -P index.c > index.xhtml
m4 -P tutorial.c > tutorial.xhtml
m4 -P tutorial_01Basics.c > tutorial_01Basics.xhtml
m4 -P tutorial_02ModifiersIntro.c > tutorial_02ModifiersIntro.xhtml
m4 -P tutorial_03BasicClasses.c > tutorial_03BasicClasses.xhtml
m4 -P tutorial_04ErrorsAndExceptions.c > tutorial_04ErrorsAndExceptions.xhtml
m4 -P tutorial_05Caching.c > tutorial_05Caching.xhtml
m4 -P tutorial_06CachingMut.c > tutorial_06CachingMut.xhtml
m4 -P tutorial_07InterfacesAndMatching.c > tutorial_07InterfacesAndMatching.xhtml
m4 -P tutorial_08Sequences.c > tutorial_08Sequences.xhtml
m4 -P tutorial_09InputOutput.c > tutorial_09InputOutput.xhtml
m4 -P tutorial_10Exercises.c > tutorial_10Exercises.xhtml
m4 -P tutorial_11MetaprogrammingIntro.c > tutorial_11MetaprogrammingIntro.xhtml
m4 -P tutorial_12Refactoring.c > tutorial_12Refactoring.xhtml
m4 -P tutorial_13MoreDecorators.c > tutorial_13MoreDecorators.xhtml
m4 -P tutorial_14ExampleOfProgram.c > tutorial_14ExampleOfProgram.xhtml
m4 -P tutorial_15DeployCode.c > tutorial_15DeployCode.xhtml


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


