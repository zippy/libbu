#define _QSORT_SWAP(a, b, t) ((void)((t = *a), (*a = *b), (*b = t)))

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
#define _QSORT_MAX_THRESH 4

/* Stack node declarations used to store unfulfilled partition obligations
 * (inlined in QSORT).
typedef struct {
  QSORT_TYPE *_lo, *_hi;
} qsort_stack_node;
 */

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* The stack needs log (total_elements) entries (we could even subtract
   log(MAX_THRESH)).  Since total_elements has type unsigned, we get as
   upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(unsigned).  */
#define _QSORT_STACK_SIZE	(8 * sizeof(unsigned))
#define _QSORT_PUSH(top, low, high)	\
	(((top->_lo = (low)), (top->_hi = (high)), ++top))
#define	_QSORT_POP(low, high, top)	\
	((--top, (low = top->_lo), (high = top->_hi)))
#define	_QSORT_STACK_NOT_EMPTY	(_stack < _top)


/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:

   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.

   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.

   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.

   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!  */

/* The main code starts here... */

template<typename QSORT_TYPE, typename QSORT_LTT>
void qsrt( QSORT_TYPE *QSORT_BASE, int QSORT_NELT )
{
  QSORT_LTT QSORT_LT;
  QSORT_TYPE *const _base = (QSORT_BASE);
  const unsigned _elems = (QSORT_NELT);
  QSORT_TYPE _hold;

  /* Don't declare two variables of type QSORT_TYPE in a single
   * statement: eg `TYPE a, b;', in case if TYPE is a pointer,
   * expands to `type* a, b;' wich isn't what we want.
   */

  if (_elems > _QSORT_MAX_THRESH) {
    QSORT_TYPE *_lo = _base;
    QSORT_TYPE *_hi = _lo + _elems - 1;
    struct {
      QSORT_TYPE *_hi; QSORT_TYPE *_lo;
    } _stack[_QSORT_STACK_SIZE], *_top = _stack + 1;

    while (_QSORT_STACK_NOT_EMPTY) {
      QSORT_TYPE *_left_ptr; QSORT_TYPE *_right_ptr;

      /* Select median value from among LO, MID, and HI. Rearrange
         LO and HI so the three values are sorted. This lowers the
         probability of picking a pathological pivot value and	
         skips a comparison for both the LEFT_PTR and RIGHT_PTR in
         the while loops. */
									
      QSORT_TYPE *_mid = _lo + ((_hi - _lo) >> 1);			
									
      if (QSORT_LT (_mid, _lo))						
        _QSORT_SWAP (_mid, _lo, _hold);					
      if (QSORT_LT (_hi, _mid))						
        _QSORT_SWAP (_mid, _hi, _hold);					
      else								
        goto _jump_over;						
      if (QSORT_LT (_mid, _lo))					
        _QSORT_SWAP (_mid, _lo, _hold);					
  _jump_over:;								
									
      _left_ptr  = _lo + 1;						
      _right_ptr = _hi - 1;					
									
      /* Here's the famous ``collapse the walls'' section of quicksort.	
         Gotta like those tight inner loops!  They are the main reason	
         that this algorithm runs much faster than others. */		
      do {								
        while (QSORT_LT (_left_ptr, _mid))				
         ++_left_ptr;							
									
        while (QSORT_LT (_mid, _right_ptr))				
          --_right_ptr;							
									
        if (_left_ptr < _right_ptr) {					
          _QSORT_SWAP (_left_ptr, _right_ptr, _hold);			
          if (_mid == _left_ptr)					
            _mid = _right_ptr;						
          else if (_mid == _right_ptr)					
            _mid = _left_ptr;						
          ++_left_ptr;						
          --_right_ptr;							
        }								
        else if (_left_ptr == _right_ptr) {				
          ++_left_ptr;							
		  --_right_ptr;							
          break;							
        }								
      } while (_left_ptr <= _right_ptr);				
									
     /* Set up pointers for next iteration.  First determine whether	
        left and right partitions are below the threshold size.  If so,	
        ignore one or both.  Otherwise, push the larger partition's	
        bounds on the stack and continue sorting the smaller one. */	
									
      if (_right_ptr - _lo <= _QSORT_MAX_THRESH) {			
        if (_hi - _left_ptr <= _QSORT_MAX_THRESH)		
          /* Ignore both small partitions. */				
          _QSORT_POP (_lo, _hi, _top);					
        else								
          /* Ignore small left partition. */				
          _lo = _left_ptr;						
      }									
      else if (_hi - _left_ptr <= _QSORT_MAX_THRESH)			
        /* Ignore small right partition. */				
        _hi = _right_ptr;						
      else if (_right_ptr - _lo > _hi - _left_ptr) {			
        /* Push larger left partition indices. */			
        _QSORT_PUSH (_top, _lo, _right_ptr);			
        _lo = _left_ptr;						
      }									
      else {								
        /* Push larger right partition indices. */			
        _QSORT_PUSH (_top, _left_ptr, _hi);				
        _hi = _right_ptr;						
      }									
    }								
  }									
									
  /* Once the BASE array is partially sorted by quicksort the rest	
     is completely sorted using insertion sort, since this is efficient	
     for partitions below MAX_THRESH size. BASE points to the		
     beginning of the array to sort, and END_PTR points at the very	
     last element in the array (*not* one beyond it!). */		
									
  {									
    QSORT_TYPE *const _end_ptr = _base + _elems - 1;			
    QSORT_TYPE *_tmp_ptr = _base;					
    register QSORT_TYPE *_run_ptr;				
    QSORT_TYPE *_thresh;						
									
    _thresh = _base + _QSORT_MAX_THRESH;				
    if (_thresh > _end_ptr)						
      _thresh = _end_ptr;					
									
    /* Find smallest element in first threshold and place it at the	
       array's beginning.  This is the smallest array element,		
       and the operation speeds up insertion sort's inner loop. */	
									
    for (_run_ptr = _tmp_ptr + 1; _run_ptr <= _thresh; ++_run_ptr)	
      if (QSORT_LT (_run_ptr, _tmp_ptr))				
        _tmp_ptr = _run_ptr;						
									
    if (_tmp_ptr != _base)						
      _QSORT_SWAP (_tmp_ptr, _base, _hold);				
									
    /* Insertion sort, running from left-hand-side			
     * up to right-hand-side.  */					
									
    _run_ptr = _base + 1;						
    while (++_run_ptr <= _end_ptr) {					
      _tmp_ptr = _run_ptr - 1;						
      while (QSORT_LT (_run_ptr, _tmp_ptr))				
        --_tmp_ptr;							
									
      ++_tmp_ptr;							
      if (_tmp_ptr != _run_ptr) {					
        QSORT_TYPE *_trav = _run_ptr + 1;				
        while (--_trav >= _run_ptr) {					
          QSORT_TYPE *_hi; QSORT_TYPE *_lo;				
          _hold = *_trav;						
									
          for (_hi = _lo = _trav; --_lo >= _tmp_ptr; _hi = _lo)		
            *_hi = *_lo;						
          *_hi = _hold;							
        }								
      }									
    }									
  }									
									
}


struct cc
{
	bool operator()( int *a, int *b )
	{
		return *a < *b;
	}
};

#include <stdio.h>

int main()
{
	int lst[] = { 43, 1, 342, 12, 491, 32, 12321, 32, 3, -3 };

	for( int j = 0; j < 10; j++ )
		printf("%s%d", (j>0)?", ":"", lst[j] );
	printf("\n");
	qsrt<int, cc>( lst, 10 );
	for( int j = 0; j < 10; j++ )
		printf("%s%d", (j>0)?", ":"", lst[j] );
	printf("\n");
}

