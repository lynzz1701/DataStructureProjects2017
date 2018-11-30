#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu {

	template<class T>
	class deque {
		struct node {
			T *data;
			node *prev, *next;

			node() :next(NULL), prev(NULL),data(NULL) {}
			node(const T &x, node * p = NULL, node * n = NULL) {
				data = new T(x);
				prev = p; next = n;
			}
			~node() { if (data) delete data; }
		};
		node *head;
		node *tail;

	public:
		class const_iterator;
		class iterator {
		public:
		    node *it;
			deque<T> *qPtr;
			iterator() { it = NULL; qPtr = NULL; }
			iterator(deque<T> &q, node *p = NULL) { qPtr = &q; it = p;}
			iterator(const iterator &other) { it = other.it; qPtr = other.qPtr; }
			iterator operator+(const int &n) const {
				if (n < 0) return this->operator-(-n);
				iterator res(*this);
				for(int i=0;i<n;i++){
					if(res.it==res.qPtr->tail) throw invalid_iterator();
					res.it = res.it->next;
				}
				return res;
			}
			iterator operator-(const int &n) const {
				if (n < 0) return this->operator+(-n);
				iterator res(*this);
				for (int i = 0; i<n; i++) {
					if (res.it == res.qPtr->head->next) throw invalid_iterator();
					res.it = res.it->prev;
				}
				return res;
			}
			int operator-(const iterator &rhs) const {
				if(qPtr!=rhs.qPtr) throw invalid_iterator();
				else {
					node *tmpL = rhs.it, *tmpR = rhs.it;
					int res = -1;
					while (++res!=-1) {
						if (tmpL == it) return -res;
						if (tmpR == it) return res;
						if(tmpL!=qPtr->head->next) tmpL = tmpL->prev;
						if(tmpR!=qPtr->tail) tmpR = tmpR->next;
					}
				}
			}
			iterator operator+=(const int &n) {
				if (n < 0) return this->operator-=(-n);
				for (int i = 0; i<n; i++) {
					if (it == qPtr->tail) throw invalid_iterator();
					it = it->next;
				}
				return *this;
			}
			iterator operator-=(const int &n) {
				if (n < 0) return this->operator+=(-n);
				for (int i = 0; i<n; i++) {
					if (it == qPtr->head->next) throw invalid_iterator();
					it = it->prev;
				}
				return *this;
			}
			iterator operator++(int) {
				if (it == qPtr->tail) throw invalid_iterator();
				iterator tmp = *this;
				it = it->next;
				return tmp;
			}
			iterator& operator++() {
				if (it == qPtr->tail) throw invalid_iterator();
				it = it->next;
				return *this;
			}
			iterator operator--(int) {
				if (it == qPtr->head->next) throw invalid_iterator();
				iterator tmp = *this;
				it = it->prev;
				return tmp;
			}
			iterator& operator--() {
				if (it == qPtr->head->next) throw invalid_iterator();
				it = it->prev;
				return *this;
			}
			T& operator*() const {
				if (it == qPtr->head || it == qPtr->tail) throw invalid_iterator();
				else return *(it->data);
			}
			T* operator->() const noexcept { return it->data; }
			bool operator==(const iterator &rhs) const { return (rhs.qPtr == this->qPtr&&rhs.it == this->it); }
			bool operator==(const const_iterator &rhs) const { return (rhs.qPtr == this->qPtr&&rhs.it == this->it); }
			bool operator!=(const iterator &rhs) const { return (rhs.qPtr != this->qPtr||rhs.it != this->it); }
			bool operator!=(const const_iterator &rhs) const { return (rhs.qPtr != this->qPtr || rhs.it != this->it); }
		};
		class const_iterator {
		public:
			node *it;
			const deque<T> *qPtr;
			const_iterator() { it = NULL; qPtr = NULL; }
			const_iterator(const deque<T> &q, node *p = NULL) { it = p; qPtr = &q; }
			const_iterator(const const_iterator &other) { it = other.it; qPtr = other.qPtr; }
			const_iterator(const iterator &other) { it = other.it; qPtr = other.qPtr; }
			const_iterator operator+(const int &n) const {
				if (n < 0) return this->operator-(-n);
				const_iterator res(*this);
				for (int i = 0; i<n; i++) {
					if (res.it == res.qPtr->tail) throw invalid_iterator();
					res.it = res.it->next;
				}
				return res;
			}
			const_iterator operator-(const int &n) const {
				if (n < 0) return this->operator+(-n);
				const_iterator res(*this);
				for (int i = 0; i<n; i++) {
					if (res.it == res.qPtr->head->next) throw invalid_iterator();
					res.it = res.it->prev;
				}
				return res;
			}
			int operator-(const const_iterator &rhs) const {
				if (qPtr != rhs.qPtr) throw invalid_iterator();
				else {
					node *tmpL = rhs.it, *tmpR = rhs.it;
					int res = -1;
					while (++res!=-1) {
						if (tmpL == it) return -res;
						if (tmpR == it) return res;
						if (tmpL != qPtr->head->next) tmpL = tmpL->prev;
						if (tmpR != qPtr->tail) tmpR = tmpR->next;
					}
				}
			}
			const_iterator operator+=(const int &n) {
				if (n < 0) return this->operator-=(-n);
				for (int i = 0; i<n; i++) {
					if (it == qPtr->tail) throw invalid_iterator();
					it = it->next;
				}
				return *this;
			}
			const_iterator operator-=(const int &n) {
				if (n < 0) return this->operator+=(-n);
				for (int i = 0; i<n; i++) {
					if (it == qPtr->head->next) throw invalid_iterator();
					it = it->prev;
				}
				return *this;
			}
			const_iterator operator++(int) {
				if (it == qPtr->tail) throw invalid_iterator();
				const_iterator tmp = *this;
				it = it->next;
				return tmp;
			}
			const_iterator& operator++() {
				if (it == qPtr->tail) throw invalid_iterator();
				it = it->next;
				return *this;
			}
			const_iterator operator--(int) {
				if (it == qPtr->head->next) throw invalid_iterator();
				const_iterator tmp = *this;
				it = it->prev;
				return tmp;
			}
			const_iterator& operator--() {
				if (it == qPtr->head->next) throw invalid_iterator();
				it = it->prev;
				return *this;
			}
			T& operator*() const {
				if (it == qPtr->head || it == qPtr->tail) throw invalid_iterator();
				else return *(it->data);
			}
			T* operator->() const noexcept { return it->data; }
			bool operator==(const iterator &rhs) const { return (rhs.qPtr == this->qPtr&&rhs.it == this->it); }
			bool operator==(const const_iterator &rhs) const { return (rhs.qPtr == this->qPtr&&rhs.it == this->it); }
			bool operator!=(const iterator &rhs) const { return (rhs.qPtr != this->qPtr || rhs.it != this->it); }
			bool operator!=(const const_iterator &rhs) const { return (rhs.qPtr != this->qPtr || rhs.it != this->it); }
		};
		deque() {
			head = new node;
			tail = new node;
			head->next = tail;
			tail->prev = head;
			head->data = NULL;
			tail->data = NULL;
		}
		deque(const deque &other) {
			node *p,*q;
			node *tmp;
			p = other.head->next;
			head = new node;
			q = head;
			while ( p != other.tail){
				tmp = new node(*(p->data));
				q->next = tmp;
				tmp->prev = q;
				p = p->next;
				q = q->next;
			}
			tail = new node;
			q->next = tail;
			tail->prev = q;
		}
		~deque(){
			node *p, *q;
			q = head;
			while (q != tail) {
				q = q->next;
				p = q->prev;
				delete p;
			}
			delete q;
		}
		deque &operator=(const deque &other)
		{
			if (this == &other) return *this;
			this->clear();
			node *p, *q;
			node *tmp;
			p = other.head->next;
			q = head;
			while (p != other.tail) {
				tmp = new node(*(p->data));
				q->next = tmp;
				tmp->prev = q;
				p = p->next;
				q = q->next;
			}
			q->next = tail;
			tail->prev = q;

			return *this;
		}
		T & at(const size_t &pos){
			node *p;
			p = head;
			for (size_t i = 0; i <= pos; i++){
				p = p->next;
				if (p == tail) throw index_out_of_bound();
			}

			return *(p->data);
		}
		const T & at(const size_t &pos) const{
			node *p;
			p = head;
			for (size_t i = 0; i <= pos; i++) {
				p = p->next;
				if (p == tail) throw index_out_of_bound();
			}

			return *(p->data);
		}
		T & operator[](const size_t &pos){
			node *p;
			p = head;
			for (size_t i = 0; i <= pos; i++) {
				p = p->next;
				if (p == tail) throw index_out_of_bound();
			}

			return *(p->data);
		}
		const T & operator[](const size_t &pos) const {
			node *p;
			p = head;
			for (size_t i = 0; i <= pos; i++) {
				p = p->next;
				if (p == tail) throw index_out_of_bound();
			}

			return *(p->data);
		}
		const T & front() const {
			if (this->empty()) throw container_is_empty();
			else return *((head->next)->data);
		}
		const T & back() const {
			if (this->empty()) throw container_is_empty();
			else return *((tail->prev)->data);
		}
		iterator begin() {
			iterator p(*this);
			p.it = head->next;
			return p;
		}
		const_iterator cbegin() const {
			const_iterator p(*this);
			p.it = head->next;
			return p;
		}
		iterator end() {
			iterator p(*this);
			p.it = tail;
			return p;
		}
		const_iterator cend() const {
			const_iterator p(*this);
			p.it = tail;
			return p;
		}
		bool empty() const {
			if (head->next == tail) return true;
			else return false;
		}
		size_t size() const {
			size_t res = 0;
			node *p;
			p = head;
			while (true){
				p = p->next;
				if (p == tail) break;
				res++;
			}
			return res;
		}
		void clear() {
			node *p, *q;
			q = head->next;
			while (q != tail) {
				q = q->next;
				p = q->prev;
				delete p;
			}
			head->next = tail;
			tail->prev = head;
		}
		iterator insert(iterator pos, const T &value)
		{
			if (this != pos.qPtr) throw invalid_iterator();
			//头结点不能插入 wrong place
			node *tmp;
			tmp = new node(value);
			tmp->next = pos.it;
			tmp->prev = pos.it->prev;
			pos.it->prev->next = tmp;
			pos.it->prev = tmp;
			iterator res(*this);
			res.it = tmp;
			return res;
		}
		iterator erase(iterator pos) {
			if (this->empty()) throw container_is_empty();
			else if (this != pos.qPtr) throw invalid_iterator();
			//wrong place;
			pos++;
			node *del = pos.it->prev;
			del->prev->next = pos.it;
			pos.it->prev = del->prev;
			delete del;
			return pos;
		}
		void push_back(const T &value) {
			node *tmp;
			tmp = new node(value);
			tmp->next = tail;
			tmp->prev = tail->prev;
			tail->prev->next = tmp;
			tail->prev = tmp;
		}
		void pop_back() {
			if (this->empty()) throw container_is_empty();
			node *del = tail->prev;
			del->prev->next = tail;
			tail->prev = del->prev;
			delete del;
		}
		void push_front(const T &value) {
			node *tmp;
			tmp = new node(value);
			tmp->prev = head;
			tmp->next = head->next;
			head->next->prev = tmp;
			head->next = tmp;
		}
		void pop_front() {
			if (this->empty()) throw container_is_empty();
			node *del = head->next;
			del->next->prev = head;
			head->next = del->next;
			delete del;
		}
	};

}

#endif
