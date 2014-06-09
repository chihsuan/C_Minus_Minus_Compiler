int main ( ) {
		int d ;
		int a ;
		int b ; 
		int c ; 
		int e [ 4 ] ;
		int f [ 3 ] ;
		char ch ; 
        a = 0 ;
        b = 1 ;
        c = 4 ;
        ch = 'a' ;

        while ( a < 20 ) {

                if ( a < 0 ) {
                        a = a + ( 4 + 5 ) ;
                         break ;
                }
                else {
                        a = a + 1 ;
                }
        }

        a = a  + 1 ;
        f [ 1 ] = 5 ;
        f [ 2 ] = 3 ;
        f [ 1 ] = f [ 1 ] + f [ 2 ] ;

		a = ! a ; 
        return 0 ;
}

