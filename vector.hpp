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
            iterator(T **tmp_pos = nullptr, vector *tmp_bel = nullptr) : it_pos(tmp_pos), it_bel(tmp_bel){}

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


            bool operator==(const iterator &rhs) const {return it_pos == rhs.it_pos;}
            bool operator==(const const_iterator &rhs) const {return it_pos == rhs.it_pos;}

            bool operator!=(const iterator &rhs) const {return it_pos != rhs.it_pos;}
            bool operator!=(const const_iterator &rhs) const {return it_pos != rhs.it_pos;}

            friend class vector<T>;
            friend class const_iterator;
        };

        class const_iterator {
        private:
            T **it_pos; vector *it_bel;

        public:
            const_iterator(T **tmp_pos = nullptr, vector *tmp_bel = nullptr) : it_pos(tmp_pos), it_bel(tmp_bel){}

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

            bool operator==(const iterator &rhs) const {return it_pos == rhs.it_pos;}
            bool operator==(const const_iterator &rhs) const {return it_pos == rhs.it_pos;}

            bool operator!=(const iterator &rhs) const {return it_pos != rhs.it_pos;}
            bool operator!=(const const_iterator &rhs) const {return it_pos != rhs.it_pos;}

            friend class vector<T>;
            friend class iterator;
        };

        vector() {
            v_size = 0; v_capacity = 1;
            arr = new T* [1];
            arr[0] = nullptr;
        }

        vector(const vector &other) {
            v_size = other.v_size;
            v_capacity = other.v_capacity;

            arr = new T* [v_capacity];
            for(int i = 0;i < v_size;++i) arr[i] = new T(*other.arr[i]);
            for(int i = v_size;i < v_capacity;++i) arr[i] = nullptr;
        }

        ~vector() {
            for(int i = 0;i < v_capacity;++i) if(arr[i] != nullptr) delete arr[i];
            delete []arr;
        }

        void doubleSpace(){
            T **tmp;
            tmp = new T* [v_capacity];
            for(int i = 0;i < v_size;++i) tmp[i] = arr[i];
            for(int i = v_size;i < v_capacity;++i) tmp[i] = nullptr;

            delete [] arr;

            v_capacity *= 2;
            arr = new T* [v_capacity];

            for(int i = 0;i < v_size;++i) arr[i] = tmp[i];
            for(int i = v_size;i < v_capacity;++i) arr[i] = nullptr;
            delete[] tmp;
        }

        vector &operator=(const vector &other) {
            if((&other) == this) return (*this);
            this->~vector();
            v_size = other.v_size; v_capacity = other.v_capacity;

            arr = new T* [v_capacity];

            for(int i = 0;i < v_size;++i) arr[i] = new T(*other.arr[i]);
            for(int i = v_size;i < v_capacity;++i) arr[i] = nullptr;
            return (*this);
        }

        T & at(const size_t &pos) {
            if(pos < 0 || pos >= v_size) throw index_out_of_bound();
            return *(arr[pos]);
        }
        const T & at(const size_t &pos) const {
            if(pos < 0 || pos >= v_size) throw index_out_of_bound();
            return *(arr[pos]);
        }

        T & operator[](const size_t &pos) { return at(pos);}
        const T & operator[](const size_t &pos) const { return at(pos);}

        const T & front() const {
            if(v_size == 0) throw container_is_empty();
            return at(0);
        }

        const T & back() const {
            if(v_size == 0) throw container_is_empty();
            return at(v_size-1);
        }

        iterator begin() {return iterator(&arr[0],this);}
        const_iterator cbegin() const {return const_iterator(&arr[0], const_cast<vector*>(this));}

        iterator end() {return iterator(&arr[v_size],this);}
        const_iterator cend() const {return const_iterator(&arr[v_size], const_cast<vector*>(this));}

        bool empty() const {return v_size == 0;}

        size_t size() const {return v_size;}

        size_t capacity() const {return v_capacity;}

        void clear() {
            this->~vector();
            v_size = 0;
            v_capacity = 1;
            arr = new T* [1];
            arr[0] = NULL;
        }

        iterator insert(iterator pos, const T &value) {
            return insert(pos.it_pos-(&arr[0]), value);
        }

        iterator insert(const size_t &ind, const T &value) {
            if(ind > v_size || ind < 0) throw index_out_of_bound();

            if(v_size == v_capacity)
                doubleSpace();

            for(int i = v_size;i > ind;--i) arr[i] = arr[i-1];
            arr[ind] = new T(value);
            ++v_size;

            return iterator(&arr[ind], this);
        }

        iterator erase(iterator pos) {
            return erase(pos.it_pos-(&arr[0]));
        }

        iterator erase(const size_t &ind) {
            if(ind >= v_size) throw index_out_of_bound();
            --v_size;
            if(arr[ind] != nullptr) delete arr[ind];
            for(int i = ind;i < v_size;++i) arr[i] = arr[i+1];

            return iterator(&arr[ind], this);
        }

        void push_back(const T &value) {
            if(v_size == v_capacity) doubleSpace();
            if(arr[v_size] != nullptr) delete arr[v_size];
            arr[v_size] = new T(value);
            v_size++;
        }

        void pop_back() {
            if(v_size == 0) throw container_is_empty();
            --v_size;
        }
    };


}

#endif
