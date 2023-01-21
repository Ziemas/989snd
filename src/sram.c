#include "sram.h"
#include "989snd.h"
#include "stdio.h"
#include "types.h"

enum {
    NUM_SRAM_NODE = 129,

    SRAM_BASE_ADDR = 0x5040,
    SRAM_MAX_FREE = 0x1FAFBF,
};

/* data 1c24 */ sSRAMNodePtr gSRAMFreeListHead = NULL;
/* data 1c28 */ SInt32 gSRAMFreeNodesInUse = 0;
/* bss 80e0 */ sSRAMNode gSRAMFreeNodeStorage[NUM_SRAM_NODE];

/* 000200a8 0002014c */ void
snd_SRAMInitMalloc() {
    /* -0x10(sp) */ SInt32 i;
    gSRAMFreeListHead = NULL;
    for (i = 0; i < NUM_SRAM_NODE; i++) {
        gSRAMFreeNodeStorage[i].in_use = 0;
    }
    snd_SRAMFree(SRAM_BASE_ADDR, SRAM_MAX_FREE);
}

/* 0002014c 00020308 */ UInt32 snd_SRAMMalloc(/* 0x0(sp) */ UInt32 size) {
    /* -0x18(sp) */ sSRAMNodePtr p;
    /* -0x14(sp) */ sSRAMNodePtr candidate;
    /* -0x10(sp) */ UInt32 ret_loc;

    if (size == 0) {
        return 0;
    }

    if (!IS_ALIGNED(size, 64)) {
        size = ALIGN(size, 64);
    }

    candidate = snd_SRAMSearch(gSRAMFreeListHead, size);
    if (candidate == NULL) {
        snd_ShowError(32, size, 0, 0, 0);
        return 0;
    }

    while (1) {
        p = snd_SRAMSearch(candidate->smaller, size);
        if (p == NULL) {
            break;
        }
        candidate = p;
    }

    ret_loc = candidate->loc;
    snd_SRAMRemoveNode(candidate);
    candidate->size -= size;
    if (candidate->size != 0) {
        candidate->loc += size;
        snd_SRAMAddNode(candidate);
    }

    return ret_loc;
}

/* 00020308 00020694 */ UInt32 snd_SRAMMallocRev(/* 0x0(sp) */ UInt32 size) {
    /* -0x20(sp) */ SInt32 i;
    /* -0x1c(sp) */ sSRAMNodePtr candidate = NULL;
    /* -0x18(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;
    /* -0x14(sp) */ UInt32 loc;
    /* -0x10(sp) */ UInt32 end;

    if (!IS_ALIGNED(size, 64)) {
        size = ALIGN(size, 64);
    }

    for (i = 0; i < NUM_SRAM_NODE; i++, storage++) {
        if (storage->in_use == 0) {
            continue;
        }

        if (storage->size < size) {
            continue;
        }

        loc = storage->loc;
        end = storage->loc + storage->size;
        if (loc < (end - (end & 0x1ffff)) && (end - (end & 0x1ffff) - loc) >= size) {
            if (candidate == NULL) {
                candidate = storage;

                continue;
            }

            if (storage->size < candidate->size) {
                candidate = storage;
            }
        }
    }

    if (candidate == NULL) {
        if (snd_SRAMGetFreeMem() >= size) {
            snd_ShowError(88, size, 0, 0, 0);
        }

        return 0;
    }

    return snd_SRAMMarkUsed(candidate->loc + candidate->size - ((candidate->loc + candidate->size) & 0x1ffff) - size, size);
}

