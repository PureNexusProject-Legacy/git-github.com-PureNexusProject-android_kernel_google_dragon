/*
 * ARM alt-syscall implementation
 *
 * Copyright (C) 2015 Google, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/alt-syscall.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/syscalls.h>

#include <asm/syscall.h>

int arch_dup_sys_call_table(struct alt_sys_call_table *entry)
{
	int err;

	if (!entry)
		return -EINVAL;
	if (entry->table)
		return -EINVAL;

	entry->size = __NR_syscalls;
	entry->table = kcalloc(entry->size, sizeof(sys_call_ptr_t), GFP_KERNEL);
	if (!entry->table) {
		err = -ENOMEM;
		goto failed;
	}
	memcpy(entry->table, sys_call_table,
	       entry->size * sizeof(sys_call_ptr_t));

	return 0;

failed:
	entry->size = 0;
	return err;
}
EXPORT_SYMBOL_GPL(arch_dup_sys_call_table);

int arch_set_sys_call_table(struct alt_sys_call_table *entry)
{
	if (!entry)
		return -EINVAL;

	current_thread_info()->nr_syscalls = entry->size;
	current_thread_info()->sys_call_table = entry->table;

	return 0;
}
EXPORT_SYMBOL_GPL(arch_set_sys_call_table);
