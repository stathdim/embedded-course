gcc -std=gnu99 main_with.c -o main_with &&
gcc -std=gnu99 main_without.c -o main_without &&
./main_with 5 0.1 &&
./main_without 5 0.1