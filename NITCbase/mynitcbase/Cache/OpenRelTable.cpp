#include "OpenRelTable.h"
#include<stdlib.h>
#include <cstring>


OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];

OpenRelTable::OpenRelTable() {

  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
  }
  for (int i = 0; i < MAX_OPEN; ++i) {
    tableMetaInfo[i].free = true;
    tableMetaInfo[i].relName[0] = '\0';
  }

  tableMetaInfo[RELCAT_RELID].free = false;
  strcpy(tableMetaInfo[RELCAT_RELID].relName, RELCAT_RELNAME);

  tableMetaInfo[ATTRCAT_RELID].free = false;
  strcpy(tableMetaInfo[ATTRCAT_RELID].relName, ATTRCAT_RELNAME);



  /************ Setting up Relation Cache entries ************/
  // (we need to populate relation cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Relation Cache Table****/
  RecBuffer relCatBlock(RELCAT_BLOCK);

  Attribute relCatRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

  struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;

  /**** setting up Attribute Catalog relation in the Relation Cache Table ****/

  relCatBlock.getRecord(relCatRecord,RELCAT_SLOTNUM_FOR_ATTRCAT);
  // set up the relation cache entry for the attribute catalog similarly
  // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT

  RelCacheEntry attrRelCacheEntry;
  RelCacheTable:: recordToRelCatEntry(relCatRecord,&attrRelCacheEntry.relCatEntry);
  // set the value at RelCacheTable::relCache[ATTRCAT_RELID]


  attrRelCacheEntry.dirty = false;
  attrRelCacheEntry.searchIndex.block = -1;
  attrRelCacheEntry.searchIndex.slot  = -1;

  attrRelCacheEntry.recId.block = RELCAT_BLOCK;
  attrRelCacheEntry.recId.slot  = RELCAT_SLOTNUM_FOR_ATTRCAT;

  RelCacheTable::relCache[ATTRCAT_RELID] =
      (RelCacheEntry*) malloc(sizeof(RelCacheEntry));

  *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrRelCacheEntry;
  /************ Setting up Attribute cache entries ************/
  // (we need to populate attribute cache with entries for the relation catalog
  //  and attribute catalog.)

  /**** setting up Relation Catalog relation in the Attribute Cache Table ****/
  RecBuffer attrCatBlock(ATTRCAT_BLOCK);

  Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

  AttrCacheEntry *head = nullptr;
  AttrCacheEntry *prev = nullptr;


  // iterate through all the attributes of the relation catalog and create a linked
  // list of AttrCacheEntry (slots 0 to 5)
  // for each of the entries, set
  //    attrCacheEntry.recId.block = ATTRCAT_BLOCK;
  //    attrCacheEntry.recId.slot = i   (0 to 5)
  //    and attrCacheEntry.next appropriately
  // NOTE: allocate each entry dynamically using malloc

   // slots 0 to 5 → attributes of RELCAT
  for (int i = 0; i < RELCAT_NO_ATTRS; i++) {

    attrCatBlock.getRecord(attrCatRecord, i);

    AttrCacheEntry *entry =(AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

    AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&entry->attrCatEntry);

    entry->dirty = false;
    entry->searchIndex.block = -1;
    entry->searchIndex.index = -1;

    entry->recId.block = ATTRCAT_BLOCK;
    entry->recId.slot  = i;

    entry->next = nullptr;

    if (head == nullptr)
      head = entry;
    else
      prev->next = entry;

    prev = entry;
  }

  // set the next field in the last entry to nullptr

  AttrCacheTable::attrCache[RELCAT_RELID] = head/* head of the linked list */;

  /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/
  head = nullptr;
  prev = nullptr;
  // set up the attributes of the attribute cache similarly.
  // read slots 6-11 from attrCatBlock and initialise recId appropriately
 // slots 6 to 11 → attributes of ATTRCAT
  for (int i = RELCAT_NO_ATTRS; i < RELCAT_NO_ATTRS + ATTRCAT_NO_ATTRS; i++) {

    attrCatBlock.getRecord(attrCatRecord, i);

    AttrCacheEntry *entry =(AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

    AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&entry->attrCatEntry);

    entry->dirty = false;
    entry->searchIndex.block = -1;
    entry->searchIndex.index = -1;

    entry->recId.block = ATTRCAT_BLOCK;
    entry->recId.slot  = i;

    entry->next = nullptr;

    if (head == nullptr)
      head = entry;
    else
      prev->next = entry;

    prev = entry;
  }
  // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
  AttrCacheTable :: attrCache[ATTRCAT_RELID] = head;

}


