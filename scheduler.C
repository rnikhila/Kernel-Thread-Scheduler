/*
 File: scheduler.C
 
 Author:
 Date  :
 
 */

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "scheduler.H"
#include "thread.H"
#include "console.H"
#include "utils.H"
#include "assert.H"
#include "simple_keyboard.H"


/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

Scheduler::node * Scheduler::head = NULL;   //initialing the static variables to null
Scheduler::node * Scheduler::rear = NULL;
/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   S c h e d u l e r  */
/*--------------------------------------------------------------------------*/

Scheduler::Scheduler() {
//  assert(false);
  q_size = 0;    // initializing the q size to 0 
  Console::puts("Constructed FIFO Scheduler.\n");
}

Thread* Scheduler::dequeue()
{

node *temp;
node *active_next;
temp = head;
active_next = head;  // the next active thread will be the current head , this is the one that is returned
if(head == NULL)
{
//	Console::puts("de_qing an empty q.\n");
	assert(false); // this is to let u know that u r dequeuing from an empty q
}	
	
	
else 
if(head->next != NULL)   // If not an empty q then new head will be the next one in the queue
{
	head = head->next;
	delete(temp);
	q_size--;  // decreasing the size every time we dequeue
}
else  // if the q has only one node, head is again poointed to NULL
{
	delete(temp);
	head = NULL;
	rear= NULL;
	q_size--; // decreasing the size every time we dequeue
}
return(active_next->data_thread); // returning the active thread's thread ID which is basically the head thread in the q
}
void Scheduler::yield() {
//Console::puts("entered in yield ...\n ");
Thread* active_next_thread = dequeue();                  // calling the d_q function
//Console::puts("in yield: after dequeue ...\n ");
Thread::dispatch_to(active_next_thread);		// dispacthing the next thread in the q to cpu
//uncomment the following line wen executing round robin
//Machine::enable_interrupts();

}
void Scheduler::enqueue(Thread * _thread) {
if(rear == NULL)      // if the q is emty 
{
	node *new_node = new node;   // creating memory for a new node using operator
	new_node->data_thread = _thread;   // assigning the function argument to the newly created node 
	new_node->next = NULL;
	rear = new_node; // rear and head will point the new node since there is only one node in the q
	head = rear;
}
else // if not an empty thread
{
	node *new_node = new node; 
	new_node->data_thread = _thread;
	new_node->next = NULL; // th next node of new node is pointed to null
	rear->next = new_node;
	rear = new_node;  //the rear will be updated to the newly created node
}
q_size++;    // increasing the size every time we dequeue
}
void Scheduler::resume(Thread * _thread) {
 // assert(false);
//uncomment the following line wen executing round robin
/*
if(Machine::interrupts_enabled())Machine::disable_interrupts();    // disabling the interrupts if the r enabled
*/
// uncomment the above line wen executing round robin
//Console::puts("In resume ...\n");
enqueue(_thread);  // resume is adding the thread back on the q
}

void Scheduler::add(Thread * _thread) {
//  assert(false);
//Console::puts("In add .....\n");
enqueue(_thread); // add is also adding the thread back on the q
}

void Scheduler::terminate(Thread * _thread) {
yield();  // yield basically dispatches to next thread in the q and add the current thread back on the q
delete (_thread); // we delete the pointr of this thread 
/*
int i;
for(i=0;i<q_size;i++)
{
Thread* head_thread = dequeue();
	if(head_thread->ThreadId() != _thread->ThreadId()) enqueue(head_thread);
}
*/
}