/* 00020694 00020b14 */ void snd_SRAMFree(/* 0x0(sp) */ UInt32 loc, /* 0x4(sp) */ UInt32 size) {
    /* -0x18(sp) */ SInt32 i;
    /* -0x14(sp) */ SInt32 free_node = NUM_SRAM_NODE;
    /* -0x10(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;
    if (size == 0) {
        snd_ShowError(93, loc, 0, 0, 0);
        return;
    }

    if (!IS_ALIGNED(size, 64)) {
        size = ALIGN(size, 64);
    }

    for (i = 0; i < NUM_SRAM_NODE; i++, storage++) {
        if (storage->in_use == 0) {
            if (i < free_node) {
                free_node = i;
            }
            continue;
        }

        if (storage->loc + storage->size == loc) {
            snd_SRAMRemoveNode(storage);
            size += storage->size;
            loc = storage->loc;
            if (i < free_node) {
                free_node = i;
            }
        } else if (storage->loc == loc + size) {
            snd_SRAMRemoveNode(storage);
            size += storage->size;
            if (i < free_node) {
                free_node = i;
            }
        }
    }

    if (free_node < NUM_SRAM_NODE) {
        gSRAMFreeNodeStorage[free_node].size = size;
        gSRAMFreeNodeStorage[free_node].loc = loc;
        gSRAMFreeNodeStorage[free_node].root = NULL;
        gSRAMFreeNodeStorage[free_node].smaller = NULL;
        gSRAMFreeNodeStorage[free_node].bigger = NULL;
        snd_SRAMAddNode(&gSRAMFreeNodeStorage[free_node]);
    } else {
        snd_ShowError(33, 0, 0, 0, 0);
    }
}

/* 00020b14 000212b4 */ UInt32 snd_SRAMMarkUsed(/* 0x0(sp) */ UInt32 loc, /* 0x4(sp) */ UInt32 size) {
    /* -0x20(sp) */ SInt32 i;
    /* -0x1c(sp) */ SInt32 nn_size = 0;
    /* -0x18(sp) */ SInt32 nn_loc = 0;
    /* -0x14(sp) */ SInt32 free_node = NUM_SRAM_NODE;
    /* -0x10(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;

    if (!IS_ALIGNED(size, 64)) {
        size = ALIGN(size, 64);
    }

    for (i = 0; i < NUM_SRAM_NODE; i++, storage++) {
        // TODO unfinished
    }

    // TODO seems pretty silly
    if (nn_size != 0 && free_node < NUM_SRAM_NODE) {
        if (nn_size >= 0) {
            gSRAMFreeNodeStorage[free_node].size = nn_size;
            gSRAMFreeNodeStorage[free_node].loc = nn_loc;
            gSRAMFreeNodeStorage[free_node].root = NULL;
            gSRAMFreeNodeStorage[free_node].smaller = NULL;
            gSRAMFreeNodeStorage[free_node].bigger = NULL;
            snd_SRAMAddNode(&gSRAMFreeNodeStorage[free_node]);
        } else {
            snd_ShowError(34, 0, 0, 0, 0);
        }
    } else {
        snd_ShowError(35, 0, 0, 0, 0);
        return 0;
    }

    return nn_loc;
}

/* 000212b4 00021420 */ void snd_SRAMRemoveNode(/* 0x0(sp) */ sSRAMNodePtr node) {
    if (node == NULL) {
        return;
    }

    if (node == gSRAMFreeListHead) {
        gSRAMFreeListHead = node->bigger;
    } else {
        if (node->root->smaller == node) {
            node->root->smaller = node->bigger;
        } else {
            node->root->bigger = node->bigger;
        }
    }

    if (node->bigger != NULL) {
        node->bigger->root = node->root;
    }
    snd_SRAMAddNode(node->smaller);
    node->in_use = 0;
    node->smaller = NULL;
    node->bigger = NULL;
    gSRAMFreeNodesInUse--;
}

/* 00021420 000215a8 */ void snd_SRAMAddNode(/* 0x0(sp) */ sSRAMNodePtr node) {
    /* -0x10(sp) */ sSRAMNodePtr work;
    if (node == NULL) {
        return;
    }

    node->in_use = 1;
    gSRAMFreeNodesInUse++;
    work = gSRAMFreeListHead;
    if (work == NULL) {
        gSRAMFreeListHead = node;
        node->root = NULL;

        return;
    }

    while (true) {
        if (node->size >= work->size) {
            if (work->bigger == NULL) {
                work->bigger = node;
                node->root = work;

                return;
            }

            work = work->bigger;
        } else {
            if (work->smaller == NULL) {
                work->smaller = node;
                work->root = work;

                return;
            }

            work = work->smaller;
        }
    }
}

/* 000215a8 00021650 */ sSRAMNodePtr snd_SRAMSearch(/* 0x0(sp) */ sSRAMNodePtr root, /* 0x4(sp) */ UInt32 size) {
    if (root == NULL) {
        return NULL;
    }
    if (root->size < size) {
        return snd_SRAMSearch(root->bigger, size);
    }

    return root;
}

/* 00021650 00021738 */ SInt32 snd_SRAMGetFreeMem() {
    /* -0x18(sp) */ SInt32 i;
    /* -0x14(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;
    /* -0x10(sp) */ SInt32 free_size = 0;
    for (i = 0; i < NUM_SRAM_NODE; i++, storage++) {
        if (storage->in_use) {
            free_size += (SInt32)storage->size;
        }
    }

    return free_size;
}

/* 00021738 00021854 */ SInt32 snd_SRAMMaxFreeBlockSize() {
    /* -0x18(sp) */ SInt32 i;
    /* -0x14(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;
    /* -0x10(sp) */ SInt32 largest = 0;
    for (i = 0; i < NUM_SRAM_NODE; i++, storage++) {
        if (storage->in_use != 0) {
            if (largest < storage->size) {
                largest = (SInt32)storage->size;
            }
        }
    }

    return largest;
}

/* 00021854 0002197c */ void snd_SRAMDumpFreeBlocks() {
    /* -0x10(sp) */ SInt32 i;
    /* -0xc(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;

    for (int i = 0; i < NUM_SRAM_NODE; i++, storage++) {
        if (storage->in_use != 0) {
            printf("\nFree SPU Block at -> %u (0x%x) size -> %d\n\n", storage->loc, storage->loc, storage->size);
        }
    }
}

/* 0002197c 00021a9c */ SInt32 snd_GetFreeBlockSize(/* 0x0(sp) */ SInt32 start, /* 0x4(sp) */ UInt32 *loc) {
    /* -0x10(sp) */ SInt32 i;
    /* -0xc(sp) */ sSRAMNodePtr storage = gSRAMFreeNodeStorage;

    for (i = start; i < NUM_SRAM_NODE; i++, storage++) {
        if (storage->in_use != 0) {
            *loc = storage->size;
            return (SInt32)storage->size;
        }
    }

    return 0;
}