OpenRelTable::~OpenRelTable() {
    for (int i = 0; i < MAX_OPEN; i++) {

    if (RelCacheTable::relCache[i] != nullptr) {
      free(RelCacheTable::relCache[i]);
      RelCacheTable::relCache[i] = nullptr;
    }

    AttrCacheEntry *curr = AttrCacheTable::attrCache[i];
    while (curr != nullptr) {
      AttrCacheEntry *next = curr->next;
      free(curr);
      curr = next;
    }
    AttrCacheTable::attrCache[i] = nullptr;
    tableMetaInfo[i].free = true;
    tableMetaInfo[i].relName[0] = '\0';
  }

  // free all the memory that you allocated in the constructor
}


// OpenRelTable::OpenRelTable() {

//   // initialize relCache and attrCache with nullptr
//   for (int i = 0; i < MAX_OPEN; ++i) {
//     RelCacheTable::relCache[i] = nullptr;
//     AttrCacheTable::attrCache[i] = nullptr;
//   }

//   /************ Setting up Relation Cache entries ************/
//   // (we need to populate relation cache with entries for the relation catalog
//   //  and attribute catalog.)

//   /**** setting up Relation Catalog relation in the Relation Cache Table****/
//   RecBuffer relCatBlock(RELCAT_BLOCK);

//   Attribute relCatRecord[RELCAT_NO_ATTRS];
//   relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

//   struct RelCacheEntry relCacheEntry;
//   RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block = RELCAT_BLOCK;
//   relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;

//   /**** setting up Attribute Catalog relation in the Relation Cache Table ****/

//   relCatBlock.getRecord(relCatRecord,RELCAT_SLOTNUM_FOR_ATTRCAT);
//   // set up the relation cache entry for the attribute catalog similarly
//   // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT

//   RelCacheEntry attrRelCacheEntry;
//   RelCacheTable:: recordToRelCatEntry(relCatRecord,&attrRelCacheEntry.relCatEntry);
//   // set the value at RelCacheTable::relCache[ATTRCAT_RELID]


//   attrRelCacheEntry.dirty = false;
//   attrRelCacheEntry.searchIndex.block = -1;
//   attrRelCacheEntry.searchIndex.slot  = -1;

//   attrRelCacheEntry.recId.block = RELCAT_BLOCK;
//   attrRelCacheEntry.recId.slot  = RELCAT_SLOTNUM_FOR_ATTRCAT;

//   RelCacheTable::relCache[ATTRCAT_RELID] =
//       (RelCacheEntry*) malloc(sizeof(RelCacheEntry));

//   *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrRelCacheEntry;
//   /************ Setting up Attribute cache entries ************/
//   // (we need to populate attribute cache with entries for the relation catalog
//   //  and attribute catalog.)

//   /**** setting up Relation Catalog relation in the Attribute Cache Table ****/
//   RecBuffer attrCatBlock(ATTRCAT_BLOCK);

//   Attribute attrCatRecord[ATTRCAT_NO_ATTRS];

//   AttrCacheEntry *head = nullptr;
//   AttrCacheEntry *prev = nullptr;


