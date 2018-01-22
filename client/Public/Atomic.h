/*$T Common/Atomic.h GC 1.140 10/10/07 10:06:26 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#ifndef __70YX_ATOMIC_H__
#define __70YX_ATOMIC_H__

// 根据编译选项判断是开启X86架构的原子锁函数还是Arm架构的原子锁 [JiangWei]
#if ANDROID || ( IOS && !TARGET_IPHONE_SIMULATOR )
	#define USE_X86_ATOMIC 0
#else
	#define USE_X86_ATOMIC 1
#endif

#if USE_X86_ATOMIC

	#define ATOMIC_INIT(_Idx)	{ (_Idx) }

	/*
	 =======================================================================================================================
			atomic_read - read atomic variable @v: pointer of type atomic_t Atomically reads the value of @v.
	 =======================================================================================================================
	 */
	#define atomic_read(v)	((v)->counter)

	/*
	 =======================================================================================================================
			atomic_set - set atomic variable @v: pointer of type atomic_t @_Idx: required value Atomically sets the value of
			@v to @_Idx.
	 =======================================================================================================================
	 */
	#define atomic_set(v, _Idx)	(((v)->counter) = (_Idx))
	#define LOCK_PREFIX		".section .smp_locks,\"a\"\n  .align 4\n  .long 661f\n" \
		/* address */ \
		".previous\n661:\n\tlock; "

	/*
	 =======================================================================================================================
			atomic_add - add integer to atomic variable @_Idx: integer value to add @v: pointer of type atomic_t Atomically
			adds @_Idx to @v.
	 =======================================================================================================================
	 */
	static __inline__ void atomic_add(int _Idx, atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__asm__ __volatile__(LOCK_PREFIX "addl %1,%0" : "+m"(v->counter) : "ir"(_Idx));
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	}

	/*
	 =======================================================================================================================
			atomic_sub - subtract the atomic variable @_Idx: integer value to subtract @v: pointer of type atomic_t Atomically
			subtracts @_Idx from @v.
	 =======================================================================================================================
	 */
	static __inline__ void atomic_sub(int _Idx, atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__asm__ __volatile__(LOCK_PREFIX "subl %1,%0" : "+m"(v->counter) : "ir"(_Idx));
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	}

	/*
	 =======================================================================================================================
			atomic_sub_and_test - subtract value from variable and test result @_Idx: integer value to subtract @v: pointer of
			type atomic_t Atomically subtracts @_Idx from @v and returns true if the result is zero, or false for all other
			cases.
	 =======================================================================================================================
	 */
	static __inline__ int atomic_sub_and_test(int _Idx, atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		unsigned char	c;
		__asm__		__volatile__
				(
					LOCK_PREFIX "subl %2,%0; sete %1" : "+m"(v->counter),
					"=qm"(c) : "ir"(_Idx) : "memory"
				);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		return c;
	}

	/*
	 =======================================================================================================================
			atomic_inc - increment atomic variable @v: pointer of type atomic_t Atomically increments @v by 1.
	 =======================================================================================================================
	 */
	static __inline__ void atomic_inc(atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__asm__ __volatile__(LOCK_PREFIX "incl %0" : "+m"(v->counter));
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	}

	/*
	 =======================================================================================================================
			atomic_dec - decrement atomic variable @v: pointer of type atomic_t Atomically decrements @v by 1.
	 =======================================================================================================================
	 */
	static __inline__ void atomic_dec(atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		__asm__ __volatile__(LOCK_PREFIX "decl %0" : "+m"(v->counter));
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	}

	/*
	 =======================================================================================================================
			atomic_dec_and_test - decrement and test @v: pointer of type atomic_t Atomically decrements @v by 1 and returns
			true if the result is 0, or false for all other cases.
	 =======================================================================================================================
	 */
	static __inline__ int atomic_dec_and_test(atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		unsigned char	c;
		__asm__		__volatile__(LOCK_PREFIX "decl %0; sete %1" : "+m"(v->counter), "=qm"(c) : : "memory");
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		return c != 0;
	}

	/*
	 =======================================================================================================================
			atomic_inc_and_test - increment and test @v: pointer of type atomic_t Atomically increments @v by 1 and returns
			true if the result is zero, or false for all other cases.
	 =======================================================================================================================
	 */
	static __inline__ int atomic_inc_and_test(atomic_t *v)
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		unsigned char	c;
		__asm__		__volatile__(LOCK_PREFIX "incl %0; sete %1" : "+m"(v->counter), "=qm"(c) : : "memory");
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		return c != 0;
	}

	/*
	=======================================================================================================================
	atomic_add_return - add and return, 这里返回的是原始值并非变更后的值
	@i: integer value to add
	@v: pointer of type atomic_t
	=======================================================================================================================
	*/
	static inline int atomic_add_return(int i, atomic_t *v)
	{
		int __i = i;
		asm volatile(LOCK_PREFIX "xaddl %0, %1"
			: "+r" (i), "+m" (v->counter)
			: : "memory");
		return i;
	}

#else	// #if USE_X86_ATOMIC

#if !IOS
	#include "sys/atomics.h"
#endif

	#define __ATOMIC_INLINE__ static __inline__ __attribute__((always_inline))

	// 原子函数，给目标内存地址的值加上指定的数值 [JiangWei]
	__ATOMIC_INLINE__ void atomic_add(int _Idx, atomic_t *v)
	{
		__sync_fetch_and_add (&v->counter, _Idx);
	}

	// 原子函数，给目标内存地址的值减去指定的数值 [JiangWei]
	__ATOMIC_INLINE__ void atomic_sub(int _Idx, atomic_t *v)
	{
		__sync_fetch_and_sub (&v->counter, _Idx);
	}
	
	// 原子函数，给目标内存地址的值加上指定的数值，并且返回相加“之前”的值 [JiangWei]
	__ATOMIC_INLINE__ int atomic_add_return(int i, atomic_t *v)
	{
		return __sync_fetch_and_add (&v->counter, i);
	}

	// 原子函数，给目标内存地址的值减去指定的数值，并且返回相减“之前”的值 [JiangWei]
	__ATOMIC_INLINE__ int atomic_sub_return(int i, atomic_t *v)
	{
		return __sync_fetch_and_sub (&v->counter, i);
	}

	// 原子函数，给目标内存地址的值加上指定的数值，判断如果结果为0，则返回TRUE，否则返回FALSE [JiangWei]
	#define atomic_add_and_test(i,v) ( atomic_add_return(i,v) == 0 )

	// 原子函数，给目标内存地址的值减去指定的数值，判断如果结果为0，则返回TRUE，否则返回FALSE [JiangWei]
	#define atomic_sub_and_test(i,v) ( atomic_sub_return(i,v) == 0 )

	// 原子函数，给目标内存地址的值加上1 [JiangWei]
	#define atomic_inc(v) atomic_add( 1,v )

	// 原子函数，给目标内存地址的值减去1 [JiangWei]
	#define atomic_dec(v) atomic_sub( 1,v )

	// 原子函数，给目标内存地址的值加上1，判断如果结果为0，则返回TRUE，否则返回FALSE [JiangWei]
	#define atomic_inc_and_test(v) ( atomic_add_return(1,v) == 0 )

	// 原子函数，给目标内存地址的值减去1，判断如果结果为0，则返回TRUE，否则返回FALSE [JiangWei]
	#define atomic_dec_and_test(v) ( atomic_sub_return(1,v) == 0 )

#endif

#endif // #define __70YX_ATOMIC_H__
