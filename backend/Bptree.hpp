#ifndef TICKETSYSTEM_BPTREE_H
#define TICKETSYSTEM_BPTREE_H
#include <cstdio>
#include <cstring>
#include <functional>
#include "exceptions.hpp"
#include "utility.hpp"
#include "vector.hpp"
#include "alloc.hpp"
//#define DEBUG
using namespace std;
const int PAGE_SIZE=4096;
template<class Key,class Value,class Compare=std::less<Key> >
class Bptree{
	typedef pair<bool,Value> find_t;
	typedef pair<Key,off_t> entry;
	typedef pair<Key,Value> pKV;
	typedef sjtu::vector<pKV> find_list;
	///rec 垃圾回收指针
	off_t root;
	filemanager file;
	struct Node{
		bool IsLeaf;
		off_t pos;//my address
		off_t nxt;//next brother address
		int sz;
		Node(bool _IsLeaf=0, off_t _pos=-1,off_t _nxt=-1,int _sz=0):
		IsLeaf(_IsLeaf),pos(_pos),nxt(_nxt),sz(_sz){}
	};

#ifndef DEBUG
	///max number of keys in nonleaf node
	const static int MAXNUM_N=(PAGE_SIZE-sizeof(Node)-sizeof(off_t))/(sizeof(Key)+sizeof(off_t))-1;
	///max number of keys in leaf node
	const static int MAXNUM_L=(PAGE_SIZE-sizeof(Node))/(sizeof(Key)+sizeof(Value))-1;
#endif
	#ifdef DEBUG
	const static int MAXNUM_N=4;
	const static int MAXNUM_L=4;
#endif
	const static int MINNUM_N=MAXNUM_N>>1;
	const static int MINNUM_L=(MAXNUM_L+1)>>1;
	///s0 k0 s1 k1...kn-1 sn  si->[ki-1,ki)
	struct NonleafNode:public Node{
		Key keys[MAXNUM_N+1];
		off_t sons[MAXNUM_N+2];
		NonleafNode():Node(){}
		///find the least x so that key[x]>key
		int getIndex(const Key &key){
			int l=0,r=this->sz-1;
			while(l<=r){
				int mid=(l+r)>>1;
				if(key<keys[mid]) r=mid-1;
				else l=mid+1;
			}
			return ++r;
		}
		find_t find(const Key &key,Bptree* bel){
			int x=getIndex(key);
			char *p=bel->file.read_node(sons[x]);
			if(((Node*)p)->IsLeaf){
				return ((LeafNode*)p)->find(key);
			}else{
				return ((NonleafNode*)p)->find(key,bel);
			}
		}
		find_list find(const Key &k1, const Key &k2,Bptree* bel){
			int x=getIndex(k1);
			char *p=bel->file.read_node(sons[x]);
			if(((Node*)p)->IsLeaf){
				return ((LeafNode*)p)->find(k1,k2,bel);
			}else{
				return ((NonleafNode*)p)->find(k1,k2,bel);
			}
		}
		bool set(const Key &key,const Value &val,Bptree* bel){
			int x=getIndex(key);
			char *p=bel->file.read_node(sons[x]);
			if(((Node*)p)->IsLeaf){
				return ((LeafNode*)p)->set(key,val,bel);
			}else{
				return ((NonleafNode*)p)->set(key,val,bel);
			}
		}
		void merge(const NonleafNode* p,Bptree* bel){
			this->nxt=p->nxt;
			for(int i=0;i<p->sz;++i){
				this->keys[this->sz+i]=p->keys[i];
				this->sons[this->sz+i]=p->sons[i];
			}
			this->sz+=p->sz;
			this->sons[this->sz]=p->sons[p->sz];
			bel->file.del_node(p->pos);
		}
		void redistribute(NonleafNode* p,Key &key,Bptree* bel){
			int n=this->sz+p->sz;
			Key* tmpk=new Key[n];
			off_t* tmpv=new off_t[n+1];
			for(int i=0;i<this->sz;++i){
				tmpk[i]=this->keys[i];
				tmpv[i]=this->sons[i];
			}
			for(int i=0;i<p->sz;++i){
				tmpk[this->sz+i]=p->keys[i];
				tmpv[this->sz+i]=p->sons[i];
			}
			tmpv[n]=p->sons[p->sz];
			this->sz=n>>1;
			for(int i=0;i<this->sz;++i){
				this->keys[i]=tmpk[i];
				this->sons[i]=tmpv[i];
			}
			this->sons[this->sz]=tmpv[this->sz];
			key=tmpk[this->sz];
			p->sz=n-this->sz-1;
			for(int i=0;i<p->sz;++i){
				p->keys[i]=tmpk[this->sz+1+i];
				p->sons[i]=tmpv[this->sz+1+i];
			}
			p->sons[p->sz]=tmpv[n];
			delete[] tmpk;
			delete[] tmpv;
		}
		void split(Key &key,off_t &val,Bptree* bel){
			char *p=bel->file.new_node();
			NonleafNode *q= reinterpret_cast<NonleafNode *>(p);
			q->pos=*(reinterpret_cast<off_t *>(p));
			q->IsLeaf=0;
			q->nxt=this->nxt;
			int nn=this->sz/2;
			q->sz=this->sz-nn-1;
			for(int i=nn+1;i<this->sz;++i){
				q->keys[i-nn-1]=this->keys[i];
				q->sons[i-nn-1]=this->sons[i];
			}
			q->sons[q->sz]=this->sons[this->sz];
			this->nxt=q->pos;
			this->sz=nn;
			key=this->keys[nn];
			val=q->pos;
		}
		///check whether to split and save the node info at father
		bool insert(const Key &key,const Value &val,Bptree* bel){
			int x=getIndex(key);
			char *p=bel->file.read_node(sons[x]);
			if(((Node*)p)->IsLeaf){
				LeafNode* pp= reinterpret_cast<LeafNode*>(p);
				bool res=pp->insert(key,val);
				if(!res) return false;
				if(pp->sz>MAXNUM_L){
					Key new_key;off_t new_son;
					pp->split(new_key,new_son,bel);
					for(int i=this->sz-1;i>=x;--i){
						keys[i+1]=keys[i];
						sons[i+2]=sons[i+1];
					}
					keys[x]=new_key;
					sons[x+1]=new_son;
					++this->sz;
				}
				return true;
			}else{
				NonleafNode* pp= reinterpret_cast<NonleafNode*>(p);
				int old_sz=pp->sz;
				bool res=pp->insert(key,val,bel);
				if(!res) return false;
				if(pp->sz==old_sz) return true;
				if(pp->sz>MAXNUM_N){
					Key new_key;
					off_t new_son=-1;
					pp->split(new_key,new_son,bel);
					for(int i=this->sz-1;i>=x;--i){
						keys[i+1]=keys[i];
						sons[i+2]=sons[i+1];
					}
					keys[x]=new_key;
					sons[x+1]=new_son;
					++this->sz;
				}
				return true;
			}
		}
		bool remove(const Key &key,bool &f,Bptree* bel){
			int x=getIndex(key);
			char *p=bel->file.read_node(sons[x]);
			if(((Node*)p)->IsLeaf){
				LeafNode* pp= reinterpret_cast<LeafNode*>(p);
				bool res=pp->remove(key);
				if(!res) return false;
				if(pp->sz<MINNUM_L){
					p=bel->file.read_node(sons[x==this->sz?x-1:x+1]);
					LeafNode* q= reinterpret_cast<LeafNode*>(p);
					f=true;//modified
					///merge
					if(q->sz+pp->sz<=MAXNUM_L){
						if(x==this->sz){//q pp
							q->merge(pp,bel);
						}else{//pp q
							pp->merge(q,bel);
							for(int i=x;i<this->sz-1;++i)
								keys[i]=keys[i+1];
							for(int i=x+1;i<this->sz;++i)
								sons[i]=sons[i+1];
						}
						--this->sz;
						return true;
					}
					///redistribute
					if(x==this->sz){//q pp
						q->redistribute(pp,keys[this->sz-1],bel);
					}else{//pp q
						pp->redistribute(q,keys[x],bel);
					}
					return true;
				}
				return true;
			}else{
				NonleafNode* pp= reinterpret_cast<NonleafNode*>(p);
				bool modified=false;
				bool res=pp->remove(key,modified,bel);
				if(!res) return false;
				if(!modified) return true;
				if(pp->sz<MINNUM_N){
					p=bel->file.read_node(sons[x==this->sz?x-1:x+1]);
					NonleafNode* q= reinterpret_cast<NonleafNode*>(p);
					f=true;
					if(x==this->sz) q->keys[q->sz++]=keys[this->sz-1];
					else pp->keys[pp->sz++]=keys[x];
					///merge
					if(q->sz+pp->sz<=MAXNUM_N){
						if(x==this->sz){//q pp
							q->merge(pp,bel);
						}else{//pp q
							pp->merge(q,bel);
							for(int i=x;i<this->sz-1;++i)
								keys[i]=keys[i+1];
							for(int i=x+1;i<this->sz;++i)
								sons[i]=sons[i+1];
						}
						--this->sz;
						return true;
					}
					///redistribute
					if(x==this->sz){//q pp
						q->redistribute(pp,keys[this->sz-1],bel);
					}else{//pp q
						pp->redistribute(q,keys[x],bel);
					}
					return true;
				}
				return true;
			}
		}
		void print(Bptree* bel){
			puts("~~~~~~~~~~~~~~~~~~");
			printf("pos: %ld\tsz: %d\tnxt: %ld\n",this->pos,this->sz,this->nxt);
			for(int i=0;i<this->sz;++i)
				cout<<sons[i]<<" "<<keys[i]<<" ";
			cout<<sons[this->sz]<<endl;
			puts("~~~~~~~~~~~~~~~~~~");
			for(int i=0;i<=this->sz;++i){
				char *p=bel->file.read_node(sons[i]);
				if(((Node*)p)->IsLeaf){
					((LeafNode*)p)->print();
				}else{
					((NonleafNode*)p)->print(bel);
				}
			}
		}
	};
	struct LeafNode:public Node{
		Key keys[MAXNUM_L+1];
		Value values[MAXNUM_L+1];
		LeafNode():Node(1){}
		///find the least x so that key[x]>=key
		int getIndex(const Key &key){
			int l=0,r=this->sz-1;
			while(l<=r){
				int mid=(l+r)>>1;
				if(keys[mid]<key) l=mid+1;
				else r=mid-1;
			}
			return ++r;
		}
		find_t find(const Key &key){
			int x=getIndex(key);
			if(x<this->sz&&keys[x]==key) return find_t(true,values[x]);
			return find_t(false,Value());
		}
		find_list find(const Key &k1,const Key &k2,Bptree* bel){
			int x=getIndex(k1);
			char* b;
			LeafNode *p=this;
			find_list res;
			while(1){
				if(x==p->sz){
					if(p->nxt==-1) break;
					b=bel->file.read_node(p->nxt);
					p=(LeafNode*)b;
					x=0;
				}
				if(!(p->keys[x]<k2)) break;
				res.push_back(pKV(p->keys[x],p->values[x]));
				++x;
			}
			return res;
		}
		bool set(const Key &key,const Value &val,Bptree* bel){
			int x=getIndex(key);
			if(x<this->sz&&keys[x]==key){
				values[x]=val;
				return true;
			}
			return false;
		}
		void merge(const LeafNode* p,Bptree* bel){
			this->nxt=p->nxt;
			for(int i=0;i<p->sz;++i){
				this->keys[this->sz+i]=p->keys[i];
				this->values[this->sz+i]=p->values[i];
			}
			this->sz+=p->sz;
			bel->file.del_node(p->pos);
		}
		void redistribute(LeafNode* p,Key &key,Bptree* bel){
			int n=this->sz+p->sz;
			Key* tmpk=new Key[n];
			Value* tmpv=new Value[n];
			for(int i=0;i<this->sz;++i){
				tmpk[i]=this->keys[i];
				tmpv[i]=this->values[i];
			}
			for(int i=0;i<p->sz;++i){
				tmpk[this->sz+i]=p->keys[i];
				tmpv[this->sz+i]=p->values[i];
			}
			this->sz=n>>1;
			for(int i=0;i<this->sz;++i){
				this->keys[i]=tmpk[i];
				this->values[i]=tmpv[i];
			}
			key=tmpk[this->sz];
			p->sz=n-this->sz;
			for(int i=0;i<p->sz;++i){
				p->keys[i]=tmpk[this->sz+i];
				p->values[i]=tmpv[this->sz+i];
			}
			delete[] tmpk;
			delete[] tmpv;
		}
		void split(Key &key,off_t &val,Bptree* bel){
			char *p=bel->file.new_node();
			LeafNode *q= reinterpret_cast<LeafNode *>(p);
			q->pos=*(reinterpret_cast<off_t *>(p));
			q->IsLeaf=1;
			q->nxt=this->nxt;
			int nn=this->sz>>1;
			q->sz=this->sz-nn;
			for(int i=nn;i<this->sz;++i){
				q->keys[i-nn]=this->keys[i];
				q->values[i-nn]=this->values[i];
			}
			this->nxt=q->pos;
			this->sz=nn;
			key=q->keys[0];
			val=q->pos;
		}
		bool insert(const Key &key,const Value &val){
			int x=getIndex(key);
			if(x<this->sz&&keys[x]==key) return false;
			for(int i=this->sz-1;i>=x;--i){
				keys[i+1]=keys[i];
				values[i+1]=values[i];
			}
			keys[x]=key;
			values[x]=val;
			++this->sz;
			return true;
		}
		bool remove(const Key &key){
			int x=getIndex(key);
			if(!(x<this->sz&&keys[x]==key)) return false;
			for(int i=x;i<this->sz-1;++i){
				keys[i]=keys[i+1];
				values[i]=values[i+1];
			}
			--this->sz;
			return true;
		}
		void print(){
			puts("~~~~~~~~~~~~~~~~~");
			printf("pos: %ld\tsz: %d\tnxt: %ld\n",this->pos,this->sz,this->nxt);
			for(int i=0;i<this->sz;++i)
				cout<<values[i]<<" "<<keys[i]<<" ";
			cout<<endl;
			puts("~~~~~~~~~~~~~~~~~");
		}
	};
public:
	///@param _filename the name of file which stores the bptree
	Bptree(const char* _filename):file(_filename,&root){
//		strcpy(filename,_filename);
	}
	~Bptree(){};
	bool empty(){
		return root==-1;
	}
	find_t find(const Key &key){
		if(empty()) return find_t(false,Value());
		char *p=file.read_node(root);
		if(((Node*)p)->IsLeaf){
			return ((LeafNode*)p)->find(key);
		}else{
			return ((NonleafNode*)p)->find(key,this);
		}
	}
	find_list find(const Key &k1, const Key &k2){
		if(empty()) return find_list();
		char *p=file.read_node(root);
		if(((Node*)p)->IsLeaf){
			return ((LeafNode*)p)->find(k1,k2,this);
		}else{
			return ((NonleafNode*)p)->find(k1,k2,this);
		}
	}
	bool count(const Key &key){
		return find(key).first;
	}
	///succeeded? true:false
	bool insert(const Key &key,const Value &val){
		if(empty()){
			char *p=file.new_node();
			LeafNode *q= reinterpret_cast<LeafNode *>(p);
			q->pos=*(reinterpret_cast<off_t *>(p));
			q->IsLeaf=1;
			q->sz=0;
			q->nxt=-1;
			q->insert(key,val);
			root=q->pos;
			return true;
		}
		char* b=file.read_node(root);
		if(((Node*)b)->IsLeaf){
			LeafNode *pp= reinterpret_cast<LeafNode*>(b);
			bool res=pp->insert(key,val);
			if(!res) return false;
			if(pp->sz>MAXNUM_L){
				Key new_key;off_t new_son;
				pp->split(new_key,new_son,this);
				char *p=file.new_node();
				NonleafNode *q= reinterpret_cast<NonleafNode *>(p);
				q->pos=*(reinterpret_cast<off_t *>(p));
				q->IsLeaf=0;
				q->nxt=-1;
				q->sons[0]=pp->pos;
				q->sons[1]=new_son;
				q->keys[0]=new_key;
				q->sz=1;
				root=q->pos;
			}
			return true;
		}else{
			NonleafNode *pp= reinterpret_cast<NonleafNode*>(b);
			int old_sz=pp->sz;
			bool res=pp->insert(key,val,this);
			if(!res) return false;
			if(pp->sz==old_sz) return true;
			if(pp->sz>MAXNUM_N){
				Key new_key;off_t new_son;
				pp->split(new_key,new_son,this);
				char *p=file.new_node();
				NonleafNode *q= reinterpret_cast<NonleafNode *>(p);
				q->pos=*(reinterpret_cast<off_t *>(p));
				q->IsLeaf=0;
				q->nxt=-1;
				q->sons[0]=pp->pos;
				q->sons[1]=new_son;
				q->keys[0]=new_key;
				q->sz=1;
				root=q->pos;
			}
			return true;
		}
	}
	///succeeded?true:false
	bool remove(const Key &key){
		if(empty()) return false;
		char *p=file.read_node(root);
		if(((Node*)p)->IsLeaf){
			LeafNode* pp= reinterpret_cast<LeafNode*>(p);
			bool res=pp->remove(key);
			if(!res) return false;
			if(pp->sz==0){
				file.del_node(pp->pos);
				root=-1;
			}
			return true;
		}else{
			NonleafNode* pp= reinterpret_cast<NonleafNode*>(p);
			bool modified=false;
			bool res=pp->remove(key,modified,this);
			if(!res) return false;
			if(!modified) return true;
			if(pp->sz==0){
				root=pp->sons[0];
				file.del_node(pp->pos);
			}
			return true;
		}
	}
	bool set(const Key &key,const Value &val){
		if(empty()) return false;
		char *p=file.read_node(root);
		if(((Node*)p)->IsLeaf){
			return ((LeafNode*)p)->set(key,val,this);
		}else{
			return ((NonleafNode*)p)->set(key,val,this);
		}
	}
	void clear(){
		file.clear();
	}
	void print(){
		puts("-------------------------------");
		if(!empty()){
			char *p=file.read_node(root);
			if(((Node*)p)->IsLeaf){
				((LeafNode*)p)->print();
			}else{
				((NonleafNode*)p)->print(this);
			}
		}
		puts("-------------------------------");
	}
};
#endif //TICKETSYSTEM_BPTREE_H