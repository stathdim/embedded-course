gcc -std=gnu99 main_with.c -o main_with &&
gcc -std=gnu99 main_without.c -o main_without &&
./main_with 7200 0.1 &&
./main_without 7200 0.1
