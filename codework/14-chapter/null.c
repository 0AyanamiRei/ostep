# include "/home/refrain/ostep/codework/whatUneed.h"


int
main()
{
    int *data = (int*)malloc(100*sizeof(int));
    free(data);
    printf("%d\n", data[2]);
    exit(0);
}