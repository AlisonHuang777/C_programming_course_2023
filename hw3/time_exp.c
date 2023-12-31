#include <stdio.h>
#include <time.h>

int main() {
    time_t t = time(NULL); //get time_t
    fputs(asctime(localtime(&t)), stdout); //time_t -> struct tm -> string -> output (local,    UTC+8)
    fputs(  ctime(          &t ), stdout); //time_t              -> string -> output (local,    UTC+8)
    fputs(asctime(   gmtime(&t)), stdout); //time_t -> struct tm -> string -> output (standard, UTC+0)
}