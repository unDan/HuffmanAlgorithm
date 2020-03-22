#pragma once
#include <iostream>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;

template<class T>
class List
{
private:
	struct List_element
	{
		T value;
		List_element* next;
		List_element* prev;
		
		List_element(T initial_value, List_element* next_element, List_element* prev_element)
		{
			value = initial_value;
			next = next_element;
			prev = prev_element;
		}

		~List_element()
		{
			next = nullptr;
			prev = nullptr;
		}
	};

	List_element* first;
	List_element* last;
	
	size_t size; // amount of elements in the list

public:
	//creates empty list
	List()
	{
		first = nullptr;
		last = nullptr;
		size = 0;
	}

	//creates list with size 'initial size' filled with 'priority'
	List(size_t initial_size, T initial_value)
	{
		first = nullptr;
		last = nullptr;
		size = 0;
		for (size_t i = 0; i < initial_size; ++i)
			push_back(initial_value);
	}

	~List()
	{
		clear();
	}

	//checks if the list has no elements
	bool isEmpty()
	{
		if (size == 0)
			return true;
		else
			return false;
	}

	//inserts priority as the last
	void push_back(T value)
	{
		//if list is empty, creates first element 
		List_element* current;
		if (!isEmpty())
		{
			current = new List_element(value, nullptr, last);
			last->next = current;
			last = current;
		}
		else
		{
			first = new List_element(value, nullptr, nullptr);
			last = first;
			current = first;
		}
		size++;
	}

	//inserts priority as the first
	void push_front(T value)
	{
		//if list is empty, creates first element 
		List_element* current;
		if (!isEmpty())
		{
			current = new List_element(value, first, nullptr);
			first->prev = current;
			first = current;
		}
		else
		{
			first = new List_element(value, nullptr, nullptr);
			last = first;
			current = first;
		}
		size++;
	}

	//inserts a list to the beginning
	void push_front(List& list)
	{
		if (!isEmpty() && !list.isEmpty())
		{
			for (size_t i = 0; i < list.getSize(); ++i)
				insert(list.at(i), i);
		}
		else
			throw std::out_of_range("Error: Can not push into empty list!");
	}


	//deletes last value
	void pop_back()
	{
		List_element* current;
		if (size > 1)
		{
			current = last->prev;
			delete last;
			current->next = nullptr;
			last = current;

			size--;
		}
		else if (size == 1)
		{
			delete last;
			first = nullptr;
			last = nullptr;
			current = nullptr;
			size = 0;
		}
	}

	//deletes first value
	void pop_front()
	{
		List_element* current;
		if (size > 1)
		{
			current = first->next;
			delete first;
			current->prev = nullptr;
			first = current;

			size--;
		}
		else if (size == 1)
		{
			delete last;
			first = nullptr;
			last = nullptr;
			current = nullptr;
			size = 0;
		}
	}

	//inserts value 'value' at position 'index'
	void insert(T value, size_t index)
	{
		if (index <= size && index >= 0)
		{
			if (index == 0) //Optimization: directly inserts value as the first
				push_front(value);
			else if (index == size) //Optimization: directly inserts value as the last
				push_back(value);
			else
			{
				/*Optimization: if the value that is need to be inserted will be contained in the
				first half of the list, cycle goes from the beginning. If the value will be contained
				in the second half of the list, cycle goes from the end*/
				List_element* current;
				if (index <= (size - 1) / 2)
				{
					current = first;
					for (size_t i = 0; i < index; ++i)
						current = current->next;

					List_element* temp = new List_element(value, current, current->prev);
					current->prev->next = temp;
					current->prev = temp;
					current = temp;
				}
				else if (index > (size - 1) / 2)
				{
					current = last;
					for (size_t i = size - 1; i > index; --i)
						current = current->prev;

					List_element* temp = new List_element(value, current, current->prev);

					current->prev->next = temp;
					current->prev = temp;
					current = temp;
				}

				size++;
			}
		}
		else
			throw std::out_of_range("Error: invalid index!");
	}

	//gets the value at position 'index'
	T at(size_t index)
	{
		if (index < size && index >= 0)
		{
			if (index == 0) //Optimization: directly returns the first value
				return first->value;
			else if (index == size - 1) //Optimization: directly returns the last value
				return last->value;
			else
			{
				/*Optimization: if value that is need to be returned contains in the
				first half of the list, cycle goes from the beginning. If the value contains
				in the second half of the list, cycle goes from the end*/
				List_element* current;
				if (index <= (size - 1) / 2)
				{
					current = first;
					for (size_t i = 0; i < index; ++i)
						current = current->next;

					return current->value;
				}
				else if (index > (size - 1) / 2)
				{
					current = last;
					for (size_t i = size - 1; i > index; --i)
						current = current->prev;

					return current->value;
				}
			}
		}
		else
			throw std::out_of_range("Error: invalid index!");
	}

	//deletes value at position 'index'
	void remove(size_t index)
	{
		//checks index, throws an exception if index is wrong
		if (index < size && index >= 0)
		{
			if (index == 0) //Optimization: directly deletes the first element
				pop_front();
			else if (index == size - 1)
				pop_back(); //Optimization: directly deletes the last element
			else
			{
				/*Optimization: if the element that needs to be removed
				is contained in the first half of the list, cycle goes from the beginning,
				if element is contained in the second half of the list, cycle goes from the end*/
				List_element* current;
				if (index <= (size - 1) / 2)
				{
					current = first;

					for (size_t i = 0; i < index; ++i)
						current = current->next;

					List_element* temp = current->next;
					current->prev->next = temp;
					temp->prev = current->prev;

					delete current;
				}
				else if (index > (size - 1) / 2)
				{
					current = last;

					for (size_t i = size - 1; i > index; --i)
						current = current->prev;

					List_element* temp = current->prev;
					current->next->prev = temp;
					temp->next = current->next;

					delete current;
				}
				size--;
			}
		}
		else
			throw std::out_of_range("Error: invalid index!");
	}

	//returns size of the list
	size_t getSize()
	{
		return size;
	}

	//prints all values to the console
	void print_to_console()
	{
		List_element* current = first;

		for (size_t i = 0; i < size; ++i)
		{
			cout << current->value << " ";
			current = current->next;
		}
	}

	//deletes all values in the list
	void clear()
	{
		//does nothing if the list is empty
		if (!isEmpty())
		{
			List_element* current = first;
			do
			{
				current = current->next;
				delete first;
				first = current;
				size--;
			} while (current != nullptr);

		}
	}

	//sets value 'value' for the element at position 'index'
	void set(size_t index, T value)
	{
		if (index < size && index >= 0)
		{
			if (index == 0) //Optimization: directly changes the first value
				first->value = value;
			else if (index == size - 1) //Optimization: directly changes the last value
				last->value = value;
			else
			{
				/*Optimization: if value that is need to be changed contains in the
				first half of the list, cycle goes from the beginning. If the value contains
				in the second half of the list, cycle goes from the end*/
				List_element* current;
				if (index <= (size - 1) / 2)
				{
					current = first;
					for (size_t i = 0; i < index; ++i)
						current = current->next;

					current->value = value;
				}
				else if (index > (size - 1) / 2)
				{
					current = last;
					for (size_t i = size - 1; i > index; --i)
						current = current->prev;

					current->value = value;
				}
			}
		}
		else
			throw std::out_of_range("Error: invalid index!");
	}

};