int main( int argc, char **argv ) {
    int n = 0;
    SRunner *sr = srunner_create( make_spx_smart_test( ) );
    srunner_run_all( sr, CK_NORMAL );
    n = srunner_ntests_failed( sr );
    srunner_free( sr );
    return ( n == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}