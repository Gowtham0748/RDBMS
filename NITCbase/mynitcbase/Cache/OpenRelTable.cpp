#include "OpenRelTable.h"
#include<stdlib.h>
#include <cstring>

OpenRelTable::OpenRelTable() {

  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
  }

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


  HeadInfo relCatHead;
  relCatBlock.getHeader(&relCatHead);
  bool inserted = false;
  for(int k=0;k<relCatHead.numEntries&&!inserted;k++)
  { Attribute record[RELCAT_NO_ATTRS];
    relCatBlock.getRecord(record,k);
    if(strcmp(record[RELCAT_REL_NAME_INDEX].sVal,"Students")==0)
    { 
      for(int i=2;i<MAX_OPEN;i++)
      {
        if(RelCacheTable::relCache[i]==nullptr)
        {
          struct RelCacheEntry relCacheEntryForStudent;
          RelCacheTable::recordToRelCatEntry(record,&relCacheEntryForStudent.relCatEntry);
          relCacheEntryForStudent.recId.block=RELCAT_BLOCK;
          relCacheEntryForStudent.recId.slot=k;
          RelCacheTable::relCache[i]=(struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
          *(RelCacheTable::relCache[i])=relCacheEntryForStudent;
  
  
          AttrCacheTable::attrCache[i]=nullptr;
          AttrCacheEntry* relPtr=nullptr;
          HeadInfo attrCatHead;
          attrCatBlock.getHeader(&attrCatHead);
          for(int j=0;j<attrCatHead.numEntries;j++)
          {
            Attribute catRecord[ATTRCAT_NO_ATTRS];
            attrCatBlock.getRecord(catRecord,j);
            if(strcmp(catRecord[ATTRCAT_REL_NAME_INDEX].sVal,"Students")==0)
            {
                AttrCacheEntry *temp=(struct AttrCacheEntry *)malloc(sizeof(AttrCacheEntry));
                AttrCacheTable::recordToAttrCatEntry(catRecord,&temp->attrCatEntry);
                temp->next=nullptr;
                temp->recId.block=ATTRCAT_BLOCK;
                temp->recId.slot=j;
                if(AttrCacheTable::attrCache[i]==nullptr)
                {
                  AttrCacheTable::attrCache[i]=temp;
                  relPtr=temp;
                }
                else{
                  relPtr->next=temp;
                  relPtr=temp;
                }
            }
          }
          inserted=true;
          break;
        }
      } } }


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
  }

  // free all the memory that you allocated in the constructor
}



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

