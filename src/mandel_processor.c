#include "types.h"
#include "mandel.h"
#include "mandel_processor.h"

#include <gmp.h>

int process_point(mpf_t cx,mpf_t cy, int er, int bailout){
    mpf_t zx , zy , zxn, zyn,
          zx2, zy2, t1;
    int   i  , k;

    mpf_init  ( zx      ) ;
    mpf_init  ( zy      ) ;
    mpf_init  ( t1      ) ;
    mpf_init  ( zxn     ) ;
    mpf_init  ( zyn     ) ;
    mpf_init  ( zx2     ) ;
    mpf_init  ( zy2     ) ;

    mpf_set_d ( zx, 0.0 ) ;
    mpf_set_d ( zy, 0.0 ) ;

    for (i = 0; i < bailout; ++i) {
        mpf_mul    ( zx2, zx , zx  ) ;
        mpf_mul    ( zy2, zy , zy  ) ;
        mpf_sub    ( zxn, zx2, zy2 ) ;
        mpf_add    ( zxn, zxn, cx  ) ;

        mpf_mul_ui ( zyn, zx , 2   ) ;
        mpf_mul    ( zyn, zyn, zy  ) ;
        mpf_add    ( zyn, zyn, cy  ) ;

        mpf_set    ( zx , zxn      ) ;
        mpf_set    ( zy , zyn      ) ;

        mpf_add    ( t1 , zx2, zy2 ) ;

        k = mpf_cmp_d( t1, er * er ) ;

        if( k > 0 )
            break;
    }

    mpf_clear( zx  );
    mpf_clear( zy  );
    mpf_clear( t1  );
    mpf_clear( zxn );
    mpf_clear( zyn );
    mpf_clear( zx2 );
    mpf_clear( zy2 );

    return i;
}
