#include <iostream>
#include <random>
#include <functional>
#include <time.h>
#include <string>
using namespace std;
template <typename T>
class Chain;


/*
 * Define class Node.
 */
template <typename T>
class ChainNode {
	friend Chain<T>;
private:
	T data;
	ChainNode<T> *link;
};

/*
 * Define class Chain.
 */
template <typename T>
class Chain {
public:
	Chain();
	//~Chain();
	bool IsEmpty() const {return first==0;}
	int Length() const;
	bool Find(int k, T& x) const;
	int Search(const T& x) const;
	int Count(T &x) const;  // counts duplicate nodes of Chain
	Chain<T>& Delete(int k, T& x);
	Chain<T>& Insert(int k, const T& x);
	void removeDuplicates();  // removes duplicate nodes
	void BinSort(int range);  // sorts the list
	void Output(ostream& out) const;

private:
	ChainNode<T> *first;  // pointer to first node
};

/*
 * Define methods of class Chain.
 */
template <typename T>
Chain<T>::Chain()
{
	first = NULL;
}

template<typename T>
int Chain<T>::Length() const
{// Return the number of elements in the chain.
ChainNode<T> *current = first;
int len = 0;
while (current) {
	len++;
	current = current->link;
}
return len;
}

template<class T>
Chain<T>& Chain<T>:: Insert(int k, const T& x){
	// Insert x after the k'th element.
	// Throw OutOfBounds exception if no k'th element.
  // Pass NoMem exception if inadequate space.
	if (k<0) throw out_of_range("Out of Bounds");
	// p will eventually point to k'th node
	ChainNode<T> *p = first;
	for (int index = 1; index<k && p; index++)
		p = p->link;
	if (k>0 && !p) throw out_of_range("Out of Bounds");  // no k'th insert
	ChainNode<T> *y = new ChainNode<T>;
	y->data = x;
	if (k){
		y->link = p->link;
		p->link = y;}
		else {
			y->link = first;
			first = y;
		}
		return *this;
	}

template<class T>
int Chain<T>:: Search(const T& x) const{
	// Locate x. Return position of x if found.
	// Return 0 if x not in the chain.
	ChainNode<T> *current = first;
	int index = 1;
	while (current && current->data != x){
		current = current->link;
		index++;
	}
	if (current) return index;
	return 0 ;
}

template<class T>
bool Chain<T>:: Find(int k, T& x) const
{ // Set x to the k'th element in the chain
	// Return false if no k'th; return true otherwise.
	if (k<1) return false;
	ChainNode<T> *current = first;
	int index = 1;  // index of current
	while(index<k && current){
		current = current->link;
		index++;
	}
	if (current){
		x = current->data;
		return true;
	}
	return false;  // no k'th element
}

template<class T>
int Chain<T>:: Count(T& x) const
{ // counts how many times x exists in list
	ChainNode<T> *current = first;
	int count = 0;
	while(current){
		if(current->data == x)
			count++;
		current = current->link;
	}return count;
}

template<class T>
Chain<T>& Chain<T>:: Delete(int k, T&x){
	 // Set x to the k'th element and delete it.
   // Throw OutOfBounds exception if no k'th element.
	if (k<1 || !first) throw out_of_range("Out of Bounds");
	ChainNode<T> *p = first;
	if (k==1) first = first->link;
	else{
		ChainNode<T> *q = first;
		for (int index = 1; index<k-1 && q; index++)
			q = q->link;
			if (!q || !q->link) throw out_of_range("Out of Bounds");  // no k'th
			p = q->link;
			q->link = p->link;}  // remove k'th
			// save k'th element and free node p
			x = p->data;
			delete p;
			return *this;
}
template<class T>
void Chain<T>::removeDuplicates()
{   // removes duplicate nodes
		ChainNode<T> *p2, *dup;
    ChainNode<T> *p1 = first;

    //Pick elements one by one
    while (p1 != NULL && p1->link != NULL)
    {
        p2 = p1;

        /* Compare the picked element with rest
           of the elements */
        while (p2->link != NULL)
        {
            /* If duplicate then delete it */
            if (p1->data == p2->link->data)
            {
                dup = p2->link;
                p2->link = p2->link->link;
                delete(dup);
            }
            else
                p2 = p2->link;
        }
        p1 = p1->link;
    }
}

