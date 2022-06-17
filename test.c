#include <string.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
    char *test = "abc,efg,txt";
    char *p;
    p = strtok(test, ",");
    while (p)
    {
        printf("%s\n", p);
        p = strtok(NULL, ",");
    }

    
    return 0;
}


#include<string.h>
#include<stdio.h>
int main()
{
      char test[] = "feng,ke,wei";
      char *p;
      char filename[128];

      p = strtok(test, ",");
      while(p)
          {
              bzero(filename,128);
              
              printf("%s\n", p);
              p = strtok(NULL, ",");
          }
      return 0;
 }