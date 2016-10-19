#include <cstdio>
#include <conio.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cpplib/cpplib#gcolor>
#include <queue>
#include <vector>
#include <string>
using namespace std;

typedef int (*IFUNCV)(void);

struct pos
{
    int x,y;
    pos(int ix,int iy):x(ix),y(iy){}
    pos(){x=1;y=1;}
};
struct icon
{
    color f,b;
    char c;
    icon(color incf,color incb,char ic):f(incf),b(incb),c(ic){}
    icon(){f=color::white;b=color::black;c='X';}
};
struct bigicon
{
    color f,b;
    int w,h;
};
struct unitinfo
{
    string name;/// Unit Name ( Mostly named as TypeString, can be defined by user. Example: `Solider` or `My Little Cat`)
    int type;/// unit Type ( 0 Solider 1 Armored Car 2 Tank 3 Plane 4 Missile)
    string techname;/// The name of production Technology
    int uid;///(Belongs to ...)
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
{
    cprint(color::yellow,color::yellow);
    gotoxy(x.x,x.y);
    printf(" ");
    fflush(stdout);
}
void clear(pos x)
{
    cprint(color::black,color::black);
    gotoxy(x.x,x.y);
    printf(" ");
    fflush(stdout);
}
void p()
{
    Sleep(100);
}

#define readonly_p(Type,Name) protected:Type Name;public:Type get##Name(){return Name;}
#define readonly(Type,Name) private:Type Name;public:Type get##Name(){return Name;}
#define datarw_p(Type,Name) protected:Type Name;public:Type get##Name(){return Name;} void set##Name(Type i##Name){Name=i##Name;}
#define datarw(Type,Name) private:Type Name;public:Type get##Name(){return Name;} void set##Name(Type i##Name){Name=i##Name;}

class engine
{
public:
    engine(int ispeed)
    {
        speed=ispeed;
    }
protected:
    datarw_p(int,speed);
};


class armor
{
    datarw_p(int,maxhp);
    datarw_p(int,hp);
};

class weapon
{
    datarw_p(int,damage)
    datarw_p(int,range)
    /// Area Of Effect (AOE)
    datarw_p(int,aoe)
};

class radar
{
    /// Field Of View (FOV)
    datarw_p(int,fov);
};

class entity
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
protected:
    pos myp;
};

class resource : public entity
{

};

class building : public entity
{

};

class moveable : public entity
{
public:
    moveable():eng(0)
    {

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
    void dotask()
    {
        if(thandle) thandle();
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
        return eng.getspeed();
    }
    void setengine(engine x)
    {
        eng=x;
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
    color getcolor()
    {
        return cc;
    }
    void setcolor(color icc)
    {
        cc=icc;
    }
protected:
    queue<pos> cque;
    vector<pos> loopcque;
    color cc;
    int isloop;
    engine eng;
    IFUNCV thandle;
};

class unit : public moveable
{
public:
    unit()
    {
        lockedtarget=nullptr;
        thandle=reinterpret_cast<IFUNCV>(&_callback_focus);
    }
    weapon* getweapon(){return &wp;}
    armor* getarmor(){return &ar;}
    radar* getradar(){return &rd;}
    unitinfo* getinfo(){return &ifo;}
    int lockon(unit* target)
    {
        lockedtarget=target;
        return 0;
    }
    void freelock()
    {
        lockedtarget=nullptr;
    }
    int _callback_focus()
    {
        if(!lockedtarget) return 0;
        clear();
        addpos(lockedtarget->getpos());
        start();
        return 1;
    }
protected:
    unitinfo ifo;
    weapon wp;
    armor ar;
    radar rd;
    unit* lockedtarget;
};

class mapcontrol
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
        for(size_t i=0; i<vec.size(); i++)
        {
            if(vec.at(i)==x)
            {
                vec.erase(vec.begin()+i);
                return 0;
            }
        }
        return -1;
    }
    void tick()
    {
        for(size_t i=0; i<vec.size(); i++)
        {
            /// Update Position Queue
            vec.at(i)->dotask();

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
        cprint();
        cprint(CCLEAR);
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


unit x;
unit y;
unit z;
unit xx;

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


    x.setpos(a);
    x.addpos(v);
    x.addpos(t);
    x.setcolor(color::yellow);
    x.setloop(1);
    x.setengine(engine(3));

    y.setpos(t);
    y.addpos(t);
    y.addpos(v);
    y.setloop(1);
    y.setengine(engine(2));
    y.setcolor(color::blue);

    z.setpos(k);
    z.addpos(k);
    z.addpos(a);
    z.addpos(v);
    z.addpos(t);
    z.setloop(1);
    z.setcolor(color::red);
    z.setengine(engine(3));


    xx.setpos(k);
    xx.lockon(&z);
    xx.setengine(engine(2));
    xx.setloop(1);
    xx.setcolor(color::lightblue);

    mapcontrol m;
    m.regist(&x);
    m.regist(&y);
    m.regist(&z);
    m.regist(&xx);
    while(1)
    {
        m.tick();
        m.draw();
        //gotoxy(1,1);cprint();printf("a: %d %d\n",x.getpos().x,x.getpos().y);
        p();
    }

    return 0;
}
