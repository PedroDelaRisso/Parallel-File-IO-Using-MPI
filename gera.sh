rm dna.txt;
touch dna.txt;
mpicc gera.c -o gera.o;
mpirun gera.o;