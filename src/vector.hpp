#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>
#include <iostream>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T* ptr;
		const vector<T>* vec;

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, vec);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, vec);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (vec != rhs.vec) throw invalid_iterator();
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator temp = *this;
			++ptr;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++ptr;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator temp = *this;
			--ptr;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--ptr;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const;
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const;

		// Constructor
		iterator(T* p, const vector<T>* v) : ptr(p), vec(v) {}
		friend class vector<T>;
		friend class const_iterator;
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T* ptr;
		const vector<T>* vec;

	public:
		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, vec);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, vec);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (vec != rhs.vec) throw invalid_iterator();
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator temp = *this;
			++ptr;
			return temp;
		}
		const_iterator& operator++() {
			++ptr;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator temp = *this;
			--ptr;
			return temp;
		}
		const_iterator& operator--() {
			--ptr;
			return *this;
		}
		const T& operator*() const{
			return *ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}

		// Constructor
		const_iterator(const T* p, const vector<T>* v) : ptr(p), vec(v) {}
		const_iterator(const iterator &it) : ptr(it.ptr), vec(it.vec) {}
		friend class vector<T>;
	};

	// Vector data members
	T* data;
	size_t current_size;
	size_t current_capacity;

	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), current_size(0), current_capacity(0) {}

	vector(const vector &other) : current_size(other.current_size), current_capacity(other.current_capacity) {
		if (current_capacity == 0) {
			data = nullptr;
		} else {
			data = static_cast<T*>(operator new[](current_capacity * sizeof(T)));
			try {
				for (size_t i = 0; i < current_size; ++i) {
					new (data + i) T(other.data[i]);
				}
			} catch (...) {
				for (size_t j = 0; j < current_size; ++j) {
					data[j].~T();
				}
				operator delete[](data);
				throw;
			}
		}
	}

	/**
	 * TODO Destructor
	 */
	~vector() {
		clear();
		operator delete[](data);
	}

	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this == &other) return *this;

		vector temp(other);
		swap(temp);

		return *this;
	}

	void swap(vector &other) {
		T* temp_data = data;
		size_t temp_size = current_size;
		size_t temp_capacity = current_capacity;

		data = other.data;
		current_size = other.current_size;
		current_capacity = other.current_capacity;

		other.data = temp_data;
		other.current_size = temp_size;
		other.current_capacity = temp_capacity;
	}

	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}

	const T & at(const size_t &pos) const {
		if (pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}

	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}

	const T & operator[](const size_t &pos) const {
		if (pos >= current_size) throw index_out_of_bound();
		return data[pos];
	}

	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (current_size == 0) throw container_is_empty();
		return data[0];
	}

	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (current_size == 0) throw container_is_empty();
		return data[current_size - 1];
	}

	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data, this);
	}

	const_iterator begin() const {
		return const_iterator(data, this);
	}

	const_iterator cbegin() const {
		return const_iterator(data, this);
	}

	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data + current_size, this);
	}

	const_iterator end() const {
		return const_iterator(data + current_size, this);
	}

	const_iterator cend() const {
		return const_iterator(data + current_size, this);
	}

	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return current_size == 0;
	}

	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return current_size;
	}

	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < current_size; ++i) {
			data[i].~T();
		}
		current_size = 0;
	}

	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		if (pos.vec != this) throw invalid_iterator();

		size_t index = pos.ptr - data;
		if (current_size == current_capacity) {
			size_t new_capacity = (current_capacity == 0) ? 1 : current_capacity * 2;
			T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

			try {
				for (size_t i = 0; i < index; ++i) {
					new (new_data + i) T(data[i]);
				}

				new (new_data + index) T(value);

				for (size_t i = index; i < current_size; ++i) {
					new (new_data + i + 1) T(data[i]);
				}
			} catch (...) {
				for (size_t j = 0; j < index; ++j) {
					new_data[j].~T();
				}
				if (index <= current_size) {
					new_data[index].~T();
				}
				for (size_t j = index; j < current_size; ++j) {
					new_data[j + 1].~T();
				}
				operator delete[](new_data);
				throw;
			}

			for (size_t i = 0; i < current_size; ++i) {
				data[i].~T();
			}

			operator delete[](data);
			data = new_data;
			current_capacity = new_capacity;
		} else {
			for (size_t i = current_size; i > index; --i) {
				new (data + i) T(data[i - 1]);
			}
			data[index].~T();
			new (data + index) T(value);
		}

		++current_size;
		return iterator(data + index, this);
	}

	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > current_size) throw index_out_of_bound();
		return insert(iterator(data + ind, this), value);
	}

	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		if (pos.vec != this) throw invalid_iterator();

		size_t index = pos.ptr - data;
		if (index >= current_size) throw invalid_iterator();

		data[index].~T();

		for (size_t i = index; i < current_size - 1; ++i) {
			new (data + i) T(data[i + 1]);
			data[i + 1].~T();
		}

		--current_size;
		return iterator(data + index, this);
	}

	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= current_size) throw index_out_of_bound();
		return erase(iterator(data + ind, this));
	}

	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (current_size == current_capacity) {
			size_t new_capacity = (current_capacity == 0) ? 1 : current_capacity * 2;
			T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

			try {
				for (size_t i = 0; i < current_size; ++i) {
					new (new_data + i) T(data[i]);
				}
				new (new_data + current_size) T(value);
			} catch (...) {
				for (size_t j = 0; j < current_size; ++j) {
					new_data[j].~T();
				}
				operator delete[](new_data);
				throw;
			}

			for (size_t i = 0; i < current_size; ++i) {
				data[i].~T();
			}

			operator delete[](data);
			data = new_data;
			current_capacity = new_capacity;
		} else {
			new (data + current_size) T(value);
		}
		++current_size;
	}

	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (current_size == 0) throw container_is_empty();
		data[current_size - 1].~T();
		--current_size;
	}
};

// Iterator comparison operators
template<typename T>
bool vector<T>::iterator::operator==(const const_iterator &rhs) const {
	return ptr == rhs.ptr;
}

template<typename T>
bool vector<T>::iterator::operator!=(const const_iterator &rhs) const {
	return ptr != rhs.ptr;
}

}

#endif
