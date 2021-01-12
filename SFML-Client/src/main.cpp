#include <iostream>

/*For now @main.cpp is for programs main loop and handling time. Might change later.*/

#include "render.h"
#include "network.h"
#include "actor.h"

/*Fancy @windows.h float calculating time*/
static float time_since( LARGE_INTEGER t, LARGE_INTEGER frequency )
{
   LARGE_INTEGER now;
   QueryPerformanceCounter( &now );

   return float( now.QuadPart - t.QuadPart ) / float( frequency.QuadPart );
}


int main()
{
    //Initialization and error handling.
    if(rdr::init_sfml())
        std::cout << "SFML 2.5 was initialized successfully.\n";
    else
        std::cout << "SFML was unable to start.\n";
    if(net::init_network()){
        std::cout << "Client connected at: " << net::GetIPa() <<".\n";
    }
    else
        std::cout << "Client unable to connect to: " << net::GetIPa() <<".\n";

    act::CreateActor(5,5);


    /*Declaring time variables for managing connection to server*/
    UINT sleep_granularity_ms = 1;
    bool sleep_granularity_was_set = timeBeginPeriod( sleep_granularity_ms ) == TIMERR_NOERROR;
    LARGE_INTEGER clock_frequency;
    QueryPerformanceFrequency( &clock_frequency );
    float SECONDS_PER_TICK = 1.0/60.0; // 60Hz, 1 tick = 0.166666 sec


    //Main loop
    while(true){
        /*Start of measuring tick time*/
        LARGE_INTEGER tick_start_time;
        QueryPerformanceCounter( &tick_start_time );

        rdr::keep_window(); /**/
        net::networking_loop(); /*Listening to server socket*/

        if(rdr::rendering_loop() == 0)break;

        /*End of measuring tick time*/
        /*
        Basically if the time of processing data was longer than SECONDS_PER_TICK
        program will wait for calculated time to gain synchronization and stability
        in connection with server.
        */
        float time_taken_s = time_since( tick_start_time, clock_frequency );
        while( time_taken_s < SECONDS_PER_TICK )
        {
            if( sleep_granularity_was_set )
            {
            DWORD time_to_wait_ms = DWORD( ( SECONDS_PER_TICK - time_taken_s ) * 1000 );
            if( time_to_wait_ms > 0 )
            {
                Sleep( time_to_wait_ms );
            }
            }
        time_taken_s = time_since( tick_start_time, clock_frequency );
        }

    }

    return 0;
}
