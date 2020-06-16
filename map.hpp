/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	enum color_type{RED, BLACK};

	bool le(const Key &a, const Key &b)const {Compare res;return res(a,b);}
	bool equ(const Key &a, const Key &b)const {return (!le(a,b)) && (!le(b,a));}

	struct node{
	    node *fa, *left, *right;
	    value_type *val;
	    color_type col;

	    node (){
	        fa = nullptr; left = nullptr; right = nullptr;
	        val = nullptr; col = BLACK;
	    }

	    node(const value_type &_val, color_type _col = RED, node *_nil = nullptr){
	        fa = _nil; left = _nil; right = _nil;
	        val = new value_type(_val);
	        col = _col;
	    }

        node &operator=(const node &other){
	        if(this == &other) return *this;
	        if(val) delete val;
	        val = nullptr;

            fa = other.fa; left = other.left; right = other.right;
            if(other.val) val = new value_type(*other.val);
            col = other.col;
            return *this;
	    }

	    ~node() {if(val) delete val;}
	};

	node *root, *nil;
	int siz;

	node* get_next(node *x)const {
	    if(x->right != nil) {
            node *res = x->right;
            while (res->left != nil) res = res->left;
            return res;
        }

	    while(x->fa != nil){
	        if(x->fa->left == x) return x->fa;
	        x = x->fa;
	    }
	    return nullptr;
	}

    node* get_prev(node *x) const{
        if(x->left != nil) {
            node *res = x->left;
            while (res->right != nil) res = res->right;
            return res;
        }

        while(x->fa != nil){
            if(x->fa->right == x) return x->fa;
            x = x->fa;
        }
        return nullptr;
    }

    node* get_lefest() const{
        node *res = root;
        while(res->left != nil) res = res->left;
        return res;
    }

    node* get_rightest()const {
        node *res = root;
        while(res->right != nil) res = res->right;
        return res;
    }

    class const_iterator;
	class iterator {
	public:
		node *p;
		map *bel;
		bool is_end;

		iterator(node *_p = nullptr, map *_bel = nullptr, bool _is_end = false) {
		    p = _p; bel = _bel; is_end = _is_end;
		}
		iterator(const iterator &other) {
		    p = other.p; bel = other.bel; is_end = other.is_end;
		}

		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    if(is_end) throw invalid_iterator();
		    iterator res = *this;
		    p = bel->get_next(p);
		    if(p == nullptr) is_end = true;
            return res;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
		    if(is_end) throw invalid_iterator();
            p = bel->get_next(p);
            if(p == nullptr) is_end = true;
            return (*this);
		}

		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator res = (*this);
            if(is_end){
		        if(bel->empty()) throw invalid_iterator();
		        p = bel->get_rightest();
		        is_end = false;
		    }
		    else p = bel->get_prev(p);
		    if(p == nullptr) throw index_out_of_bound();
		    return res;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
		    if(is_end){
                if(bel->empty()) throw invalid_iterator();
                p = bel->get_rightest();
                is_end = false;
            }
            else p = bel->get_prev(p);
            if(p == nullptr) throw index_out_of_bound();
            return (*this);
		}

		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {return *(p->val);}
		bool operator==(const iterator &rhs) const {return p == rhs.p && bel == rhs.bel && is_end == rhs.is_end;}
		bool operator==(const const_iterator &rhs) const {return p == rhs.p && bel == rhs.bel && is_end == rhs.is_end;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return p != rhs.p || bel != rhs.bel || is_end != rhs.is_end;}
		bool operator!=(const const_iterator &rhs) const {return p != rhs.p || bel != rhs.bel || is_end != rhs.is_end;}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {return p->val;}
	};
	class const_iterator {
		public:
            node *p;
            map *bel;
            bool is_end;

            const_iterator(node *_p = nullptr, map *_bel = nullptr, bool _is_end = false) {
                p = _p; bel = _bel; is_end = _is_end;
            }
            const_iterator(const iterator &other) {
                p = other.p; bel = other.bel; is_end = other.is_end;
            }
            const_iterator(const const_iterator &other) {
                p = other.p; bel = other.bel; is_end = other.is_end;
            }
            /**
            * TODO iter++
            */
            const_iterator operator++(int) {
                if(is_end) throw invalid_iterator();
                const_iterator res = *this;
                p = bel->get_next(p);
                if(p == nullptr) is_end = true;
                return res;
            }
            /**
             * TODO ++iter
             */
            const_iterator & operator++() {
                if(is_end) throw invalid_iterator();
                p = bel->get_next(p);
                if(p == nullptr) is_end = true;
                return (*this);
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator res = (*this);
                if(is_end){
                    if(bel->empty()) throw invalid_iterator();
                    p = bel->get_rightest();
                    is_end = false;
                }
                else p = bel->get_prev(p);
                if(p == nullptr) throw index_out_of_bound();
                return res;
            }
            /**
             * TODO --iter
             */
            const_iterator & operator--() {
                if(is_end){
                    if(bel->empty()) throw invalid_iterator();
                    p = bel->get_rightest();
                    is_end = false;
                }
                else p = bel->get_prev(p);
                if(p == nullptr) throw index_out_of_bound();
                return (*this);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type & operator*() const {return *(p->val);}
            bool operator==(const iterator &rhs) const {return p == rhs.p && bel == rhs.bel && is_end == rhs.is_end;}
            bool operator==(const const_iterator &rhs) const {return p == rhs.p && bel == rhs.bel && is_end == rhs.is_end;}
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {return p != rhs.p || bel != rhs.bel || is_end != rhs.is_end;}
            bool operator!=(const const_iterator &rhs) const {return p != rhs.p || bel != rhs.bel || is_end != rhs.is_end;}

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept {return p->val;}
	};

    void left_rotate(node *x){
        node *y = x->right;
        x->right = y->left;
        if(y->left != nil) y->left->fa = x;
        y->fa = x->fa;
        if(x->fa == nil) root = y;
        else if(x == x->fa->left) x->fa->left = y;
        else x->fa->right = y;
        y->left = x;
        x->fa = y;
    }

    void right_rotate(node *x){
	    node *y = x->left;
	    x->left = y->right;
	    if(y->right != nil) y->right->fa = x;
	    y->fa = x->fa;
	    if(x->fa == nil) root = y;
	    else if(x == x->fa->left) x->fa->left = y;
	    else x->fa->right = y;
	    y->right = x;
	    x->fa = y;
	}

	void rbtree_copy(node *x, node *y, node *ot_nil){// x = y
        if(y->left != ot_nil){
	        x->left = new node;
	        if(y->left->val) x->left->val = new value_type(*y->left->val);
	        x->left->left = nil; x->left->right = nil;
	        x->left->col = y->left->col;

	        x->left->fa = x;
	        rbtree_copy(x->left, y->left, ot_nil);
	    }
	    else x->left = nil;

        if(y->right != ot_nil){
            x->right = new node;
            if(y->right->val) x->right->val = new value_type(*y->right->val);
            x->right->left = nil; x->right->right = nil;
            x->right->col = y->right->col;

            x->right->fa = x;
            rbtree_copy(x->right, y->right, ot_nil);
        }
        else x->right = nil;
	}

	void rb_clear(node *x){
	    if(x == nullptr || x == nil) return;
	    rb_clear(x->left); rb_clear(x->right);
	    delete x;
	}

	/**
	 * TODO two constructors
	 */
	map() {
	    nil = new node;
	    nil->fa = nil; nil->left = nil; nil->right = nil;
	    nil->col = BLACK; nil->val = nullptr;
	    root = nil; siz = 0;
	}

	map(const map &other) {
	    siz = other.siz;
        nil = new node;
        nil->fa = nil; nil->left = nil; nil->right = nil;
        nil->col = BLACK; nil->val = nullptr;
        root = nil;

        if(other.root == other.nil) return;
	    root = new node;
	    root->fa = nil; root->left = nil; root->right = nil;
	    root->col = BLACK; root->val = new value_type (*other.root->val);

        rbtree_copy(root, other.root, other.nil);
	}

	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
	    if((&other) == this) return (*this);
	    this->~map();

        nil = new node;
        nil->fa = nil; nil->left = nil; nil->right = nil;
        nil->col = BLACK; nil->val = nullptr;
        root = nil;

        siz = other.siz;
        if(other.root != other.nil){
            root = new node;
            root->fa = nil; root->left = nil; root->right = nil;
            root->col = BLACK; root->val = new value_type (*other.root->val);
            rbtree_copy(root, other.root, other.nil);
        }
        return *this;
	}

	/**
	 * TODO Destructors
	 */
	~map() {
	    rb_clear(root);
	    if(nil) delete nil;
	}

	node* find_kth(const Key &key) const{
	    node *x = root;
	    while(x != nil){
            if(equ(x->val->first, key)) return x;
	        if(le(key, x->val->first)) x = x->left;
	        else x = x->right;
	    }
	    return nullptr;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
	    node *res = find_kth(key);
	    if(res == nullptr) throw index_out_of_bound();
	    return res->val->second;
	}
	const T & at(const Key &key) const {
        node *res = find_kth(key);
        if(res == nullptr) throw index_out_of_bound();
        return res->val->second;
    }

    void rb_insert_fixup(node *z){
	    node *y;
	    while(z->fa->col == RED){
	        if(z->fa == z->fa->fa->left){
	            y = z->fa->fa->right;
	            if(y->col == RED){
	                z->fa->col = BLACK;
	                y->col = BLACK;
	                z->fa->fa->col = RED;
	                z = z->fa->fa;
	            }
	            else{
	                if(z == z->fa->right){
	                    z = z->fa;
	                    left_rotate(z);
	                }
	                z->fa->col = BLACK;
	                z->fa->fa->col = RED;
	                right_rotate(z->fa->fa);
	            }
	        }
	        else{
                y = z->fa->fa->left;
                if(y->col == RED){
                    z->fa->col = BLACK;
                    y->col = BLACK;
                    z->fa->fa->col = RED;
                    z = z->fa->fa;
                }
                else{
                    if(z == z->fa->left){
                        z = z->fa;
                        right_rotate(z);
                    }
                    z->fa->col = BLACK;
                    z->fa->fa->col = RED;
                    left_rotate(z->fa->fa);
                }
            }
	    }
	    root->col = BLACK;
	}

    node* rb_insert(node *z){
	    ++siz;
	    node *y = nil, *x = root;
	    while(x != nil){
	        y = x;
	        if(le(z->val->first, x->val->first)) x = x->left;
	        else x = x->right;
	    }

	    z->fa = y;
	    if(y == nil) root = z;
	    else if(le(z->val->first, y->val->first)) y->left = z;
	    else y->right = z;
	    z->left = nil; z->right = nil;
	    z->col = RED;
	    rb_insert_fixup(z);
	    return z;
	}

	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
	    node *res = find_kth(key);
        if(res == nullptr){
            node *tmp = new node(value_type(key, T()));
            res = rb_insert(tmp);
        }
	    return res->val->second;
	}

	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
	    return at(key);
	}

	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
	    if(empty()) return iterator(nullptr,this,true);
	    node *res = get_lefest();
	    return iterator(res,this,false);
	}
	const_iterator cbegin() const {
	    if(empty()) return const_iterator(nullptr, const_cast<map*>(this),true);
	    node *res = get_lefest();
	    return const_iterator(res, const_cast<map*>(this),false);
	}

	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {return iterator(nullptr,this,true);}
	const_iterator cend() const {return const_iterator(nullptr, const_cast<map*>(this),true);}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {return siz == 0;}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {return siz;}

	void clear() {
	    this->~map();
        nil = new node;
        nil->fa = nil; nil->left = nil; nil->right = nil;
        nil->col = BLACK; nil->val = nullptr;
        root = nil; siz = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion),
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
        node *res = find_kth(value.first);
        if(res == nullptr){
            node *tmp = new node(value);
            res = rb_insert(tmp);
            return pair<iterator, bool>(iterator(res,this,false),true);
        }
        else return pair<iterator, bool>(iterator(res,this,false),false);
	}

	void transplant(node *u, node *v){// let u = v
	    if(u->fa == nil) root = v;
	    else {
            if (u == u->fa->left) u->fa->left = v;
            else u->fa->right = v;
        }
	    v->fa = u->fa;
	}

	void rb_delete_fixup(node *x){
	    node *w;
	    while(x != root && x->col == BLACK){
	        if(x == x->fa->left){
	            w = x->fa->right;
	            if(w->col == RED){
	                w->col = BLACK;
	                x->fa->col = RED;
	                left_rotate(x->fa);
	                w = x->fa->right;
	            }
	            if(w->left->col == BLACK && w->right->col == BLACK){
	                w->col = RED; x = x->fa;
	            }
	            else{
	                if(w->right->col == BLACK){
	                    w->left->col = BLACK;
	                    w->col = RED;
	                    right_rotate(w);
	                    w = x->fa->right;
	                }
	                w->col = x->fa->col;
	                x->fa->col = BLACK;
	                w->right->col = BLACK;
	                left_rotate(x->fa);
	                x = root;
	            }
	        }
	        else{
                w = x->fa->left;
                if(w->col == RED){
                    w->col = BLACK;
                    x->fa->col = RED;
                    right_rotate(x->fa);
                    w = x->fa->left;
                }
                if(w->left->col == BLACK && w->right->col == BLACK){
                    w->col = RED; x = x->fa;
                }
                else{
                    if(w->left->col == BLACK){
                        w->right->col = BLACK;
                        w->col = RED;
                        left_rotate(w);
                        w = x->fa->left;
                    }
                    w->col = x->fa->col;
                    x->fa->col = BLACK;
                    w->left->col = BLACK;
                    right_rotate(x->fa);
                    x = root;
                }
            }
	    }
	    x->col = BLACK;
    }

	void rb_delete(node *z){
	    node *y = z, *x;
	    color_type y_original_color = y->col;
	    if(z->left == nil) {x = z->right; transplant(z,z->right);}
	    else if(z->right == nil) {x = z->left; transplant(z,z->left);}
	    else{
	        y = z->right;
	        while(y->left != nil) {
	            y = y->left;
	        }

	        x = y->right;
	        y_original_color = y->col;

	        if(y->fa == z) x->fa = y;
	        else{
	            transplant(y,y->right);
	            y->right = z->right;
	            y->right->fa = y;
	        }

	        transplant(z, y);
	        y->left = z->left;
	        y->left->fa = y;
            y->col = z->col;
	    }

	    if(y_original_color == BLACK)
	        rb_delete_fixup(x);
	    delete z;
	}

	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
	    if(pos.p == nullptr || siz == 0 || pos.bel != this) throw invalid_iterator();
	    --siz;
        rb_delete(pos.p);
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
	    node *res = find_kth(key);
	    if(res == nullptr)return 0;
	    return 1;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
	    node *res = find_kth(key);
	    if(res == nullptr) return end();
	    return iterator(res, this, false);
	}
	const_iterator find(const Key &key) const {
	    node *res = find_kth(key);
	    if(res == nullptr) return cend();
	    return const_iterator(res, const_cast<map*>(this), false);
	}
};

}

#endif
