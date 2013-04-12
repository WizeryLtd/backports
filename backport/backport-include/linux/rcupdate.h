#ifndef __BACKPORT_LINUX_RCUPDATE_H
#define __BACKPORT_LINUX_RCUPDATE_H
#include_next <linux/rcupdate.h>

/* 
 * This adds a nested function everywhere kfree_rcu() was called. This
 * function frees the memory and is given as a function to call_rcu().
 * The rcu callback could happen every time also after the module was
 * unloaded and this will cause problems. To address that problem, we
 * put rcu_barrier() into each module_exit() in module.h.
 */
#if !defined(kfree_rcu)
#define kfree_rcu(data, rcuhead)		do {			\
		void __kfree_rcu_fn(struct rcu_head *rcu_head)		\
		{							\
			void *___ptr;					\
			___ptr = container_of(rcu_head, typeof(*(data)), rcuhead);\
			kfree(___ptr);					\
		}							\
		call_rcu(&(data)->rcuhead, __kfree_rcu_fn);		\
	} while (0)
#endif

#endif /* __BACKPORT_LINUX_RCUPDATE_H */