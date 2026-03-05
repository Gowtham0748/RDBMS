#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <cstring>
#include <iostream>


/*<----------------Stage 4---------------->*/

int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  return FrontendInterface::handleFrontend(argc, argv);
}









/*<----------------------stage 3 code----------------------->*/
// int main(){
//     Disk disk_run;
//     StaticBuffer buffer;
//     OpenRelTable cache;

//     RelCatEntry relCatEntry;
//     AttrCatEntry attrCatEntry;

//     for(int i=0;i<=1; i++){
//         RelCacheTable::getRelCatEntry(i, &relCatEntry);

//         printf("Relation: %s\n",relCatEntry.relName);

//         for(int j=0; j< relCatEntry.numAttrs;j++){
//             AttrCacheTable:: getAttrCatEntry(i,j,&attrCatEntry);

//             printf(" %s: ",attrCatEntry.attrName);

//             if(attrCatEntry.attrType == NUMBER){
//                 printf("NUM\n");
//             }else if(attrCatEntry.attrType == STRING){
//                 printf("STR\n");
//             }
//         }
//         printf("\n");
//     }

//     return 0;
// }








/*<----------------stage 2 exercises-------------------->*/

// //when multiple attribute blocks present, linked as a ll
// //stage 2 exercises and example
// int main(int argc, char *argv[]) {
//     Disk disk_run;
//     StaticBuffer buffer;

//     RecBuffer relCatBuffer(RELCAT_BLOCK);
//     HeadInfo relCatHeader;
//     relCatBuffer.getHeader(&relCatHeader);

//     for (int i = 0; i < relCatHeader.numEntries; i++) {

//         Attribute relCatRecord[RELCAT_NO_ATTRS];
//         relCatBuffer.getRecord(relCatRecord, i);

//         printf("Relation: %s\n",
//                relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

//         // ---- Attribute catalog traversal (MULTI-BLOCK) ----
//         int currBlock = ATTRCAT_BLOCK;

//         while (currBlock != -1) {

//             RecBuffer attrCatBuffer(currBlock);
//             HeadInfo attrHead;
//             attrCatBuffer.getHeader(&attrHead);

//             for (int j = 0; j < attrHead.numEntries; j++) {

//                 Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
//                 attrCatBuffer.getRecord(attrCatRecord, j);

//                 if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {

//                     const char *attrType =
//                         (attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER)
//                         ? "NUM"
//                         : "STR";

//                     printf("  %s: %s\n",attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,attrType);
//                 }
//             }

//             // move to next attribute catalog block
//             currBlock = attrHead.rblock;
//         }

//         printf("\n");
//     }

//     return 0;
// }









// //printing relational catalog and attribute catalog, attributes and its types
// int main(int argc, char *argv[]) {
//   Disk disk_run;

//   // create objects for the relation catalog and attribute catalog
//   RecBuffer relCatBuffer(RELCAT_BLOCK);
//   RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

//   HeadInfo relCatHeader;
//   HeadInfo attrCatHeader;

//   // load the headers of both the blocks into relCatHeader and attrCatHeader.
//   // (we will implement these functions later)
//   relCatBuffer.getHeader(&relCatHeader);
//   attrCatBuffer.getHeader(&attrCatHeader);

//   for (int i=0; i< relCatHeader.numEntries;i++/* i = 0 to total relation count */) {

//     Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

//     relCatBuffer.getRecord(relCatRecord, i);

//     printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

//     for (int j=0;j<attrCatHeader.numEntries;j++/* j = 0 to number of entries in the attribute catalog */) {

//       // declare attrCatRecord and load the attribute catalog entry into it
//       Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
//       attrCatBuffer.getRecord(attrCatRecord,j);

//       if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal)==0/* attribute catalog entry corresponds to the current relation */) {
//         const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
//         printf("  %s: %s\n", /* get the attribute name */attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
//       }
//     }
//     printf("\n");
//   }

//   return 0;
// }




/*<---------------stage 1--------------------->*/
// int main(int argc, char *argv[])
// {
//   /* Initialize the Run Copy of Disk */
//   Disk disk_run;
//   // reading blocks and and printing its values
//   unsigned char buffer[BLOCK_SIZE];
//   Disk::readBlock(buffer, 0);
//   for (int i = 0; i < 10; i++)
//   {
//     std::cout << (int)buffer[i] << " ";
//   }
//   std::cout << "\n";

//   return 0;
// }

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
