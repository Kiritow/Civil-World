#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cpplib/cpplib#gcolor>
#include <queue>
#include <vector>
using namespace std;

struct pos
{
	int x,y;
};
int direct(int dis)
{
	return dis>0?1:dis<0?-1:0;
}
int manhatten(pos a,pos b)
{
	return abs(a.x-b.x)+abs(a.y-b.y);
}
int issame(pos a,pos b)
{
	return a.x==b.x&&a.y==b.y;
}
pos go(pos from,pos to,int speed)
{
	if(from.x==to.x)
	{
		if(from.y==to.y)
		{
			return from;
		}
		int dis=to.y-from.y;
		if(abs(dis)<=speed)
		{
			return to;
		}
		from.y+=direct(dis)*speed;
		return from;
	}
	else if(from.y==to.y)
	{
		int dis=to.x-from.x;
		if(abs(dis)<=speed)
		{
			return to;
		}
		from.x+=direct(dis)*speed;
		return from;
	}
	else
	{
		int dis=manhatten(from,to);
		if(dis<=speed)
		{
			return to;
		}
		int disx=to.x-from.x;
		int disy=to.y-from.y;
		int A=(abs(disx)*speed)/(abs(disx)+abs(disy));
		int B=speed-A;
		from.x+=A*direct(disx);
		from.y+=B*direct(disy);
		return from;
	}
}
void draw(pos x)
{	cprint(color::yellow,color::yellow);
	gotoxy(x.x,x.y);printf(" ");
	fflush(stdout);
}
void clear(pos x)
{
	cprint(color::black,color::black);
	gotoxy(x.x,x.y);printf(" ");
	fflush(stdout);
}
void p()
{
	Sleep(100);
}
class moveable
{
	public:
	pos getpos()
	{
		return myp;
	}
	void setpos(pos x)
	{
		myp=x;
	}
	pos getnextpos()
	{
		if(cque.size()<1)
		{
			if(isloop)
			{
				start();
				return cque.front();
			}
			return myp;
		}
		return cque.front();
	}
	void donenextpos()
	{
		if(cque.size()>0)
		{
			cque.pop();
		}
	}
	int getspeed()
	{
		return speed;
	}
	void setspeed(int x)
	{
		speed=x;
	}
	void clear()
	{
		while(!cque.empty()) cque.pop();
		loopcque.clear();
		isloop=0;
	}
	void addpos(pos x)
	{
		loopcque.push_back(x);
	}
	void start()
	{
		for(auto& x:loopcque)
		{
			cque.push(x);
		}
	}
	void setloop(int x)
	{
		isloop=x;
	}
	color getcolor(){return cc;}
	void setcolor(color icc){cc=icc;}
	protected:
	queue<pos> cque;
	vector<pos> loopcque;
	pos myp;
	color cc;
	int isloop;
	int speed;
};
class map
{
	public:
	int regist(moveable* x)
	{
		x->start();
		vec.push_back(x);
		return 0;
	}
	int unregist(moveable* x)
	{
		for(int i=0;i<vec.size();i++)
		{
			if(vec.at(i)==x) {vec.erase(vec.begin()+i);return 0;}
		}
		return -1;
	}
	void tick()
	{
		for(int i=0;i<vec.size();i++)
		{
			pos cur=vec.at(i)->getpos();
			pos target=vec.at(i)->getnextpos();
			cur=go(cur,target,vec.at(i)->getspeed());
			if(issame(cur,target))
			{
				vec.at(i)->donenextpos();
			}
			vec.at(i)->setpos(cur);
		}
	}
	void draw()
	{
		cprint();cprint(CCLEAR);
		for(auto& p:vec)
		{
			gotoxy(p->getpos().x,p->getpos().y);
			cprint(p->getcolor(),p->getcolor());
			printf(" ");
		}
		fflush(stdout);
	}
	private:
	vector<moveable*> vec;
};

int main()
{
	cprint(CHIDE);
	pos t;
	t.x=20;
	t.y=2;
	pos v;
	v.x=4;
	v.y=7;
	pos a;
	a.x=1;
	a.y=1;
	pos k;
	k.x=36;
	k.y=10;

	moveable x;
	x.setpos(a);
	x.addpos(v);
	x.addpos(t);
	x.setcolor(color::yellow);
	x.setloop(1);
	x.setspeed(3);

	moveable y;
	y.setpos(t);
	y.addpos(t);
	y.addpos(v);
	y.setloop(1);
	y.setspeed(2);
	y.setcolor(color::blue);

	moveable z;
	z.setpos(k);
	z.addpos(k);
	z.addpos(a);
	z.addpos(v);
	z.addpos(t);
	z.setloop(1);
	z.setcolor(color::red);
	z.setspeed(4);


	map m;
	m.regist(&x);
	m.regist(&y);
	m.regist(&z);
	while(1)
	{
		m.tick();
		m.draw();
		//gotoxy(1,1);cprint();printf("a: %d %d\n",x.getpos().x,x.getpos().y);
		p();
	}
}