//   // iterate through all the attributes of the relation catalog and create a linked
//   // list of AttrCacheEntry (slots 0 to 5)
//   // for each of the entries, set
//   //    attrCacheEntry.recId.block = ATTRCAT_BLOCK;
//   //    attrCacheEntry.recId.slot = i   (0 to 5)
//   //    and attrCacheEntry.next appropriately
//   // NOTE: allocate each entry dynamically using malloc

//    // slots 0 to 5 → attributes of RELCAT
//   for (int i = 0; i < RELCAT_NO_ATTRS; i++) {

//     attrCatBlock.getRecord(attrCatRecord, i);

//     AttrCacheEntry *entry =(AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&entry->attrCatEntry);

//     entry->dirty = false;
//     entry->searchIndex.block = -1;
//     entry->searchIndex.index = -1;

//     entry->recId.block = ATTRCAT_BLOCK;
//     entry->recId.slot  = i;

//     entry->next = nullptr;

//     if (head == nullptr)
//       head = entry;
//     else
//       prev->next = entry;

//     prev = entry;
//   }

//   // set the next field in the last entry to nullptr

//   AttrCacheTable::attrCache[RELCAT_RELID] = head/* head of the linked list */;

//   /**** setting up Attribute Catalog relation in the Attribute Cache Table ****/
//   head = nullptr;
//   prev = nullptr;
//   // set up the attributes of the attribute cache similarly.
//   // read slots 6-11 from attrCatBlock and initialise recId appropriately
//  // slots 6 to 11 → attributes of ATTRCAT
//   for (int i = RELCAT_NO_ATTRS; i < RELCAT_NO_ATTRS + ATTRCAT_NO_ATTRS; i++) {

//     attrCatBlock.getRecord(attrCatRecord, i);

//     AttrCacheEntry *entry =(AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

//     AttrCacheTable::recordToAttrCatEntry(attrCatRecord,&entry->attrCatEntry);

//     entry->dirty = false;
//     entry->searchIndex.block = -1;
//     entry->searchIndex.index = -1;

//     entry->recId.block = ATTRCAT_BLOCK;
//     entry->recId.slot  = i;

//     entry->next = nullptr;

//     if (head == nullptr)
//       head = entry;
//     else
//       prev->next = entry;

//     prev = entry;
//   }
//   // set the value at AttrCacheTable::attrCache[ATTRCAT_RELID]
//   AttrCacheTable :: attrCache[ATTRCAT_RELID] = head;


//   HeadInfo relCatHead;
//   relCatBlock.getHeader(&relCatHead);
//   bool inserted = false;
//   for(int k=0;k<relCatHead.numEntries&&!inserted;k++)
//   { Attribute record[RELCAT_NO_ATTRS];
//     relCatBlock.getRecord(record,k);
//     if(strcmp(record[RELCAT_REL_NAME_INDEX].sVal,"Students")==0)
//     { 
//       for(int i=2;i<MAX_OPEN;i++)
//       {
//         if(RelCacheTable::relCache[i]==nullptr)
//         {
//           struct RelCacheEntry relCacheEntryForStudent;
//           RelCacheTable::recordToRelCatEntry(record,&relCacheEntryForStudent.relCatEntry);
//           relCacheEntryForStudent.recId.block=RELCAT_BLOCK;
//           relCacheEntryForStudent.recId.slot=k;
//           RelCacheTable::relCache[i]=(struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//           *(RelCacheTable::relCache[i])=relCacheEntryForStudent;
  
  
//           AttrCacheTable::attrCache[i]=nullptr;
//           AttrCacheEntry* relPtr=nullptr;
//           HeadInfo attrCatHead;
//           attrCatBlock.getHeader(&attrCatHead);
//           for(int j=0;j<attrCatHead.numEntries;j++)
//           {
//             Attribute catRecord[ATTRCAT_NO_ATTRS];
//             attrCatBlock.getRecord(catRecord,j);
//             if(strcmp(catRecord[ATTRCAT_REL_NAME_INDEX].sVal,"Students")==0)
//             {
//                 AttrCacheEntry *temp=(struct AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
//                 AttrCacheTable::recordToAttrCatEntry(catRecord,&temp->attrCatEntry);
//                 temp->next=nullptr;
//                 temp->recId.block=ATTRCAT_BLOCK;
//                 temp->recId.slot=j;
//                 if(AttrCacheTable::attrCache[i]==nullptr)
//                 {
//                   AttrCacheTable::attrCache[i]=temp;
//                   relPtr=temp;
//                 }
//                 else{
//                   relPtr->next=temp;
//                   relPtr=temp;
//                 }
//             }
//           }
//           inserted=true;
//           break;
//         }
//       } } }


