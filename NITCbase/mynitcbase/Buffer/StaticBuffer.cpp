#include "StaticBuffer.h"
#include <string.h>


// the declarations for this class can be found at "StaticBuffer.h"

unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];

StaticBuffer::StaticBuffer() {
    int bufferIndex=0;
  // initialise all blocks as free
  /*bufferIndex = 0 to BUFFER_CAPACITY-1*/
  for (bufferIndex; bufferIndex<BUFFER_CAPACITY;bufferIndex++) {
    metainfo[bufferIndex].free = true;
    metainfo[bufferIndex].dirty = false;
    metainfo[bufferIndex].blockNum = -1;
    metainfo[bufferIndex].timeStamp = -1;
  }
}

/*
At this stage, we are not writing back from the buffer to the disk since we are
not modifying the buffer. So, we will define an empty destructor for now. In
subsequent stages, we will implement the write-back functionality here.
*/
//StaticBuffer::~StaticBuffer() {}

StaticBuffer::~StaticBuffer() {
  /*iterate through all the buffer blocks,
    write back blocks with metainfo as free=false,dirty=true
    using Disk::writeBlock()
    */

   int bufferIndex=0;
   for(bufferIndex;bufferIndex<BUFFER_CAPACITY;bufferIndex++){
    if(metainfo[bufferIndex].free ==false && metainfo[bufferIndex].dirty == true){
      Disk::writeBlock(blocks[bufferIndex],metainfo[bufferIndex].blockNum);
    }
   }

}

// int StaticBuffer::getFreeBuffer(int blockNum) {
//   if (blockNum < 0 || blockNum > DISK_BLOCKS) {
//     return E_OUTOFBOUND;
//   }
//   int allocatedBuffer;

//   // iterate through all the blocks in the StaticBuffer
//   // find the first free block in the buffer (check metainfo)
//   // assign allocatedBuffer = index of the free block

//   for(int i=0;i<BUFFER_CAPACITY;i++){
//     if(metainfo[i].free==true){
//         allocatedBuffer =i;
//         break;
//     }
//   }

//   metainfo[allocatedBuffer].free = false;
//   metainfo[allocatedBuffer].blockNum = blockNum;

//   return allocatedBuffer;
// }

int StaticBuffer::getFreeBuffer(int blockNum) {
   // Check if blockNum is valid (non zero and less than DISK_BLOCKS)
    // and return E_OUTOFBOUND if not valid
  if (blockNum < 0 || blockNum >= DISK_BLOCKS) {
    return E_OUTOFBOUND;
  }
// increase the timeStamp in metaInfo of all occupied buffers.

  for(int i=0;i<BUFFER_CAPACITY;i++){
    if(metainfo[i].free == false){
      metainfo[i].timeStamp++;
    }
  }


// let bufferNum be used to store the buffer number of the free/freed buffer.
  int bufferNum=-1;

  // iterate through all the blocks in the StaticBuffer
  // find the first free block in the buffer (check metainfo)
  // assign allocatedBuffer = index of the free block
  for(int Index=0;Index<BUFFER_CAPACITY;Index++){
    if(metainfo[Index].free==true){
        bufferNum = Index;
        break;
    }
  }
  // if a free buffer is not available,
    //     find the buffer with the largest timestamp
    //     IF IT IS DIRTY, write back to the disk using Disk::writeBlock()
    //     set bufferNum = index of this buffer

  if (bufferNum == -1) {
    int max=0;
    int index=-1;
    for(int i=0;i<BUFFER_CAPACITY;i++){
      if(metainfo[i].timeStamp>=max){
        max = metainfo[i].timeStamp;
        index=i;
      }
    }
    if(metainfo[index].dirty==true){
      Disk::writeBlock(blocks[index],metainfo[index].blockNum);
      bufferNum = index;
    }

  }
  // update the metaInfo entry corresponding to bufferNum with
    // free:false, dirty:false, blockNum:the input block number, timeStamp:0.
  metainfo[bufferNum].free = false;
  metainfo[bufferNum].blockNum = blockNum;

  return bufferNum;
}



/* Get the buffer index where a particular block is stored
   or E_BLOCKNOTINBUFFER otherwise
*/
int StaticBuffer::getBufferNum(int blockNum) {
  // Check if blockNum is valid (between zero and DISK_BLOCKS)
  // and return E_OUTOFBOUND if not valid.
 if (blockNum < 0 || blockNum >= DISK_BLOCKS) {
    return E_OUTOFBOUND;
  }
  // find and return the bufferIndex which corresponds to blockNum (check metainfo)
  for (int bufferIndex = 0; bufferIndex < BUFFER_CAPACITY; bufferIndex++) {
    if (metainfo[bufferIndex].free == false &&
        metainfo[bufferIndex].blockNum == blockNum) {
      return bufferIndex;
    }
  }
  // if block is not in the buffer
  return E_BLOCKNOTINBUFFER;
}

int StaticBuffer::setDirtyBit(int blockNum){
    // find the buffer index corresponding to the block using getBufferNum().
    int bufferIndex = StaticBuffer::getBufferNum(blockNum);
    // if block is not present in the buffer (bufferNum = E_BLOCKNOTINBUFFER)
    //     return E_BLOCKNOTINBUFFER
    if(bufferIndex == E_BLOCKNOTINBUFFER){
      return E_BLOCKNOTINBUFFER;
    }
    // if blockNum is out of bound (bufferNum = E_OUTOFBOUND)
    //     return E_OUTOFBOUND
    if(bufferIndex == E_OUTOFBOUND){
      return E_OUTOFBOUND;
    }else{
      metainfo[bufferIndex].dirty = true;
    }
    // else
    //     (the bufferNum is valid)
    //     set the dirty bit of that buffer to true in metainfo

    // return SUCCESS
    return SUCCESS;
}
