#include <iostream>

using namespace std;

#include <SocketServer.h>

/**
 * Main test function
 **/
int main(void)
{
   //Create a socket Server
   SocketServer server;
   server.Initialize( 9000 );

   cout << "Starting"<<endl;
   server.Start();

   std::this_thread::sleep_for(std::chrono::milliseconds(2000));

   cout << "Done"<<endl;

   server.Stop();
   server.Join();
}
