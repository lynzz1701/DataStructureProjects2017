/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

	template< class Key, class T, class Compare = std::less<Key>>
	class map {
	public:
		typedef pair<const Key, T> value_type;
	private:
		struct RedBlackNode {
			value_type *data;
			RedBlackNode *left;
			RedBlackNode *right;
			RedBlackNode *prev;
			RedBlackNode *next;
			int colour; //0-red,1-black

			RedBlackNode() :data(NULL), left(NULL), right(NULL), prev(NULL), next(NULL), colour(0) {}
			RedBlackNode(const value_type &element, RedBlackNode *lt = NULL, RedBlackNode *rt = NULL, RedBlackNode *pt = NULL, RedBlackNode *nt = NULL, int h = 0) {
				data = new value_type(element);
				left = lt; right = rt;
				prev = pt; next = nt;
				colour = h;
			}
			~RedBlackNode() { if (data != NULL) delete data;}
		};

		class linkStack {
		private:
			struct node {
				RedBlackNode *data;
				node *next;
				node(RedBlackNode *x = NULL, node *N = NULL) { data = x; next = N; }
				~node() {}
			};
			node *elem;

		public:
			linkStack() { elem = NULL; }
			~linkStack() {
				node *tmp;
				while (elem != NULL) {
					tmp = elem;
					elem = elem->next;
					delete tmp;
				}
			}
			bool isEmpty() const { return elem == NULL; }
			void push(RedBlackNode *x) {
				node *tmp = new node(x, elem);
				elem = tmp;
			}
			RedBlackNode* pop(){
				node *tmp = elem;
				RedBlackNode *x = tmp->data;
				elem = elem->next;
				delete tmp;
				return x;
			}
			RedBlackNode* top() const {
				if (elem == NULL) return NULL;
				return elem->data;
			}
		};

		RedBlackNode *root;
		RedBlackNode *head;
		RedBlackNode *tail;
		Compare compare;
		size_t siz;

	public:
		class const_iterator;
		class iterator {
		public:
			RedBlackNode *it;
			map<Key, T, Compare> *mPtr;
			iterator() { it = NULL; mPtr = NULL; }
			iterator(map<Key, T, Compare> &m, RedBlackNode *p = NULL) { mPtr = &m; it = p; }
			iterator(const iterator &other) { it = other.it; mPtr = other.mPtr; }
			iterator operator++(int) {
				if (it == mPtr->tail) throw invalid_iterator();
				iterator tmp(*this);
				it = it->next;
				return tmp;
			}
			iterator & operator++() {
				if (it == mPtr->tail) throw invalid_iterator();
				it = it->next;
				return *this;
			}
			iterator operator--(int) {
				if (it == mPtr->head->next) throw invalid_iterator();
				iterator tmp(*this);
				it = it->prev;
				return tmp;
			}
			iterator & operator--() {
				if (it == mPtr->head->next) throw invalid_iterator();
				it = it->prev;
				return *this;
			}
			bool operator==(const iterator &rhs) const { return (rhs.mPtr == mPtr&&rhs.it == it); }
			bool operator==(const const_iterator &rhs) const { return (rhs.mPtr == mPtr&&rhs.it == it); }
			bool operator!=(const iterator &rhs) const { return (rhs.mPtr != mPtr || rhs.it != it); }
			bool operator!=(const const_iterator &rhs) const { return (rhs.mPtr != mPtr || rhs.it != it); }
			value_type & operator*() const {
				if (it == mPtr->head || it == mPtr->tail) throw invalid_iterator();
				else return *(it->data);
			}
			value_type* operator->() const noexcept { return it->data; }
		};
		class const_iterator {
		public:
			RedBlackNode *it;
			const map<Key, T, Compare> *mPtr;
			const_iterator() { it = NULL; mPtr = NULL; }
			const_iterator(const map<Key, T, Compare> &m, RedBlackNode *p = NULL) { mPtr = &m; it = p; }
			const_iterator(const iterator &other) { it = other.it; mPtr = other.mPtr; }
			const_iterator(const const_iterator &other) { it = other.it; mPtr = other.mPtr; }
			const_iterator operator++(int) {
				if (it == mPtr->tail) throw invalid_iterator();
				const_iterator tmp(*this);
				it = it->next;
				return tmp;
			}
			const_iterator & operator++() {
				if (it == mPtr->tail) throw invalid_iterator();
				it = it->next;
				return *this;
			}
			const_iterator operator--(int) {
				if (it == mPtr->head->next) throw invalid_iterator();
				const_iterator tmp(*this);
				it = it->prev;
				return tmp;
			}
			const_iterator & operator--() {
				if (it == mPtr->head->next) throw invalid_iterator();
				it = it->prev;
				return *this;
			}
			bool operator==(const iterator &rhs) const { return (rhs.mPtr == mPtr&&rhs.it == it); }
			bool operator==(const const_iterator &rhs) const { return (rhs.mPtr == mPtr&&rhs.it == it); }
			bool operator!=(const iterator &rhs) const { return (rhs.mPtr != mPtr || rhs.it != it); }
			bool operator!=(const const_iterator &rhs) const { return (rhs.mPtr != mPtr || rhs.it != it); }
			value_type & operator*() const {
				if (it == mPtr->head || it == mPtr->tail) throw invalid_iterator();
				else return *(it->data);
			}
			value_type* operator->() const noexcept { return it->data; }
		};
		map() {
			root = NULL;
			head = new RedBlackNode;
			tail = new RedBlackNode;
			head->next = tail;
			tail->prev = head;
			siz = 0;
		}
		map(const map &other) {
			head = new RedBlackNode;
			tail = new RedBlackNode;
			head->next = tail;
			tail->prev = head;
			if (other.empty()) { root = NULL; siz = 0; }
			else {
				root = new RedBlackNode;
				copyNode(root, other.root);
				siz = other.siz;
			}
		}
		map & operator=(const map &other) {
			if (this == &other) return *this;
			this->clear();
			head = new RedBlackNode;
			tail = new RedBlackNode;
			head->next = tail;
			tail->prev = head;
			if (other.empty()) { root = NULL; siz = 0; }
			else {
				root = new RedBlackNode;
				copyNode(root, other.root);
				siz = other.siz;
			}
			return *this;
		}
		~map() {
			RedBlackNode *p, *q;
			q = head;
			while (q != tail) {
				q = q->next;
				p = q->prev;
				delete p;
			}
			delete q;
		}
		T & at(const Key &key) {
			RedBlackNode *t = root;
			while (t != NULL&& (compare(key, t->data->first) || compare(t->data->first, key))) {
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t != NULL) return t->data->second;
			else throw index_out_of_bound();
		}
		const T & at(const Key &key) const {
			RedBlackNode *t = root;
			while (t != NULL&& (compare(key, t->data->first) || compare(t->data->first, key))) {
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t != NULL) return t->data->second;
			else throw index_out_of_bound();
		}
		T & operator[](const Key &key) {
			RedBlackNode *t = root;
			while (t != NULL&&(compare(key,t->data->first)||compare(t->data->first,key))){
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t != NULL) return t->data->second;
			else {
				pair<iterator, bool> ans = this->insert(value_type(key, T()));
				return ans.first.it->data->second;
			}
		}
		const T & operator[](const Key &key) const {
			RedBlackNode *t = root;
			while (t != NULL&& (compare(key, t->data->first) || compare(t->data->first, key))) {
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t != NULL) return t->data->second;
			else throw index_out_of_bound();
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
		bool empty() const { return siz == 0; }
		size_t size() const { return siz; }
		void clear() {
			RedBlackNode *p, *q;
			q = head->next;
			while (q != tail) {
				q = q->next;
				p = q->prev;
				delete p;
			}
			head->next = tail;
			tail->prev = head;
			root = NULL;
			siz = 0;
		}
		pair<iterator, bool> insert(const value_type &x) {
			pair<iterator, bool> ans;
			ans.first.mPtr = this;
			linkStack path;//path保存根到被调整结点的路径
			RedBlackNode *t, *parent;

			if (root == NULL) {//在空树上插入
				root = new RedBlackNode(x, NULL, NULL, head, tail, 1);
				siz++;
				head->next = root;
				tail->prev = root;
				ans.first.it = root;
				ans.second = true;
				return ans;
			}

			t = root;
			while (t != NULL && (compare(t->data->first, x.first) || compare(x.first, t->data->first))) {//寻找插入位置，并将路径信息放入栈中
				path.push(t);
				if (compare(t->data->first, x.first)) t = t->right;
				else t = t->left;
			}
			if (t != NULL) {//找到重复结点，放弃插入
				ans.first.it = t;
				ans.second = false;
				return ans;
			}
			//执行插入过程
			t = new RedBlackNode(x, NULL, NULL, NULL, NULL);
			siz++; 
			parent = path.pop();
			if (compare(x.first, parent->data->first)) {
				parent->left = t;
				t->next = parent;
				t->prev = parent->prev;
				parent->prev = t;
				t->prev->next = t;
			}
			else {
				parent->right = t;
				t->prev = parent;
				t->next = parent->next;
				parent->next = t;
				t->next->prev = t;
			}

			ans.first.it = t;
			ans.second = true;

			if (parent->colour == 0) {//如果父结点不是黑色，需要调整
									  //将父结点压入栈中，从插入结点开始调整平衡
				path.push(parent);
				insertReBalance(t, path);
			}
			return ans;
		}
		void erase(iterator pos) {
			if (this->empty()) throw invalid_iterator();
			if (pos.mPtr != this) throw invalid_iterator();
			if (pos == this->end()) throw index_out_of_bound();

			linkStack path;
			RedBlackNode *t = root, *old, *parent = NULL;
			bool flag = false;

			while (t != NULL && (compare(t->data->first, (*pos).first) || compare((*pos).first, t->data->first))) {//寻找删除结点，并保存路径
				path.push(t);
				if (compare((*pos).first, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t == NULL) return;//没有找到被删结点，放弃删除
			 //找替代结点并替代
			RedBlackNode *parent2 = NULL;
			if (t->left != NULL&&t->right != NULL) {
				parent2 = path.top();
				path.push(t->next);
				old = t;
				t = t->right;
				while (t->left != NULL) {
					path.push(t);
					t = t->left;
				}
				flag = true;
			}

			//执行删除
			siz--;
			if (t == root) {//删除根结点
				root = (t->left ? t->left : t->right);
				if (root != NULL) root->colour = 1;
				t->prev->next = t->next;
				t->next->prev = t->prev;
				delete t;
				return;
			}
			//删除叶结点或只有一个儿子的结点
			parent = path.pop();
			old = t;
			t = (t->left ? t->left : t->right);
			if (parent->left == old) parent->left = t;
			else parent->right = t;

			bool flag2 = (old->colour == 0);
			if (flag) {
				RedBlackNode *del = old->prev;
				if (parent2 == NULL) root = old;
				else if (parent2->left == del) parent2->left = old;
				else parent2->right = old;

				if (del->right == old) old->right = del->right->right;
				else old->right = del->right;

				old->left = del->left;
				old->colour = del->colour;
				old->prev = del->prev;
				del->prev->next = old;
				delete del;
			}
			else {
				old->prev->next = old->next;
				old->next->prev = old->prev;
				delete old;
			}
			if (flag2) return;
			if (t != NULL) { t->colour = 1; return; }//有一个红儿子
			 //删除的是黑儿子，开始调整
			path.push(parent);
			removeReBalance(t, path);
		}
		size_t count(const Key &key) const {
			RedBlackNode *t = root;
			while (t != NULL && (compare(t->data->first, key) || compare(key, t->data->first))) {
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t == NULL) return 0;
			else return 1;
		}
		iterator find(const Key &key) {
			RedBlackNode *t = root;
			while (t != NULL && (compare(t->data->first, key) || compare(key, t->data->first))) {
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t == NULL) return this->end();
			else return iterator(*this, t);
		}
		const_iterator find(const Key &key) const {
			RedBlackNode *t = root;
			while (t != NULL && (compare(t->data->first, key) || compare(key, t->data->first))) {
				if (compare(key, t->data->first)) t = t->left;
				else t = t->right;
			}
			if (t == NULL) return this->cend();
			else return const_iterator(*this, t);
		}

	private:
		void copyNode(RedBlackNode *newp, RedBlackNode *oldp) {
			if (oldp->left != NULL) {
				newp->left = new RedBlackNode;
				copyNode(newp->left, oldp->left);
			}
			newp->data = new value_type(*(oldp->data));
			newp->colour = oldp->colour;
			newp->prev = tail->prev;
			newp->next = tail;
			tail->prev->next = newp;
			tail->prev = newp;
			if (oldp->right != NULL) {
				newp->right = new RedBlackNode;
				copyNode(newp->right, oldp->right);
			}
		}
		void makeEmpty(RedBlackNode * &t) {
			if (t != NULL) {
				makeEmpty(t->left);
				makeEmpty(t->right);
				delete t;
			}
			t = NULL;
		}
		void LL(RedBlackNode * &t) {
			RedBlackNode *t1 = t->left;
			t->left = t1->right;
			t1->right = t;
			t = t1;
		}
		void RR(RedBlackNode * &t) {
			RedBlackNode *t1 = t->right;
			t->right = t1->left;
			t1->left = t;
			t = t1;
		}
		void LR(RedBlackNode * &t) {
			RR(t->left);
			LL(t);
		}
		void RL(RedBlackNode * &t) {
			LL(t->right);
			RR(t);
		}
		void reLink(RedBlackNode *oldp, RedBlackNode *newp, linkStack &path){//path保存根到被调整结点的路径
			if (path.isEmpty()) root = newp;//路径为空，则新结点是树根
			else {
				RedBlackNode *grandParent = path.pop();//弹出新结点的父亲
				if (grandParent->left == oldp) grandParent->left = newp;
				else grandParent->right = newp;
				path.push(grandParent);
			}
		}
		void insertReBalance(RedBlackNode *t, linkStack &path) {
			RedBlackNode *parent, *grandParent, *uncle, *rootOfSubTree;
			parent = path.pop();
			while (parent->colour == 0) {//当父结点是红色时，需要调整
				if (parent == root) { parent->colour = 1; return; }//父结点是根

				grandParent = rootOfSubTree = path.pop();
				if (compare(parent->data->first,grandParent->data->first)) uncle = grandParent->right;
				else uncle = grandParent->left;//找出叔叔结点

				if (uncle == NULL || uncle->colour == 1) {//情况一
					if (grandParent->left == parent) {
						if (t == parent->left) {            //LLb
							parent->colour = 1;
							grandParent->colour = 0;
							LL(grandParent);
						}
						else {                              //LRb
							grandParent->colour = 0;
							t->colour = 1;
							LR(grandParent);
						}
					}
					else{
					    if (t == parent->right) {           //RRb
						    parent->colour = 1;
						    grandParent->colour = 0;
						    RR(grandParent);
					    }
					    else {                              //RLb
					    	grandParent->colour = 0;
					    	t->colour = 1;
					    	RL(grandParent);
					    }
					}
					reLink(rootOfSubTree, grandParent, path);
					return;
				}
				else {                                      //情况二
					grandParent->colour = 0;
					parent->colour = 1;
					uncle->colour = 1;
					if (root == grandParent) { root->colour = 1; return; }
					t = grandParent;
					parent = path.pop();
				}
			}
		}
		void removeReBalance(RedBlackNode *t, linkStack &path)
		{
			RedBlackNode *parent, *sibling = NULL, *rootOfSubTree;
			parent = rootOfSubTree = path.pop();

			while (parent != NULL){
				if (parent->left == t)  sibling = parent->right;
				else  sibling = parent->left;

				if (sibling->colour == 0)	{    //情况二
					sibling->colour = 1;
					parent->colour = 0;
					if (parent->left == t) RR(parent);
					else LL(parent);
					reLink(rootOfSubTree, parent, path);
					path.push(parent);
					parent = rootOfSubTree;
				}
				else{ //兄弟是黑结点
					if ((sibling->left == NULL || sibling->left->colour == 1) && (sibling->right == NULL || sibling->right->colour == 1)){
						//L0或R0
						sibling->colour = 0;
						if (parent->colour == 0) { parent->colour = 1; return; }
						else{             
							t = parent;
							if (t == root) return;
							else parent = rootOfSubTree = path.pop();
						}
					}
					else break;
				}

			}//end of while

			if (parent->left == t){//兄弟是右儿子
				if (sibling->left != NULL && sibling->left->colour == 0){
					sibling->left->colour = parent->colour;
					parent->colour = 1;
					RL(parent);
					reLink(rootOfSubTree, parent, path);
				}
				else{
					sibling->colour = parent->colour;
					sibling->right->colour = 1;
					parent->colour = 1;
					RR(parent);
					reLink(rootOfSubTree, parent, path);
				}
			}
			else{                    //兄弟是左儿子
				if (sibling->right != NULL && sibling->right->colour == 0){
					sibling->right->colour = parent->colour;
					parent->colour = 1;
					LR(parent);
					reLink(rootOfSubTree, parent, path);
				}
				else{
					sibling->colour = parent->colour;
					sibling->left->colour = 1;
					parent->colour = 1;
					LL(parent);
					reLink(rootOfSubTree, parent, path);
				}

			}
		}
	};

}
#endif
