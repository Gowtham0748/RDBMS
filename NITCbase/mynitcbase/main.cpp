#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
  /* Initialize the Run Copy of Disk */
  Disk disk_run;
  // reading blocks and and printing its values
  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, 0);
  for (int i = 0; i < 10; i++)
  {
    std::cout << (int)buffer[i] << " ";
  }
  std::cout << "\n";

  return 0;
}

/*using this we can rad from a block and we can write back to it*/
// unsigned char buffer[BLOCK_SIZE];
// char message[] = "hello";
// memcpy(buffer + 20, message, 6);
// Disk::writeBlock(buffer, 7000);

// unsigned char buffer2[BLOCK_SIZE];
// char message2[6];
// Disk::readBlock(buffer2, 7000);
// memcpy(message2, buffer2 + 20, 6);
// std::cout << message2 << std::endl;
