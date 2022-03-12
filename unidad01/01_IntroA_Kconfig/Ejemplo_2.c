#include <stdio.h>         /* comment */
#ifdef CONFIG_EJEMPLO__II
int main(void)
{
 int a,b;

 printf("Enter two numbers: ");
 scanf("%i %i",&a,&b);

 printf("%i - %i = %i\n",a,b,a-b);

 return 0;
}/*end main()*/
#endif
