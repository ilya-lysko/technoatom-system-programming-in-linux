#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main ()
{
 int f;
 struct flock lock;
 
 if ((f = open("test.txt", O_RDWR)) == -1) {
  printf("error in open!\n");
  return 0;
 }

 memset(&lock, 0, sizeof(lock));
 lock.l_type = F_WRLCK;
 fcntl(f, F_SETLKW, &lock);

 char buf[32];
 int readlen;
 lseek(f, -3, SEEK_END);
 if ((readlen = read(f, buf, 32)) == -1) {
  printf("error in read!\n");
  return 0;
 }
 buf[31] = '\0';
 int val = atoi(buf) + 1;
 printf("%d\n", val);

 char s[sizeof(val)]; 
 sprintf(s,"%d", val);
 
 lseek(f,0,SEEK_END);
 if (write(f, s, sizeof(s) - 2) == -1) {
  printf("error in write!\n");
  return 0;
 }
 write(f, "\n", 1);

 lock.l_type = F_UNLCK;
 fcntl(f, F_SETLKW, &lock);

 close(f);
 return 0;
}
