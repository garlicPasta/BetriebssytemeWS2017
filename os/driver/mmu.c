#define ENRTY_COUNT 4096
#define PAGE_SIZE 1024*1024*1024
#include "../system/memlayout.h"

struct page_table{
    int entry[4096];
};

static volatile struct page_table * const page_table = 
    (struct page_table *)PAGE_TABLE_BASE;

void fill_page_table(){
    int i= 0;
    for (i;i< ENRTY_COUNT; i++){
        int entry = (USER_STACK_BOTTOM + i*PAGE_SIZE) & 0xFFF<<20;
        entry = entry | 0x3<<10;
        entry = entry | 0x1<<5;
        entry = entry | 0x1<<4;
        entry = entry | 0x00<<4;
        entry = entry | 0x00<<2; // CB
        entry = entry | 0x10<<0; // L1 Cache
        page_table->entry[i] = USER_STACK_BOTTOM + i*PAGE_SIZE ;
    }
}

void init_page_table_L1(){
    fill_page_table();
}

void create_entry_at(int index, int entry){

}

