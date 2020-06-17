#ifndef CODE_ALLOC_HPP
#define CODE_ALLOC_HPP
#include "map.hpp"
#include "Bptree.hpp"
using namespace std;
typedef char buffer[4096];
class filemanager{
	const int N=150;
	char filename[20];
	FILE *file;
	int sz;
	off_t rec;
	off_t *root;
	struct node{
		node *nxt,*pre;
		buffer b;
		off_t pos;
		node(off_t _pos=-1):nxt(nullptr),pre(nullptr),pos(_pos){}
	}*head,*tail;
	sjtu::map<off_t,node*>mp;
public:
	void save_info(){
		fseek(file,0,SEEK_SET);
		fwrite(root,sizeof(off_t),1,file);
		fwrite(&rec,sizeof(off_t),1,file);
	}
	void read_info(){
		fseek(file,0,SEEK_SET);
		fread(root,sizeof(off_t),1,file);
		fread(&rec,sizeof(off_t),1,file);
	}
	void init(){
		*root=-1;
		rec=-1;
		save_info();
	}
	filemanager(const char* _filename,off_t* _root):sz(0),root(_root){
		head=tail=nullptr;
		strcpy(filename,_filename);
		file=fopen(filename,"rb+");
		if(!file){
			file=fopen(filename,"wb+");
			init();
		}
		else{
			read_info();
		}
	}
	~filemanager(){
		save_info();
		node *p=head;
		while(p!= nullptr){
			node *q=p->nxt;
			fseek(file,p->pos,SEEK_SET);
			fwrite(p->b,4096,1,file);
			delete p;
			p=q;
		}
		if(file) fclose(file);
	}
	void clear(){
		node *p=head;
		while(p!= nullptr){
			node *q=p->nxt;
			delete p;
			p=q;
		}
		head=tail= nullptr;
		sz=0;
		mp.clear();
		fclose(file);
		file=fopen(filename,"wb+");
		init();
	}
	void movetohead(node *p){
		if(p==head) return;
		if(p==tail) tail=tail->pre;
		if(p->pre!=nullptr) p->pre->nxt=p->nxt;
		if(p->nxt!=nullptr) p->nxt->pre=p->pre;
		head->pre=p;
		p->nxt=head;
		head=p;
	}
	char* read_node(off_t pos){
		node *p=mp[pos];
		if(p== nullptr){
			if(sz==N){
				fseek(file,tail->pos,SEEK_SET);
				fwrite(tail->b,4096,1,file);
				mp.erase(mp.find(tail->pos));
				tail->pre->nxt=nullptr;
				p=tail;
				tail=tail->pre;
				p->pos=pos;
				p->nxt=nullptr;
				p->pre=nullptr;
			}
			else{
				p=new node(pos);
				++sz;
				if(sz==1) head=tail=p;
			}
			mp[pos]=p;

			fseek(file,pos,SEEK_SET);
			fread(p->b,4096,1,file);
		}
		movetohead(p);
		return p->b;
	}
	char* new_node(){
		node *p= nullptr;
		if(sz==N){
			fseek(file,tail->pos,SEEK_SET);
			fwrite(tail->b,4096,1,file);
			mp.erase(mp.find(tail->pos));
//			try{
//
//			}catch(...){
//				cout<<tail->pos<<endl;
//				exit(0);
//			}
			tail->pre->nxt=nullptr;
			p=tail;
			tail=tail->pre;
			p->nxt=nullptr;
			p->pre=nullptr;
		}
		else{
			p=new node();
			++sz;
			if(sz==1) head=tail=p;
		}

		if(rec!=-1){
			p->pos=rec;
			fseek(file,rec,SEEK_SET);
			fread(&rec,sizeof(off_t),1,file);
			fseek(file,-sizeof(off_t),SEEK_CUR);
			fwrite(p->b,4096,1,file);
		}else{
			fseek(file,0,SEEK_END);
			p->pos=ftell(file);
			fwrite(p->b,4096,1,file);
		}
		mp[p->pos]=p;
		*(reinterpret_cast<off_t *>(p->b))=p->pos;
		movetohead(p);
		return p->b;
	}
	void del_node(off_t pos){
		fseek(file,pos,SEEK_SET);
		fwrite(&rec,sizeof(off_t),1,file);
		rec=pos;
	}
};
#endif //CODE_ALLOC_HPP