template<class T>
void Chain<T>:: Output(ostream& out) const
{
	ChainNode<T> *current;
	for (current=first; current; current=current->link)
		out<<current->data<<"  ";
}
//overload <<
template<class T>
ostream& operator<<(ostream& out, const Chain<T> x){
	x.Output(out);
	return out;
}

/*template<class T>
Chain<T>::~Chain()
{// Chain destructor. Delete all nodes in chain.
	ChainNode<T> *next; // next node
	while (first) {
		next = first->link;
		delete first;
		first = next;
	}
}*/

template<class T>
void Chain<T>:: BinSort(int range)
{
	int b;  // bin index
	ChainNode<T> **bottom, **top;
	// initialize the bins
	bottom = new ChainNode<T> *[range + 1];
	top = new ChainNode<T> *[range + 1];
	for (b=0; b<=range; b++)
		bottom[b] = 0;

  // distribute to bins
	for(; first; first = first->link){  // add to bin
		b = first->data;
		if (bottom[b]){  // bin not empty
			top[b]->link = first;
			top[b] = first;
		}else  // bin empty
			bottom[b] = top[b] = first;
	}

  // collect from bins into sorted list
	ChainNode<T> *y = 0;
	for (b=0; b<=range; b++)
		if (bottom[b]){  // bin not empty
			if (y)  // not first nonempty list
				y->link = bottom[b];
			else  // first nonempty list
				first = bottom[b];
			y = top[b];
		}
	if (y) y->link = 0;

	delete [] bottom;
	delete [] top;
}



int main(){
	default_random_engine generator;
	generator.seed((unsigned)time(0));
	uniform_int_distribution<int> list_size_distribution(2,45);
	uniform_int_distribution<int> data_element_distribution(0,50);
	auto random_list_size = bind(list_size_distribution, generator);
	auto random_element = bind(data_element_distribution, generator);
  int numOfLists;  // number of Lists, given from user
  cout<<"Give the number of Lists: "<<endl;
  cin>>numOfLists;
	Chain<int> output;  // new empty Chain output
	int cnt=0;  // counts the number of nodes of Chain output
  for (int i=0; i<numOfLists; i++){  // for each list
    int list_size = random_list_size();  // random number of nodes for each list
		Chain<int> L;  // new empty List Li
		for (int j=1; j<=list_size; j++){  // for each node of list Li
			int data_element = random_element();  // random data for each node
			if (L.Search(data_element)==0){  // if data_element doesn't already exist in L,
				output.Insert(cnt, data_element);  // then insert data_element to output,
				cnt+=1;  // and increase the number of nodes of output list (number of nodes equals to the sum of number of non-duplicate nodes
					       //of every Li list)
			}
			L.Insert(j-1, data_element); // insert data_element after (j-1)'th element
		}
		cout<<"L list: "<<L<<endl;  // print Li list
  }
	int halfNumOfLists;  // half number of lists
  if (numOfLists%2==1) halfNumOfLists = (numOfLists/2)+1;
	else halfNumOfLists = numOfLists/2;
	int z;
	for (int k=1; k<=cnt; k++){  // for each node of output list
			output.Find(k,z);  // find k'th element and set z to it
			if (output.Count(z)<halfNumOfLists){  // if number of duplicate nodes are less than half number of lists
					while (output.Search(z)!=0){  // while z exists in output list
							output.Delete(output.Search(z), k);  // delete z from output list
					}
			};
			// now output list contains only the data_elements that exist at least to the half number of lists Li, but not only one time
	}
	output.removeDuplicates();  // remove duplicate nodes
	// now output list contains only the data_elements that exist at least to the half number of lists Li, and only one time, but not sorted
	cout<<"Unsorted Output list: "<<output<<endl;  // unsorted output
	output.BinSort(50);  // range of numbers' value = 0-50
	// now output list is also sorted
	cout<<"Sorted Output list: "<<output;  // sorted output
	return 0;
}
