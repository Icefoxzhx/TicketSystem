#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {

    template <class T>
    class vector {
    private:
        T **arr;
        size_t v_size, v_capacity;

    public:

        class const_iterator;
        class iterator {
        private:
            T **it_pos; vector *it_bel;

        public:
            iterator(T **tmp_pos = NULL, vector *tmp_bel = NULL) : it_pos(tmp_pos), it_bel(tmp_bel){}

            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
            iterator operator+(const int &n) const {
                iterator tmp = (*this);
                tmp.it_pos += n;
                return tmp;
            }

            iterator operator-(const int &n) const {
                iterator tmp = (*this);
                tmp.it_pos -= n;
                return tmp;
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if(rhs.it_bel != it_bel) throw invalid_iterator();
                int res = rhs.it_pos - it_pos;
                return (res < 0) ? (-res) : res;
            }

            iterator& operator+=(const int &n) {
                (*this) = (*this) + n;
                return (*this);
            }
            iterator& operator-=(const int &n) {
                (*this) = (*this) - n;
                return (*this);
            }

            //iter++
            iterator operator++(int) {
                if((*this) == it_bel->end()) throw invalid_iterator();
                iterator tmp = (*this);
                it_pos++;
                return tmp;
            }

            //++iter
            iterator& operator++() {
                if((*this) == it_bel->end()) throw invalid_iterator();
                ++it_pos;
                return (*this);
            }

            //iter--
            iterator operator--(int) {
                if((*this) == it_bel->begin()) throw invalid_iterator();
                iterator tmp = (*this);
                it_pos--;
                return tmp;
            }

            //--iter
            iterator& operator--() {
                if((*this) == it_bel->begin()) throw invalid_iterator();
                --it_pos;
                return (*this);
            }

            //*iter
            T& operator*() const{
                if((*this) == it_bel->end()) throw invalid_iterator();
                return **(it_pos);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {return it_pos == rhs.it_pos;}
            bool operator==(const const_iterator &rhs) const {return it_pos == rhs.it_pos;}

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {return it_pos != rhs.it_pos;}
            bool operator!=(const const_iterator &rhs) const {return it_pos != rhs.it_pos;}

            friend class vector<T>;
            friend class const_iterator;
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
        private:
            T **it_pos; vector *it_bel;

        public:
            const_iterator(T **tmp_pos = NULL, vector *tmp_bel = NULL) : it_pos(tmp_pos), it_bel(tmp_bel){}

            /**
             * return a new iterator which pointer n-next elements
             *   even if there are not enough elements, just return the answer.
             * as well as operator-
             */
            const_iterator operator+(const int &n) const {
                const_iterator tmp = (*this);
                tmp.it_pos += n;
                return tmp;
            }

            const_iterator operator-(const int &n) const {
                const_iterator tmp = (*this);
                tmp.it_pos -= n;
                return tmp;
            }

            // return th distance between two iterator,
            // if these two iterators points to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const {
                if(rhs.it_bel != it_bel) throw invalid_iterator();
                int res = rhs.it_pos - it_pos;
                return (res < 0) ? (-res) : res;
            }

            const_iterator& operator+=(const int &n) {
                (*this) = (*this) + n;
                return (*this);
            }
            const_iterator& operator-=(const int &n) {
                (*this) = (*this) - n;
                return (*this);
            }

            //iter++
            const_iterator operator++(int) {
                if((*this) == it_bel->cend()) throw invalid_iterator();
                const_iterator tmp = (*this);
                it_pos++;
                return tmp;
            }

            //++iter
            const_iterator& operator++() {
                if((*this) == it_bel->cend()) throw invalid_iterator();
                ++it_pos;
                return (*this);
            }

            //iter--
            const_iterator operator--(int) {
                if((*this) == it_bel->cbegin()) throw invalid_iterator();
                const_iterator tmp = (*this);
                it_pos--;
                return tmp;
            }

            //--iter
            const_iterator& operator--() {
                if((*this) == it_bel->cbegin()) throw invalid_iterator();
                --it_pos;
                return (*this);
            }

            //*iter
            T& operator*() const{
                if((*this) == it_bel->cend()) throw invalid_iterator();
                return **(it_pos);
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            bool operator==(const iterator &rhs) const {return it_pos == rhs.it_pos;}
            bool operator==(const const_iterator &rhs) const {return it_pos == rhs.it_pos;}

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {return it_pos != rhs.it_pos;}
            bool operator!=(const const_iterator &rhs) const {return it_pos != rhs.it_pos;}

            friend class vector<T>;
            friend class iterator;
        };

        /**
         * TODO Constructs
         * Atleast three: default constructor, copy constructor and a constructor for std::vector
         */
        vector() {
            v_size = 0; v_capacity = 1;
            arr = new T* [1];
            arr[0] = NULL;
        }

        vector(const vector &other) {
            v_size = other.v_size;
            v_capacity = other.v_capacity;

            arr = new T* [v_capacity];
            for(int i = 0;i < v_size;++i) arr[i] = new T(*other.arr[i]);
            for(int i = v_size;i < v_capacity;++i) arr[i] = NULL;
        }

        ~vector() {
            for(int i = 0;i < v_size;++i) if(arr[i]) delete arr[i];
            delete []arr;
        }

        void doubleSpace(){
            T **tmp;
            tmp = new T* [v_capacity];
            for(int i = 0;i < v_size;++i) tmp[i] = arr[i];
            for(int i = v_size;i < v_capacity;++i) tmp[i] = NULL;

            delete [] arr;

            v_capacity *= 2;
            arr = new T* [v_capacity];

            for(int i = 0;i < v_size;++i) arr[i] = tmp[i];
            for(int i = 0;i < v_capacity;++i) arr[i] = NULL;
            delete[] tmp;
        }

        vector &operator=(const vector &other) {
            if((&other) == this) return (*this);
            this->~vector();
            v_size = other.v_size; v_capacity = other.v_capacity;

            arr = new T* [v_capacity];

            for(int i = 0;i < v_size;++i) arr[i] = new T(*other.arr[i]);
            for(int i = v_size;i < v_capacity;++i) arr[i] = NULL;
            return (*this);
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T & at(const size_t &pos) {
            if(pos < 0 || pos >= v_size) throw index_out_of_bound();
            return *(arr[pos]);
        }
        const T & at(const size_t &pos) const {
            if(pos < 0 || pos >= v_size) throw index_out_of_bound();
            return *(arr[pos]);
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T & operator[](const size_t &pos) { return at(pos);}
        const T & operator[](const size_t &pos) const { return at(pos);}

        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T & front() const {
            if(v_size == 0) throw container_is_empty();
            return at(0);
        }

        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T & back() const {
            if(v_size == 0) throw container_is_empty();
            return at(v_size-1);
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {return iterator(&arr[0],this);}
        const_iterator cbegin() const {return const_iterator(&arr[0], const_cast<vector*>(this));}

        /**
         * returns an iterator to the end.
         */
        iterator end() {return iterator(&arr[v_size],this);}
        const_iterator cend() const {return const_iterator(&arr[v_size], const_cast<vector*>(this));}

        /**
         * checks whether the container is empty
         */
        bool empty() const {return v_size == 0;}

        /**
         * returns the number of elements
         */
        size_t size() const {return v_size;}

        /**
         * returns the number of elements that can be held in currently allocated storage.
         */
        size_t capacity() const {return v_capacity;}

        /**
         * clears the contents
         */
        void clear() {
            this->~vector();
            v_size = 0;
            v_capacity = 1;
            arr = new T* [1];
            arr[0] = NULL;
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            return insert(pos.it_pos-(&arr[0]), value);
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value is true
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if(ind > v_size || ind < 0) throw index_out_of_bound();

            if(v_size == v_capacity)
                doubleSpace();

            for(int i = v_size;i > ind;--i) arr[i] = arr[i-1];
            arr[ind] = new T(value);
            ++v_size;

            return iterator(&arr[ind], this);
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {
            return erase(pos.it_pos-(&arr[0]));
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if(ind >= v_size) throw index_out_of_bound();
            --v_size;
            if(arr[ind]) delete arr[ind];
            for(int i = ind;i < v_size;++i) arr[i] = arr[i+1];

            return iterator(&arr[ind], this);
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if(v_size == v_capacity) doubleSpace();
            if(arr[v_size]) delete arr[v_size];
            arr[v_size] = new T(value);
            v_size++;
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if(v_size == 0) throw container_is_empty();
            else --v_size;
        }
    };


}

#endif