// }




// OpenRelTable::~OpenRelTable() {
//     for (int i = 0; i < MAX_OPEN; i++) {

//     if (RelCacheTable::relCache[i] != nullptr) {
//       free(RelCacheTable::relCache[i]);
//       RelCacheTable::relCache[i] = nullptr;
//     }

//     AttrCacheEntry *curr = AttrCacheTable::attrCache[i];
//     while (curr != nullptr) {
//       AttrCacheEntry *next = curr->next;
//       free(curr);
//       curr = next;
//     }
//     AttrCacheTable::attrCache[i] = nullptr;
//   }

//   // free all the memory that you allocated in the constructor
// }



/* This function will open a relation having name `relName`.
Since we are currently only working with the relation and attribute catalog, we
will just hardcode it. In subsequent stages, we will loop through all the relations
and open the appropriate one.
*/
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

  // if relname is RELCAT_RELNAME, return RELCAT_RELID
  if(strcmp(relName,RELCAT_RELNAME)==0){
    return RELCAT_RELID;
  }
  // if relname is ATTRCAT_RELNAME, return ATTRCAT_RELID
  if(strcmp(relName,ATTRCAT_RELNAME)==0){
    return ATTRCAT_RELID;
  }

  for (int i = 0; i < MAX_OPEN; i++) {
    
    if (RelCacheTable::relCache[i]!=nullptr && strcmp(RelCacheTable::relCache[i]->relCatEntry.relName, relName) == 0) {
      return i;
    }
  }


  return E_RELNOTOPEN;
}



int OpenRelTable::getFreeOpenRelTableEntry() {
  for (int i = 2; i < MAX_OPEN; ++i) {
    if (tableMetaInfo[i].free) {
      return i;
    }
  }
  return E_CACHEFULL;
}

