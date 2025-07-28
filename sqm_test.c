
int int main(int argc, char const *argv[]) {
    unsigned int x[3], i;
    x[0] = 1000970 ;
    x[1] = 100043 ;
    x[2] =  20021 ;

    for ( i = 0 ; i < 15000 ; i++){
        sqm(x[j],);

    }
    return 0;
}







unsigned int sqm(unsigned int x, unsigned int n, unsigned int m){
   unsigned  int c,k,i=0;
   int r[100];
   c=n;
   do
   {
       r[i]=c%2;
       c=c/2;
       i++;
   }while(c!=0);
   k=--i;
   return samalgo(k,x,m,r);
}
