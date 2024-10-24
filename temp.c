#include <stdio.h>
#include <string.h>
int main(){
    char n[20];
    fgets(n, sizeof(n), stdin);
    printf("%d", strlen(n));
    return 0;
}