int OpenRelTable::openRel(char relName[ATTR_SIZE]) {

  /* 1. If already open, return its relId */
  int existingRelId = getRelId((char*)relName);
  if (existingRelId != E_RELNOTOPEN) {
    return existingRelId;
  }

  /* 2. Find a free slot in Open Relation Table */
  int relId = getFreeOpenRelTableEntry();
  if (relId == E_CACHEFULL) {
    return E_CACHEFULL;
  }

  /******** Setting up Relation Cache entry ********/

  /* Reset search index of RELCAT */
  RelCacheTable::resetSearchIndex(RELCAT_RELID);

  /* Search RELCAT for relation name */
  RecId relcatRecId =BlockAccess::linearSearch(RELCAT_RELID,RELCAT_ATTR_RELNAME,*(union Attribute*)relName,EQ);

  if (relcatRecId.block == -1 && relcatRecId.slot == -1) {
    return E_RELNOTEXIST;
  }

  /* Read RELCAT record */
  RecBuffer relCatBlock(relcatRecId.block);
  Attribute relRecord[RELCAT_NO_ATTRS];
  relCatBlock.getRecord(relRecord, relcatRecId.slot);

  /* Create Relation Cache Entry */
  RelCacheEntry *relEntry =
      (RelCacheEntry*) malloc(sizeof(RelCacheEntry));
      

  RelCacheTable::recordToRelCatEntry(
      relRecord, &relEntry->relCatEntry
  );

  relEntry->recId = relcatRecId;
  relEntry->dirty = false;
  relEntry->searchIndex.block = -1;
  relEntry->searchIndex.slot  = -1;

  RelCacheTable::relCache[relId] = relEntry;

  /******** Setting up Attribute Cache entries ********/

  AttrCacheEntry *listHead = nullptr;
  AttrCacheEntry *tail = nullptr;

  /* Reset search index of ATTRCAT */
  RelCacheTable::resetSearchIndex(ATTRCAT_RELID);

  while (true) {

    RecId attrcatRecId =
        BlockAccess::linearSearch(ATTRCAT_RELID,(char*)ATTRCAT_ATTR_RELNAME,*(union Attribute*)relName,
            EQ
        );

    if (attrcatRecId.block == -1 && attrcatRecId.slot == -1) {
      break;  // no more attributes
    }

    RecBuffer attrCatBlock(attrcatRecId.block);
    Attribute attrRecord[ATTRCAT_NO_ATTRS];
    attrCatBlock.getRecord(attrRecord, attrcatRecId.slot);

    AttrCacheEntry *attrEntry =
        (AttrCacheEntry*) malloc(sizeof(AttrCacheEntry));

    AttrCacheTable::recordToAttrCatEntry(
        attrRecord, &attrEntry->attrCatEntry
    );

    attrEntry->recId = attrcatRecId;
    attrEntry->dirty = false;
    attrEntry->searchIndex.block = -1;
    attrEntry->searchIndex.index = -1;
    attrEntry->next = nullptr;

    if (listHead == nullptr) {
      listHead = attrEntry;
      tail = attrEntry;
    } else {
      tail->next = attrEntry;
      tail = attrEntry;
    }
  }

  AttrCacheTable::attrCache[relId] = listHead;

  /******** Update OpenRelTable metadata ********/

  tableMetaInfo[relId].free = false;
  strcpy(tableMetaInfo[relId].relName, (char*)relName);

  return relId;
}



int OpenRelTable::closeRel(int relId) {

  if (relId == RELCAT_RELID || relId == ATTRCAT_RELID) {
    return E_NOTPERMITTED;
  }

  if (relId < 0 || relId >= MAX_OPEN) {
    return E_OUTOFBOUND;
  }

  if (tableMetaInfo[relId].free) {
    return E_RELNOTOPEN;
  }

  /****** Releasing the Relation Cache entry of the relation ******/
/* RelCatEntry of the relId-th Relation Cache entry has been modified */
  if (RelCacheTable::relCache[relId]->dirty ==true)
  {

    /* Get the Relation Catalog entry from RelCacheTable::relCache
    Then convert it to a record using RelCacheTable::relCatEntryToRecord(). */
    union Attribute record[RELCAT_NO_ATTRS];
    RelCatEntry relCatEntry;
    relCatEntry = RelCacheTable::relCache[relId]->relCatEntry;
    RelCacheTable::relCatEntryToRecord(&relCatEntry,record);
    RecId recId = RelCacheTable::relCache[relId]->recId;
    // declaring an object of RecBuffer class to write back to the buffer
    RecBuffer relCatBlock(recId.block);

    // Write back to the buffer using relCatBlock.setRecord() with recId.slot
    relCatBlock.setRecord(record,recId.slot);
  }

  if (RelCacheTable::relCache[relId] != nullptr) {
    free(RelCacheTable::relCache[relId]);
    RelCacheTable::relCache[relId] = nullptr;
  }

  AttrCacheEntry* curr = AttrCacheTable::attrCache[relId];
  while (curr) {
    AttrCacheEntry* next = curr->next;
    free(curr);
    curr = next;
  }
  AttrCacheTable::attrCache[relId] = nullptr;

  tableMetaInfo[relId].free = true;
  tableMetaInfo[relId].relName[0] = '\0';

  return SUCCESS;
}


