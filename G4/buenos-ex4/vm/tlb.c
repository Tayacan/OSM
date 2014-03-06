/*
 * TLB handling
 *
 * Copyright (C) 2003 Juha Aatrokoski, Timo Lilja,
 *   Leena Salmela, Teemu Takanen, Aleksi Virtanen.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: tlb.c,v 1.6 2004/04/16 10:54:29 ttakanen Exp $
 *
 */

#include "kernel/panic.h"
#include "kernel/assert.h"
#include "vm/tlb.h"
#include "vm/pagetable.h"
#include "kernel/thread.h"

#define DEBUG 0

void tlb_modified_exception(void)
{
    if(DEBUG)
    {
        tlb_exception_state_t state;
        int odd;
        _tlb_get_exception_state(&state);

        thread_table_t *thr = thread_get_current_thread_entry();
        kprintf("pid = %d\n", thr->process_id);
        kprintf("badvaddr: 0x%8.8x\n", state.badvaddr);
        kprintf("badvpn2: 0x%8.8x\n", state.badvpn2);
        kprintf("asid: %d\n", state.asid);
        odd = state.badvaddr & 0x1000;
        kprintf("odd = %d\n", odd);
    }

    KERNEL_PANIC("Unhandled TLB modified exception");
}

void tlb_load_store_exception(void)
{
    int i;
    int odd;

    tlb_exception_state_t state;
    tlb_entry_t *entry;
    _tlb_get_exception_state(&state);
    if(DEBUG)
    {
        kprintf("badvaddr: 0x%8.8x\n", state.badvaddr);
        kprintf("badvpn2: 0x%8.8x\n", state.badvpn2);
        kprintf("asid: %d\n", state.asid);
    }

    thread_table_t *thr = thread_get_current_thread_entry();
    if(DEBUG) kprintf("pid = %d\n", thr->process_id);

    odd = state.badvaddr & 0x1000;
    if(DEBUG) kprintf("odd = %d\n", odd);

    for(i = 0; i < PAGETABLE_ENTRIES; i++)
    {
        entry = &thr->pagetable->entries[i];
        if(state.badvpn2 == entry->VPN2)
        {
            if((!odd && entry->V0) || (odd && entry->V1))
            {
                _tlb_write_random(entry);
                if(state.asid == 4 && DEBUG)
                {
                    kprintf("Pagetable ASID: %d\n", thr->pagetable->ASID);
                    kprintf("Entry ASID: %d\n", entry->ASID);
                    kprintf("Thread ID: %d\n", thread_get_current_thread());
                    //KERNEL_PANIC("ASID 4");
                }

                return;
            }
            else
            {
                if(DEBUG) kprintf("Not valid: 0x%8.8x. Odd: %d\n", entry->VPN2, odd);
                break;
            }
        }
    }

    KERNEL_PANIC("ARRRGH!");
}

void tlb_load_exception(void)
{
    if(DEBUG) kwrite("load\n");
    tlb_load_store_exception();
}

void tlb_store_exception(void)
{
    if(DEBUG) kwrite("store\n");
    tlb_load_store_exception();
}

/**
 * Fill TLB with given pagetable. This function is used to set memory
 * mappings in CP0's TLB before we have a proper TLB handling system.
 * This approach limits the maximum mapping size to 128kB.
 *
 * @param pagetable Mappings to write to TLB.
 *
 */

void tlb_fill(pagetable_t *pagetable)
{
    if(pagetable == NULL)
	return;

    /* Check that the pagetable can fit into TLB. This is needed until
     we have proper VM system, because the whole pagetable must fit
     into TLB. */
    KERNEL_ASSERT(pagetable->valid_count <= (_tlb_get_maxindex()+1));

    _tlb_write(pagetable->entries, 0, pagetable->valid_count);

    /* Set ASID field in Co-Processor 0 to match thread ID so that
       only entries with the ASID of the current thread will match in
       the TLB hardware. */
    _tlb_set_asid(pagetable->ASID);
}
